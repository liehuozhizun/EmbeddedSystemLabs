// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries:
#include "MatrixMath.h"

#define TRUE   "TRUE"
#define FALSE  "FALSE"

// Module-level variables:
float zero[3][3] = {{},{},{}};
float max[3][3] = {{1, 2.0001, -3.0002},
                   {10.00005, 20.00006, -30.00019},
                   {100.000048,200.000058,-300.000158}};
float max2[3][3] = {{1, 2.0001, -3.0002},{10, 20.0001, -30.0002},{100,200.002,-300.0002}};
float basic1[3][3] = {{1, 1, 1},{2, 2, 2},{3, 3, 3}};
float basic2[3][3] = {{3, 3, 3},{2, 2, 2},{1, 1, 1}};
float basic3[3][3] = {{-1, -2, -3},{-4, -5, -6},{-7, -8, -9}};
int passInTotal = 0, failNumber = 0;

// Testing helper functions
void TestReport();
int  TestHelper(float comp1[2][2], const float comp2[2][2]);
void ErrorMessage(char *functName, float correct[3][3], float wrong[3][3], int tNum);
void ErrorMessage2(char *functName, float correct, float wrong, int tNum);
void ErrorMessage3(char *functName, const float correct[2][2], float wrong[2][2], int tNum);
void ErrorMessage4(float comp1[3][3], float comp2[3][3], int result, int tNum);

// Testing functions
void Test_MatrixPrint();
void Test_MatrixEquals();
void Test_MatrixAdd();
void Test_MatrixMultiply();
void Test_MatrixScalarAdd();
void Test_MatrixScalarMultiply();
void Test_MatrixTrace();
void Test_MatrixTranspose();
void Test_MatrixSubmatrix();
void Test_MatrixDeterminant();
void Test_MatrixInverse();

int main()
{
    BOARD_Init();

    printf("Beginning hyuan3's mml test harness, compiled on %s %s\n", __DATE__, __TIME__);
    
    // Add more tests here
    printf("\n*********** TEST BEGINS ***********\n\n");
    
    Test_MatrixEquals();
    Test_MatrixAdd();
    Test_MatrixMultiply();
    Test_MatrixScalarAdd();
    Test_MatrixScalarMultiply();
    Test_MatrixTrace();
    Test_MatrixTranspose();
    Test_MatrixSubmatrix();
    Test_MatrixDeterminant();
    Test_MatrixInverse();
    
    // Output test result
    TestReport();
    Test_MatrixPrint();
        
    printf("\n*********** TEST ENDS ***********\n");
    
    while (1);
}

void Test_MatrixPrint()
{
    printf("Output of MatrixPrint():\n");
    MatrixPrint(max);
    printf("Expected output of MatrixPrint():\n");
    printf(" ___________________________________ \n");
    printf("|    1.0000 |    2.0001 |   -3.0002 |\n");
    printf(" ----------------------------------- \n");
    printf("|   10.0000 |   20.0001 |  -30.0002 |\n");
    printf(" ----------------------------------- \n");
    printf("|  100.0000 |  200.0001 | -300.0002 |\n");
    printf(" ----------------------------------- \n");
}

void Test_MatrixEquals()
{
    if (MatrixEquals(zero, zero) == 0) {                  // #1
        ErrorMessage4(zero, zero, 0, 1);
    } else {
        passInTotal++;
    }
    if (MatrixEquals(zero, max) != 0) {                  // #2
        ErrorMessage4(zero, max, 1, 2);
    } else {
        passInTotal++;
    }
    if (MatrixEquals(max, max2) != 0) {                  // #3
        ErrorMessage4(max, max2, 0, 3);
    } else {
        passInTotal++;
    }
    if (MatrixEquals(basic1, basic2) != 0) {             // #4
        ErrorMessage4(basic1, basic2, 0, 4);
    } else {
        passInTotal++;
    }
    printf("PASSED (%d/4): MatrixEquals()\n", passInTotal);
}

void Test_MatrixAdd()
{
    // Initialization
    float result[3][3] = {{},{},{}};
    float MatrixAdd_Result_1[3][3] = {{4, 4, 4},{4, 4, 4},{4, 4, 4}};
    float MatrixAdd_Result_2[3][3] = {{0, -1, -2},{-2, -3, -4},{-4, -5, -6}};
    float MatrixAdd_Result_3[3][3] = {{-2, -4, -6},{-8, -10, -12},{-14, -16, -18}};
    float MatrixAdd_Result_4[3][3] = {{2, 3.0001, -2.0002},
                                      {12, 22.0001, -28.0002},
                                      {103, 203.0020, -297.0002}};
    int pass = 0;

    // Test script
    MatrixAdd(basic1, basic2, result);                  // #1
    if (MatrixEquals(result, MatrixAdd_Result_1) == 0) {
        ErrorMessage("MatrixAdd()", MatrixAdd_Result_1, result, 1);
    } else {
        pass++;
    }
    MatrixAdd(basic1, basic3, result);                  // #2
    if (MatrixEquals(result, MatrixAdd_Result_2) == 0) {
        ErrorMessage("MatrixAdd()", MatrixAdd_Result_2, result, 2);
    } else {
        pass++;
    }
    MatrixAdd(basic3, basic3, result);                  // #3
    if (MatrixEquals(result, MatrixAdd_Result_3) == 0) {
        ErrorMessage("MatrixAdd()", MatrixAdd_Result_3, result, 3);
    } else {
        pass++;
    }
    MatrixAdd(max2, basic1, result);                    // #4
    if (MatrixEquals(result, MatrixAdd_Result_4) == 0) {
        ErrorMessage("MatrixAdd()", MatrixAdd_Result_4, result, 4);
    } else {
        pass++;
    }
    MatrixAdd(zero, max2, result);                      // #5
    if (MatrixEquals(result, max2) == 0) {
        ErrorMessage("MatrixAdd()", result, max2, 5);
    } else {
        pass++;
    }
    MatrixAdd(basic2, basic1, result);                  // #6
    if (MatrixEquals(result, MatrixAdd_Result_1) == 0) {
        ErrorMessage("MatrixAdd()", result, MatrixAdd_Result_1, 6);
    } else {
        pass++;
    }
    printf("PASSED (%d/6): MatrixAdd()\n", pass);
    passInTotal += pass;
}

void Test_MatrixMultiply()
{
    // Initialization
    float result[3][3] = {{},{},{}};
    float MatrixMultiply_Result_1[3][3] = {{6, 6, 6},{12, 12, 12},{18, 18, 18}};
    float MatrixMultiply_Result_2[3][3] = {{30, 36, 42},{66, 81, 96},{102, 126, 150}};
    float MatrixMultiply_Result_3[3][3] = {{-12, -15, -18},{-24, -30, -36},{-36, -45, -54}};
    int pass = 0;

    // Test script
    MatrixMultiply(basic1, basic2, result);             // #1
    if (MatrixEquals(result, MatrixMultiply_Result_1) == 0) {
        ErrorMessage("MatrixMultiply()", MatrixMultiply_Result_1, result, 1);
    } else {
        pass++;
    }
    MatrixMultiply(basic3, basic3, result);             // #2
    if (MatrixEquals(result, MatrixMultiply_Result_2) == 0) {
        ErrorMessage("MatrixMultiply()", MatrixMultiply_Result_2, result, 2);
    } else {
        pass++;
    }
    MatrixMultiply(basic1, basic3, result);             // #3
    if (MatrixEquals(result, MatrixMultiply_Result_3) == 0) {
        ErrorMessage("MatrixMultiply()", MatrixMultiply_Result_3, result, 3);
    } else {
        pass++;
    }
    MatrixMultiply(zero, max2, result);                 // #4
    if (MatrixEquals(result, zero) == 0) {
        ErrorMessage("MatrixMultiply()", zero, result, 4);
    } else {
        pass++;
    }
    MatrixMultiply(basic2, basic1, result);             // #5
    if (MatrixEquals(result, MatrixMultiply_Result_1) != 0) {
        ErrorMessage("MatrixMultiply()", MatrixMultiply_Result_1, result, 5);
    } else {
        pass++;
    }
    
    printf("PASSED (%d/5): MatrixMultiply()\n", pass);
    passInTotal += pass;
}

void Test_MatrixScalarAdd()
{
    // Initialization
    float result[3][3] = {{},{},{}};
    float MatrixScalarAdd_Result_1[3][3] = {{223.2222, 223.2222, 223.2222},
                                            {224.2222, 224.2222, 224.2222},
                                            {225.2222, 225.2222, 225.2222}};
    float MatrixScalarAdd_Result_2[3][3] = {{221.2222, 220.2222, 219.2222},
                                            {218.2222, 217.2222, 216.2222},
                                            {215.2222, 214.2222, 213.2222}};
    float MatrixScalarAdd_Result_3[3][3] = {{-554.5555, -554.5555, -554.5555},
                                            {-553.5555, -553.5555, -553.5555},
                                            {-552.5555, -552.5555, -552.5555}};
    float MatrixScalarAdd_Result_5[3][3] = {{-999.9999, -999.9999, -999.9999},
                                            {-999.9999, -999.9999, -999.9999},
                                            {-999.9999, -999.9999, -999.9999}};
    int pass = 0;
    
    // Test script
    MatrixScalarAdd(222.2222, basic1, result);          // #1
    if (MatrixEquals(result, MatrixScalarAdd_Result_1) == 0) {
        ErrorMessage("MatrixScalarAdd()", MatrixScalarAdd_Result_1, result, 1);
    } else {
        pass++;
    }
    MatrixScalarAdd(222.2222, basic3, result);          // #2
    if (MatrixEquals(result, MatrixScalarAdd_Result_2) == 0) {
        ErrorMessage("MatrixScalarAdd()", MatrixScalarAdd_Result_2, result, 2);
    } else {
        pass++;
    }
    MatrixScalarAdd(-555.5555, basic1, result);         // #3
    if (MatrixEquals(result, MatrixScalarAdd_Result_3) == 0) {
        ErrorMessage("MatrixScalarAdd()", MatrixScalarAdd_Result_3, result, 3);
    } else {
        pass++;
    }
    MatrixScalarAdd(0, max2, result);                   // #4
    if (MatrixEquals(result, max2) == 0) {
        ErrorMessage("MatrixScalarAdd()", max2, result, 4);
    } else {
        pass++;
    }
    MatrixScalarAdd(-999.9999, zero, result);           // #5
    if (MatrixEquals(result, MatrixScalarAdd_Result_5) == 0) {
        ErrorMessage("MatrixScalarAdd()", MatrixScalarAdd_Result_5, result, 5);
    } else {
        pass++;
    }
    
    printf("PASSED (%d/5): MatrixScalarAdd()\n", pass);
    passInTotal += pass;
}

void Test_MatrixScalarMultiply()
{
    // Initialization
    float result[3][3] = {{},{},{}};
    float MatrixScalarMultiply_Result_1[3][3] = {{222.2222, 222.2222, 222.2222},
                                                {444.4444, 444.4444, 444.4444},
                                                {666.6666, 666.6666, 666.6666}};
    float MatrixScalarMultiply_Result_2[3][3] = {{-111.1111, -222.2222, -333.3333},
                                                {-444.4444, -555.5555, -666.6666},
                                                {-777.7777, -888.8888, -999.9999}};
    float MatrixScalarMultiply_Result_3[3][3] = {{111.1111, 222.2222, 333.3333},
                                                {444.4444, 555.5555, 666.6666},
                                                {777.7777, 888.8888, 999.9999}};
    float MatrixScalarMultiply_Result_4[3][3] = {{-1, -2.0001, 3.0002},
                                                {-10, -20.0001, 30.0002},
                                                {-100, -200.0020, 300.0002}};
    int pass = 0;
    
    // Test script
    MatrixScalarMultiply(222.2222, basic1, result);     // #1
    if (MatrixEquals(result, MatrixScalarMultiply_Result_1) == 0) {
        ErrorMessage("MatrixScalarMultiply()", MatrixScalarMultiply_Result_1, result, 1);
    } else {
        pass++;
    }
    MatrixScalarMultiply(111.1111, basic3, result);     // #2
    if (MatrixEquals(result, MatrixScalarMultiply_Result_2) == 0) {
        ErrorMessage("MatrixScalarMultiply()", MatrixScalarMultiply_Result_2, result, 2);
    } else {
        pass++;
    }
    MatrixScalarMultiply(-111.1111, basic3, result);    // #3
    if (MatrixEquals(result, MatrixScalarMultiply_Result_3) == 0) {
        ErrorMessage("MatrixScalarMultiply()", MatrixScalarMultiply_Result_3, result, 3);
    } else {
        pass++;
    }
    MatrixScalarMultiply(-1, max2, result);             // #4
    if (MatrixEquals(result, MatrixScalarMultiply_Result_4) == 0) {
        ErrorMessage("MatrixScalarMultiply()", MatrixScalarMultiply_Result_4, result, 4);
    } else {
        pass++;
    }
    MatrixScalarMultiply(0, max, result);               // #5
    if (MatrixEquals(result, zero) == 0) {
        ErrorMessage("MatrixScalarMultiply()", zero, result, 5);
    } else {
        pass++;
    }
    
    printf("PASSED (%d/5): MatrixScalarMultiply()\n", pass);
    passInTotal += pass;
}

void Test_MatrixTrace()
{
    // Initialization
    float result = 0.0;
    float MatrixTrace_Result_1 = 0.0;
    float MatrixTrace_Result_2 = 6.0;
    float MatrixTrace_Result_3 = 6.0;
    float MatrixTrace_Result_4 = -15;
    float MatrixTrace_Result_5 = -279.0001;
    int pass = 0;
    
    // Test script
    result = MatrixTrace(zero);
    if (result != MatrixTrace_Result_1) {               // #1
        ErrorMessage2("MatrixTrace()", MatrixTrace_Result_1, result, 1);
    } else {
        pass++;
    }
    result = MatrixTrace(basic1);
    if (result != MatrixTrace_Result_2) {               // #2
        ErrorMessage2("MatrixTrace()", MatrixTrace_Result_2, result, 2);
    } else {
        pass++;
    }
    result = MatrixTrace(basic2);
    if (result != MatrixTrace_Result_3) {                // #3
        ErrorMessage2("MatrixTrace()", MatrixTrace_Result_3, result, 3);
    } else {
        pass++;
    }
    result = MatrixTrace(basic3);
    if (result != MatrixTrace_Result_4) {                // #4
        ErrorMessage2("MatrixTrace()", MatrixTrace_Result_4, result, 4);
    } else {
        pass++;
    }
    result = MatrixTrace(max2);
    if (result - MatrixTrace_Result_5 > 0.0001) {        // #5
        ErrorMessage2("MatrixTrace()", MatrixTrace_Result_5, result, 5);
    } else {
        pass++;
    }
    
    printf("PASSED (%d/5): Test_MatrixTrace()\n", pass);
    passInTotal += pass;
}

void Test_MatrixTranspose()
{
    // Initialization
    float result[3][3] = {{},{},{}};
    float MatrixTranspose_Result_1[3][3] = {{1, 2, 3},{1, 2, 3},{1, 2, 3}};
    float MatrixTranspose_Result_2[3][3] = {{-1, -4, -7},{-2, -5, -8},{-3, -6, -9}};
    float MatrixTranspose_Result_3[3][3] = {{1, 10, 100},
                                            {2.0001, 20.0001, 200.002},
                                            {-3.0002, -30.0002, -300.0002}};
    float MatrixTranspose_Result_4[3][3] = {{},{},{}};
    int pass = 0;
    
    // Test script
    MatrixTranspose(basic1, result);                    // #1
    if (MatrixEquals(result, MatrixTranspose_Result_1) == 0) {
        ErrorMessage("MatrixTranspose()", MatrixTranspose_Result_1, result, 1);
    } else {
        pass++;
    }
    MatrixTranspose(basic3, result);                    // #2
    if (MatrixEquals(result, MatrixTranspose_Result_2) == 0) {
        ErrorMessage("MatrixTranspose()", MatrixTranspose_Result_2, result, 2);
    } else {
        pass++;
    }
    MatrixTranspose(max2, result);                      // #3
    if (MatrixEquals(result, MatrixTranspose_Result_3) == 0) {
        ErrorMessage("MatrixTranspose()", MatrixTranspose_Result_3, result, 3);
    } else {
        pass++;
    }
    MatrixTranspose(zero, result);                      // #4
    if (MatrixEquals(result, MatrixTranspose_Result_4) == 0) {
        ErrorMessage("MatrixTranspose()", MatrixTranspose_Result_4, result, 4);
    } else {
        pass++;
    }
    
    printf("PASSED (%d/4): MatrixTranspose()\n", pass);
    passInTotal += pass;
}

void Test_MatrixSubmatrix()
{
    // Initialization
    float result[2][2] = {{},{}};
    const float MatrixSubmatrix_Result_1[2][2] = {{-5, -6},{-8, -9}};
    const float MatrixSubmatrix_Result_2[2][2] = {{-1, -3},{-7, -9}};
    const float MatrixSubmatrix_Result_3[2][2] = {{-1, -2},{-4, -5}};
    const float MatrixSubmatrix_Result_4[2][2] = {{-4, -5},{-7, -8}};
    const float MatrixSubmatrix_Result_5[2][2] = {{-2, -3},{-8, -9}};
    const float MatrixSubmatrix_Result_6[2][2] = {{-1, -3},{-4, -6}};
    int pass = 0;
    
    // Test script
    MatrixSubmatrix(0, 0, basic3, result);              // #1
    if (TestHelper(result, MatrixSubmatrix_Result_1) == 0) {
        ErrorMessage3("MatrixSubmatrix()", MatrixSubmatrix_Result_1, result, 1);
    } else {
        pass++;
    }
    MatrixSubmatrix(1, 1, basic3, result);              // #2
    if (TestHelper(result, MatrixSubmatrix_Result_2) == 0) {
        ErrorMessage3("MatrixSubmatrix()", MatrixSubmatrix_Result_2, result, 2);
    } else {
        pass++;
    }
    MatrixSubmatrix(2, 2, basic3, result);              // #3
    if (TestHelper(result, MatrixSubmatrix_Result_3) == 0) {
        ErrorMessage3("MatrixSubmatrix()", MatrixSubmatrix_Result_3, result, 3);
    } else {
        pass++;
    }
    MatrixSubmatrix(0, 2, basic3, result);              // #4
    if (TestHelper(result, MatrixSubmatrix_Result_4) == 0) {
        ErrorMessage3("MatrixSubmatrix()", MatrixSubmatrix_Result_4, result, 4);
    } else {
        pass++;
    }
    MatrixSubmatrix(1, 0, basic3, result);              // #5
    if (TestHelper(result, MatrixSubmatrix_Result_5) == 0) {
        ErrorMessage3("MatrixSubmatrix()", MatrixSubmatrix_Result_5, result, 5);
    } else {
        pass++;
    }
    MatrixSubmatrix(2, 1, basic3, result);              // #6
    if (TestHelper(result, MatrixSubmatrix_Result_6) == 0) {
        ErrorMessage3("MatrixSubmatrix()", MatrixSubmatrix_Result_6, result, 6);
    } else {
        pass++;
    }
    
    printf("PASSED (%d/6): Test_MatrixSubmatrix()\n", pass);
    passInTotal += pass;
}

void Test_MatrixDeterminant()
{
    // Initialization
    float result = 0.0;
    float MatrixDeterminant_Test[3][3] = {{522, 8, -5},{0, -50, 999},{-888, 6, 4}};
    float MatrixDeterminant_Result_1 = 0.0;
    float MatrixDeterminant_Result_2 = 0.0;
    float MatrixDeterminant_Result_3 = 0.0;
    float MatrixDeterminant_Result_4 = -0.0001;
    float MatrixDeterminant_Result_5 = -10108164;
    int pass = 0;
    
    // Test script
    result = MatrixDeterminant(basic1);                    // #1
    if (result != MatrixDeterminant_Result_1) {
        ErrorMessage2("MatrixDeterminant()", MatrixDeterminant_Result_1, result, 1);
    } else {
        pass++;
    }
    result = MatrixDeterminant(basic2);                    // #2
    if (result != MatrixDeterminant_Result_2) {
        ErrorMessage2("MatrixDeterminant()", MatrixDeterminant_Result_2, result, 2);
    } else {
        pass++;
    }
    result = MatrixDeterminant(basic3);                    // #3
    if (result != MatrixDeterminant_Result_3) {
        ErrorMessage2("MatrixDeterminant()", MatrixDeterminant_Result_3, result, 3);
    } else {
        pass++;
    }
    result = MatrixDeterminant(max2);                      // #4
    if (result - MatrixDeterminant_Result_4 > 0.0001) {
        ErrorMessage2("MatrixDeterminant()", MatrixDeterminant_Result_4, result, 4);
    } else {
        pass++;
    }
    result = MatrixDeterminant(MatrixDeterminant_Test);    // #5
    if (result != MatrixDeterminant_Result_5) {
        ErrorMessage2("MatrixDeterminant()", MatrixDeterminant_Result_5, result, 5);
    } else {
        pass++;
    }
    
    printf("PASSED (%d/5): MatrixDeterminant()\n", pass);
    passInTotal += pass;
}

void Test_MatrixInverse()
{
    // Initialization
    float result[3][3] = {{},{},{}};
    float MatrixInverse_Test_1[3][3] = {{-1, -2, -3},{0, 0, -1},{1, 0, -1}};
    float MatrixInverse_Test_2[3][3] = {{1, 0, -3},{2, 1, -1},{1, 2, -1}};
    float MatrixInverse_Test_3[3][3] = {{1.1, 5, -2},{2, -1.2, 1},{-1, 2, -999}};
    float MatrixInverse_Result_1[3][3] = {{0, -1, 1},{-0.5, 2, -0.5},{0, -1, 0}};
    float MatrixInverse_Result_2[3][3] = {{-0.125, 0.75, -0.375},
                                          {-0.125, -0.25, 0.625},
                                          {-0.375, 0.25, -0.125}};
    float MatrixInverse_Result_3[3][3] = {{0.10595, 0.44184, 0.00023},
                                          {0.17679, -0.09746, -0.00045},
                                          {0.00024, -0.00063, -0.001}};
    int pass = 0;
    
    // Test script
    MatrixInverse(MatrixInverse_Test_1, result);        // #1
    if (MatrixEquals(result, MatrixInverse_Result_1) == 0) {
        ErrorMessage("MatrixInverse()", MatrixInverse_Result_1, result, 1);
    } else {
        pass++;
    }
    MatrixInverse(MatrixInverse_Test_2, result);        // #2
    if (MatrixEquals(result, MatrixInverse_Result_2) == 0) {
        ErrorMessage("MatrixInverse()", MatrixInverse_Result_2, result, 2);
    } else {
        pass++;
    }
    MatrixInverse(MatrixInverse_Test_3, result);        // #3
    if (MatrixEquals(result, MatrixInverse_Result_3) == 0) {
        ErrorMessage("MatrixInverse()", MatrixInverse_Result_3, result, 3);
    } else {
        pass++;
    }
    
    printf("PASSED (%d/3): MatrixInverse()\n", pass);
    passInTotal += pass;
}


/* Test I/O interfaces */
void TestReport()
{
    printf("\nTest Report: %2d test(s) passed\n", passInTotal);
    printf("             % 2d test(s) failed\n", failNumber);
    printf("              1 test(s) need(s) manually check: MatrixPrint()\n");
}

int  TestHelper(float comp1[2][2], const float comp2[2][2])
{
    if (comp1[0][0] != comp2[0][0] || comp1[0][1] != comp2[0][1] || 
        comp1[1][0] != comp2[1][0] || comp1[1][1] != comp2[1][1])
        return 0;
    return 1;
}

void ErrorMessage(char *functName, float correct[3][3], float wrong[3][3], int tNum)
{
    failNumber++;
    printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("ERROR DETECTED: %s #%d\n", functName, tNum);
    printf("Wrong result:\n");
    MatrixPrint(wrong);
    printf("Correct result:\n");
    MatrixPrint(correct);
}

void ErrorMessage2(char *functName, float correct, float wrong, int tNum)
{
    failNumber++;
    printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("ERROR DETECTED: %s #%d\n", functName, tNum);
    printf("Wrong result:     % 9.4f\n", (double)wrong);
    printf("Correct result:  % 9.4f\n", (double)correct);
}

void ErrorMessage3(char *functName, const float correct[2][2], float wrong[2][2], int tNum)
{
    failNumber++;
    printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("ERROR DETECTED: %s #%d\n", functName, tNum);
    printf("Wrong result:\n");
    printf("| % 9.4f | % 9.4f |\n| % 9.4f | % 9.4f |\n",
       (double)wrong[0][0], (double)wrong[0][1], (double)wrong[1][0], (double)wrong[1][1]);
    printf("Correct result:\n");
    printf("| % 9.4f | % 9.4f |\n| % 9.4f | % 9.4f |\n",
       (double)correct[0][0], (double)correct[0][1], (double)correct[1][0], (double)correct[1][1]);
}

void ErrorMessage4(float comp1[3][3], float comp2[3][3], int result, int tNum)
{
    failNumber++;
    printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("ERROR DETECTED: MatrixEquals() #%d\n", tNum);
    printf("Wrong result: %s    Correct result: %s\n", result? TRUE:FALSE, result? FALSE:TRUE);
    printf("First matrix:\n");
    MatrixPrint(comp1);
    printf("Second matrix:\n");
    MatrixPrint(comp2);
}
