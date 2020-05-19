/**
 * Name: Hang Yuan
 * StdID: 1564348
 * CruzID: hyuan3
 */

#include <ctype.h>
#include "rpn.h"
#include "stack.h"
#include "string.h"

#define MAX_STRING_LENGTH 255
#define TRUE              1
#define FALSE             0

const char delimiter[2] = " ";

int RPN_Evaluate(char * rpn_string, double * result) {
    // Initialize a stack
    struct Stack stack = {};
    StackInit(&stack);
    
    // Obtain the valid string with the limit of 255
    char rpn[MAX_STRING_LENGTH + 1];
    memset(rpn, 0, MAX_STRING_LENGTH + 1);
    memcpy(rpn, rpn_string, strlen(rpn_string));
    
    // Split the string until its end point and do the appropriate operation 
    char *token;
    token = strtok(rpn, delimiter);
    double operand1 = 0, operand2 = 0;
    while (token != NULL) {
        // Operator +
        if (strlen(token) == 1 && token[0] == '+') {
            if (StackPop(&stack, &operand2) != SUCCESS || StackPop(&stack, &operand1) != SUCCESS) {
                return RPN_ERROR_STACK_UNDERFLOW;
            }
            *result = operand1 + operand2;
            
            // Push the result into the stack
            if (StackPush(&stack, *result) != SUCCESS) {
                return RPN_ERROR_STACK_OVERFLOW;
            }
        }
        // Operator -
        else if (strlen(token) == 1 && token[0] == '-') {
            if (StackPop(&stack, &operand2) != SUCCESS || StackPop(&stack, &operand1) != SUCCESS) {
                return RPN_ERROR_STACK_UNDERFLOW;
            }
            *result = operand1 - operand2;
            
            // Push the result into the stack
            if (StackPush(&stack, *result) != SUCCESS) {
                return RPN_ERROR_STACK_OVERFLOW;
            }
        }
        // operator *
        else if (strlen(token) == 1 && token[0] == '*') {
            if (StackPop(&stack, &operand2) != SUCCESS || StackPop(&stack, &operand1) != SUCCESS) {
                return RPN_ERROR_STACK_UNDERFLOW;
            }
            *result = operand1 * operand2;
            
            // Push the result into the stack
            if (StackPush(&stack, *result) != SUCCESS) {
                return RPN_ERROR_STACK_OVERFLOW;
            }
        }
        // Operator /
        else if (strlen(token) == 1 && token[0] == '/') {
            if (StackPop(&stack, &operand2) != SUCCESS || StackPop(&stack, &operand1) != SUCCESS) {
                return RPN_ERROR_STACK_UNDERFLOW;
            }

            // Check if the divisor is 0.0
            if (operand2 == 0.0) {
                return RPN_ERROR_DIVIDE_BY_ZERO;
            }
            *result = operand1 / operand2;
            
            // Push the result into the stack
            if (StackPush(&stack, *result) != SUCCESS) {
                return RPN_ERROR_STACK_OVERFLOW;
            }
        }
        // Convert string to double
        else {
            // Check if each char is a valid number in string 
            int index = 0;
            int dotFlag = FALSE, minusFlag = FALSE;
            for (; index < strlen(token); ++index) {
                if (isdigit(token[index])) {
                    continue;
                }
                if (token[index] == '.') {
                    if (dotFlag == FALSE) {
                        dotFlag = TRUE;
                    } else {
                        return RPN_ERROR_INVALID_TOKEN;
                    }
                } else if (token[index] == '-') { 
                    if (minusFlag == FALSE) {
                        minusFlag = TRUE;
                    } else {
                        return RPN_ERROR_INVALID_TOKEN;
                    }
                } else {
                    return RPN_ERROR_INVALID_TOKEN;
                }
            }
            
            // Convert string to double
            double number = atof(token);
            
            // Push this number into the stack
            if (StackPush(&stack, number) != SUCCESS) {
                return RPN_ERROR_STACK_OVERFLOW;
            }
        }
        token = strtok(NULL, delimiter);
    }
    
    // Check if the stack has exactly one number remaining
    if (StackGetSize(&stack) > 1) {
        return RPN_ERROR_TOO_MANY_ITEMS_REMAIN;
    } else if (StackGetSize(&stack) < 1) {
        return RPN_ERROR_TOO_FEW_ITEMS_REMAIN;
    }
    
    // Obtain the final result
    if (StackPop(&stack, result) != SUCCESS) {
        return RPN_ERROR_STACK_UNDERFLOW;
    }
    
    return RPN_NO_ERROR;
}

int ProcessBackspaces(char *rpn_sentence) {
    // Obtain the valid string with the limit of 255
    char rpn[MAX_STRING_LENGTH + 1];
    memset(rpn, 0, MAX_STRING_LENGTH + 1);
    memcpy(rpn, rpn_sentence, strlen(rpn_sentence));
    int length = strlen(rpn_sentence);
    
    // Traverse the string to eliminate the \b and the preceeding char
    int index = 0;
    for (; index < length; ++index) {
        if (rpn[index] == '\b') {
            // If the \b is the first char, only shift the following chars
            if (index == 0) {
                memcpy(rpn, rpn + 1, length);
                length--;
                index--;
            }
            // If the \b in the middle, delete \b and the preceeding char
            else{
                memcpy(rpn + index - 1, rpn + index + 1, length - index);
                rpn[strlen(rpn)] = '\0';
                length -= 2;
                index -= 2;
            }
        }
    }
    
    // Copy the final and correct result back to rpn_sentence
    memcpy(rpn_sentence, rpn, length + 1);
    
    return length;
}
