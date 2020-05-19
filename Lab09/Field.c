/*
 * File:   Field.c
 * Author: Hang Yuan, hyuan3@ucsc.edu
 *         Yujia Li, yli302@ucsc.edu
 * 
 * Filed.c is implemented by Yujia Li
 *
 */


// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Field.h"

#define SIZE_OF_BOAT_DIRECTION 2
#define TRY_ONE_STEP 1

enum {
    EAST = 0,
    SOUTH,
    WEST,
    NORTH,
};

// this struct for store data related to FieldAIDecideGuess.

typedef struct {
    // if there is a square which waits for asserting attack result, this variable is TRUE.
    uint8_t waitForCheck;
    // store TRUE, if squares which is hit but not sinked.
    uint8_t grid[FIELD_ROWS][FIELD_COLS];
    // if FieldAIDecideGuess hit a boat, store the direction for this try.
    // for example, 1. (0, 0) hit, try (0, 1) 
    //              2. (0, 1) hit, try (0, 2) store EAST to dir.
    uint8_t dir;
    // FieldAIDecideGuess return this row and store this row at local for check next time.
    uint8_t checkRow;
    // FieldAIDecideGuess return this col and store this col at local for check next time.
    uint8_t checkCol;
    uint8_t smallBoatLives;
    uint8_t mediumBoatLives;
    uint8_t largeBoatLives;
    uint8_t hugeBoatLives;
} AiGuess;

static AiGuess aiGuess;

void FieldPrint_UART(Field *own_field, Field * opp_field)
{
    printf("    0 1 2 3 4 5 6 7 8 9          0 1 2 3 4 5 6 7 8 9\n");
    printf("   ---------------------        --------------------- \n");
    int r, c;
    for (r = 0; r < FIELD_ROWS; ++r) {
        // print own field
        printf("%d [ ", r);
        for (c = 0; c < FIELD_COLS; ++c) {
            switch (FieldGetSquareStatus(own_field, r, c)) {
            case FIELD_SQUARE_EMPTY:
                printf("  ");
                break;
            case FIELD_SQUARE_SMALL_BOAT:
                printf("3 ");
                break;
            case FIELD_SQUARE_MEDIUM_BOAT:
                printf("4 ");
                break;
            case FIELD_SQUARE_LARGE_BOAT:
                printf("5 ");
                break;
            case FIELD_SQUARE_HUGE_BOAT:
                printf("6 ");
                break;
            case FIELD_SQUARE_UNKNOWN:
                printf("U ");
                break;
            case FIELD_SQUARE_HIT:
                printf("X ");
                break;
            case FIELD_SQUARE_MISS:
                printf("M ");
                break;
            case FIELD_SQUARE_CURSOR:
            case FIELD_SQUARE_INVALID:
                break;
            }
        }
        printf("]    %d [ ", r);
        // print opp field
        for (c = 0; c < FIELD_COLS; ++c) {
            switch (FieldGetSquareStatus(opp_field, r, c)) {
            case FIELD_SQUARE_EMPTY:
                printf("  ");
                break;
            case FIELD_SQUARE_SMALL_BOAT:
                printf("3 ");
                break;
            case FIELD_SQUARE_MEDIUM_BOAT:
                printf("4 ");
                break;
            case FIELD_SQUARE_LARGE_BOAT:
                printf("5 ");
                break;
            case FIELD_SQUARE_HUGE_BOAT:
                printf("6 ");
                break;
            case FIELD_SQUARE_UNKNOWN:
                printf(". ");
                break;
            case FIELD_SQUARE_HIT:
                printf("X ");
                break;
            case FIELD_SQUARE_MISS:
                printf("M ");
                break;
            case FIELD_SQUARE_CURSOR:
            case FIELD_SQUARE_INVALID:
                break;
            }
        }
        printf("]\n");
    }
    printf("   ---------------------        --------------------- \n");
}

void FieldInit(Field *own_field, Field * opp_field)
{
    int r, c;
    // initial field's grid for both field
    for (r = 0; r < FIELD_ROWS; ++r) {
        for (c = 0; c < FIELD_COLS; ++c) {
            own_field->grid[r][c] = FIELD_SQUARE_EMPTY;
            opp_field->grid[r][c] = FIELD_SQUARE_UNKNOWN;
        }
    }
    // initial ownField's boat status
    own_field->smallBoatLives = 0;
    own_field->mediumBoatLives = 0;
    own_field->largeBoatLives = 0;
    own_field->hugeBoatLives = 0;
    // initial oppField's boat status
    opp_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
    opp_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
    opp_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
    opp_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;

    // initial struct for AIDecideGuess.
    aiGuess.waitForCheck = FALSE;
    for (r = 0; r < FIELD_ROWS; ++r) {
        for (c = 0; c < FIELD_COLS; ++c) {
            aiGuess.grid[r][c] = FALSE;
        }
    }
    aiGuess.dir = EAST;
    aiGuess.checkRow = 0;
    aiGuess.checkCol = 0;
    aiGuess.smallBoatLives = FIELD_BOAT_SIZE_SMALL;
    aiGuess.mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
    aiGuess.largeBoatLives = FIELD_BOAT_SIZE_LARGE;
    aiGuess.hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
}

SquareStatus FieldGetSquareStatus(const Field *f, uint8_t row, uint8_t col)
{
    SquareStatus squareStatus;
    // check for invalid row or col
    if (row >= FIELD_ROWS || col >= FIELD_COLS) {
        squareStatus = FIELD_SQUARE_INVALID;
    } else {
        squareStatus = f->grid[row][col];
    }
    return squareStatus;
}

SquareStatus FieldSetSquareStatus(Field *f, uint8_t row, uint8_t col, SquareStatus p)
{
    SquareStatus squareStatus;
    // check for invalid row or col
    if (row >= FIELD_ROWS || col >= FIELD_COLS) {
        squareStatus = FIELD_SQUARE_INVALID;
    } else {
        squareStatus = f->grid[row][col];
        f->grid[row][col] = p;
    }
    return squareStatus;
}

uint8_t FieldAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType boat_type)
{
    BoatSize boatSize, boat_nr;
    SquareStatus boatInSquare;
    uint8_t currRow, currCol;
    // set boat size, and square type
    switch (boat_type) {
    case FIELD_BOAT_TYPE_SMALL:
        boatSize = FIELD_BOAT_SIZE_SMALL;
        boatInSquare = FIELD_SQUARE_SMALL_BOAT;
        break;
    case FIELD_BOAT_TYPE_MEDIUM:
        boatSize = FIELD_BOAT_SIZE_MEDIUM;
        boatInSquare = FIELD_SQUARE_MEDIUM_BOAT;
        break;
    case FIELD_BOAT_TYPE_LARGE:
        boatSize = FIELD_BOAT_SIZE_LARGE;
        boatInSquare = FIELD_SQUARE_LARGE_BOAT;
        break;
    case FIELD_BOAT_TYPE_HUGE:
        boatSize = FIELD_BOAT_SIZE_HUGE;
        boatInSquare = FIELD_SQUARE_HUGE_BOAT;
        break;
    }
    // add boat, if boat can fit in the desired position, and return SUCCESS 
    // Otherwise return STANDARD_ERROR
    switch (dir) {
    case FIELD_DIR_SOUTH:
        // check whether desired square is valid
        if (col >= FIELD_COLS) {
            return STANDARD_ERROR;
        }
        for (boat_nr = 0; boat_nr < boatSize; ++boat_nr) {
            currRow = row + boat_nr;
            if (currRow >= FIELD_ROWS ||
                    FieldGetSquareStatus(f, currRow, col) != FIELD_SQUARE_EMPTY) {
                return STANDARD_ERROR;
            }
        }
        // desired square is valid, add boat
        for (boat_nr = 0; boat_nr < boatSize; ++boat_nr) {
            currRow = row + boat_nr;
            FieldSetSquareStatus(f, currRow, col, boatInSquare);
            switch (boat_type) {
            case FIELD_BOAT_TYPE_SMALL:
                f->smallBoatLives = boatSize;
                break;
            case FIELD_BOAT_TYPE_MEDIUM:
                f->mediumBoatLives = boatSize;
                break;
            case FIELD_BOAT_TYPE_LARGE:
                f->largeBoatLives = boatSize;
                break;
            case FIELD_BOAT_TYPE_HUGE:
                f->hugeBoatLives = boatSize;
                break;
            }
        }
        break;
    case FIELD_DIR_EAST:
        // check whether desired square is valid
        if (row >= FIELD_ROWS) {
            return STANDARD_ERROR;
        }
        for (boat_nr = 0; boat_nr < boatSize; ++boat_nr) {
            currCol = col + boat_nr;
            if (currCol >= FIELD_COLS ||
                    FieldGetSquareStatus(f, row, currCol) != FIELD_SQUARE_EMPTY) {
                return STANDARD_ERROR;
            }
        }
        // desired square is valid, add boat
        for (boat_nr = 0; boat_nr < boatSize; ++boat_nr) {
            currCol = col + boat_nr;
            FieldSetSquareStatus(f, row, currCol, boatInSquare);
            switch (boat_type) {
            case FIELD_BOAT_TYPE_SMALL:
                f->smallBoatLives = boatSize;
                break;
            case FIELD_BOAT_TYPE_MEDIUM:
                f->mediumBoatLives = boatSize;
                break;
            case FIELD_BOAT_TYPE_LARGE:
                f->largeBoatLives = boatSize;
                break;
            case FIELD_BOAT_TYPE_HUGE:
                f->hugeBoatLives = boatSize;
                break;
            }
        }
        break;
    }
    return SUCCESS;
}

SquareStatus FieldRegisterEnemyAttack(Field *f, GuessData *gData)
{
    SquareStatus squareStatus;
    // check square status of guess square
    squareStatus = FieldGetSquareStatus(f, gData->row, gData->col);
    // 1. update field
    // 2. update boatLives
    // 3. update GuessData
    switch (squareStatus) {
    case FIELD_SQUARE_EMPTY:
        FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_MISS);
        gData->result = RESULT_MISS;
        break;
    case FIELD_SQUARE_SMALL_BOAT:
        FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        if (--f->smallBoatLives) {
            gData->result = RESULT_HIT;
        } else {
            gData->result = RESULT_SMALL_BOAT_SUNK;
        }
        break;
    case FIELD_SQUARE_MEDIUM_BOAT:
        FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        if (--f->mediumBoatLives) {
            gData->result = RESULT_HIT;
        } else {
            gData->result = RESULT_MEDIUM_BOAT_SUNK;
        }
        break;
    case FIELD_SQUARE_LARGE_BOAT:
        FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        if (--f->largeBoatLives) {
            gData->result = RESULT_HIT;
        } else {
            gData->result = RESULT_LARGE_BOAT_SUNK;
        }
        break;
    case FIELD_SQUARE_HUGE_BOAT:
        FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        if (--f->smallBoatLives) {
            gData->result = RESULT_HIT;
        } else {
            gData->result = RESULT_HUGE_BOAT_SUNK;
        }
        break;
    case FIELD_SQUARE_UNKNOWN:
    case FIELD_SQUARE_HIT:
    case FIELD_SQUARE_MISS:
    case FIELD_SQUARE_CURSOR:
    case FIELD_SQUARE_INVALID:
        break;
    }
    return squareStatus;
}

SquareStatus FieldUpdateKnowledge(Field *f, const GuessData *gData)
{
    SquareStatus squareStatus;
    // check square status of guess square
    squareStatus = FieldGetSquareStatus(f, gData->row, gData->col);
    // 1. update field
    // 2. update boatLives
    switch (gData->result) {
    case RESULT_MISS:
        FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_EMPTY);
        break;
    case RESULT_HIT:
        FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        break;
    case RESULT_SMALL_BOAT_SUNK:
        FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        f->smallBoatLives = 0; // small boat sink
        break;
    case RESULT_MEDIUM_BOAT_SUNK:
        FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        f->mediumBoatLives = 0; // medium boat sink
        break;
    case RESULT_LARGE_BOAT_SUNK:
        FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        f->largeBoatLives = 0; // large boat sink
        break;
    case RESULT_HUGE_BOAT_SUNK:
        FieldSetSquareStatus(f, gData->row, gData->col, FIELD_SQUARE_HIT);
        f->hugeBoatLives = 0; // huge boat sink
        break;
    }
    return squareStatus;
}

uint8_t FieldGetBoatStates(const Field *f)
{
    uint8_t boatStatus = 0x00;
    if (f->smallBoatLives) {
        boatStatus |= FIELD_BOAT_STATUS_SMALL;
    }
    if (f->mediumBoatLives) {
        boatStatus |= FIELD_BOAT_STATUS_MEDIUM;
    }
    if (f->largeBoatLives) {
        boatStatus |= FIELD_BOAT_STATUS_LARGE;
    }
    if (f->hugeBoatLives) {
        boatStatus |= FIELD_BOAT_STATUS_HUGE;
    }
    return boatStatus;
}

uint8_t FieldAIPlaceAllBoats(Field *f)
{
    int row, col;
    BoatDirection dir;
    // add huge boat
    while (1) {
        // row from random number range 0~0
        // col from random number range 0~4
        // col from random number range 0~1
        row = rand() % (FIELD_ROWS - FIELD_BOAT_TYPE_HUGE);
        col = rand() % (FIELD_COLS - FIELD_BOAT_TYPE_HUGE);
        dir = rand() % SIZE_OF_BOAT_DIRECTION;
        if (FieldAddBoat(f, row, col, dir, FIELD_BOAT_TYPE_HUGE)) {
            break;
        }
    }
    // add large boat
    while (1) {
        // row from random number range 0~1
        // col from random number range 0~5
        // col from random number range 0~1
        row = rand() % (FIELD_ROWS - FIELD_BOAT_TYPE_LARGE);
        col = rand() % (FIELD_COLS - FIELD_BOAT_TYPE_LARGE);
        dir = rand() % SIZE_OF_BOAT_DIRECTION;
        if (FieldAddBoat(f, row, col, dir, FIELD_BOAT_TYPE_LARGE)) {
            break;
        }
    }
    // add medium boat
    while (1) {
        // row from random number range 0~2
        // col from random number range 0~6
        // col from random number range 0~1
        row = rand() % (FIELD_ROWS - FIELD_BOAT_TYPE_MEDIUM);
        col = rand() % (FIELD_COLS - FIELD_BOAT_TYPE_MEDIUM);
        dir = rand() % SIZE_OF_BOAT_DIRECTION;
        if (FieldAddBoat(f, row, col, dir, FIELD_BOAT_TYPE_MEDIUM)) {
            break;
        }
    }
    // add small boat
    while (1) {
        // row from random number range 0~3
        // col from random number range 0~7
        // col from random number range 0~1
        row = rand() % (FIELD_ROWS - FIELD_BOAT_TYPE_SMALL);
        col = rand() % (FIELD_COLS - FIELD_BOAT_TYPE_SMALL);
        dir = rand() % SIZE_OF_BOAT_DIRECTION;
        if (FieldAddBoat(f, row, col, dir, FIELD_BOAT_TYPE_SMALL)) {
            break;
        }
    }
    return SUCCESS;
}

/* Help function for FieldAIDecideGuess()
 * this function find a square whose status is hit but not sinked
 **/
void FindHitNotSinkSquare(uint8_t * row, uint8_t * col)
{
    int r, c;
    int flag = FALSE;
    for (r = 0; r < FIELD_ROWS && flag != TRUE; ++r) {
        for (c = 0; c < FIELD_COLS && flag != TRUE; ++c) {
            if (aiGuess.grid[r][c]) {
                *row = r;
                *col = c;
                flag = TRUE;
            }
        }
    }
}

/* Help function for FieldAIDecideGuess()
 * this function return TRUE if exist squares whose status is hit but not sinked 
 **/
int HitNotSinkSquareExist()
{
    int rc = FALSE;
    int r, c;
    for (r = 0; r < FIELD_ROWS; ++r) {
        for (c = 0; c < FIELD_COLS; ++c) {
            if (aiGuess.grid[r][c]) {
                rc = TRUE;
            }
        }
    }
    return rc;
}

/* Help function for FieldAIDecideGuess()
 * Find a square near a hit but not sinked square. (means try EAST or SOUTH or WEST or NORTH)
 * Then return the guessData of this square (contain row and col, without result)
 **/
GuessData FindSquareNear(const Field *f, const uint8_t row, const uint8_t col)
{
    GuessData guessData;
    // try east
    if (FieldGetSquareStatus(f, row, col + TRY_ONE_STEP) == FIELD_SQUARE_UNKNOWN) {
        aiGuess.checkRow = row;
        aiGuess.checkCol = col + TRY_ONE_STEP;
        guessData.row = row;
        guessData.col = col + TRY_ONE_STEP;
        aiGuess.dir = EAST;
    }// try south
    else if (FieldGetSquareStatus(f, row + TRY_ONE_STEP, col) == FIELD_SQUARE_UNKNOWN) {
        aiGuess.checkRow = row + TRY_ONE_STEP;
        aiGuess.checkCol = col;
        guessData.row = row + TRY_ONE_STEP;
        guessData.col = col;
        aiGuess.dir = SOUTH;
    }// try west
    else if (FieldGetSquareStatus(f, row, col - TRY_ONE_STEP) == FIELD_SQUARE_UNKNOWN) {
        aiGuess.checkRow = row;
        aiGuess.checkCol = col - TRY_ONE_STEP;
        guessData.row = row;
        guessData.col = col - TRY_ONE_STEP;
        aiGuess.dir = WEST;
    }// try north
    else if (FieldGetSquareStatus(f, row - TRY_ONE_STEP, col) == FIELD_SQUARE_UNKNOWN) {
        aiGuess.checkRow = row - TRY_ONE_STEP;
        aiGuess.checkCol = col;
        guessData.row = row - TRY_ONE_STEP;
        guessData.col = col;
        aiGuess.dir = NORTH;
    }
    return guessData;
}

/* Help function for FieldAIDecideGuess()
 * try another square if no hit but not sink square exist.
 * assume all attempt missed (this will not happen), field will looks like this
 *  ---------------------
 * | m . . m . . m . . m |
 * | . m . . m . . m . . |
 * | . . m . . m . . m . |
 * | m . . m . . m . . m |
 * | . m . . m . . m . . |
 * | . . m . . m . . m . |
 *  ---------------------
 **/
GuessData TryRandomSquare(const Field *f)
{
    GuessData guessData;
    uint8_t skipValue = FIELD_BOAT_SIZE_SMALL;
    int r = 0;
    int c = 0;
    for (r = 0; r < FIELD_ROWS; ++r) {
        // calculate start col for each row. see above example diagram
        c = 0;
        if ((c += r) >= skipValue) {
            c -= skipValue;
        }
        for (; c < FIELD_COLS; c += skipValue) {
            if (FieldGetSquareStatus(f, r, c) == FIELD_SQUARE_UNKNOWN) {
                aiGuess.checkRow = r;
                aiGuess.checkCol = c;
                guessData.row = r;
                guessData.col = c;
                return guessData;
            }
        }
    }
    return guessData;
}

GuessData FieldAIDecideGuess(const Field *f)
{
    GuessData guessData;
    uint8_t currentRow = 0, currentCol = 0;
    int i;

    // check the previse FieldAIDecideGuess's guessdata first
    switch (FieldGetSquareStatus(f, aiGuess.checkRow, aiGuess.checkCol)) {
    case FIELD_SQUARE_UNKNOWN:
        // this is the initial field. try (0, 0)
        guessData.row = 0;
        guessData.col = 0;
        aiGuess.checkRow = 0;
        aiGuess.checkCol = 0;
        break;
    case FIELD_SQUARE_EMPTY:
        // previous try is missed. 
        // if a hit but not sink square exist, try square near this one.
        // 
        if (HitNotSinkSquareExist()) {
            // find a squares which is hit but not sinked
            FindHitNotSinkSquare(&currentRow, &currentCol);
            // find a square near a hit but not sinked square (EAST, SOUTH, WEST, NORTH)
            guessData = FindSquareNear(f, currentRow, currentCol);
        } else {
            // try another random square
            guessData = TryRandomSquare(f);
        }
        break;
    case FIELD_SQUARE_HIT:
        // small boat sink, mark small boat's squares as sink, and try next square
        if (aiGuess.smallBoatLives != f->smallBoatLives) {
            aiGuess.smallBoatLives = f->smallBoatLives;
            for (i = 0; i < FIELD_BOAT_SIZE_SMALL; ++i) {
                if (aiGuess.dir == EAST) {
                    aiGuess.grid[aiGuess.checkRow][aiGuess.checkCol - i] = FALSE;
                } else if (aiGuess.dir == SOUTH) {
                    aiGuess.grid[aiGuess.checkRow - i][aiGuess.checkCol] = FALSE;
                } else if (aiGuess.dir == WEST) {
                    aiGuess.grid[aiGuess.checkRow][aiGuess.checkCol + i] = FALSE;
                } else if (aiGuess.dir == NORTH) {
                    aiGuess.grid[aiGuess.checkRow + i][aiGuess.checkCol] = FALSE;
                }
            }
            aiGuess.dir = EAST;
            if (HitNotSinkSquareExist()) {
                // find a squares which is hit but not sinked
                FindHitNotSinkSquare(&currentRow, &currentCol);
                // find a square near a hit but not sinked square (EAST, SOUTH, WEST, NORTH)
                guessData = FindSquareNear(f, currentRow, currentCol);
            } else {
                guessData = TryRandomSquare(f);
            }
        } else if (aiGuess.mediumBoatLives != f->mediumBoatLives) {
            // medium boat sink, mark small boat's squares as sink, and try next square
            aiGuess.mediumBoatLives = f->mediumBoatLives;
            for (i = 0; i < FIELD_BOAT_SIZE_MEDIUM; ++i) {
                if (aiGuess.dir == EAST) {
                    aiGuess.grid[aiGuess.checkRow][aiGuess.checkCol - i] = FALSE;
                } else if (aiGuess.dir == SOUTH) {
                    aiGuess.grid[aiGuess.checkRow - i][aiGuess.checkCol] = FALSE;
                } else if (aiGuess.dir == WEST) {
                    aiGuess.grid[aiGuess.checkRow][aiGuess.checkCol + i] = FALSE;
                } else if (aiGuess.dir == NORTH) {
                    aiGuess.grid[aiGuess.checkRow + i][aiGuess.checkCol] = FALSE;
                }
            }
            aiGuess.dir = EAST;
            if (HitNotSinkSquareExist()) {
                // find a squares which is hit but not sinked
                FindHitNotSinkSquare(&currentRow, &currentCol);
                // find a square near a hit but not sinked square (EAST, SOUTH, WEST, NORTH)
                guessData = FindSquareNear(f, currentRow, currentCol);
            } else {
                guessData = TryRandomSquare(f);
            }
        } else if (aiGuess.largeBoatLives != f->largeBoatLives) {
            // large boat sink, mark small boat's squares as sink, and try next square
            aiGuess.largeBoatLives = f->largeBoatLives;
            for (i = 0; i < FIELD_BOAT_SIZE_LARGE; ++i) {
                if (aiGuess.dir == EAST) {
                    aiGuess.grid[aiGuess.checkRow][aiGuess.checkCol - i] = FALSE;
                } else if (aiGuess.dir == SOUTH) {
                    aiGuess.grid[aiGuess.checkRow - i][aiGuess.checkCol] = FALSE;
                } else if (aiGuess.dir == WEST) {
                    aiGuess.grid[aiGuess.checkRow][aiGuess.checkCol + i] = FALSE;
                } else if (aiGuess.dir == NORTH) {
                    aiGuess.grid[aiGuess.checkRow + i][aiGuess.checkCol] = FALSE;
                }
            }
            aiGuess.dir = EAST;
            if (HitNotSinkSquareExist()) {
                // find a squares which is hit but not sinked
                FindHitNotSinkSquare(&currentRow, &currentCol);
                // find a square near a hit but not sinked square (EAST, SOUTH, WEST, NORTH)
                guessData = FindSquareNear(f, currentRow, currentCol);
            } else {
                guessData = TryRandomSquare(f);
            }
        } else if (aiGuess.hugeBoatLives != f->hugeBoatLives) {
            // huge boat sink, mark small boat's squares as sink, and try next square
            aiGuess.hugeBoatLives = f->hugeBoatLives;
            for (i = 0; i < FIELD_BOAT_SIZE_HUGE; ++i) {
                if (aiGuess.dir == EAST) {
                    aiGuess.grid[aiGuess.checkRow][aiGuess.checkCol - i] = FALSE;
                } else if (aiGuess.dir == SOUTH) {
                    aiGuess.grid[aiGuess.checkRow - i][aiGuess.checkCol] = FALSE;
                } else if (aiGuess.dir == WEST) {
                    aiGuess.grid[aiGuess.checkRow][aiGuess.checkCol + i] = FALSE;
                } else if (aiGuess.dir == NORTH) {
                    aiGuess.grid[aiGuess.checkRow + i][aiGuess.checkCol] = FALSE;
                }
            }
            aiGuess.dir = EAST;
            if (HitNotSinkSquareExist()) {
                // find a squares which is hit but not sinked
                FindHitNotSinkSquare(&currentRow, &currentCol);
                // find a square near a hit but not sinked square (EAST, SOUTH, WEST, NORTH)
                guessData = FindSquareNear(f, currentRow, currentCol);
            } else {
                guessData = TryRandomSquare(f);
            }
        } else {
            // hit without any sink, set aiGuess grid as TRUE
            aiGuess.grid[aiGuess.checkRow][aiGuess.checkCol] = TRUE;
            // find next square on the last direction.
            switch (aiGuess.dir) {
            case EAST:
                if (FieldGetSquareStatus(f, aiGuess.checkRow, aiGuess.checkCol + TRY_ONE_STEP)
                        == FIELD_SQUARE_UNKNOWN) {
                    aiGuess.checkCol += TRY_ONE_STEP;
                    guessData.row = aiGuess.checkRow;
                    guessData.col = aiGuess.checkCol;
                } else {
                    // find a squares which is hit but not sinked
                    FindHitNotSinkSquare(&currentRow, &currentCol);
                    // find a square near a hit but not sinked square (EAST, SOUTH, WEST, NORTH)
                    guessData = FindSquareNear(f, currentRow, currentCol);
                }
                break;
            case SOUTH:
                if (FieldGetSquareStatus(f, aiGuess.checkRow + TRY_ONE_STEP, aiGuess.checkCol)
                        == FIELD_SQUARE_UNKNOWN) {
                    aiGuess.checkRow += TRY_ONE_STEP;
                    guessData.row = aiGuess.checkRow;
                    guessData.col = aiGuess.checkCol;
                } else {
                    // find a squares which is hit but not sinked
                    FindHitNotSinkSquare(&currentRow, &currentCol);
                    // find a square near a hit but not sinked square (EAST, SOUTH, WEST, NORTH)
                    guessData = FindSquareNear(f, currentRow, currentCol);
                }
                break;
            case WEST:
                if (FieldGetSquareStatus(f, aiGuess.checkRow, aiGuess.checkCol - TRY_ONE_STEP)
                        == FIELD_SQUARE_UNKNOWN) {
                    aiGuess.checkCol -= TRY_ONE_STEP;
                    guessData.row = aiGuess.checkRow;
                    guessData.col = aiGuess.checkCol;
                } else {
                    // find a squares which is hit but not sinked
                    FindHitNotSinkSquare(&currentRow, &currentCol);
                    // find a square near a hit but not sinked square (EAST, SOUTH, WEST, NORTH)
                    guessData = FindSquareNear(f, currentRow, currentCol);
                }
                break;
            case NORTH:
                if (FieldGetSquareStatus(f, aiGuess.checkRow - TRY_ONE_STEP, aiGuess.checkCol)
                        == FIELD_SQUARE_UNKNOWN) {
                    aiGuess.checkRow -= TRY_ONE_STEP;
                    guessData.row = aiGuess.checkRow;
                    guessData.col = aiGuess.checkCol;
                } else {
                    // find a squares which is hit but not sinked
                    FindHitNotSinkSquare(&currentRow, &currentCol);
                    // find a square near a hit but not sinked square (EAST, SOUTH, WEST, NORTH)
                    guessData = FindSquareNear(f, currentRow, currentCol);
                }
                break;
            }
        }
        break;
    default:
        break;
    }
    return guessData;
}