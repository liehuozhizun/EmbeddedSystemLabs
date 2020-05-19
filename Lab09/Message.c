/*
 * Message.c
 * Author: Hang Yuan (hyuan3, 1564348)
 * Partner: Yujia Li (yli302, 1538297)
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Message.h"

// Define macro
#define OFF                 0
#define ON                  1
#define ONE_BYTE            1
#define UNSIGNED_INT_LENGTH 4

// Define struct

typedef struct {
    uint8_t start_flag;
    uint8_t checksum_flag;
    char payload[MESSAGE_MAX_PAYLOAD_LEN + ONE_BYTE];
    char checksum[MESSAGE_CHECKSUM_LEN + ONE_BYTE];
} MessageRecordState;
static MessageRecordState mrs;

/**
 * Given a payload string, calculate its checksum
 * 
 * @param payload       //the string whose checksum we wish to calculate
 * @return   //The resulting 8-bit checksum 
 */
uint8_t Message_CalculateChecksum(const char* payload)
{
    uint8_t checkSum = 0;
    ssize_t length = strlen(payload);
    // XOR every char
    for (; length > 0; --length) {
        checkSum ^= payload[length - ONE_BYTE];
    }
    return checkSum;
}

int Message_ParseMessage(const char* payload, const char* checksum_string, BB_Event *message_event)
{
    message_event->type = BB_EVENT_ERROR;

    // Check if checksum string is MESSAGE_CHECKSUM_LEN or payload is over MESSAGE_MAX_PAYLOAD_LEN
    if (strlen(checksum_string) != MESSAGE_CHECKSUM_LEN ||
            strlen(payload) > MESSAGE_MAX_PAYLOAD_LEN) {
        return STANDARD_ERROR;
    }
    char payload_copy[MESSAGE_MAX_PAYLOAD_LEN + ONE_BYTE];
    memcpy(payload_copy, payload, strlen(payload));

    // Check if the payload matches checksum
    uint8_t expected_checkSum = Message_CalculateChecksum(payload);
    uint8_t compare_checkSum = 0;
    if (checksum_string[0] >= 0x30 && checksum_string[0] <= 0x39) { // 0-9
        compare_checkSum += (checksum_string[0] & 0xF) * 16;
    } else if (checksum_string[0] >= 0x41 && checksum_string[0] <= 0x46) { // A-F
        compare_checkSum += (checksum_string[0] - 0x41 + 10) * 16;
    }
    if (checksum_string[1] >= 0x30 && checksum_string[1] <= 0x39) { // 0-9
        compare_checkSum += (checksum_string[1] & 0xF);
    } else if (checksum_string[1] >= 0x41 && checksum_string[1] <= 0x46) { // A-F
        compare_checkSum += (checksum_string[1] - 0x41 + 10);
    }
    if (expected_checkSum != compare_checkSum) {
        return STANDARD_ERROR;
    }

    // Parse payload and check if the message matches message template
    char* token = strtok(payload_copy, ",");
    if (strcmp(token, "CHA") == 0 ||
            strcmp(token, "ACC") == 0 ||
            strcmp(token, "REV") == 0) {
        // Mark the appropriate event type 
        if (token[0] == 'C') {
            message_event->type = BB_EVENT_CHA_RECEIVED;
        } else if (token[0] == 'A') {
            message_event->type = BB_EVENT_ACC_RECEIVED;
        } else if (token[0] == 'R') {
            message_event->type = BB_EVENT_REV_RECEIVED;
        }

        // Store value
        token = strtok(NULL, ",");
        if (token == NULL) {
            message_event->type = BB_EVENT_NO_EVENT;
            return STANDARD_ERROR;
        }
        message_event->param0 = (uint16_t) atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) {
            message_event->type = BB_EVENT_NO_EVENT;
            return STANDARD_ERROR;
        }
        message_event->param1 = 0;
        message_event->param2 = 0;
    }
    else if (strcmp(token, "SHO") == 0) {
        // Store value
        token = strtok(NULL, ",");
        if (token == NULL) {
            return STANDARD_ERROR;
        }
        message_event->param0 = (uint16_t) atoi(token);
        token = strtok(NULL, ",");
        if (token == NULL) {
            return STANDARD_ERROR;
        }
        message_event->param1 = (uint16_t) atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) {
            return STANDARD_ERROR;
        }
        message_event->param2 = 0;
        // Mark the appropriate event type 
        message_event->type = BB_EVENT_SHO_RECEIVED;
    }
    else if (strcmp(token, "RES") == 0) {
        // Store value
        token = strtok(NULL, ",");
        if (token == NULL) {
            return STANDARD_ERROR;
        }
        message_event->param0 = (uint16_t) atoi(token);
        token = strtok(NULL, ",");
        if (token == NULL) {
            return STANDARD_ERROR;
        }
        message_event->param1 = (uint16_t) atoi(token);
        token = strtok(NULL, ",");
        if (token == NULL) {
            return STANDARD_ERROR;
        }
        message_event->param2 = (uint16_t) atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) {
            return STANDARD_ERROR;
        }

        // Mark the appropriate event type 
        message_event->type = BB_EVENT_RES_RECEIVED;
    } else {
        return STANDARD_ERROR;
    }

    return SUCCESS;
}

int Message_Encode(char *message_string, Message message_to_encode)
{
    // If the type if NONE or ERROR, return 0
    if (message_to_encode.type == MESSAGE_NONE || message_to_encode.type == MESSAGE_ERROR) {
        memcpy(message_string, "$*00\n\0", 6);
        return 0;
    }

    // Define payload and initialize each char to \0
    memset(message_string, 0, MESSAGE_MAX_LEN + ONE_BYTE);
    char payload[MESSAGE_MAX_PAYLOAD_LEN + ONE_BYTE];
    memset(payload, 0, MESSAGE_MAX_PAYLOAD_LEN + ONE_BYTE);

    // Generate payload based on the message type
    switch (message_to_encode.type) {
    case MESSAGE_CHA:
        sprintf(payload, PAYLOAD_TEMPLATE_CHA, message_to_encode.param0);
        break;
    case MESSAGE_ACC:
        sprintf(payload, PAYLOAD_TEMPLATE_ACC, message_to_encode.param0);
        break;
    case MESSAGE_REV:
        sprintf(payload, PAYLOAD_TEMPLATE_REV, message_to_encode.param0);
        break;
    case MESSAGE_SHO:
        sprintf(payload, PAYLOAD_TEMPLATE_SHO, message_to_encode.param0,
                message_to_encode.param1);
        break;
    case MESSAGE_RES:
        sprintf(payload, PAYLOAD_TEMPLATE_RES, message_to_encode.param0,
                message_to_encode.param1, message_to_encode.param2);
        break;
    default:
        break;
    }

    // Obtain the checkSum of the payload
    uint8_t checkSum = Message_CalculateChecksum(payload);
    sprintf(message_string, MESSAGE_TEMPLATE, payload, checkSum);

    return strlen(message_string);
}

int Message_Decode(unsigned char char_in, BB_Event * decoded_message_event)
{
    switch (char_in) {
    case '$':
        if (mrs.start_flag == ON || mrs.checksum_flag == ON) {
            decoded_message_event->type = BB_EVENT_NO_EVENT;
            memset(&mrs, 0, sizeof (MessageRecordState));
            return STANDARD_ERROR;
        } else {
            memset(&mrs, 0, sizeof (MessageRecordState));
            mrs.start_flag = ON;
        }
        break;
    case '*':
        if (mrs.start_flag == OFF || mrs.checksum_flag == ON) {
            decoded_message_event->type = BB_EVENT_NO_EVENT;
            memset(&mrs, 0, sizeof (MessageRecordState));
            return STANDARD_ERROR;
        } else {
            mrs.start_flag = OFF;
            mrs.checksum_flag = ON;
        }
        break;
    case '\n':
        if (mrs.start_flag == ON || mrs.checksum_flag == OFF) {
            decoded_message_event->type = BB_EVENT_NO_EVENT;
            memset(&mrs, 0, sizeof (MessageRecordState));
            return STANDARD_ERROR;
        } else {
            mrs.checksum_flag = OFF;
            int parse_result = Message_ParseMessage(mrs.payload, mrs.checksum, decoded_message_event);
            memset(&mrs, 0, sizeof (MessageRecordState));
            if (parse_result == STANDARD_ERROR) {
                decoded_message_event->type = BB_EVENT_NO_EVENT;
                return STANDARD_ERROR;
            }
        }
        break;
    default:
        if (mrs.start_flag == ON && mrs.checksum_flag == OFF) {
            if (strlen(mrs.payload) >= MESSAGE_MAX_PAYLOAD_LEN) {
                decoded_message_event->type = BB_EVENT_NO_EVENT;
                memset(&mrs, 0, sizeof (MessageRecordState));
                return STANDARD_ERROR;
            }
            mrs.payload[strlen(mrs.payload)] = char_in;
        } else if (mrs.start_flag == OFF && mrs.checksum_flag == ON) {
            if (strlen(mrs.checksum) >= MESSAGE_CHECKSUM_LEN) {
                decoded_message_event->type = BB_EVENT_NO_EVENT;
                memset(&mrs, 0, sizeof (MessageRecordState));
                return STANDARD_ERROR;
            }
            mrs.checksum[strlen(mrs.checksum)] = char_in;
        }
        break;
    }

    return SUCCESS;
}
