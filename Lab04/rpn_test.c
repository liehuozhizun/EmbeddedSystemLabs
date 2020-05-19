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

// Helper functions
void ValidTestHelper (int num, char * test, double expected);
void InvalidTestHelper (int num, char * test);
void ProcessBackspacesTest (int num, char * test, char * expected);

int passNumber = 0, failNumber = 0;
double result = 0.0;

int main()
{
    BOARD_Init();

    printf("\n###### Beginning hyuan3's rpn test harness: ####\n\n");

    // Valid RPN calculation tests (should have no errors)
    char test0[] = "1 1 +";
    char test1[] = "1 1 -";
    char test2[] = "1 5 *";
    char test3[] = "1 2 /";
    char test4[] = "1 1 1 1 1  1 1 + + - * / -";
    char test5[] = "4 65 1 * - 156 2 / +";
    char test6[] = "465.156 1.2 - 165.5 0 * - 5 3 + -   ";
    char test7[] = "135.1516 1351 1 - * 6541 / 4 +";
    char test8[] = "-561.15 -25 * 777.999 - 444.0005 / 22 +";
    char test9[] = "0 0 * 1 + 5 7 * 4 + -";
    char test10[] = "6814 684 + 48 - 891 * 894 - 1 +";

    double expected0 = 2;
    double expected1 = 0;
    double expected2 = 5;
    double expected3 = 0.5;
    double expected4 = 1.5;
    double expected5 = 17;
    double expected6 = 455.956;
    double expected7 = 31.894001;
    double expected8 = 51.844000;
    double expected9 = -38;
    double expected10 = 6637057;
    
    ValidTestHelper (0, test0, expected0);
    ValidTestHelper (1,test1, expected1);
    ValidTestHelper (2,test2, expected2);
    ValidTestHelper (3,test3, expected3);
    ValidTestHelper (4,test4, expected4);
    ValidTestHelper (5,test5, expected5);
    ValidTestHelper (6,test6, expected6);
    ValidTestHelper (7,test7, expected7);
    ValidTestHelper (8,test8, expected8);
    ValidTestHelper (9,test9, expected9);
    ValidTestHelper (10, test10, expected10);
    

    // Invalid RPN calculation (should have at least one error)
    char test11[] = "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1";
    char test12[] = "-";
    char test13[] = "1 +";       
    char test14[] = "1 2 @ 2 +";
    char test15[] = "1 2 1.1.1 + -";
    char test16[] = "1 2+ *";
    char test17[] = "1 !2 3 + -";
    char test18[] = "1 0 /";
    char test19[] = " 12 156 + \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
    char test20[] = "3  5 1351 151 + + + 1";
    
    InvalidTestHelper (11, test11);
    InvalidTestHelper (12, test12);
    InvalidTestHelper (13, test13);
    InvalidTestHelper (14, test14);
    InvalidTestHelper (15, test15);
    InvalidTestHelper (16, test16);
    InvalidTestHelper (17, test17);
    InvalidTestHelper (18, test18);
    printf("[test19] Testing RPN_Evaluate with \" 156 1651 \\b \\b\"\n");
    InvalidTestHelper (19, test19);
    InvalidTestHelper (20, test20);
    

    // ProcessBackspaces Tests (should have no error))
    char test21[] = "122\b345";
    char test22[] = "111\b\b233\b\b345";
    char test23[] = "1\b2\b3\b4\b5\b";
    char test24[] = "\b1\b\b\b\b\b\b2\b\b\b\b\b\b\b\b3\b\b\b\b4\b\b\b5";
    char test25[] = "12345\b\b\b\b\b\b\b\b";
    
    char expected21[] = "12345";
    char expected22[] = "12345";
    char expected23[] = "";
    char expected24[] = "5";
    char expected25[] = "";
    
    ProcessBackspacesTest (21, test21, expected21);
    ProcessBackspacesTest (22, test22, expected22);
    ProcessBackspacesTest (23, test23, expected23);
    ProcessBackspacesTest (24, test24, expected24);
    ProcessBackspacesTest (25, test25, expected25);
    
    
    // Brief Test Report
    printf("\n-----------------------------------\nBrief Test Report:\n");
    printf("   passed : %d\n", passNumber);
    printf("   failed : %d\n", failNumber);

    while (1);
}

/*
 * Helper function: Do the calculation of valid RPN tests 
 * (no error should occurs)
 * @param: num - test number.
 * @param: test - a string in polish notation.
 * @param: expected - a double as the answer of the RPN.
 */
void ValidTestHelper (int num, char * test, double expected) {
    printf("[test%d] Testing RPN_Evaluate with \"%s\"... \n ", num, test);
    ProcessBackspaces(test);
    int error = RPN_Evaluate(test, &result);
    if (error) {
        printf("   Failed, RPN_Evaluate produced an error #%d : ", error);
        switch (error) {
        case RPN_ERROR_STACK_OVERFLOW:
            printf("No more room on stack.\n");
            break;
        case RPN_ERROR_STACK_UNDERFLOW:
            printf("Not enough operands before operator.\n");
            break;
        case RPN_ERROR_INVALID_TOKEN:
            printf("Invalid character in RPN string.\n");
            break;
        case RPN_ERROR_DIVIDE_BY_ZERO:
            printf("Divide by zero.\n");
            break;
        case RPN_ERROR_TOO_FEW_ITEMS_REMAIN:
            printf("Invalid RPN calculation: less than one item in stack.\n");
            break;
        case RPN_ERROR_TOO_MANY_ITEMS_REMAIN:
            printf("Invalid RPN calculation: more than one item in stack.\n");
            break;
        default:
            printf("Unknown error occurred.\n");
            break;
        }
        failNumber++;
    } else if (result != expected) {
        printf("   Failed, expected = %f , result = %f\n", expected, result);
        failNumber++;
    } else {
        printf("   Success!\n");
        passNumber++;
    }
}

/*
 * Helper function: Do the calculation of invalid RPN tests 
 * (all the tests must have at least one error)
 * @param: num - test number.
 * @param: test - a string in polish notation.
 */
void InvalidTestHelper (int num, char * test) {
    printf("[test%d] Testing RPN_Evaluate with \"%s\"... \n ", num, test);
    ProcessBackspaces(test);
    int error = RPN_Evaluate(test, &result);
    if (error) {
        printf("   Success! With error #%d :", error);
        switch (error) {
        case RPN_ERROR_STACK_OVERFLOW:
            printf("No more room on stack.\n");
            break;
        case RPN_ERROR_STACK_UNDERFLOW:
            printf("Not enough operands before operator.\n");
            break;
        case RPN_ERROR_INVALID_TOKEN:
            printf("Invalid character in RPN string.\n");
            break;
        case RPN_ERROR_DIVIDE_BY_ZERO:
            printf("Divide by zero.\n");
            break;
        case RPN_ERROR_TOO_FEW_ITEMS_REMAIN:
            printf("Invalid RPN calculation: less than one item in stack.\n");
            break;
        case RPN_ERROR_TOO_MANY_ITEMS_REMAIN:
            printf("Invalid RPN calculation: more than one item in stack.\n");
            break;
        default:
            printf("Unknown error occurred.\n");
            break;
        }
        passNumber++;
    } else {
        printf("   Failed, this test should fail : result = %f\n", result);
        failNumber++;
    }
}

/*
 * Helper function: Do the process of function ProcessBackspaces()
 * (no error should be returned)
 * @param: num - test number.
 * @param: test - a string needs to be processed.
 * @param: expected - a string that is the expected result.
 */
void ProcessBackspacesTest (int num, char * test, char * expected) {
    printf("[test%d] Testing ProcessBackspaces:\n", num);
    int length = ProcessBackspaces(test);
    if (strcmp(test, expected) == 0 && length == strlen(expected)) {
        printf("   Success! Length = %d\n", length);
        passNumber++;
    } else {
        printf("   Fail!\n");
        printf("result    : %s\n", test);
        printf("should be : %s\n", expected);
        failNumber++;
    }
}
