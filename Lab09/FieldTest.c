/*
 * FieldTest.c
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
#include "Field.h"

enum {
    TEST_SUCCESS,
    TEST_FAIL
};

static Field ownField;
static Field oppField;

int main(void)
{
    BOARD_Init();
    int r, c;
    int testResult = TEST_SUCCESS;

    printf("\n###### Beginning yli302 & hyuan3's Field test harness: ####\n\n");

    printf("******************* FieldInit() Unit Tests *******************\n");
    FieldInit(&ownField, &oppField);
    printf("test grid in own field\n");
    printf("test grid in opp field\n");
    for (r = 0; r < FIELD_ROWS; ++r) {
        for (c = 0; c < FIELD_COLS; ++c) {
            // ------ every grid in ownField should be FIELD_SQUARE_EMPTY ------
            if (ownField.grid[r][c] != FIELD_SQUARE_EMPTY) {
                testResult = TEST_FAIL;
            }
            // ------ every grid in oppField should be FIELD_SQUARE_UNKNOWN ------
            if (oppField.grid[r][c] != FIELD_SQUARE_UNKNOWN) {
                testResult = TEST_FAIL;
            }
        }
    }
    // ------ boat live data for ownField should be zero ------
    printf("test boat live data of own field\n");
    if (ownField.smallBoatLives != 0 || ownField.mediumBoatLives != 0 ||
            ownField.largeBoatLives != 0 || ownField.hugeBoatLives != 0) {
        testResult = TEST_FAIL;
    }
    // ------ boat live data for oppField should be 3(SMALL), 4(MEDIUN), 5(LARGE), 6(HUGE) ------
    printf("test boat live data for opp field\n");
    if (oppField.smallBoatLives != FIELD_BOAT_SIZE_SMALL ||
            oppField.mediumBoatLives != FIELD_BOAT_SIZE_MEDIUM ||
            oppField.largeBoatLives != FIELD_BOAT_SIZE_LARGE ||
            oppField.hugeBoatLives != FIELD_BOAT_SIZE_HUGE) {
        testResult = TEST_FAIL;
    }
    FieldPrint_UART(&ownField, &oppField);
    // ------ assert test result ------
    testResult == TEST_SUCCESS ? printf("FieldInit() SUCCESS\n\n") :
            printf("FieldInit() FAIL\n\n");

    printf("******************* FieldGetSquareStatus() Unit Tests *******************\n");
    // test FieldGetSquareStatus() by ownField and oppField get from FieldInit() function.
    printf("test get square of initialized own field and opp field\n");
    for (r = 0; r < FIELD_ROWS; ++r) {
        for (c = 0; c < FIELD_COLS; ++c) {
            // ------ every grid in ownField should be FIELD_SQUARE_EMPTY ------
            if (FieldGetSquareStatus(&ownField, r, c) != FIELD_SQUARE_EMPTY) {
                testResult = TEST_FAIL;
            }
            // ------ every grid in oppField should be FIELD_SQUARE_UNKNOWN ------
            if (FieldGetSquareStatus(&oppField, r, c) != FIELD_SQUARE_UNKNOWN) {
                testResult = TEST_FAIL;
            }
        }
    }
    // ------ test FieldGetSquareStatus() by manually set data ------
    printf("test manually set data can be get correctly\n");
    ownField.grid[0][1] = FIELD_SQUARE_SMALL_BOAT;
    ownField.grid[0][2] = FIELD_SQUARE_MEDIUM_BOAT;
    ownField.grid[0][3] = FIELD_SQUARE_LARGE_BOAT;
    ownField.grid[0][4] = FIELD_SQUARE_HUGE_BOAT;
    ownField.grid[0][5] = FIELD_SQUARE_UNKNOWN;
    ownField.grid[0][6] = FIELD_SQUARE_HIT;
    ownField.grid[0][7] = FIELD_SQUARE_MISS;
    ownField.grid[0][8] = FIELD_SQUARE_CURSOR;
    ownField.grid[0][9] = FIELD_SQUARE_INVALID;
    if (FieldGetSquareStatus(&ownField, 0, 1) != FIELD_SQUARE_SMALL_BOAT) {
        testResult = TEST_FAIL;
    }
    if (FieldGetSquareStatus(&ownField, 0, 2) != FIELD_SQUARE_MEDIUM_BOAT) {
        testResult = TEST_FAIL;
    }
    if (FieldGetSquareStatus(&ownField, 0, 3) != FIELD_SQUARE_LARGE_BOAT) {
        testResult = TEST_FAIL;
    }
    if (FieldGetSquareStatus(&ownField, 0, 4) != FIELD_SQUARE_HUGE_BOAT) {
        testResult = TEST_FAIL;
    }
    if (FieldGetSquareStatus(&ownField, 0, 5) != FIELD_SQUARE_UNKNOWN) {
        testResult = TEST_FAIL;
    }
    if (FieldGetSquareStatus(&ownField, 0, 6) != FIELD_SQUARE_HIT) {
        testResult = TEST_FAIL;
    }
    if (FieldGetSquareStatus(&ownField, 0, 7) != FIELD_SQUARE_MISS) {
        testResult = TEST_FAIL;
    }
    if (FieldGetSquareStatus(&ownField, 0, 8) != FIELD_SQUARE_CURSOR) {
        testResult = TEST_FAIL;
    }
    if (FieldGetSquareStatus(&ownField, 0, 9) != FIELD_SQUARE_INVALID) {
        testResult = TEST_FAIL;
    }
    // reset ownField to initial status
    FieldInit(&ownField, &oppField);
    // ------ test if row and col are not valid field locations ------
    printf("test invalid row or col\n");
    if (FieldGetSquareStatus(&ownField, 0, 10) != FIELD_SQUARE_INVALID) {
        testResult = TEST_FAIL;
    }
    if (FieldGetSquareStatus(&ownField, 10, 0) != FIELD_SQUARE_INVALID) {
        testResult = TEST_FAIL;
    }
    // ------ assert test result ------
    testResult == TEST_SUCCESS ? printf("FieldGetSquareStatus() SUCCESS\n\n") :
            printf("FieldGetSquareStatus() FAIL\n\n");

    printf("******************* FieldSetSquareStatus() Unit Tests *******************\n");
    // ------ test FieldSetSquareStatus by testing set all kinds of SquareStatus ------
    printf("test set data can be get correctly\n");
    if (FieldSetSquareStatus(&ownField, 0, 0, FIELD_SQUARE_SMALL_BOAT) != FIELD_SQUARE_EMPTY) {
        testResult = TEST_FAIL;
    }
    if (FieldSetSquareStatus(&ownField, 0, 0, FIELD_SQUARE_MEDIUM_BOAT) != FIELD_SQUARE_SMALL_BOAT) {
        testResult = TEST_FAIL;
    }
    if (FieldSetSquareStatus(&ownField, 0, 0, FIELD_SQUARE_LARGE_BOAT) != FIELD_SQUARE_MEDIUM_BOAT) {
        testResult = TEST_FAIL;
    }
    if (FieldSetSquareStatus(&ownField, 0, 0, FIELD_SQUARE_HUGE_BOAT) != FIELD_SQUARE_LARGE_BOAT) {
        testResult = TEST_FAIL;
    }
    if (FieldSetSquareStatus(&ownField, 0, 0, FIELD_SQUARE_UNKNOWN) != FIELD_SQUARE_HUGE_BOAT) {
        testResult = TEST_FAIL;
    }
    if (FieldSetSquareStatus(&ownField, 0, 0, FIELD_SQUARE_HIT) != FIELD_SQUARE_UNKNOWN) {
        testResult = TEST_FAIL;
    }
    if (FieldSetSquareStatus(&ownField, 0, 0, FIELD_SQUARE_MISS) != FIELD_SQUARE_HIT) {
        testResult = TEST_FAIL;
    }
    if (FieldSetSquareStatus(&ownField, 0, 0, FIELD_SQUARE_CURSOR) != FIELD_SQUARE_MISS) {
        testResult = TEST_FAIL;
    }
    if (FieldSetSquareStatus(&ownField, 0, 0, FIELD_SQUARE_INVALID) != FIELD_SQUARE_CURSOR) {
        testResult = TEST_FAIL;
    }
    if (FieldSetSquareStatus(&ownField, 0, 0, FIELD_SQUARE_EMPTY) != FIELD_SQUARE_INVALID) {
        testResult = TEST_FAIL;
    }
    // ------ assert test result ------
    testResult == TEST_SUCCESS ? printf("FieldSetSquareStatus() SUCCESS\n\n") :
            printf("FieldSetSquareStatus() FAIL\n\n");

    printf("******************* FieldAddBoat() Unit Tests *******************\n");
    // reset ownField to initial status
    printf("Initial Field for add boat test\n");
    FieldInit(&ownField, &oppField);
    // ------ This is a correct test of add boat------ 
    printf("Boats' position is the same as example from Field.h\n");
    if (FieldAddBoat(&ownField, 0, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL) != SUCCESS) {
        testResult = TEST_FAIL;
    }
    if (FieldAddBoat(&ownField, 1, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM) != SUCCESS) {
        testResult = TEST_FAIL;
    }
    if (FieldAddBoat(&ownField, 0, 6, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_SMALL) != SUCCESS) {
        testResult = TEST_FAIL;
    }
    if (ownField.smallBoatLives != 3 || ownField.mediumBoatLives != 4) {
        testResult = TEST_FAIL;
    }
    FieldPrint_UART(&ownField, &oppField);
    // ------ test if boat cannot fit in the desired position ------
    printf("test invalid add boat\n");
    if (FieldAddBoat(&ownField, 1, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_HUGE) != STANDARD_ERROR) {
        testResult = TEST_FAIL;
    }
    if (FieldAddBoat(&ownField, 2, 4, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM) != STANDARD_ERROR) {
        testResult = TEST_FAIL;
    }
    if (FieldAddBoat(&ownField, 2, 2, FIELD_DIR_EAST, FIELD_BOAT_TYPE_HUGE) != STANDARD_ERROR) {
        testResult = TEST_FAIL;
    }
    if (FieldAddBoat(&ownField, 0, 9, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL) != STANDARD_ERROR) {
        testResult = TEST_FAIL;
    }
    if (FieldAddBoat(&ownField, 4, 5, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_LARGE) != STANDARD_ERROR) {
        testResult = TEST_FAIL;
    }
    if (FieldAddBoat(&ownField, 2, 3, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_HUGE) != STANDARD_ERROR) {
        testResult = TEST_FAIL;
    }
    // ------ test if row and col are not valid field locations ------
    if (FieldAddBoat(&ownField, 0, 10, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL) != STANDARD_ERROR) {
        testResult = TEST_FAIL;
    }
    if (FieldAddBoat(&ownField, 10, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL) != STANDARD_ERROR) {
        testResult = TEST_FAIL;
    }
    // ------ assert test result ------
    testResult == TEST_SUCCESS ? printf("FieldAddBoat() SUCCESS\n\n") :
            printf("FieldAddBoat() FAIL\n\n");

    printf("******************* FieldRegisterEnemyAttack() Unit Tests *******************\n");
    // ------ testing attack small boat ------
    printf("test attack hit\n");
    GuessData attackEnemyHit1 = {0, 0}; // there is a small boat at (0, 0), (0, 1), (0, 2) 
    // there is a small boat at (0, 0)
    if (FieldRegisterEnemyAttack(&ownField, &attackEnemyHit1) != FIELD_SQUARE_SMALL_BOAT) {
        testResult = TEST_FAIL;
    }
    // this attack should be HIT
    if (attackEnemyHit1.result != RESULT_HIT) {
        testResult = TEST_FAIL;
    }
    // smallBoat have been attacked, smallBoatLives should decrease one.
    if (ownField.smallBoatLives != FIELD_BOAT_SIZE_SMALL - 1) {
        testResult = TEST_FAIL;
    }
    // ------ testing attack medium boat ------
    GuessData attackEnemyHit2 = {1, 0}; // there is a medium boat at (1, 0), (1, 1), (1, 2), (1, 3)
    // there is a medium boat at (1, 0)
    if (FieldRegisterEnemyAttack(&ownField, &attackEnemyHit2) != FIELD_SQUARE_MEDIUM_BOAT) {
        testResult = TEST_FAIL;
    }
    // this attack should be HIT
    if (attackEnemyHit2.result != RESULT_HIT) {
        testResult = TEST_FAIL;
    }
    // smallBoat have been attacked, mediumBoatLives should decrease one.
    if (ownField.mediumBoatLives != FIELD_BOAT_SIZE_MEDIUM - 1) {
        testResult = TEST_FAIL;
    }
    // ------ testing attack medium boat until sink ------
    printf("test attack medium boat until sink\n");
    GuessData attackEnemyHit3 = {1, 1};
    GuessData attackEnemyHit4 = {1, 2};
    GuessData attackEnemyHit5 = {1, 3};
    if (FieldRegisterEnemyAttack(&ownField, &attackEnemyHit3) != FIELD_SQUARE_MEDIUM_BOAT) {
        testResult = TEST_FAIL;
    }
    if (attackEnemyHit3.result != RESULT_HIT) {
        testResult = TEST_FAIL;
    }
    if (FieldRegisterEnemyAttack(&ownField, &attackEnemyHit4) != FIELD_SQUARE_MEDIUM_BOAT) {
        testResult = TEST_FAIL;
    }
    if (attackEnemyHit4.result != RESULT_HIT) {
        testResult = TEST_FAIL;
    }
    if (FieldRegisterEnemyAttack(&ownField, &attackEnemyHit5) != FIELD_SQUARE_MEDIUM_BOAT) {
        testResult = TEST_FAIL;
    }
    if (attackEnemyHit5.result != RESULT_MEDIUM_BOAT_SUNK) {
        testResult = TEST_FAIL;
    }
    if (ownField.mediumBoatLives != 0) {
        testResult = TEST_FAIL;
    }
    // ------ testing miss attack ------
    printf("test attack miss\n");
    GuessData attackEnemyMiss = {2, 0}; // there is nothing at (2, 0)
    if (FieldRegisterEnemyAttack(&ownField, &attackEnemyMiss) != FIELD_SQUARE_EMPTY) {
        testResult = TEST_FAIL;
    }
    // this attack should be MISS
    if (attackEnemyMiss.result != RESULT_MISS) {
        testResult = TEST_FAIL;
    }
    // ------ print attack result ------
    printf("Attack small boat at (0, 0), HIT\n"
            "Attack medium boat at (1, 0), HIT\n"
            "Attack medium boat at (1, 1), HIT\n"
            "Attack medium boat at (1, 2), HIT\n"
            "Attack medium boat at (1, 3), MEDIUM BOAT SINK\n"
            "Attack empty field at (2, 0), MISS\n");
    FieldPrint_UART(&ownField, &oppField);
    // ------ assert test result ------
    testResult == TEST_SUCCESS ? printf("FieldRegisterEnemyAttack() SUCCESS\n\n") :
            printf("FieldRegisterEnemyAttack() FAIL\n\n");

    printf("******************* FieldUpdateKnowledge() Unit Tests *******************\n");
    // ------ attack a small boat until sink ------
    printf("test attack small boat until sink\n");
    GuessData attackSmall1 = {0, 0, RESULT_SMALL_BOAT_SUNK};
    GuessData attackSmall2 = {0, 1, RESULT_HIT};
    GuessData attackSmall3 = {0, 2, RESULT_SMALL_BOAT_SUNK};
    if (FieldUpdateKnowledge(&oppField, &attackSmall1) != FIELD_SQUARE_UNKNOWN ||
            FieldGetSquareStatus(&oppField, 0, 0) != FIELD_SQUARE_HIT) {
        testResult = TEST_FAIL;
    }
    if (FieldUpdateKnowledge(&oppField, &attackSmall2) != FIELD_SQUARE_UNKNOWN ||
            FieldGetSquareStatus(&oppField, 0, 1) != FIELD_SQUARE_HIT) {
        testResult = TEST_FAIL;
    }
    if (FieldUpdateKnowledge(&oppField, &attackSmall3) != FIELD_SQUARE_UNKNOWN ||
            FieldGetSquareStatus(&oppField, 0, 2) != FIELD_SQUARE_HIT ||
            oppField.smallBoatLives != 0) {
        testResult = TEST_FAIL;
    }
    // ------ attack a large boat until sink ------
    printf("test attack large boat until sink\n");
    GuessData attackLarge1 = {1, 0, RESULT_HIT};
    GuessData attackLarge2 = {1, 1, RESULT_HIT};
    GuessData attackLarge3 = {1, 2, RESULT_HIT};
    GuessData attackLarge4 = {1, 3, RESULT_HIT};
    GuessData attackLarge5 = {1, 4, RESULT_LARGE_BOAT_SUNK};
    if (FieldUpdateKnowledge(&oppField, &attackLarge1) != FIELD_SQUARE_UNKNOWN ||
            FieldGetSquareStatus(&oppField, 1, 0) != FIELD_SQUARE_HIT) {
        testResult = TEST_FAIL;
    }
    if (FieldUpdateKnowledge(&oppField, &attackLarge2) != FIELD_SQUARE_UNKNOWN ||
            FieldGetSquareStatus(&oppField, 1, 1) != FIELD_SQUARE_HIT) {
        testResult = TEST_FAIL;
    }
    if (FieldUpdateKnowledge(&oppField, &attackLarge3) != FIELD_SQUARE_UNKNOWN ||
            FieldGetSquareStatus(&oppField, 1, 2) != FIELD_SQUARE_HIT) {
        testResult = TEST_FAIL;
    }
    if (FieldUpdateKnowledge(&oppField, &attackLarge4) != FIELD_SQUARE_UNKNOWN ||
            FieldGetSquareStatus(&oppField, 1, 3) != FIELD_SQUARE_HIT) {
        testResult = TEST_FAIL;
    }
    if (FieldUpdateKnowledge(&oppField, &attackLarge5) != FIELD_SQUARE_UNKNOWN ||
            FieldGetSquareStatus(&oppField, 1, 4) != FIELD_SQUARE_HIT ||
            oppField.largeBoatLives != 0) {
        testResult = TEST_FAIL;
    }
    // ------ attack miss ------
    printf("test attack miss\n");
    GuessData attackMiss = {2, 0, RESULT_MISS};
    if (FieldUpdateKnowledge(&oppField, &attackMiss) != FIELD_SQUARE_UNKNOWN ||
            FieldGetSquareStatus(&oppField, 2, 0) != FIELD_SQUARE_EMPTY) {
        testResult = TEST_FAIL;
    }
    // ------ print attack result ------
    printf("Attack small boat at (0, 0), HIT\n"
            "Attack small boat at (0, 1), HIT\n"
            "Attack small boat at (0, 2), SMALL BOAT SINK\n"
            "Attack large boat at (1, 0), HIT\n"
            "Attack large boat at (1, 1), HIT\n"
            "Attack large boat at (1, 2), HIT\n"
            "Attack large boat at (1, 3), HIT\n"
            "Attack large boat at (1, 4), LARGE BOAT SINK\n"
            "Attack empty field at (2, 0), MISS\n");
    FieldPrint_UART(&ownField, &oppField);
    // ------ assert test result ------
    testResult == TEST_SUCCESS ? printf("FieldUpdateKnowledge() SUCCESS\n\n") :
            printf("FieldUpdateKnowledge() FAIL\n\n");

    printf("******************* FieldGetBoatStates() Unit Tests *******************\n");
    // ------ check ownField ------
    printf("check own field\n");
    if (FieldGetBoatStates(&ownField) != FIELD_BOAT_STATUS_SMALL) {
        testResult = TEST_FAIL;
    }
    // ------ check oppField ------
    printf("check opp field\n");
    if (FieldGetBoatStates(&oppField) != (FIELD_BOAT_STATUS_MEDIUM | FIELD_BOAT_STATUS_HUGE)) {
        testResult = TEST_FAIL;
    }
    // ------ assert test result ------
    testResult == TEST_SUCCESS ? printf("FieldGetBoatStates() SUCCESS\n\n") :
            printf("FieldGetBoatStates() FAIL\n\n");

    printf("******************* FieldAIPlaceAllBoats() Unit Tests *******************\n");
    // reset ownField to initial status
    printf("Initial Field for FieldAIPlaceAllBoats()\n Call FieldAIPlaceAllBoats()\n");
    FieldInit(&ownField, &oppField);
    if (FieldAIPlaceAllBoats(&ownField) != SUCCESS) {
        testResult = TEST_FAIL;
    }
    // ------ print add boat result ------
    FieldPrint_UART(&ownField, &oppField);
    // ------ assert test result ------
    testResult == TEST_SUCCESS ? printf("FieldAIPlaceAllBoats() SUCCESS\n\n") :
            printf("FieldAIPlaceAllBoats() FAIL\n\n");

    printf("******************* FieldAIDecideGuess() Unit Tests *******************\n");
    // reset ownField to initial status
    printf("Initial Field for FieldAIDecideGuess()\n");
    FieldInit(&ownField, &oppField);
    // assume all guess is correct.
    printf("assume all guess is HIT\n");
    GuessData aiGuess = {};
    BoatSize boat;
    // sink smallBoat
    for (boat = 0; boat < FIELD_BOAT_SIZE_SMALL - 1; ++boat) {
        aiGuess = FieldAIDecideGuess(&oppField);
        aiGuess.result = RESULT_HIT;
        FieldUpdateKnowledge(&oppField, &aiGuess);
    }
    aiGuess = FieldAIDecideGuess(&oppField);
    aiGuess.result = RESULT_SMALL_BOAT_SUNK;
    FieldUpdateKnowledge(&oppField, &aiGuess);
    // sink mediumBoat
    for (boat = 0; boat < FIELD_BOAT_SIZE_MEDIUM - 1; ++boat) {
        aiGuess = FieldAIDecideGuess(&oppField);
        aiGuess.result = RESULT_HIT;
        FieldUpdateKnowledge(&oppField, &aiGuess);
    }
    aiGuess = FieldAIDecideGuess(&oppField);
    aiGuess.result = RESULT_MEDIUM_BOAT_SUNK;
    FieldUpdateKnowledge(&oppField, &aiGuess);
    // sink largeBoat
    for (boat = 0; boat < FIELD_BOAT_SIZE_LARGE - 1; ++boat) {
        aiGuess = FieldAIDecideGuess(&oppField);
        aiGuess.result = RESULT_HIT;
        FieldUpdateKnowledge(&oppField, &aiGuess);
    }
    aiGuess = FieldAIDecideGuess(&oppField);
    aiGuess.result = RESULT_LARGE_BOAT_SUNK;
    FieldUpdateKnowledge(&oppField, &aiGuess);
    // sink hugeBoat
    for (boat = 0; boat < FIELD_BOAT_SIZE_HUGE - 1; ++boat) {
        aiGuess = FieldAIDecideGuess(&oppField);
        aiGuess.result = RESULT_HIT;
        FieldUpdateKnowledge(&oppField, &aiGuess);
    }
    aiGuess = FieldAIDecideGuess(&oppField);
    aiGuess.result = RESULT_HUGE_BOAT_SUNK;
    FieldUpdateKnowledge(&oppField, &aiGuess);
    if (FieldGetBoatStates(&oppField) != 0) {
        testResult = TEST_FAIL;
    }
    // ------ print attack result ------
    printf("OppField should have four sinked boat.\n");
    FieldPrint_UART(&ownField, &oppField);
    // ------ assert test result ------
    testResult == TEST_SUCCESS ? printf("FieldAIDecideGuess() SUCCESS\n\n") :
            printf("FieldAIDecideGuess() FAIL\n\n");

    while (1);
}
