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
#include "Oled.h"
#include "OledDriver.h"

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****
struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

struct AdcResult {
    uint8_t  event;
    uint16_t voltage;
};

struct SwitchState {
    uint8_t sw_1;
    uint8_t sw_2;
    uint8_t sw_3;
    uint8_t sw_4;
};

struct LedFreeze {
    uint8_t led_12;
    uint8_t led_34;
    uint8_t led_56;
    uint8_t led_78;
};

// **** Define global, module-level, or external variables here ****
#define DOWN         0
#define UP           1
#define TIMER_SECOND 25
#define SIZE         5
#define HIGH_VALUE   1023
#define LOW_VALUE    0
#define HIGH_VOLTAGE 1021
#define LOW_VOLTAGE  3
#define DIVIDE       10.23

enum {
    LEFT,
    RIGHT,
};

static struct Timer timer = {DOWN, TIMER_SECOND};
static struct SwitchState sw = {DOWN, DOWN, DOWN, DOWN};
static struct AdcResult adc = {DOWN, DOWN};
static struct LedFreeze led = {DOWN, DOWN, DOWN, DOWN};
static double prev = 0;
static double curr = 0;
static double percentage = 0;

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


    // Enable interrupts for the ADC
    IPC6bits.AD1IP = 2;
    IPC6bits.AD1IS = 0;
    IEC1bits.AD1IE = 1;

    // Set B2 to an input so AN2 can be used by the ADC.
    TRISBbits.TRISB2 = 1;

    // Configure and start the ADC
    AD1CHSbits.CH0SA = 2; // add B2 to the mux
    AD1PCFGbits.PCFG2 = 0; // add b2 to the ADC
    AD1CSSLbits.CSSL2 = 1; // and add b2 to the scanner

    AD1CON1 = 0; // start with 0
    AD1CON1bits.SSRC = 0b111; // auto conversion
    AD1CON1bits.ASAM = 1; // auto sample

    AD1CON2 = 0; // start with 0
    AD1CON2bits.SMPI = 7; // one interrupt per 8 samples

    AD1CON3 = 0; // start with 0
    AD1CON3bits.SAMC = 29; // long sample time
    AD1CON3bits.ADCS = 50; // long conversion time

    AD1CON1bits.ADON = 1; // and finally turn it on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    LEDS_INIT();
    ButtonsInit();
    OledInit();
    int led_state = '\1';
    
    printf("Welcome to hyuan3's lab6 part6 (Extra Credit).  Compiled on %s %s.\n",__TIME__,__DATE__);
    char string[100];
    int direction = LEFT;
    
    while (1) {
        // Update OLED with pot changing
        if (adc.event == UP) {
            adc.event = DOWN;
            percentage = adc.voltage / DIVIDE;
            sprintf(string, "Potentiometer value:\n  %d\n  %d%%", adc.voltage, (int)percentage);
            OledClear(OLED_COLOR_BLACK);
            OledDrawString(string);
            OledUpdate();
        }
        
        // Control the freeze of LEDs
        uint8_t btn_state = ButtonsCheckEvents();
        if (sw.sw_1 == DOWN) {
            if (btn_state & BUTTON_EVENT_1DOWN) {
                (led.led_12 == DOWN)?(led.led_12 = UP):(led.led_12 = DOWN);
            }
        } else {
            if (btn_state & BUTTON_EVENT_1UP) {
                (led.led_12 == DOWN)?(led.led_12 = UP):(led.led_12 = DOWN);
            }
        }
        if (sw.sw_2 == DOWN) {
            if (btn_state & BUTTON_EVENT_2DOWN) {
                (led.led_34 == DOWN)?(led.led_34 = UP):(led.led_34 = DOWN);
            }
        } else {
            if (btn_state & BUTTON_EVENT_2UP) {
                (led.led_34 == DOWN)?(led.led_34 = UP):(led.led_34 = DOWN);
            }
        }
        if (sw.sw_3 == DOWN) {
            if (btn_state & BUTTON_EVENT_3DOWN) {
                (led.led_56 == DOWN)?(led.led_56 = UP):(led.led_56 = DOWN);
            }
        } else {
            if (btn_state & BUTTON_EVENT_3UP) {
                (led.led_56 == DOWN)?(led.led_56 = UP):(led.led_56 = DOWN);
            }
        }
        if (sw.sw_4 == DOWN) {
            if (btn_state & BUTTON_EVENT_4DOWN) {
                (led.led_78 == DOWN)?(led.led_78 = UP):(led.led_78 = DOWN);
            }
        } else {
            if (btn_state & BUTTON_EVENT_4UP) {
                (led.led_78 == DOWN)?(led.led_78 = UP):(led.led_78 = DOWN);
            }
        }
        
        // Control the LED light change direction
        if (timer.event == UP) {
            timer.event = DOWN;
            
            // The most left LED is on
            if (led_state & 0b10000000) {
                direction = RIGHT;
                led_state = 0b01000000;
                LEDS_SET(led_state);
                continue;
            }
            
            // The most right LED is on
            if (led_state & 0b00000001) {
                direction = LEFT;
            }
            
            if (direction == LEFT) {
                led_state <<= 1;
            } else {
                led_state >>= 1;
            }
        }
        
        // Freeze the LEDs specified
        uint8_t real_led = led_state;
        if (led.led_12 == UP) {
            real_led |= 0b00000011;
        }
        if (led.led_34 == UP) {
            real_led |= 0b00001100;
        }
        if (led.led_56 == UP) {
            real_led |= 0b00110000;
        }
        if (led.led_78 == UP) {
            real_led |= 0b11000000;
        }
        LEDS_SET(real_led);
    }
    
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;
    
    // Control the LED speed
    --timer.timeRemaining;
    if (timer.timeRemaining == 0) {
        timer.event = UP;
        timer.timeRemaining = TIMER_SECOND - (int)(percentage/100 * TIMER_SECOND) + 1;
    }
    
    // Detect the state of SWITCH
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
}

/**
 * This is the ISR for the ADC1 peripheral.
 */
void __ISR(_ADC_VECTOR, IPL2AUTO) AdcHandler(void)
{
    // Clear the interrupt flag.
    IFS1bits.AD1IF = 0;

    curr = ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3 + ADC1BUF4 + ADC1BUF5 + ADC1BUF6 + ADC1BUF7;
    curr /= 8;
    if ((curr - prev) >= SIZE || (prev - curr) >= SIZE) {
        adc.event = UP;
        prev = curr;
        adc.voltage = (uint16_t)curr;
        if (curr >= HIGH_VOLTAGE) {
            adc.voltage = HIGH_VALUE;
        } else if (curr <= LOW_VOLTAGE) {
            adc.voltage = LOW_VALUE;
        }
    }
}