/*
 * Name  : Hang Yuan
 * StdID : 1564348
 * CruzID: hyuan3
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Buttons.h"
#include "Leds_Lab06.h"

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****
struct SwitchState {
    uint8_t sw_1;
    uint8_t sw_2;
    uint8_t sw_3;
    uint8_t sw_4;
};

// **** Define global, module-level, or external variables here ****
#define DOWN 0
#define UP   1
static struct SwitchState sw = {DOWN, DOWN, DOWN, DOWN};
static uint8_t led = 0;

// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    ButtonsInit();
    LEDS_INIT();
    
    printf("Welcome to hyuan3's lab6 part5 (bounce_buttons).  Compiled on %s %s.\n", __TIME__, __DATE__);
//    If switch 1 is down, then each Button DOWN event toggles the appropriate LEDs,
//        and button up events do nothing.
//    If switch 1 is up, then each Button UP event toggles the appropriate LEDs, and
//        button down events do nothing.
    while (1) {
        uint8_t state = ButtonsCheckEvents();
        if (state & BUTTON_EVENT_NONE) {
            continue;
        }
        if (sw.sw_1 == DOWN) {
            if (state & BUTTON_EVENT_1DOWN) {
                led ^= 0b00000011;
            }
        } else {
            if (state & BUTTON_EVENT_1UP) {
                led ^= 0b00000011;
            }
        }
        if (sw.sw_2 == DOWN) {
            if (state & BUTTON_EVENT_2DOWN) {
                led ^= 0b00001100;
            }
        } else {
            if (state & BUTTON_EVENT_2UP) {
                led ^= 0b00001100;
            }
        }
        if (sw.sw_3 == DOWN) {
            if (state & BUTTON_EVENT_3DOWN) {
                led ^= 0b00110000;
            }
        } else {
            if (state & BUTTON_EVENT_3UP) {
                led ^= 0b00110000;
            }
        }
        if (sw.sw_4 == DOWN) {
            if (state & BUTTON_EVENT_4DOWN) {
                led ^= 0b11000000;
            }
        } else {
            if (state & BUTTON_EVENT_4UP) {
                led ^= 0b11000000;
            }
        }
            
        LEDS_SET(led);
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. It should check for button events and stores them in a
 * module-level variable.
 * 
 * You should not modify this function for ButtonsTest.c or bounce_buttons.c!
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    uint8_t state = SWITCH_STATES();
    if (state & SWITCH_STATE_SW1) {
        sw.sw_1 = UP;
    } else {
        sw.sw_1 = DOWN;
    }
    if (state & SWITCH_STATE_SW2) {
        sw.sw_2 = UP;
    } else {
        sw.sw_2 = DOWN;
    }
    if (state & SWITCH_STATE_SW3) {
        sw.sw_3 = UP;
    } else {
        sw.sw_3 = DOWN;
    }
    if (state & SWITCH_STATE_SW4) {
        sw.sw_4 = UP;
    } else {
        sw.sw_4 = DOWN;
    }
    
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}