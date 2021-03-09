#define TSI_DATA ((TSI0->DATA) & 0xFFFF)

void TSI_init();			//function that initializes the touch slider parameters (clock gating, setting TSI0_GENCS register values)

int TSI_scan();				//polls(scans) for the input

