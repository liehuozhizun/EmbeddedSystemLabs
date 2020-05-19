/**
 * Name: Hang Yuan
 * StdID: 1564348
 * CruzID: hyuan3
 */

#include "stack.h"

#define TRUE    1
#define FALSE   0


void StackInit(struct Stack *stack) {
    stack->currentItemIndex = -1;
    stack->initialized = TRUE;
}

int StackPush(struct Stack *stack, double value) {
    int index = stack->currentItemIndex + 1;
    if (StackIsFull(stack) == TRUE || stack->initialized != TRUE) {
        return STANDARD_ERROR;
    }
    
    stack->stackItems[index] = value;
    stack->currentItemIndex++;
    
    return SUCCESS;
}

int StackPop(struct Stack *stack, double *value) {
    int index = stack->currentItemIndex;
    if (StackIsEmpty(stack) == TRUE || stack->initialized != TRUE) {
        return STANDARD_ERROR;
    }
    
    *value = stack->stackItems[index];
    stack->currentItemIndex = index - 1;
    
    return SUCCESS;
}

int StackIsEmpty(const struct Stack *stack) {
    int index = stack->currentItemIndex;
    if (stack->initialized != TRUE) {
        return FALSE;
    }
    if (index <= -1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int StackIsFull(const struct Stack *stack) {
    int index = stack->currentItemIndex;
    if (index >= STACK_SIZE - 1 && stack->initialized == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int StackGetSize(const struct Stack *stack) {
    int index = stack->currentItemIndex + 1;
    if (stack->initialized != TRUE) {
        return SIZE_ERROR;
    } else {
        return index;
    }
}