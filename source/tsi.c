#include <stdio.h>
#include "MKL25Z4.h"


#define TSI_DATA ((TSI0->DATA) & 0xFFFF)

//Following function initializes the touch slider parameters
void TSI_init()
{
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK; 			// enabling the clock

	TSI0->GENCS = TSI_GENCS_MODE(0u) | 			//operating in non-noise mode
					TSI_GENCS_REFCHRG(0u) | 	//reference oscillator charge and discharge value 500nA
					TSI_GENCS_DVOLT(0u) | 		//oscillator voltage rails set to default
					TSI_GENCS_EXTCHRG(0u) |		 //electrode oscillator charge and discharge value 500nA
					TSI_GENCS_PS(0u) |  		// frequency clcok divided by one
					TSI_GENCS_NSCN(31u) | 		//scanning the electrode 32 times
					TSI_GENCS_TSIEN_MASK | 		//enabling the TSI module
					TSI_GENCS_EOSF_MASK; 		// writing one to clear the end of scan flag
}

//Following function scans for input (performs polling)
//returns the value scanned by the slider
int TSI_scan()
{
	unsigned int scan = 0;
		TSI0->DATA =  TSI_DATA_TSICH(10u); //setting channel 10
		TSI0->DATA |= TSI_DATA_SWTS_MASK; //software trigger to start the scan
		while (!(TSI0->GENCS & TSI_GENCS_EOSF_MASK )) // waiting for the scan to complete 32 times
		__asm volatile("NOP");
	scan = TSI_DATA;
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK ; //writing one to clear the end of scan flag
	return scan;
}



/****************************************************************************
 CREDITS:
 Alexander G Dean's github repository for NXP KL25Z Touch Scan Slider

 https://github.com/alexander-g-dean/ESF/tree/master/NXP/Misc/Touch%20Sense
 ****************************************************************************/
