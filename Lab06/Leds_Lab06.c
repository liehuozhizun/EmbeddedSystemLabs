/*
 * Name  : Hang Yuan
 * StdID : 1564348
 * CruzID: hyuan3
 */

// **** Include libraries here ****

// Microchip libraries
#include <xc.h>

// User libraries
#include "Leds_Lab06.h"

void LEDS_INIT() {
    TRISE = 0x00;
    LATE  = 0x00;
}

void LEDS_SET(char newPattern) {
    LATE = newPattern;
}

char LEDS_GET() {
    char state = '\0';
    state = state | LATEbits.LATE7;
    state = (state << 1) | LATEbits.LATE6;
    state = (state << 1) | LATEbits.LATE5;
    state = (state << 1) | LATEbits.LATE4;
    state = (state << 1) | LATEbits.LATE3;
    state = (state << 1) | LATEbits.LATE2;
    state = (state << 1) | LATEbits.LATE1;
    state = (state << 1) | LATEbits.LATE0;
    return state;
}
