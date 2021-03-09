#include <stdio.h>
#include <MKL25Z4.h>
#include <stdint.h>
#include "core_cm0plus.h"
#include "led.h"
#include "systick.h"

typedef uint32_t ticktime_t;

ticktime_t tick = 0;
ticktime_t temp = 0;

//Systick timer initialization
void Init_SysTick()
{
	SysTick->LOAD = 30000;				//for 10ms res
	NVIC_SetPriority (SysTick_IRQn, 3);
	SysTick->VAL = 0;					//Initial value of counter
	SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk); 		//frequency = 3Mhz (ext clock)
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; //enable interrupt
}

//interrupt handler
void SysTick_Handler()
{
	tick += 10;
}

//returns current time since startup
ticktime_t time_now()
{
	return tick;
}

//resets the timer
void reset_timer()
{
	temp = tick ;
}

//returns time
ticktime_t get_timer()
{
	return (tick - temp);
}


