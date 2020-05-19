/*
 * Name: Hang Yuna
 * StdID: 1564348
 * CruzID: hyuan3
 */

// Standard libraries
#include <stdio.h>
#include <string.h>

//CMPE13 Support Library
#include "UNIXBOARD.h"

// User libraries
#include "Game.h"
#include "Player.h"

// **** Set any macros or preprocessor directives here ****
#define FALSE             0
#define TRUE              1
#define ONE_BYTE          1
#define RPG_OFFSET        3
#define DEFAULT_SIZE      5
#define FILE_NAME_SIZE    21
#define MAX_ROOM_NUMBER   65
#define WINDOW_SIZE       60
#define FILE_NAME_HEADER "RoomFiles/room"

// **** Declare any data types here ****
typedef uint8_t Item;
typedef uint8_t Exit;
typedef Item*   pItem;

typedef struct Room {
    char title[GAME_MAX_ROOM_TITLE_LENGTH + ONE_BYTE];
    Item itemRequired[INVENTORY_SIZE];
    char description[GAME_MAX_ROOM_DESC_LENGTH + ONE_BYTE];
    Exit north;
    Exit east;
    Exit south;
    Exit west;
} Room;

// **** Define any global or external variables here ****
FILE *fp;
static Room room;

// **** Declare any function prototypes here ****
uint8_t currentRoomNumber;
int SetFilePointer(uint8_t roomNumber);
void CloseFilePointer(void);
int CheckEveryItem (void);
int GetRoomProperty(uint8_t roomNumber);
void ResetAllInRoom (void);

// Transite room - North
int GameGoNorth(void)
{
    int returnCode = STANDARD_ERROR;
    uint8_t exit = GameGetCurrentRoomExits();
    if (exit && GAME_ROOM_EXIT_NORTH_EXISTS) {
        returnCode = SUCCESS;
        uint8_t roomNumber = room.north;
        GetRoomProperty(roomNumber);
    }
    return returnCode;
}

// Transite room - East
int GameGoEast(void)
{
    int returnCode = STANDARD_ERROR;
    uint8_t exit = GameGetCurrentRoomExits();
    if (exit && GAME_ROOM_EXIT_EAST_EXISTS) {
        returnCode = SUCCESS;
        uint8_t roomNumber = room.east;
        GetRoomProperty(roomNumber);
    }
    return returnCode;
}

// Transite room - South
int GameGoSouth(void)
{
    int returnCode = STANDARD_ERROR;
    uint8_t exit = GameGetCurrentRoomExits();
    if (exit & GAME_ROOM_EXIT_SOUTH_EXISTS) {
        returnCode = SUCCESS;
        uint8_t roomNumber = room.south;
        if (GetRoomProperty(roomNumber) != SUCCESS) {
            returnCode = STANDARD_ERROR;
        }
    }
    return returnCode;
}

// Transite room - West
int GameGoWest(void)
{
    int returnCode = STANDARD_ERROR;
    uint8_t exit = GameGetCurrentRoomExits();
    if (exit && GAME_ROOM_EXIT_WEST_EXISTS) {
        returnCode = SUCCESS;
        uint8_t roomNumber = room.west;
        GetRoomProperty(roomNumber);
    }
    return returnCode;
}

//This function sets up anything that needs to happen at the start of the game.
int GameInit(void)
{
    int returnCode = GetRoomProperty(STARTING_ROOM);
    if (returnCode == STANDARD_ERROR) {
        return returnCode;
    }
    return returnCode;
}

// Copies the current room title as a NULL-terminated string.
int GameGetCurrentRoomTitle(char *title)
{
    memcpy(title, room.title, strlen(room.title));
    title[strlen(room.title)] = '\0';
    return strlen(room.title) + 1;
}

// Copy the description of the current room into the argument desc as a string
int GameGetCurrentRoomDescription(char *desc)
{
    char * dst = room.description;
    ssize_t total_length = strlen(dst);
    ssize_t rem_length = total_length;
    ssize_t cur_length = 0;
    ssize_t copy_length = 0;

    // Add '\n' char every 80(WINDOW_SIZE) chars
    while (rem_length > 0) {
        if (rem_length > WINDOW_SIZE) {
            memcpy(desc + copy_length, dst + cur_length, WINDOW_SIZE);
            cur_length += WINDOW_SIZE;
            copy_length += WINDOW_SIZE;
            desc[copy_length] = '\n';
            rem_length -= WINDOW_SIZE;
            ++copy_length;
        } else {
            memcpy(desc + copy_length, dst + cur_length, rem_length);
            copy_length += rem_length;
            rem_length = 0;
            desc[copy_length] = '\0';
        }
    }

    return cur_length;
}

// Returns the exits from the current room
uint8_t GameGetCurrentRoomExits(void)
{
    uint8_t exit = 0;
    if (room.north != 0) {
        exit |= GAME_ROOM_EXIT_NORTH_EXISTS;
    }
    if (room.east != 0) {
        exit |= GAME_ROOM_EXIT_EAST_EXISTS;
    }
    if (room.south != 0) {
        exit |= GAME_ROOM_EXIT_SOUTH_EXISTS;
    }
    if (room.west != 0) {
        exit |= GAME_ROOM_EXIT_WEST_EXISTS;
    }
    return exit;
}

// Get the file's pointer based on the given roomNumber
int SetFilePointer (uint8_t roomNumber)
{
    int returnCode = STANDARD_ERROR;
    CloseFilePointer();
    char fileName[FILE_NAME_SIZE];
    sprintf(fileName, "%s%d.txt%c", FILE_NAME_HEADER, roomNumber, '\0');
    fp = fopen(fileName, "r");
    if (fp != NULL) {
        returnCode = SUCCESS;
    }

    return returnCode;
}

// Close the file pointer
void CloseFilePointer (void)
{
    if (fp != NULL){
        fclose(fp);
    }
}

// Obtain all properties we need in the room file
int GetRoomProperty(uint8_t roomNumber)
{
    // Get the file pointer first
    int returnCode;
    returnCode = SetFilePointer(roomNumber);
    if (returnCode != SUCCESS) {
        return returnCode;
    }

    // Reset all elements in current Room
    memset(&room, 0, sizeof(Room));

    // Get room title
    size_t cur_length = RPG_OFFSET;
    uint8_t length = 0;
    fseek(fp, RPG_OFFSET, SEEK_SET);
    cur_length += fread(&length, ONE_BYTE, ONE_BYTE, fp);
    cur_length += fread(&room.title, ONE_BYTE, length, fp);
    room.title[length] = '\0';

    // Get the appropriate properties of the room
    while(!feof(fp)){
        // Get item requirment
        cur_length += fread(&length, ONE_BYTE, ONE_BYTE, fp);
        cur_length += length;
        size_t index = 0;
        while (length > 0) {
            fread(&room.itemRequired[index], ONE_BYTE, ONE_BYTE, fp);
            ++index;
            --length;
        }

        // Check if we have all required items
        if (CheckEveryItem() == STANDARD_ERROR) {
            cur_length += fread(&length, ONE_BYTE, ONE_BYTE, fp);
            cur_length += length;
            fseek(fp, cur_length, SEEK_SET);
            cur_length += fread(&length, ONE_BYTE, ONE_BYTE, fp);
            cur_length += length + 4;
            fseek(fp, cur_length, SEEK_SET);
            ResetAllInRoom();
            continue;
        }

        // Get description
        cur_length += fread(&length, ONE_BYTE, ONE_BYTE, fp);
        cur_length += fread(&room.description, ONE_BYTE, length, fp);
        room.description[length] = '\0';

        // Get item contained
        cur_length += fread(&length, ONE_BYTE, ONE_BYTE, fp);
        cur_length += length;
        while (length > 0) {
            Item item = fgetc(fp);
            if (AddToInventory(item) != SUCCESS) {
                return STANDARD_ERROR;
            }
            --length;
        }

        // Get exits
        cur_length += fread(&room.north, ONE_BYTE, ONE_BYTE, fp);
        cur_length += fread(&room.east, ONE_BYTE, ONE_BYTE, fp);
        cur_length += fread(&room.south, ONE_BYTE, ONE_BYTE, fp);
        cur_length += fread(&room.west, ONE_BYTE, ONE_BYTE, fp);
        break;
    }
    return SUCCESS;
}

int CheckEveryItem (void)
{
    int returnCode = SUCCESS;
    size_t index = 0;
    while (index < INVENTORY_SIZE) {
        if (FindInInventory(room.itemRequired[index]) == STANDARD_ERROR) {
            returnCode = STANDARD_ERROR;
            break;
        }
        ++index;
    }
    return returnCode;
}

void ResetAllInRoom (void) {
    room.itemRequired[0] = 0;
    room.itemRequired[1] = 0;
    room.itemRequired[2] = 0;
    room.itemRequired[4] = 0;
}
