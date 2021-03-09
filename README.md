# PES_4_Buffahiti

PRINCIPLES OF EMBEDDED SOFTWARE – ASSIGNMENT 4 (BUFFAHITI)

Introduction

The code is made in the MCUXpresso environment using C language. The code consists of multiple .c and .h files – Buffahiti.c (main), systick.c, tsi.c, led.c, state_machine.c, Buffahiti.h, led.h, tsi.h, state_machine.h, log.h, systick.h.
Files:

.c Files:

Buffahiti.c :

Mainly initializes the PWM for three LEDs and contains main(). Following function are present:

Void Init_PWM(uint32_t period)

This Function initializes the PWM for red, green, and blue leds mapping them to their respective TPM timers and setting the load value. The resolution of the clock is 10ms.

Int main(void)

The main function of the program. It calls the state machine main loop after calling all the init (configuration) functions for different modules.

led.c:

This file deals with initialization of three leds (RGB) – setting MUX values and providing clock, etc - and function to turn the led on and off. 

Systick.c

This file contains the initialization and setting resolution of the systick timer (with interrupt handler) which has been used for timing throughout the program. It also contains functions to reset the timer, return time after startup, get time.

tsi.c

This file initializes the TSI sensor and also has a function to scan (poll) for touch.

state_machine.c

This file contains the main state machine being used. The file contains an enum for declaring states and two structures for led color values (for RGB) and machine’s current and next state. It also defines two modes – DEBUG and PRODUCTION – for different timing configurations. The function Loop() is contained inside this file which contains the main working of the state machine described below.

The State Machine:

The state machine contains 4 states – stop, go, warning, and crosswalk. Stop, go, and warning states are run in main loop one after the other with each active for 5 or 20 seconds depending on the mode choice. Each state starts with a particular code values of colors RGB and stays with that for active time. Transition of one state to other takes 1 seconds with each led changing its code value according to a specifies formula. When the button is pressed (TSI touch), the transition happens (in its normal way) to the crosswalk state. Then after blinking the led for 10 seconds, the transition to GO state takes place.

