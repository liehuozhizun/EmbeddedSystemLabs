// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <math.h>
#include <stdio.h>

//CMPE13 Support Library
#include "UNIXBOARD.h"

// User libraries
#include "Game.h"

// **** Set any macros or preprocessor directives here ****
#define ONE_BYTE   1
#define MAX_OPTION 10
// **** Declare any data types here ****

// **** Define any global or external variables here ****

// **** Declare any function prototypes here ****
void GenerateOptions(uint8_t exit_status, char *result);

int main()
{
   // Initialize Game
   int returnCode;
   returnCode = GameInit();
   if (returnCode != SUCCESS) {
       FATAL_ERROR();
   }

   while (1) {
       // Print prompts
       char title[GAME_MAX_ROOM_TITLE_LENGTH + ONE_BYTE] = "";
       GameGetCurrentRoomTitle(title);
       printf("\n----------------- %s -----------------\n\n", title);
       char desc[GAME_MAX_ROOM_DESC_LENGTH + 4 * ONE_BYTE] = "";
       GameGetCurrentRoomDescription(desc);
       printf("%s\n\n", desc);
       printf("You can choose the following direction(s):\n");
       uint8_t exit_status = GameGetCurrentRoomExits();

       // Print options to output
       char options[MAX_OPTION];
       if (exit_status & GAME_ROOM_EXIT_NORTH_EXISTS) {
           printf("  North(n)\n");
       }
       if (exit_status & GAME_ROOM_EXIT_EAST_EXISTS) {
           printf("  East(e)\n");
       }
       if (exit_status & GAME_ROOM_EXIT_SOUTH_EXISTS) {
           printf("  South(s)\n");
       }
       if (exit_status & GAME_ROOM_EXIT_WEST_EXISTS) {
           printf("  West(w)\n");
       }
       printf("  * Quit(q)\n");
       GenerateOptions(exit_status, options);

       // Read user's option
       printf("\nYou choose (%s)? ", options);
       uint8_t invalidInput = TRUE;
       while (invalidInput) {
           char command = getchar();
           switch (command) {
           // North direction
           case 'n':
               if (exit_status & GAME_ROOM_EXIT_NORTH_EXISTS) {
                   if (GameGoNorth() == SUCCESS) {
                       invalidInput = FALSE;
                   } else {
                       FATAL_ERROR();
                   }
               }
               break;
           // East direction
           case 'e':
               if (exit_status & GAME_ROOM_EXIT_EAST_EXISTS) {
                   if (GameGoEast() == SUCCESS) {
                       invalidInput = FALSE;
                   } else {
                       FATAL_ERROR();
                   }
               }
               break;
           // South direction
           case 's':
               if (exit_status & GAME_ROOM_EXIT_SOUTH_EXISTS) {
                   if (GameGoSouth() == SUCCESS) {
                       invalidInput = FALSE;
                   } else {
                       FATAL_ERROR();
                   }
               }
               break;
           // West direction
           case 'w':
               if (exit_status & GAME_ROOM_EXIT_WEST_EXISTS) {
                   if (GameGoWest() == SUCCESS) {
                       invalidInput = FALSE;
                   } else {
                       FATAL_ERROR();
                   }
               }
               break;
           // Quit
           case 'q':
               exit(EXIT_SUCCESS);
               break;
           default:
               break;
           }
       }

       // Read the useless input chars
       fgets(desc, GAME_MAX_ROOM_DESC_LENGTH, stdin);
   }
   return (0);
}

// GenerateOptions for main to print
void GenerateOptions(uint8_t exit_status, char *result)
{
    memset(result, 0, MAX_OPTION);
    uint8_t offset= 0;
    if (exit_status & GAME_ROOM_EXIT_NORTH_EXISTS) {
        result[offset] = 'n';
        ++offset;
        result[offset] = '/';
        ++offset;
    }
    if (exit_status & GAME_ROOM_EXIT_EAST_EXISTS) {
        result[offset] = 'e';
        ++offset;
        result[offset] = '/';
        ++offset;
    }
    if (exit_status & GAME_ROOM_EXIT_SOUTH_EXISTS) {
        result[offset] = 's';
        ++offset;
        result[offset] = '/';
        ++offset;
    }
    if (exit_status & GAME_ROOM_EXIT_WEST_EXISTS) {
        result[offset] = 'w';
        ++offset;
        result[offset] = '/';
        ++offset;
    }
    result[offset] = 'q';
}
