/*
 * Name:   Hang Yuan
 * StdID:  1564348
 * CruzID: hyuan3
 */

#include <stdio.h>
#include "BOARD.h"
#include "LinkedList.h"

int passNum = 0, failNum = 0;

int main ()
{
    printf("\n\nWelcome to hyuan3's LinkedList test, compiled on %s %s.\n", __DATE__, __TIME__);

    char *data = "1";
    
    // Test LinkedListNew()
    printf("[Test 1/2] LinkedListNew()\n");
    ListItem *node1 = LinkedListNew(data);
    if (node1 == NULL) {
        printf("  Failed!\n");
        failNum++;
    } else {
        printf("  Succeess!\n");
        passNum++;
    }
    
    printf("[Test 2/2] LinkedListNew()\n");
    ListItem *node = LinkedListNew(NULL);
    if (node == NULL) {
        printf("  Failed!\n");
        failNum++;
    } else {
        printf("  Success!\n");
        passNum++;
    }
    
    // Test LinkedListCreateAfter()
    printf("[Test 1/2] LinkedListCreateAfter()\n");
    ListItem *node2 = LinkedListCreateAfter(node1, "2");
    if (node2 == NULL) {
        printf("  Failed!\n");
        failNum++;
    } else {
        printf("  Success!\n");
        passNum++;
    }
    
    printf("[Test 2/2] LinkedListCreateAfter()\n");
    if (LinkedListCreateAfter(node, NULL) == NULL) {
        printf("  Failed!\n");
        failNum++;
    } else {
        printf("  Success!\n");
        passNum++;
    }
    
    // Test LinkedListRemove()
    printf("[Test 1/2] LinkedListRemove()\n");
    if (LinkedListRemove(node2) == NULL) {
        printf("  Failed! size = %d\n", LinkedListSize(node1));
        failNum++;
    } else {
        printf("  Success! size = %d\n", LinkedListSize(node1));
        passNum++;
    }
    
    printf("[Test 2/2] LinkedListRemove()\n");
    if (LinkedListRemove(NULL) != NULL) {
        printf("  Failed!\n");
        failNum++;
    } else {
        printf("  Success!\n");
        passNum++;
    }
    
    // Test LinkedListSize()
    printf("[Test 1/2] LinkedListSize()\n");
    if (LinkedListSize(node1) == 0) {
        printf("  Failed! Size = %d\n", LinkedListSize(node1));
        failNum++;
    } else {
        printf("  Success! Size = %d\n", LinkedListSize(node1));
        passNum++;
    }
    
    printf("[Test 2/2] LinkedListSize()\n");
    if (LinkedListSize(NULL) != 0) {
        printf("  Failed! Size = %d\n", LinkedListSize(NULL));
        failNum++;
    } else {
        printf("  Success! Size = %d\n", LinkedListSize(NULL));
        passNum++;
    }
    
    // Test LinkedListGetFirst()
    ListItem *node6 = LinkedListCreateAfter(node1, "6");
    LinkedListCreateAfter(node1, "5");
    LinkedListCreateAfter(node1, "4");
    LinkedListCreateAfter(node1, "3");
    node2 = LinkedListCreateAfter(node1, "2");
    
    printf("[Test 1/2] LinkedListGetFirst()\n");
    if (LinkedListGetFirst(node6) != node1) {
        printf("  Failed! Size = %d\n", LinkedListSize(node));
        failNum++;
    } else {
        printf("  Success! Size = %d\n", LinkedListSize(node));
        passNum++;
    }
    
    printf("[Test 2/2] LinkedListGetFirst()\n");
    if (LinkedListGetFirst(NULL) != NULL) {
        printf("  Failed!\n");
        failNum++;
    } else {
        printf("  Success!\n");
        passNum++;
    }
    
    // Test LinkedListGetLast()
    printf("[Test 1/2] LinkedListGetLast()\n");
    if (LinkedListGetLast(node1) != node6) {
        printf("  Failed! Size = %d\n", LinkedListSize(node));
        failNum++;
    } else {
        printf("  Success! Size = %d\n", LinkedListSize(node));
        passNum++;
    }
    
    printf("[Test 2/2] LinkedListGetFirst()\n");
    if (LinkedListGetLast(NULL) != NULL) {
        printf("  Failed!\n");
        failNum++;
    } else {
        printf("  Success!\n");
        passNum++;
    }
    
    // Test LinkedListSwapData()
    printf("[Test 1/2] LinkedListSwapData()\n");
    if (LinkedListSwapData(node1, node6) != SUCCESS && node1->data[0] == '6') {
        printf("  Failed! Size = %d\n", LinkedListSize(node));
        failNum++;
    } else {
        printf("  Success! Size = %d\n", LinkedListSize(node));
        passNum++;
    }
    
    printf("[Test 2/2] LinkedListSwapData()\n");
    if (LinkedListSwapData(node1, NULL) == SUCCESS) {
        printf("  Failed!\n");
        failNum++;
    } else {
        printf("  Success!\n");
        passNum++;
    }
    
    // Test LinkedListPrint()
    printf("[Test 1/2] LinkedListPrint()\n");
    printf("Expected:\n[6, 2, 3, 4, 5, 1]\n");
    printf("Result  : ");
    if (LinkedListPrint(node1) != SUCCESS) {
        printf("  Failed! Size = %d\n", LinkedListSize(node));
        failNum++;
    } else {
        passNum++;
    }
    
    printf("\n[Test 2/2] LinkedListPrint()\n");
    printf("Expected:\n");
    printf("Result  : ");
    if (LinkedListPrint(NULL) == SUCCESS) {
        printf("  Failed! Size = %d\n", LinkedListSize(node));
        failNum++;
    } else {
        passNum++;
    }
    
    // Test Report
    printf("\n\n***********************************\n");
    printf("Brief Test Report:\n");
    printf("   pass : %d\n", passNum);
    printf("   fail : %d\n", failNum);
    
    while(1);
}