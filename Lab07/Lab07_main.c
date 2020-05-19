/*
 * Name: Hang Yuan
 * CruzID: hyuan3
 * StdID: 1564348
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Adc.h" 
#include "Ascii.h"
#include "Buttons.h"
#include "Leds.h"
#include "Oled.h"

// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)


// **** Set any local typedefs here ****
typedef enum {
    SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING, ALERT
} OvenState;

typedef enum {
    BAKE, TOAST, BROIL
} Mode;

typedef enum {
    TIME, TEMP
} Selector;

// Define type of ovenData
typedef struct {
    Mode mode;                  // Mode: BAKE, TOAST, BROIL
    OvenState state;            // OvenState: SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING, ALERT
    uint16_t temperature;       // In number (300-550)
    uint16_t button_press_time; // The freeRunningTime when button is pushed
    uint16_t initial_time;      // The initial time set up before cooking (1-256)
    uint16_t remaining_time;    // Remaining seconds for COOKING state (1-256)
    Selector selector;          // Selector: TIME, TEMP, NONE
} OvenData;

// Define event
typedef struct {
    uint8_t adc;
    uint8_t btn_3_up;
    uint8_t btn_3_down;
    uint8_t btn_4_up;
    uint8_t btn_4_down;
    uint8_t timer;
    uint8_t alert_state;
} Event;

// **** Define any module-level, global, or external variables here ****
#define DOWN          0
#define UP            1
#define LONG_PRESS    5
#define BASE_TEMP     300  // TEMP range : 300-555
#define DEFAULT_TEMP  350  // BAKE default TEMP
#define TICK_PER_SEC  5
#define SECOND_DIV    60   // 1 minute = 60 seconds
#define OLED_MAX_SIZE 71   // OLED buffer max length
#define OLED_TOP_LEN  15   // Length of OLED top line
#define OLED_BOT_LEN  8    // Length of OLED bottom line
#define OLED_STD_LEN  21   // Standard OLED line length
#define OLED_TIME_SEL 29   // Position where selector > should be in the TIME line
#define OLED_TEMP_SEL 50   // Position where selector > should be in the TEMP line

// **** Declare any datatypes here ****
static OvenData ovenData = {BAKE, SETUP, DEFAULT_TEMP, LONG_PRESS, UP, UP, TIME};
static Event event = {DOWN, DOWN, DOWN, DOWN, DOWN};
static uint16_t freeRunningCounter = 0;
static uint8_t timerTick = TICK_PER_SEC;

// **** Define const string here ****
const char *TOP_ON  = "|" OVEN_TOP_ON OVEN_TOP_ON OVEN_TOP_ON OVEN_TOP_ON OVEN_TOP_ON "|  Mode: ";
const char *TOP_OFF = "|" OVEN_TOP_OFF OVEN_TOP_OFF OVEN_TOP_OFF OVEN_TOP_OFF OVEN_TOP_OFF \
                        "|  Mode: ";
const char *BOTTOM_ON = "|" OVEN_BOTTOM_ON OVEN_BOTTOM_ON OVEN_BOTTOM_ON OVEN_BOTTOM_ON \
                            OVEN_BOTTOM_ON "|\0";
const char *BOTTOM_OFF = "|" OVEN_BOTTOM_OFF OVEN_BOTTOM_OFF OVEN_BOTTOM_OFF \
                            OVEN_BOTTOM_OFF OVEN_BOTTOM_OFF "|\0";
const char *BROIL_DEFAULT = "|-----|  Temp: 500" DEGREE_SYMBOL "F\n";
const char *EMPTY_TEMP = "|-----|             \n";
const char *TIME_BLOCK = "|     |  Time:  ";
const char *TEMP_BLOCK = "|-----|  Temp: ";

/*This function will update your OLED to reflect the state .*/
void updateOvenOLED(OvenData ovenData){
    
    //update OLED here
    char s[OLED_MAX_SIZE];
    size_t offset = OLED_TOP_LEN;
    
    // First Line: Mode
    if ((ovenData.state == COOKING || ovenData.state == RESET_PENDING) && ovenData.mode != TOAST) {
        memcpy(s, TOP_ON, offset);
    } else {
        memcpy(s, TOP_OFF, offset);
    }

    if (ovenData.mode == BAKE) {
        memcpy(s + offset, "Bake \n", 6);
    } else if (ovenData.mode == TOAST) {
        memcpy(s + offset, "Toast\n", 6);
    } else if (ovenData.mode == BROIL) {
        memcpy(s + offset, "Broil\n", 6);
    }
    offset = OLED_STD_LEN;
    
    // Second Line: Time
    memcpy(s + offset, TIME_BLOCK, 16);
    offset += 16;
    if (ovenData.state == COOKING || ovenData.state == RESET_PENDING) {
        sprintf(s + offset,"%d:%02d\n", 
                ovenData.remaining_time / SECOND_DIV, ovenData.remaining_time % SECOND_DIV);
    } else {
        sprintf(s + offset,"%d:%02d\n", 
                ovenData.initial_time / SECOND_DIV, ovenData.initial_time % SECOND_DIV);
        if (ovenData.mode == BAKE && ovenData.selector == TIME) {
            s[OLED_TIME_SEL] = '>';
        }
    }
    offset = OLED_STD_LEN + OLED_STD_LEN;
    
    // Third Line: Temp
    if (ovenData.mode == BAKE) {
        memcpy(s + offset, TEMP_BLOCK, 15);
        offset += 15;
        sprintf(s + offset,"%d%sF\n", ovenData.temperature, DEGREE_SYMBOL);
        if (ovenData.selector == TEMP) {
            s[OLED_TEMP_SEL] = '>';
        }
    } else if (ovenData.mode == BROIL) {
        memcpy(s + offset, BROIL_DEFAULT, OLED_STD_LEN);
    } else {
        memcpy(s + offset, EMPTY_TEMP, OLED_STD_LEN);
    }
    offset = OLED_STD_LEN + OLED_STD_LEN + OLED_STD_LEN;
    
    // Forth Line: none
    if ((ovenData.state == COOKING || ovenData.state == RESET_PENDING)&& ovenData.mode != BROIL) {
        memcpy(s + offset, BOTTOM_ON, OLED_BOT_LEN);
    } else {
        memcpy(s + offset, BOTTOM_OFF, OLED_BOT_LEN);
    }
    OledClear(OLED_COLOR_BLACK);
    OledDrawString(s);
    OledUpdate();
}

/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void)
{
    //write your SM logic here.
    switch (ovenData.state){
    case SETUP:
        // ADC_CHANGED: update TIME/TEMP; update OLED
        if (event.adc) {
            event.adc = DOWN;
            uint16_t adc_state = AdcRead();
            adc_state >>= 2;
            if (ovenData.mode != BAKE || ovenData.selector == TIME) {
                ovenData.initial_time = adc_state + 1;
            } else {
                ovenData.temperature = adc_state + BASE_TEMP;
            }
        }
        
        // BTN_EVENT_3DOWN: change state to SELECTOR_CHANGE_PENDING; store freeRunningCounter
        if (event.btn_3_down) {
            event.btn_3_down = DOWN;
            ovenData.button_press_time = freeRunningCounter;
            ovenData.state = SELECTOR_CHANGE_PENDING;
        }
        
        // BTN_EVENT_4DOWN: change state to COOKIING
        if (event.btn_4_down) {
            event.btn_4_down = DOWN;
            ovenData.state = COOKING;
            ovenData.remaining_time = ovenData.initial_time;
            LEDS_SET(0b11111111);
        }
        updateOvenOLED(ovenData);
        break;
    case SELECTOR_CHANGE_PENDING:
        // DECIDE: elapsed time < LOND_PRESS ? swtich mode : change selector
        if (event.btn_3_up) {
            event.btn_3_up = DOWN;
            if ((freeRunningCounter - ovenData.button_press_time) < LONG_PRESS) {
                if (ovenData.mode == BROIL) {
                    ovenData.mode = BAKE;
                    ovenData.temperature = DEFAULT_TEMP;
                } else {
                    ++ovenData.mode;
                }
                ovenData.selector = TIME;
            } else if (ovenData.mode == BAKE) {
                (ovenData.selector == TIME)? (ovenData.selector = TEMP):(ovenData.selector = TIME);
                uint16_t adc_state = AdcRead();
                adc_state >>= 2;
                if (ovenData.selector == TIME) {
                    ovenData.initial_time = adc_state + 1;
                } else {
                    ovenData.temperature = adc_state + BASE_TEMP;
                }
            }
            ovenData.state = SETUP;
            updateOvenOLED(ovenData);
        }
        break;
    case COOKING:
        // TIMER_TICK: reduce remaining_time > 0 ? update LED & OLED : reset settings
        if (event.timer){
            event.timer = DOWN;
            if (--ovenData.remaining_time > 0) {
                uint8_t shift = 8 - ((ovenData.remaining_time * 8) / ovenData.initial_time);
                LEDS_SET(0b11111111 << shift);                
            } else {
                LEDS_SET(0);
                updateOvenOLED(ovenData);
                ovenData.state = ALERT;
                break;
            }
        }
        event.btn_4_up = DOWN;
        
        // BTN_EVENT_4DOWN: change state to RESET_PENDING; store freeRunningCounter
        if (event.btn_4_down) {
            event.btn_4_down = DOWN;
            ovenData.state = RESET_PENDING;
            ovenData.button_press_time = freeRunningCounter;
        }
        updateOvenOLED(ovenData);
        break;
    case RESET_PENDING:
        // BTN_EVENT_4UP: change state to COOKING
        if (event.btn_4_up) {
            event.btn_4_up = DOWN;
            ovenData.state = COOKING;
            break;
        }
        
        // TIMER_TICK: elapsed time >= LONG_PRESS ? end COOKING, rest settings : update LED/OLED
        if (event.timer) {
            event.timer = DOWN;
            if ((freeRunningCounter - ovenData.button_press_time) >= LONG_PRESS) {
                ovenData.state = SETUP;
                LEDS_SET(0);
                updateOvenOLED(ovenData);
            } else {
                if (--ovenData.remaining_time > 0) {
                    uint8_t shift = 8 - ((ovenData.remaining_time * 8) / ovenData.initial_time);
                    LEDS_SET(0b11111111 << shift);
                    updateOvenOLED(ovenData);
                } else {
                    LEDS_SET(0);
                    updateOvenOLED(ovenData);
                    ovenData.state = ALERT;
                }
            }
        }
        break;
    case ALERT:
        // TIMER_TICK: 1 second to blink the screen
        if (event.timer) {
            event.timer = DOWN;
            if (event.alert_state) {
                OledSetDisplayNormal();
                event.alert_state = DOWN;
            } else {
                OledSetDisplayInverted();
                event.alert_state = UP;
            }
        }
        
        // BTN_EVENT_4DOWN: Exit ALERT state and back to SETUP state
        if (event.btn_4_down) {
            event.btn_4_down = DOWN;
            event.alert_state = DOWN;
            ovenData.state = SETUP;
            OledSetDisplayNormal();
            updateOvenOLED(ovenData);
        }
        break;
    default:
        break;
    }
}


int main()
{
    BOARD_Init();

     //initalize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">
    
    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>
   
    printf("Welcome to hyuan3's Lab07 (Toaster Oven).  Compiled on %s %s.\n", __TIME__, __DATE__);
    
    //initialize state machine (and anything else you need to init) here
    LEDS_INIT();
    ButtonsInit();
    AdcInit();
    OledInit();

    updateOvenOLED(ovenData);
    
    while (1){
        // Add main loop code here:
        // check for events
        // on event, run runOvenSM()
        // clear event flags

        // Go into different state to check event flags
        switch (ovenData.state) {
        case SETUP:
            if (event.adc == UP || event.btn_3_down == UP || event.btn_4_down == UP) {
                runOvenSM();
            }
            break;
        case SELECTOR_CHANGE_PENDING:
            if (event.btn_3_up == UP) {
                runOvenSM();
            }
            break;
        case COOKING:
            if (event.timer == UP || event.btn_4_down == UP) {
                runOvenSM();
            }
            break;
        case RESET_PENDING:
            if (event.btn_4_up == UP || event.timer == UP) {
                runOvenSM();
            }
            break;
        case ALERT:
            runOvenSM();
            break;
        }
    }
    
    while(1);
}


/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;
    

    //add event-checking code here
    ++freeRunningCounter;

    // Reduce remaining_time for cooking 
    if (--timerTick == 0) {
        timerTick = TICK_PER_SEC;
        event.timer = UP;
    }
}


/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //add event-checking code here
    
    // Checking ADC event
    if (AdcChanged() == UP) {
        event.adc = UP;
    }
    
    // Checking BUTTON event
    else {
        uint8_t btn_state = ButtonsCheckEvents();
        if (btn_state & BUTTON_EVENT_3DOWN) {
            event.btn_3_down = UP;
        } else if (btn_state & BUTTON_EVENT_3UP) {
            event.btn_3_up = UP;
        } else if (btn_state & BUTTON_EVENT_4DOWN) {
            event.btn_4_down = UP;
        } else if (btn_state & BUTTON_EVENT_4UP) {
            event.btn_4_up = UP;
        }
    }
}