/*
 * File:   Agent.c
 * Author: Hang Yuan, hyuan3@ucsc.edu
 *         Yujia Li, yli302@ucsc.edu
 * 
 * Agent.c is implemented by Yujia Li
 *
 */

// **** Include libraries here ****
// Standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Oled.h"
#include "FieldOled.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Field.h"
#include "Agent.h"
#include "Message.h"
#include "Negotiation.h"

// Define macro
#define OFF   0
#define ON    1

// Define global structure

typedef struct {
    uint8_t state;
    uint16_t round;
    NegotiationData a;
    NegotiationData b;
    NegotiationData hasha;
} AgentStateMachine;

#define OLED_PRINT_SIZE 256

static AgentStateMachine agentSM;
static Field own_field, opp_field;
static char oledMassage[OLED_PRINT_SIZE];

void AgentInit(void)
{
    FieldInit(&own_field, &opp_field);
    FieldAIPlaceAllBoats(&own_field);
    agentSM.state = AGENT_STATE_START;
    agentSM.round = 0; // count game round
    agentSM.a = 0; // A send from Chanllenging
    agentSM.b = 0; // B send from Accepting
    agentSM.hasha = 0;
}

Message AgentRun(BB_Event event)
{
    Message message = {MESSAGE_NONE, 0, 0, 0};
    GuessData guessData = {};

    // Based on different event do different things
    switch (event.type) {
    case BB_EVENT_NO_EVENT:
        return message;
        break;
    case BB_EVENT_START_BUTTON:
        if (agentSM.state == AGENT_STATE_START) {
            // generate A, #a
            agentSM.a = rand();
            agentSM.hasha = NegotiationHash(agentSM.a);
            // package CHA
            message.type = MESSAGE_CHA;
            message.param0 = agentSM.hasha;
            // initialize Fields
            FieldInit(&own_field, &opp_field);
            FieldAIPlaceAllBoats(&own_field);
            agentSM.round = 0;
            // change stage 
            agentSM.state = AGENT_STATE_CHALLENGING;
            // update OLED
            memset(oledMassage, 0, OLED_PRINT_SIZE);
            sprintf(oledMassage, "CHALLENGING\n %5d = A\n %5d = hash_a", agentSM.a, agentSM.hasha);
            OledClear(OLED_COLOR_BLACK);
            OledDrawString(oledMassage);
            OledUpdate();
        }
        break;
    case BB_EVENT_RESET_BUTTON:
        // reset all data
        AgentInit();
        // update OLED
        memset(oledMassage, 0, OLED_PRINT_SIZE);
        sprintf(oledMassage, "START\n\nReady for a new game?\nPress BTN4 to");
        OledClear(OLED_COLOR_BLACK);
        OledDrawString(oledMassage);
        OledUpdate();
        break;
    case BB_EVENT_CHA_RECEIVED:
        if (agentSM.state == AGENT_STATE_START) {
            // generate B
            agentSM.b = rand();
            // store # a
            agentSM.hasha = event.param0;
            // package ACC
            message.type = MESSAGE_ACC;
            message.param0 = agentSM.b;
            // initialize Fields
            FieldInit(&own_field, &opp_field);
            FieldAIPlaceAllBoats(&own_field);
            agentSM.round = 0;
            // change stage
            agentSM.state = AGENT_STATE_ACCEPTING;
            // update OLED
            memset(oledMassage, 0, OLED_PRINT_SIZE);
            sprintf(oledMassage, "ACCEPTING\n %5d = hash_a\n %5d = B", agentSM.hasha, agentSM.b);
            OledClear(OLED_COLOR_BLACK);
            OledDrawString(oledMassage);
            OledUpdate();
        }
        break;
    case BB_EVENT_ACC_RECEIVED:
        if (agentSM.state == AGENT_STATE_CHALLENGING) {
            // store B
            agentSM.b = event.param0;
            // package REV
            message.type = MESSAGE_REV;
            message.param0 = agentSM.a;
            // check wither head or tail, and change stage
            if (NegotiateCoinFlip(agentSM.a, agentSM.b) == HEADS) {
                agentSM.state = AGENT_STATE_WAITING_TO_SEND;
                // update OLED
                OledClear(OLED_COLOR_BLACK);
                FieldOledDrawScreen(&own_field, &opp_field, FIELD_OLED_TURN_MINE, agentSM.round);
                OledUpdate();
            } else {
                agentSM.state = AGENT_STATE_DEFENDING;
                // update OLED
                OledClear(OLED_COLOR_BLACK);
                FieldOledDrawScreen(&own_field, &opp_field, FIELD_OLED_TURN_THEIRS, agentSM.round);
                OledUpdate();
            }
        }
        break;
    case BB_EVENT_REV_RECEIVED:
        if (agentSM.state == AGENT_STATE_ACCEPTING) {
            // store A
            agentSM.a = event.param0;
            // check wither head or tail or challenger cheating, and change stage
            if (NegotiationVerify(agentSM.a, agentSM.hasha)) {
                if (NegotiateCoinFlip(agentSM.a, agentSM.b) == HEADS) {
                    agentSM.state = AGENT_STATE_DEFENDING;
                    // update OLED
                    OledClear(OLED_COLOR_BLACK);
                    FieldOledDrawScreen(&own_field, &opp_field, FIELD_OLED_TURN_THEIRS, agentSM.round);
                    OledUpdate();
                } else {
                    agentSM.state = AGENT_STATE_ATTACKING;
                    // update OLED
                    OledClear(OLED_COLOR_BLACK);
                    FieldOledDrawScreen(&own_field, &opp_field, FIELD_OLED_TURN_MINE, agentSM.round);
                    OledUpdate();
                }
            } else {
                // set cheating end massage and change to end stage
                agentSM.state = AGENT_STATE_END_SCREEN;
                // update OLED
                memset(oledMassage, 0, OLED_PRINT_SIZE);
                sprintf(oledMassage, "YOU ARE A CHEATER!!!");
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(oledMassage);
                OledUpdate();
            }
        }
        break;
    case BB_EVENT_SHO_RECEIVED:
        if (agentSM.state == AGENT_STATE_DEFENDING) {
            // update own field
            guessData.row = event.param0;
            guessData.col = event.param1;
            FieldRegisterEnemyAttack(&own_field, &guessData);
            // package RES
            message.type = MESSAGE_RES;
            message.param0 = guessData.row;
            message.param1 = guessData.col;
            message.param2 = guessData.result;
            // change stage
            if (FieldGetBoatStates(&own_field)) {
                agentSM.state = AGENT_STATE_WAITING_TO_SEND;
                // update OLED
                OledClear(OLED_COLOR_BLACK);
                FieldOledDrawScreen(&own_field, &opp_field, FIELD_OLED_TURN_MINE, agentSM.round);
                OledUpdate();
            } else {
                // defeat
                agentSM.state = AGENT_STATE_END_SCREEN;
                // update OLED
                memset(oledMassage, 0, OLED_PRINT_SIZE);
                sprintf(oledMassage, "...Alas, defeat\n :( \n - Yujia & Hang");
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(oledMassage);
                OledUpdate();
            }
        }
        break;
    case BB_EVENT_RES_RECEIVED:
        if (agentSM.state == AGENT_STATE_ATTACKING) {
            // update opp field
            guessData.row = event.param0;
            guessData.col = event.param1;
            guessData.result = event.param2;
            FieldUpdateKnowledge(&opp_field, &guessData);
            // change stage
            if (FieldGetBoatStates(&opp_field)) {
                agentSM.state = AGENT_STATE_DEFENDING;
                // update OLED
                OledClear(OLED_COLOR_BLACK);
                FieldOledDrawScreen(&own_field, &opp_field, FIELD_OLED_TURN_THEIRS, agentSM.round);
                OledUpdate();
            } else {
                // victory
                agentSM.state = AGENT_STATE_END_SCREEN;
                // update OLED
                memset(oledMassage, 0, OLED_PRINT_SIZE);
                sprintf(oledMassage, "* VICTORY HOORAY!!! *\n \n - Yujia & Hang");
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(oledMassage);
                OledUpdate();
            }
        }
        break;
    case BB_EVENT_MESSAGE_SENT:
        if (agentSM.state == AGENT_STATE_WAITING_TO_SEND) {
            // increment turn count
            agentSM.round += 1;
            // decide guess
            guessData = FieldAIDecideGuess(&opp_field);
            // package SHO
            message.type = MESSAGE_SHO;
            message.param0 = guessData.row;
            message.param1 = guessData.col;
            // change stage
            agentSM.state = AGENT_STATE_ATTACKING;
            // update OLED
            OledClear(OLED_COLOR_BLACK);
            FieldOledDrawScreen(&own_field, &opp_field, FIELD_OLED_TURN_MINE, agentSM.round);
            OledUpdate();
        }
        break;
    case BB_EVENT_ERROR:
        agentSM.state = AGENT_STATE_END_SCREEN;
        OledClear(OLED_COLOR_BLACK);
        OledDrawString("Error, error, here is an error!");
        OledUpdate();
        break;
    case BB_EVENT_SOUTH_BUTTON:
        // TOO HARD :(
        // NOT IMPLEMENT
        break;
    case BB_EVENT_EAST_BUTTON:
        // TOO HARD :(
        // NOT IMPLEMENT
        break;
    default:
        message.type = MESSAGE_NONE;
        return message;
        break;
    }
    return message;
}

AgentState AgentGetState(void)
{
    return agentSM.state;
}

void AgentSetState(AgentState newState)
{
    agentSM.state = newState;
}
