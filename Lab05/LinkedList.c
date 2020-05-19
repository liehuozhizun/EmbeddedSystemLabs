/*
 * Name:   Hang Yuan
 * StdID:  1564348
 * CruzID: hyuan3
 */

#include <stdio.h>
#include "BOARD.h"
#include "LinkedList.h"

ListItem *LinkedListNew(char *data) {
    ListItem *item = malloc(sizeof(ListItem));
    if (item == NULL) {
        return NULL;
    } else {
        item->data = data;
        item->previousItem = NULL;
        item->nextItem = NULL;
    }
    return item;
}

ListItem *LinkedListCreateAfter(ListItem *item, char *data) {
    if (item == NULL) {
        return LinkedListNew(data);
    }
    
    // Create a new node
    ListItem *newItem = malloc(sizeof(ListItem));
    if (newItem == NULL) {
        return NULL;
    }
    
    // Modify the original node's reference to accommodate the new one
    ListItem *oldNextItem = item->nextItem;
    if (oldNextItem == NULL) {  // It's tail
        newItem->nextItem = NULL;
    } else {                    // It's in the middle
        newItem->nextItem = oldNextItem;
        oldNextItem->previousItem = newItem;
    }
    
    // Set up new node
    item->nextItem = newItem;
    newItem->data = data;
    newItem->previousItem = item;

    return newItem;
}

char *LinkedListRemove(ListItem *item) {
    if (item == NULL) {
        return NULL;
    }
    
    ListItem *previous = item->previousItem;
    ListItem *next     = item->nextItem;
    
    // Head node
    if (previous == NULL) {
        // Only one head node in the LinkedList
        if (next != NULL) {
            next->previousItem = NULL;
        }
    }
    
    // Tail node
    else if (next == NULL) {
        previous->nextItem = NULL;
    }
    
    // Middle node
    else {
        previous->nextItem = next;
        next->previousItem = previous;
    }
    
    item->previousItem = NULL;
    item->nextItem = NULL;
    free(item);
    return item->data;
}

int LinkedListSize(ListItem *list) {
    if (list == NULL) {
        return 0;
    }
    
    int size = 1;
    // Look front
    ListItem *tempItem = list->previousItem;
    while (tempItem != NULL) {
        size++;
        tempItem = tempItem->previousItem;
    }
    // Look back
    tempItem = list->nextItem;
    while (tempItem != NULL) {
        size++;
        tempItem = tempItem->nextItem;
    }
    
    return size;
}

ListItem *LinkedListGetFirst(ListItem *list) {
    if (list == NULL) {
        return NULL;
    }
    
    ListItem *tempItem = list->previousItem;
    ListItem *curItem = list;
    while (tempItem != NULL) {
        curItem = tempItem;
        tempItem = tempItem->previousItem;
    }
    
    return curItem;
}

ListItem *LinkedListGetLast(ListItem *list) {
    if (list == NULL) {
        return NULL;
    }
    
    ListItem *tempItem = list->nextItem;
    ListItem *curItem = list;
    while (tempItem != NULL) {
        curItem = tempItem;
        tempItem = tempItem->nextItem;
    }
    
    return curItem;
}

int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem) {
    if (firstItem == NULL || secondItem == NULL) {
        return STANDARD_ERROR;
    }
    
    char *data = firstItem->data;
    firstItem->data = secondItem->data;
    secondItem->data = data;
    
    return SUCCESS;
}

int LinkedListPrint(ListItem *list) {
    if (list == NULL) {
        return STANDARD_ERROR;
    }
    
    ListItem *headNode = LinkedListGetFirst(list);
    int size = LinkedListSize(list);
    
    printf("\n[");
    while (headNode != NULL) {
        printf("%s", headNode->data);
        size--;
        if (size > 0) {
            printf(", ");
        }
        headNode = headNode->nextItem;
    }
    printf("]\n");
    
    return SUCCESS;
}

