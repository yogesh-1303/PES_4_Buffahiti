/*
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    .c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include <stdint.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "state_machine.h"
#include "led.h"
#include "log.h"
#include "tsi.h"
#include "systick.h"

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 *
 *
 */
#define BLUE_LED_PIN	1
#define RED_LED_PIN		18
#define GREEN_LED_PIN	19
#define PWM_PERIOD 60000L

void Init_PWM(uint32_t period)
{
	//Providing clocks to ports and TMP0, TPM2
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;

	//setting MUX values
	PORTD->PCR[BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_PIN] |= PORT_PCR_MUX(4);
	PORTB->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_PIN] |= PORT_PCR_MUX(3);
	PORTB->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(3);


	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK); //Enable TPM clock

	TPM0->MOD = period - 1;
	TPM0->SC = TPM_SC_PS(3);      //prescaler of 8
	TPM2->MOD = period - 1;
	TPM2->SC = TPM_SC_PS(3);

	//TPM debug mode
	TPM0->CONF |= TPM_CONF_DBGMODE(3);
	TPM2->CONF |= TPM_CONF_DBGMODE(3);

	//TPM low true pulses
	TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;

	//PWM values of three colors
	TPM0->CONTROLS[1].CnV = 0;			//Blue
	TPM2->CONTROLS[0].CnV = 0;			//Red
	TPM2->CONTROLS[1].CnV = 0;			//Green

	//enabling
	TPM0->SC |= TPM_SC_CMOD(1);
	TPM2->SC |= TPM_SC_CMOD(1);
}

int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
    TSI_init();
    Init_SysTick();
    Init_PWM(PWM_PERIOD);
    /* Force the counter to be placed into memory. */
    //volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */

//Initial Values
TPM2->CONTROLS[0].CnV = ((0x61)*(PWM_PERIOD))/256;
TPM2->CONTROLS[1].CnV = ((0x1E)*(PWM_PERIOD))/256;
TPM0->CONTROLS[1].CnV = ((0x3C)*(PWM_PERIOD))/256;

LOG("Main loop starting\n");
    while(1) {

    	Loop();		//state machine loop

    }
    return 0 ;
}
