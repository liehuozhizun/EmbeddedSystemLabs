/*
 * Message.c
 * Author: Hang Yuan (hyuan3, 1564348)
 * Partner: Yujia Li (yli302, 1538297)
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
#include "Negotiation.h"

#define BIT_DIGIT 2
#define EVEN 0
#define ODD 1
#define ZERO 0

NegotiationData NegotiationHash(NegotiationData secret)
{
    uint32_t commitment = secret * secret;
    return (commitment % PUBLIC_KEY);
}

int NegotiationVerify(NegotiationData secret, NegotiationData commitment)
{
    return (commitment == NegotiationHash(secret));
}

NegotiationOutcome NegotiateCoinFlip(NegotiationData A, NegotiationData B)
{
    NegotiationData A_result = ZERO;
    NegotiationData B_result = ZERO;
    NegotiationOutcome resultOut;
    while (A != ZERO) {
        A_result += A % BIT_DIGIT;
        A = A / BIT_DIGIT;
    }

    while (B != ZERO) {
        B_result += B % BIT_DIGIT;
        B = B / BIT_DIGIT;
    }

    if (A_result % BIT_DIGIT == EVEN) {
        A_result = EVEN;
    } else {
        A_result = ODD;
    }

    if (B_result % BIT_DIGIT == EVEN) {
        B_result = EVEN;
    } else {
        B_result = ODD;
    }

    if ((A_result ^ B_result) == ODD) {
        resultOut = HEADS;
    } else {
        resultOut = TAILS;
    }

    return resultOut;
}

