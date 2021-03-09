///*
// * state_machine.c
// *
// *  Created on: Mar 8, 2021
// *      Author: Yogesh
// */
//
//
///*
// * state_t is an enum of all of the states in the FSM
// */
#include <Buffahiti.h>
#include <Buffahiti.h>
#include <stdint.h>
#include <stdio.h>
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "log.h"
#include "tsi.h"
#include "led.h"
#include "systick.h"

//declares color codes
typedef struct
{
	int R;
	int G;
	int B;
}color_code;

//declares states and transition
typedef enum {
	STOP,
	GO,
	WARN,
	CROSSWALK,
	TRANSITION
} signal_state_t;

//state and color
typedef struct {
	signal_state_t state;
	signal_state_t next_state;
	color_code start_col;
	color_code end_col;
} col_sig_state_t;


#define CROSSWALK_SIGNAL_TIMEOUT 10000
#define TRANSITION_TIMEOUT 1000

#ifdef DEBUG

#define STOP_SIGNAL_TIMEOUT 5000
#define GO_SIGNAL_TIMEOUT 5000
#define WARN_SIGNAL_TIMEOUT 3000


#else

#define STOP_SIGNAL_TIMEOUT 20000
#define GO_SIGNAL_TIMEOUT 20000
#define WARN_SIGNAL_TIMEOUT 5000

#endif

//Color codes (values of RGB) in each state
const color_code COLOR_STOP = { .R=0x61, .G=0x1E, .B=0x3C }; // 0x611E3C
const color_code COLOR_GO   = { .R=0x22, .G=0x96, .B=0x22 }; // 0x229622
const color_code COLOR_WARN = { .R=0xFF, .G=0xB2, .B=0x00 }; // 0xFFB200
const color_code COLOR_CROSSWALK = { .R=0x00, .G=0x10, .B=0x30 }; // 0x001030
color_code COLOR_ONGOING = COLOR_STOP;

//Initial Stop
static col_sig_state_t StopSig = {
	STOP,         // state
	TRANSITION,		  // next_state
	COLOR_STOP,   // start_col
	COLOR_STOP,   // end_col
};

// The state machine function
uint32_t scan_timestamp, blink_timestamp;


void Loop()
{
	//Touch scanning always except when with crosswalk state
		if((StopSig.state != CROSSWALK)&&(StopSig.next_state != CROSSWALK))
		{
		if(((time_now()) - (scan_timestamp)) > 50)					//polling in every 50ms
		{
			uint32_t scan_val = TSI_scan();							//poll
				if(scan_val > 1000)
				{
					//transit to crosswalk state
					StopSig.state = TRANSITION;
					StopSig.end_col = COLOR_CROSSWALK;
					StopSig.next_state = CROSSWALK;
					LOG("touch value = %u", scan_val);
					reset_timer();
				}
				scan_timestamp = time_now();
		}
		}

		switch (StopSig.state) {
//states with their start and end color values as well as information for next state to be called
		case STOP:
			StopSig.start_col = COLOR_STOP;
			if(get_timer() >= STOP_SIGNAL_TIMEOUT) 						//while timeout (5s/20s) has happened
			{
				StopSig.state = TRANSITION;								//transition
				StopSig.end_col = COLOR_GO;								//set colors
				StopSig.start_col = COLOR_STOP;
				StopSig.next_state = GO;								//provide next state
				LOG("%u GO state called\n",  time_now());
				reset_timer();
			}
			break;

		case GO:
			StopSig.start_col = COLOR_GO;
			if (get_timer() >= GO_SIGNAL_TIMEOUT)
			{
				StopSig.state = TRANSITION;
				StopSig.end_col = COLOR_WARN;
				StopSig.next_state = WARN;
				LOG("%u WARN state called\n",  time_now());
				reset_timer();
			}
			break;

		case WARN:
			StopSig.start_col = COLOR_WARN;
			if (get_timer() >= WARN_SIGNAL_TIMEOUT)
			{
				StopSig.state = TRANSITION;
				StopSig.end_col = COLOR_STOP;
				StopSig.next_state = STOP;
				LOG("%u STOP state called\n",  time_now());
				reset_timer();
			}
			break;

		case CROSSWALK:
			StopSig.start_col = COLOR_CROSSWALK;
			if (get_timer() <= CROSSWALK_SIGNAL_TIMEOUT)
			{
				if(((time_now()) - (blink_timestamp)) <= 250)
				{
					TPM2->CONTROLS[0].CnV = 0;
					TPM2->CONTROLS[1].CnV = 0;
					TPM0->CONTROLS[1].CnV = 0;
				}
				else if(((time_now()) - (blink_timestamp)) <= 1000)
				{
					TPM2->CONTROLS[0].CnV = (( StopSig.start_col.R)*(PWM_PERIOD))/256;
					TPM2->CONTROLS[1].CnV = (( StopSig.start_col.G)*(PWM_PERIOD))/256;
					TPM0->CONTROLS[1].CnV = (( StopSig.start_col.B)*(PWM_PERIOD))/256;
				}
				else
				{
					blink_timestamp = time_now();
				}
			}
			else
			{
				StopSig.state = TRANSITION;
				StopSig.end_col = COLOR_GO;
				StopSig.next_state = GO;
				LOG("%u CROSSWALK state called\n",  time_now());
				reset_timer();
			}
			break;

		case TRANSITION:
			//light leds according to pattern(formula) provided
			if (get_timer() < TRANSITION_TIMEOUT) {
				TPM2->CONTROLS[0].CnV = (uint32_t) ((uint8_t) ((((StopSig.end_col.R - StopSig.start_col.R) * (int16_t) get_timer())/1000) + (int16_t) StopSig.start_col.R)*(PWM_PERIOD))/256;
				TPM2->CONTROLS[1].CnV = (uint32_t) ((uint8_t) ((((StopSig.end_col.G - StopSig.start_col.G) * (int16_t) get_timer())/1000) + (int16_t) StopSig.start_col.G)*(PWM_PERIOD))/256;
				TPM0->CONTROLS[1].CnV = (uint32_t) ((uint8_t) ((((StopSig.end_col.B - StopSig.start_col.B) * (int16_t) get_timer())/1000) + (int16_t) StopSig.start_col.B)*(PWM_PERIOD))/256;
			}
			else		//calls next state
			{
				StopSig.state = StopSig.next_state;
				LOG("%u next state called\n", time_now());
				reset_timer();
				if(StopSig.state == CROSSWALK)
					blink_timestamp = time_now();
			}
			break;
		}
}
