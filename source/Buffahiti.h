#include <stdint.h>

#define BLUE_LED_PIN	1
#define RED_LED_PIN		18
#define GREEN_LED_PIN	19
#define PWM_PERIOD 60000L		//period for 10ms res

void Init_PWM(uint32_t period);		//initialization function of PWM
