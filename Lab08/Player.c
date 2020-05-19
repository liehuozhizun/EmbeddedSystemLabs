/*
 * Name: Hang Yuna
 * StdID: 1564348
 * CruzID: hyuan3
 */

// Include user libraries here
#include "Player.h"

// Define the macro varibales here
static uint8_t inventory[INVENTORY_SIZE];
static uint8_t inventory_size_in_use;

// Adds the specified item to the player's inventory if the inventory isn't full
int AddToInventory(uint8_t item)
{
   int returnCode;

   // Check if the inventory is full
   if (inventory_size_in_use >= 4) {
      returnCode = STANDARD_ERROR;
   }

   // Add the item into the inventory
   else {
      inventory[inventory_size_in_use] = item;
      ++inventory_size_in_use;
      returnCode = SUCCESS;
   }
   return returnCode;
}

// Check if the given item exists in the player's inventory
int FindInInventory(uint8_t item)
{
   int returnCode = STANDARD_ERROR;
   int index = 0;

   // Traverse the inventory to find the item
   for (; index < inventory_size_in_use; ++index) {
      if (inventory[index] == item) {
         returnCode = SUCCESS;
         break;
      }
   }
   if (item == 0) {
       returnCode = SUCCESS;
   }
   return returnCode;
}
