/** 
 * Name: Hang Yuan
 * CruzID: hyuan3
 * StdID: 1564348
 */

#include <stdio.h>
#include "MatrixMath.h"

#define FALSE 0
#define TRUE  1

void MatrixPrint(float mat[3][3]) {
    printf(" ___________________________________ \n");
    int row = 0;
    for (; row < DIM; ++row) {
        printf("|");
        int column = 0;
        for(; column < DIM; ++column) {
            printf(" % 9.4f |",(double)mat[row][column]);
        }
        printf("\n ----------------------------------- \n");
    }
}

int MatrixEquals(float mat1[3][3], float mat2[3][3]) {
    int row = 0;
    for (; row < DIM; ++row) {
        int column = 0;
        for (; column < DIM; ++column) {
            if ((mat1[row][column] - mat2[row][column]) < -FP_DELTA ||
                (mat1[row][column] - mat2[row][column]) > FP_DELTA) {
                return FALSE;
            }
        }
    }
    return TRUE;
}
    
void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    int row = 0;
    for (; row < DIM; ++row) {
        int column = 0;
        for (; column < DIM; ++column) {
            result[row][column] = mat1[row][column] + mat2[row][column];
        }
    }
}

void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    int row = 0;
    for (; row < DIM; ++row) {
        int column = 0;
        for (; column < DIM; ++column) {
            result[row][column] = (mat1[row][0] * mat2[0][column] +
                                   mat1[row][1] * mat2[1][column] +
                                   mat1[row][2] * mat2[2][column]);
        }
    }
}

void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]) {
    int row = 0;
    for (; row < DIM; ++row) {
        int column = 0;
        for (; column < DIM; ++column) {
            result[row][column] = mat[row][column] + x;
        }
    }
}

void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]) {
    int row = 0;
    for (; row < DIM; ++row) {
        int column = 0;
        for (; column < DIM; ++column) {
            result[row][column] = mat[row][column] * x;
        }
    }
}

float MatrixTrace(float mat[3][3]) {
    return mat[0][0] + mat[1][1] + mat[2][2];
}

void MatrixTranspose(float mat[3][3], float result[3][3]) {
    int row = 0;
    for (; row < DIM; ++row) {
        int column = 0;
        for (; column < DIM; ++column) {
            if (row == column) {
                result[row][column] = mat[row][column];
            } else {
                result[row][column] = mat[column][row];
            }
        }
    }
}

void MatrixSubmatrix(int i, int j, float mat[3][3], float result[2][2]) {
    int row = 0, resultRow = 0, resultColumn = 0;
    for (; row < DIM; ++row) {
        int column = 0;
        if (row == i) {
            continue;
        }
        for (; column < DIM; ++column) {
            if (column == j) {
                continue;
            }
            result[resultRow][resultColumn] = mat[row][column];
            resultColumn++;
            if (resultColumn > 1) {
                resultRow++;
                resultColumn = 0;
            }
        }
    }
}

float MatrixDeterminant2x2(float mat[2][2]) {
    float result = 0.0;
    result = mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
    return result;
}

float MatrixDeterminant(float mat[3][3]) {
    float result = 0.0;
    float temp[2][2];
    MatrixSubmatrix(0, 0, mat, temp);
    result += mat[0][0] * MatrixDeterminant2x2(temp);
    MatrixSubmatrix(0, 1, mat, temp);
    result -= mat[0][1] * MatrixDeterminant2x2(temp);
    MatrixSubmatrix(0, 2, mat, temp);
    result += mat[0][2] * MatrixDeterminant2x2(temp);
    return result;
}

void MatrixInverse(float mat[3][3], float result[3][3]) {
    int row = 0;
    for (; row < DIM; ++row) {
        int column = 0;
        for (; column < DIM; ++column) {
            float temp2x2[2][2] = {{},{}};
            MatrixSubmatrix(row, column, mat, temp2x2);
            float d = MatrixDeterminant2x2(temp2x2);
            result[row][column] = ((row + column) % 2 == 0) ? d:-d;
        }
    }
    
    float temp[3][3] = {{},{},{}};
    MatrixTranspose(result, temp);
    float determinant = MatrixDeterminant(mat);
    MatrixScalarMultiply(1 / determinant, temp, result);
}

