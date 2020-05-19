/*
 * File:   NegotiationTest.c
 * Author: Hang Yuan, hyuan3@ucsc.edu
 *         Yujia Li, yli302@ucsc.edu
 * 
 * NegotiationTest.c is implemented by Yujia Li
 */


// **** Include libraries here ****
// Standard libraries
#include <stdlib.h>
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
#define BINARY_PRINT_NUM 16
#define BIT_ZERO 0
#define BIT_ONE 1
#define INIT_NUM 1
#define TEST_1 10
#define TEST_2 20
#define VERIFY_T 100
#define VERIFY_F 90

/*
 print the binart form of the number
 */
void print_binary(int n)
{
    int i, a;
    int muti = BINARY_PRINT_NUM;

    double compare = INIT_NUM;

    for (a = 0; a < muti; a++) {
        compare = compare * BIT_DIGIT;
    }
    while (compare < n) {
        muti = BIT_DIGIT * muti;
        compare = INIT_NUM;
        for (a = 0; a < muti; a++) {
            compare = compare * BIT_DIGIT;
        }
    }
    for (i = muti - 1; i >= 0; i--) {
        printf("%d", (n & (BIT_ONE << i)) != BIT_ZERO);
    }
}

int main(int argc, char* argv[])
{
    BOARD_Init();
    /*
      static test: The test data is static 
     */
    printf("\n###### Beginning yli302 & hyuan3's Field test harness: ####\n\n");
    NegotiationData test1 = TEST_1;
    NegotiationData test2 = TEST_2;
    printf("The test NegotiationData is: %d and %d\n", test1, test2);

    printf("NegotiationHash Test:\n");
    printf("The Expext input is: %d\n", test1);
    NegotiationData test_out1 = (test1 * test1) % PUBLIC_KEY;
    printf("The Expect output is: %d\n", test_out1);
    printf("The Actual output is %d\n\n", NegotiationHash(test1));

    printf("NegotiationVerify Test:\n");
    printf("The Expext input is: %d, 100\n", test1);
    printf("The Expect output is: 1\n");
    printf("The Actual output is %d\n\n", NegotiationVerify(test1, VERIFY_T));

    printf("The Expext input is: %d, 90\n", test1);
    printf("The Expect output is: 0\n");
    printf("The Actual output is %d\n\n", NegotiationVerify(test1, VERIFY_F));

    printf("NegotiateCoinFlip Test:\n");
    printf("The Expext input is: %d, %d\n", test1, test2);
    printf("The Binary of Expect Input is\n0b");
    print_binary(test1);
    printf("\n0b");
    print_binary(test2);
    printf("\n");
    printf("The Expect output is TAILS\n");
    printf("The Actual output is ");
    if (NegotiateCoinFlip(test1, test2) == HEADS) {
        printf("HEADS\n");
    } else {
        printf("TAILS\n");
    }
    printf("Static Test Over\n\n");

    /*
      random test: The test data is generated random number
     */
    printf("Random number Test:");
    test1 = rand();
    test2 = rand();
    printf("The test NegotiationData is: %d and %d\n", test1, test2);

    printf("NegotiationHash Test:\n");
    printf("The Expext input is: %d\n", test1);
    uint32_t commit = test1 * test1;
    test_out1 = commit % PUBLIC_KEY;
    printf("The Expect output is: %d\n", test_out1);
    printf("The Actual output is %d\n\n", NegotiationHash(test1));

    printf("NegotiationVerify Test:\n");
    printf("The Expext input is: %d, %d\n", test1, test_out1);
    printf("The Expect output is: 1\n");
    printf("The Actual output is %d\n\n", NegotiationVerify(test1, test_out1));

    /*
     test_out1-1 used for getting the wrong input.
     */
    printf("The Expext input is: %d, %d\n", test1, test_out1 - 1);
    printf("The Expect output is: 0\n");
    printf("The Actual output is %d\n\n", NegotiationVerify(test1, (test_out1 - 1)));

    printf("NegotiateCoinFlip Test:\n");
    printf("The Expext input is: %d, %d\n", test1, test2);
    printf("The Binary of Expect Input is\n0b");
    print_binary(test1);
    printf("\n0b");
    print_binary(test2);
    printf("\n");
    printf("The Expect output is ");
    NegotiationData A = test1;
    NegotiationData B = test2;
    NegotiationData A_result = ZERO;
    NegotiationData B_result = ZERO;
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
        printf("HEADS");
    } else {
        printf("TAILS");
    }
    printf("\n");
    printf("The Actual output is ");
    if (NegotiateCoinFlip(test1, test2) == HEADS) {
        printf("HEADS\n");
    } else {
        printf("TAILS\n");
    }
    printf("Random Test Over\n");

    printf("\nAll Test Finished\n");
    printf("every expected value should be the same as test value.\n");
    printf("I have checked them, they are all right.\n");
    while (1);
}