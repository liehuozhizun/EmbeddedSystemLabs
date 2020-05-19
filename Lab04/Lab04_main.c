/**
 * Name: Hang Yuan
 * StdID: 1564348
 * CruzID: hyuan3
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include "rpn.h"

// **** Set macros and preprocessor directives ****
// The lab calls for 60 characters of user input
#define MAX_INPUT_LENGTH 60

int main() {
    BOARD_Init();

    char rpn_sentence[MAX_INPUT_LENGTH + 2];
    double result;

    // Print the prompts
    printf("Welcome to hyuan3's RPN calculator.  Compiled on %s %s", __DATE__, __TIME__);
    printf("\nEnter floats and + - / * in RPN format:\n");
    
    while (1) {
        memset(rpn_sentence, 0, MAX_INPUT_LENGTH + 2);
        printf("> ");
        
        // Read string from stdin
        fgets(rpn_sentence, sizeof(rpn_sentence), stdin);
        rpn_sentence[strlen(rpn_sentence) - 1] = '\0';
        fflush(stdin);
        if (strlen(rpn_sentence) == 0) {
            continue;
        }
        
        // Eliminate backspaces
        ProcessBackspaces(rpn_sentence);
        
        // Do the RPN calculation
        int status = RPN_Evaluate(rpn_sentence, &result);
        
        // Print the result or the corresponding error message
        switch (status) {
        case RPN_NO_ERROR:
            printf("Result = %f\n", result);
            break;
        case RPN_ERROR_STACK_OVERFLOW:
            printf("ERROR: No more room on stack.\n");
            break;
        case RPN_ERROR_STACK_UNDERFLOW:
            printf("ERROR: Not enough operand(s) before operator.\n");
            break;
        case RPN_ERROR_INVALID_TOKEN:
            printf("ERROR: Invalid character(s) in RPN string.\n");
            break;
        case RPN_ERROR_DIVIDE_BY_ZERO:
            printf("ERROR: Divide by zero.\n");
            break;
        case RPN_ERROR_TOO_FEW_ITEMS_REMAIN:
            printf("ERROR: Invalid RPN calculation: less than one item in stack.\n");
            break;
        case RPN_ERROR_TOO_MANY_ITEMS_REMAIN:
            printf("ERROR: Invalid RPN calculation: more than one item in stack.\n");
            break;
        default:
            printf("ERROR: Unknown error occurred.\n");
            break;
        }
        
    }

    while (1);
}
