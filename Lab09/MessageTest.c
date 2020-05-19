/*
 * File:   NegotiationTest.c
 * Author: Hang Yuan, hyuan3@ucsc.edu
 *         Yujia Li, yli302@ucsc.edu
 * 
 * NegotiationTest.c is implemented by Yujia Li
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Message.h"

// Test helper function 
void Test_Message_CalculateChecksum();
void Test_Message_Encode();
void Test_Message_ParseMessage();
void Test_Message_Decode();

int main()
{
    BOARD_Init();
    
    Test_Message_CalculateChecksum();
    Test_Message_Encode();
    Test_Message_ParseMessage();
    Test_Message_Decode();

    return 0;
}

void Test_Message_CalculateChecksum()
{
    printf("Test - Message_CalculateChecksum()\n");
    // #1
    uint8_t expected_checkSum = 0x5E;
    char test_text[] = "ACC,57203\0";
    printf("Test 1/3 : ACC,57203\n");
    uint8_t checkSum = Message_CalculateChecksum(test_text);
    if (checkSum == expected_checkSum) {
        printf("  Success: 0x%X\n", checkSum);
    } else {
        printf("  Fail:\n    Expected:0x%x\n    Got:     0x%x\n", expected_checkSum, checkSum);
    }

    // #2
    expected_checkSum = 0x56;
    char test_text2[] = "SHO,2,0\0";
    printf("Test 2/3 : SHO,2,0\n");
    checkSum = Message_CalculateChecksum(test_text2);
    if (checkSum == expected_checkSum) {
        printf("  Success: 0x%X\n", checkSum);
    } else {
        printf("  Fail:\n    Expected:0x%x\n    Got:     0x%x\n", expected_checkSum, checkSum);
    }

    // #3
    expected_checkSum = 0x59;
    char test_text3[] = "RES,3,3,1\0";
    printf("Test 3/3 : RES,3,3,1\n");
    checkSum = Message_CalculateChecksum(test_text3);
    if (checkSum == expected_checkSum) {
        printf("  Success: 0x%X\n", checkSum);
    } else {
        printf("  Fail:\n    Expected:0x%x\n    Got:     0x%x\n", expected_checkSum, checkSum);
    }
}

void Test_Message_Encode()
{
    printf("\nTest - Test_Message_Encode()\n");
    Message message = {MESSAGE_NONE, 57203, 1, 2};
    char result[MESSAGE_MAX_LEN + 1];

    // #1 - NONE
    printf("Test 1/3: MESSAGE_NONE\n");
    if (Message_Encode(result, message) == 0) {
        printf("  Success, returned:(nothing)\n");
    } else {
        printf("  Fail, returned: %s", result);
    }

    // #2 - CHA
    printf("Test 2/3: MESSAGE_ACC\n");
    message.type = MESSAGE_ACC;
    char expected_ACC[MESSAGE_MAX_LEN + 1];
    memset(expected_ACC, 0, MESSAGE_MAX_LEN + 1);
    char template[] = "$ACC,%u*%02X\n";
    sprintf(expected_ACC, template, 57203, 0x5E);
    Message_Encode(result, message);
    if (strcmp(result, expected_ACC) == 0) {
        printf("  Success, returned:%s", result);
    } else {
        printf("  Fail\n    expected: %s    returned: %s", expected_ACC, result);
    }

    // #2 - RES
    printf("Test 3/3: MESSAGE_RES\n");
    message.type = MESSAGE_RES;
    char expected_RES[MESSAGE_MAX_LEN + 1];
    memset(expected_RES, 0, MESSAGE_MAX_LEN + 1);
    char template2[] = "$RES,%u,%u,%u*%02X\n";
    sprintf(expected_RES, template2, 57203, 1, 2, 0x58);
    Message_Encode(result, message);
    if (strcmp(result, expected_RES) == 0) {
        printf("  Success, returned:%s", result);
    } else {
        printf("  Fail\n    expected: %s    returned: %s", expected_RES, result);
    }
}

void Test_Message_ParseMessage()
{
    printf("\nTest - Test_Message_ParseMessage()\n");
    BB_Event event = {BB_EVENT_NO_EVENT, 0, 0, 0};
    char payload[MESSAGE_MAX_PAYLOAD_LEN + 1];
    memset(payload, 0, MESSAGE_MAX_PAYLOAD_LEN + 1);
    char checksum[] = "59\0";

    // #1 correct format
    printf("Test 1/4: correct format\n");
    sprintf(payload, PAYLOAD_TEMPLATE_RES, 3, 3, 1);
    if (Message_ParseMessage(payload, checksum, &event) == SUCCESS) {
        printf("  Success, returned:(nothing)\n");
    } else {
        printf("  Fail, returned: %d\n", event.type);
    }

    // #2 payload doesn't match checksum
    printf("Test 2/4: payload doesn't match checksum\n");
    char bad_checksum[] = "58\0";
    if (Message_ParseMessage(payload, bad_checksum, &event) == STANDARD_ERROR) {
        printf("  Success, returned:(nothing)\n");
    } else {
        printf("  Fail, returned: %d\n", event.type);
    }

    // #3 checksum string is not two characters long
    printf("Test 3/4: payload doesn't match checksum\n");
    char large_checksum[4] = "581\0";
    if (Message_ParseMessage(payload, large_checksum, &event) == STANDARD_ERROR) {
        printf("  Success, returned:(nothing)\n");
    } else {
        printf("  Fail, returned: %d\n", event.type);
    }

    // #4 message doesn't match message template
    printf("Test 4/4: message doesn't match message template\n");
    memcpy(payload, "ACC", 3);
    if (Message_ParseMessage(payload, checksum, &event) == STANDARD_ERROR) {
        printf("  Success, returned:(nothing)\n");
    } else {
        printf("  Fail, returned: %d\n", event.type);
    }
}

void Test_Message_Decode()
{
    printf("\nTest - Test_Message_Decode()\n");
    BB_Event event = {BB_EVENT_NO_EVENT, 0, 0, 0};
    char string[MESSAGE_MAX_PAYLOAD_LEN + 1];
    char template[] = "$RES,3,3,1*%02X\n\0";

    // #1 correct format
    printf("Test 1/4: correct format\n");
    sprintf(string, template, 0x59);
    int index = 0;
    int flag = SUCCESS;
    for (; index < strlen(string); ++index) {
        if (Message_Decode(string[index], &event) != SUCCESS) {
            flag = STANDARD_ERROR;
            break;
        }
    }
    if (flag == SUCCESS) {
        printf("  Success, returned:(nothing)\n");
    } else {
        printf("  Fail, type in: %s  returned: %d\n", string, event.type);
    }

    // #2 wrong checksum
    printf("Test 2/4: wrong checksum\n");
    sprintf(string, template, 0x58);
    index = 0;
    flag = SUCCESS;
    for (; index < strlen(string); ++index) {
        if (Message_Decode(string[index], &event) != SUCCESS) {
            flag = STANDARD_ERROR;
            break;
        }
    }
    if (flag != SUCCESS) {
        printf("  Success, returned:(nothing)\n");
    } else {
        printf("  Fail, type in: %s  returned: %d\n", string, event.type);
    }

    // #3 invalid char
    printf("Test 3/4: invalid char\n");
    char template2[] = "$RE$,3,3,1%02X\n";
    sprintf(string, template2, 0x59);
    index = 0;
    flag = SUCCESS;
    for (; index < strlen(string); ++index) {
        if (Message_Decode(string[index], &event) != SUCCESS) {
            flag = STANDARD_ERROR;
            break;
        }
    }
    if (flag != SUCCESS) {
        printf("  Success, returned:(nothing)\n");
    } else {
        printf("  Fail, type_in: %s  returned: %d\n", string, event.type);
    }

    // #4 invalid format
    printf("Test 3/4: invalid format\n");
    char template3[] = "$RES,3,3\n";
    index = 0;
    flag = SUCCESS;
    for (; index < strlen(template3); ++index) {
        if (Message_Decode(template3[index], &event) != SUCCESS) {
            flag = STANDARD_ERROR;
            break;
        }
    }
    if (flag != SUCCESS) {
        printf("  Success, returned:(nothing)\n");
    } else {
        printf("  Fail, type_in: %s  returned: %d\n", string, event.type);
    }
}