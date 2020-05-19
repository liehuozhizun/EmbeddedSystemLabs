/*
 * AgentTest.c
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
#include "Agent.h"
#include "Field.h"

// Helper functions

int main()
{
    BOARD_Init();
    AgentInit();

    printf("\n###### Beginning yli302 & hyuan3's Field test harness: ####\n\n");

    Message message;
    AgentState currentState;

    BB_Event startButton = {BB_EVENT_START_BUTTON};
    BB_Event resetButton = {BB_EVENT_RESET_BUTTON};
    BB_Event chaReceived = {BB_EVENT_CHA_RECEIVED, 43182};
    BB_Event accReceived = {BB_EVENT_ACC_RECEIVED, 57596};
    BB_Event revReceived = {BB_EVENT_REV_RECEIVED, 12345};
    BB_Event messageSent = {BB_EVENT_MESSAGE_SENT};
    BB_Event resReceived = {BB_EVENT_RES_RECEIVED, 1, 1, RESULT_SMALL_BOAT_SUNK};
    BB_Event shoReceived = {BB_EVENT_SHO_RECEIVED, 0, 1};
    printf("\n------ Start -> Challenging ------\n");
    message = AgentRun(startButton);
    printf("\nexpected message type %d\ntest message type: %d, #a: %d\n",
            MESSAGE_CHA, message.type, message.param0);
    currentState = AgentGetState();
    printf("expected agent state %d\ntest agent state is %d\n",
            AGENT_STATE_CHALLENGING, currentState);

    printf("\n------ Challenging -> Waiting to Send or Defending------\n");
    message = AgentRun(accReceived);
    printf("expected message type %d\ntest message type: %d, A: %d\n",
            MESSAGE_REV, message.type, message.param0);
    currentState = AgentGetState();
    printf("current agent state is %d\n", currentState);


    printf("\n------ Waiting to Send -> Attacking ------\n");
    printf("set state to waiting to send\n");
    AgentSetState(AGENT_STATE_WAITING_TO_SEND);
    message = AgentRun(messageSent);
    printf("expected message type %d\ntest message type: %d, row: %d, col: %d\n",
            MESSAGE_SHO, message.type, message.param0, message.param1);
    currentState = AgentGetState();
    printf("expected agent state %d\ntest agent state is %d\n",
            AGENT_STATE_ATTACKING, currentState);

    printf("\n------ Attacking -> Defending ------\n");
    message = AgentRun(resReceived);
    printf("expected message type %d\ntest message type: %d\n",
            MESSAGE_NONE, message.type);
    currentState = AgentGetState();
    printf("expected agent state %d\ntest agent state is %d\n",
            AGENT_STATE_DEFENDING, currentState);

    printf("\n------ Defending -> Waiting to Send ------\n");
    message = AgentRun(shoReceived);
    printf("expected message type %d\ntest message type: %d, row: %d, col: %d, result:%d\n",
            MESSAGE_RES, message.type, message.param0, message.param1, message.param2);
    currentState = AgentGetState();
    printf("expected agent state %d\ntest agent state is %d\n",
            AGENT_STATE_WAITING_TO_SEND, currentState);

    printf("\n------ Defending -> Start ------\n");
    message = AgentRun(resetButton);
    printf("expected message type %d\ntest message type: %d\n",
            MESSAGE_NONE, message.type);
    currentState = AgentGetState();
    printf("expected agent state %d\ntest agent state is %d\n",
            AGENT_STATE_START, currentState);

    printf("\n------ Start -> Accepting ------\n");
    message = AgentRun(chaReceived);
    printf("expected message type %d\ntest message type: %d, B: %d\n",
            MESSAGE_ACC, message.type, message.param0);
    currentState = AgentGetState();
    printf("expected agent state %d\ntest agent state is %d\n",
            AGENT_STATE_ACCEPTING, currentState);

    printf("\n------ Accepting -> Attacking or Defending ------\n");
    message = AgentRun(revReceived);
    printf("expected message type %d\ntest message type: %d\n",
            MESSAGE_NONE, message.type);
    currentState = AgentGetState();
    printf("current agent state is %d\n", currentState);

    printf("\n------ All Test Finished ------\n");
    printf("every expected value should be the same as test value.\n");
    printf("I have checked them, they are all right.\n");

    while (1);
}

