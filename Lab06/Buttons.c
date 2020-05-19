/*
 * Name  : Hang Yuan
 * StdID : 1564348
 * CruzID: hyuan3
 */

// **** Include libraries here ****
//CMPE13 Support Library
#include "BOARD.h"
#include <stdio.h>

// User libraries
#include "Buttons.h"
#include "Oled.h"
#include "OledDriver.h"

// **** Define global, module-level, or external variables here ****
static uint8_t prev_state;
static uint8_t count_1, count_2, count_3, count_4;

void ButtonsInit() {
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
}

uint8_t ButtonsCheckEvents() {
    uint8_t state  = BUTTON_STATES();
    uint8_t result = 0, temp = 0;
    if (prev_state == state) {
        return BUTTON_EVENT_NONE;
    } else {
        temp = state ^ prev_state;
    }
    
    // Decrease debounce count for each button
    if (count_1 > 0) {
        --count_1;
    }
    if (count_2 > 0) {
        --count_2;
    }
    if (count_3 > 0) {
        --count_3;
    }
    if (count_4 > 0) {
        --count_4;
    }
    
    // First button changed
    if (count_1 <= 0 && (temp & BUTTON_STATE_1)) {
        if (state & BUTTON_STATE_1) {
            result |= BUTTON_EVENT_1DOWN;
            prev_state |= BUTTON_STATE_1;
        } else {
            result |= BUTTON_EVENT_1UP;
            prev_state &= 0b1110;
        }
        count_1 = BUTTONS_DEBOUNCE_PERIOD;
    }
    
    // Second button changed
    if (count_2 <= 0 && (temp & BUTTON_STATE_2)) {
        if (state & BUTTON_STATE_2) {
            result |= BUTTON_EVENT_2DOWN;
            prev_state |= BUTTON_STATE_2;
        } else {
            result |= BUTTON_EVENT_2UP;
            prev_state &= 0b1101;
        }
        count_2 = BUTTONS_DEBOUNCE_PERIOD;
    }
    
    // Third button changed
    if (count_3 <= 0 && (temp & BUTTON_STATE_3)) {
        if (state & BUTTON_STATE_3) {
            result |= BUTTON_EVENT_3DOWN;
            prev_state |= BUTTON_STATE_3;
        } else {
            result |= BUTTON_EVENT_3UP;
            prev_state &= 0b1011;
        }
        count_3 = BUTTONS_DEBOUNCE_PERIOD;
    }
    
    // Fourth button changed
    if (count_4 <= 0 && (temp & BUTTON_STATE_4)) {
        if (state & BUTTON_STATE_4) {
            result |= BUTTON_EVENT_4DOWN;
            prev_state |= BUTTON_STATE_4;
        } else {
            result |= BUTTON_EVENT_4UP;
            prev_state &= 0b0111;
        }
        count_4 = BUTTONS_DEBOUNCE_PERIOD;
    }
    
    return result;
}