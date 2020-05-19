/*
 * Name  : Hang Yuan
 * StdID : 1564348
 * CruzID: hyuan3
 */

// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
#include <GenericTypeDefs.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Leds_Lab06.h"


// **** Declare any datatypes here ****

struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

// **** Define global, module-level, or external variables here ****
#define TRUE         1
#define FALSE        0
#define TWO_SECOND   76
#define THREE_SECOND 114
#define FIVE_SECOND  190

static struct Timer timerA = {FALSE, TWO_SECOND};
static struct Timer timerB = {FALSE, THREE_SECOND};
static struct Timer timerC = {FALSE, FIVE_SECOND};

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
    printf("Welcome to hyuan3's lab6 part1 (timers).  Compiled on %s %s.\n", __TIME__, __DATE__);

    LEDS_INIT();
    char state = '\0';
    
    while (1) {
        if (timerA.event == TRUE) {
            printf(" A");
            timerA.event = FALSE;
            state = state ^ 0x01;
            LEDS_SET(state);
        }
        if (timerB.event == TRUE) {
            printf(" B");
            timerB.event = FALSE;
            state = state ^ 0x02;
            LEDS_SET(state);
        }
        if (timerC.event == TRUE) {
            printf(" C");
            timerC.event = FALSE;
            state = state ^ 0x04;
            LEDS_SET(state);
        }
    }


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}

/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 prescaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;
    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    
    --timerA.timeRemaining;
    if (timerA.timeRemaining == 0) {
        timerA.event = TRUE;
        timerA.timeRemaining = TWO_SECOND;
    }
    --timerB.timeRemaining;
    if (timerB.timeRemaining == 0) {
        timerB.event = TRUE;
        timerB.timeRemaining = THREE_SECOND;
    }
    --timerC.timeRemaining;
    if (timerC.timeRemaining == 0) {
        timerC.event = TRUE;
        timerC.timeRemaining = FIVE_SECOND;
    }
    
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}