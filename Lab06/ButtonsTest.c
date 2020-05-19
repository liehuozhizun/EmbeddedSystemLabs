// Microchip libraries
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

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 2; // 1:64 prescaler
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
    printf("Welcome to hyuan3's lab6 part4 (ButtonsTest).  Compiled on %s %s.\n", __TIME__, __DATE__);
    printf("Please press some buttons!\n");
    ButtonsInit();
    
    while (1) {
        uint8_t state = ButtonsCheckEvents();
        if (state == BUTTON_EVENT_NONE) {
            continue;
        }
        
        printf ("EVENT: ");
        if (state & BUTTON_EVENT_4UP) {
            printf("4:  UP ");
        } else if (state & BUTTON_EVENT_4DOWN) {
            printf("4:DOWN ");
        } else {
            printf("4:---- ");
        }
        
        if (state & BUTTON_EVENT_3UP) {
            printf("3:  UP ");
        } else if (state & BUTTON_EVENT_3DOWN) {
            printf("3:DOWN ");
        } else {
            printf("3:---- ");
        }
        
        if (state & BUTTON_EVENT_2UP) {
            printf("2:  UP ");
        } else if (state & BUTTON_EVENT_2DOWN) {
            printf("2:DOWN ");
        } else {
            printf("2:---- ");
        }
        if (state & BUTTON_EVENT_1UP) {
            printf("1:  UP\n");
        } else if (state & BUTTON_EVENT_1DOWN) {
            printf("1:DOWN\n");
        } else {
            printf("1:----\n");
        }
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


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}