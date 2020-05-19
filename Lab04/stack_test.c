/**
 * Name: Hang Yuan
 * StdID: 1564348
 * CruzID: hyuan3
 */

// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// C standard libraries
#include <stdio.h>

// User libraries
#include "stack.h"

// Test purpose defines
#define TRUE 1

int main()
{
    BOARD_Init();

    printf("\n###### Beginning hyuan3's stack test harness: ####\n\n");
    int passedNumber = 0, failedNumber = 0;

    // Test stackInit():
    printf("Does StackInit() set the currentItemIndex appropriately?\n"); // # 1
    struct Stack stack = {};
    StackInit(&stack);
    if (stack.currentItemIndex == -1) {
        printf("passed\n");
        passedNumber++;
    } else {
        printf("failed\n");
        failedNumber++;
    }
    
    printf("Does StackInit() set the initialized appropriately?\n");     // # 2
    struct Stack empty = {};
    StackInit(&empty);
    if (stack.initialized == TRUE) {
        printf("passed\n\n");
        passedNumber++;
    } else {
        printf("failed\n\n");
        failedNumber++;
    }
    
    // Test StackPush():
    printf("Does StackPush() successfully push number 1.2345 ?\n");     // # 1
    if (StackPush(&stack, 1.2345) == SUCCESS && stack.stackItems[0] == 1.2345) {
       printf("passed\n");
        passedNumber++;
    } else {
        printf("failed\n");
        failedNumber++;
    }
    
    printf("Does StackPush() return false when stack is full ?\n");     // # 2
    int index = 1;
    for (; index < 20; ++index) {
        StackPush(&stack, (double)index);
    }
    if (StackPush(&stack, 21.0) == SUCCESS) {
        printf("failed\n\n");
        failedNumber++;
    } else {
        printf("passed\n\n");
        passedNumber++;
    }
    
    // Test StackPop():
    printf("Does StackPop() successfully pop number 19 ?\n");           // # 1
    double valueReturned = 0;
    if (StackPop(&stack, &valueReturned) == SUCCESS && valueReturned == 19) {
        printf("passed\n");
        passedNumber++;
    } else {
        printf("failed - returned value : %f\n", valueReturned);
        failedNumber++;
    }
    
    printf("Does StackPop() return false when stack is empty ?\n");     // # 2
    if (StackPop(&empty, &valueReturned) == SUCCESS) {
        printf("failed\n\n");
        failedNumber++;
    } else {
        printf("passed\n\n");
        passedNumber++;
    }
    
    // Test StackIsEmpty():
    printf("Does StackIsEmpty() successfully detects non-empty stack ?\n");  // # 1
    if (StackIsEmpty(&stack) != TRUE) {
        printf("passed\n");
        passedNumber++;
    } else {
        printf("failed\n");
        failedNumber++;
    }
    
    printf("Does StackIsEmpty() successfully detects empty stack ?\n");      // # 2
    if (StackIsEmpty(&empty) == TRUE) {
        printf("passed\n\n");
        passedNumber++;
    } else {
        printf("failed\n\n");
        failedNumber++;
    }
    
    // Test StackIsFull():
    StackPush(&stack, 19);
    printf("Does StackIsFull() successfully detects non-empty stack ?\n");     // # 1
    if (StackIsFull(&stack) == TRUE) {
        printf("passed\n");
        passedNumber++;
    } else {
        printf("failed\n");
        failedNumber++;
    }
    
    printf("Does StackIsFull() successfully detects empty stack ?\n");     // # 2
    if (StackIsFull(&empty) != TRUE) {
        printf("passed\n\n");
        passedNumber++;
    } else {
        printf("failed\n\n");
        failedNumber++;
    }
    
    // Test StackGetSize():
    printf("Does StackGetSize() successfully returns the size 20 ?\n");     // # 1
    int size = 0;
    if ((size = StackGetSize(&stack)) == 20) {
        printf("passed\n");
        passedNumber++;
    } else {
        printf("failed - returned number : %d\n", size);
        failedNumber++;
    }
    
    printf("Does StackGetSize() successfully returns the size 0 ?\n");     // # 2
    if ((size = StackGetSize(&empty)) == 0) {
        printf("passed\n\n");
        passedNumber++;
    } else {
        printf("failed - returned number : %d\n", size);
        failedNumber++;
    }
    
    // Print test report
    printf("\n------------------------------------------------\nBrief Test Report:\n");
    printf("   passed: %d\n   failed: %d\n", passedNumber, failedNumber);
    
    while (1);
}


