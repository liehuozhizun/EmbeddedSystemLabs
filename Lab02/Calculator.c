/* Name  : Hang Yuan
 * CruzID: hyuan3 
 * E-mail: hyuan3@ucsc.edu
 */

// Standard libraries
#include <stdio.h>
#include <math.h>
#include <xc.h>

// User libraries:
#include "BOARD.h"

// Individual macros
#define PI 3.14159

// **** Declare function prototypes ****
double Add(double operand1, double operand2);
double Multiply(double operand1, double operand2);
double AbsoluteValue(double operand);
double FahrenheitToCelsius(double operand);
double Tangent(double operand);
//add more prototypes here
double Subtract(double operand1, double operand2);
double Divide(double operand1, double operand2);
double CelsiusToFahrenheit(double operand);
double Average(double operand1, double operand2);
double Round(double operand);

void CalculatorRun(void)
{
    printf("\n\nWelcome to Hang's calculator program! Compiled at %s %s\n",__DATE__, __TIME__);

    // Your code here
    while (1) {
        char operation, uselessEnter;
        double operand1 = 0, operand2 = 0, result = 0;
        fflush(stdin);
        
        printf("\nEnter a Mathematical operation to perform (*,/,+,-,m,a,c,f,t,r): ");
        scanf(" %c", &operation);
        
        switch (operation) {
        case '+':
        case '-':
        case '*':
        case '/':
        case 'm':
            printf("\n   Enter the first operand: ");
            if (scanf(" %lf %c", &operand1, &uselessEnter) == 0) {
                printf("\n   Error, not a valid operand!\n");
                continue;
            }
            printf("\n   Enter the second operand: ");
            if (scanf(" %lf", &operand2) == 0) {
                printf("\n   Error, not a valid operand!\n");
                continue;
            }
            break;
        case 'a':
        case 'c':
        case 'f':
        case 't':
        case 'r':
            printf("\n   Enter the first operand: ");
            if (scanf(" %lf", &operand1) == 0) {
               printf("\n   Error, not a valid operand!\n");
               continue; 
            }
            break;
        default:
            printf("\n   Wrong operation code!\n");
            break;
        }
        
        switch (operation) {
        case '+':
            result = Add(operand1, operand2);
            printf("\n   Result of (%f + %f): %f\n", operand1, operand2, result);
            break;
        case '-':
            result = Subtract(operand1, operand2);
            printf("\n   Result of (%f - %f): %f\n", operand1, operand2, result);
            break;
        case '*':
            result = Multiply(operand1, operand2);
            printf("\n   Result of (%f * %f): %f\n", operand1, operand2, result);
            break;
        case '/':
            if (operand2 != 0) {
                result = Divide(operand1, operand2);
                printf("\n   Result of (%f / %f): %f\n", operand1, operand2, result);
            } else {
                printf("\n   Divide by zero error!\n");
            }
            break;
        case 'm':
            result = Average(operand1, operand2);
            printf("\n   Result of average(%f, %f): %f\n", operand1, operand2, result);
            break;
        case 'a':
            result = AbsoluteValue(operand1);
            printf("\n   Result of |%f|: %f\n", operand1, result);
            break;
        case 'c':
            result = FahrenheitToCelsius(operand1);
            printf("\n   Result of (%f deg->C): %f\n", operand1, result);
            break;
        case 'f':
            result = CelsiusToFahrenheit(operand1);
            printf("\n   Result of (%f deg->C): %f\n", operand1, result);
            break;
        case 't':
            result = Tangent(operand1);
            printf("\n   Result of tan(%f): %f\n", operand1, result);
            break;
        case 'r':
            result = Round(operand1);
            printf("\n   Result of round(%f): %f\n", operand1, result);
            break;
        default:
            break; 
        }
    }
    
    while (1);
}


/********************************************************************************
 * Define the Add function here.
 ********************************************************************************/
double Add(double operand1, double operand2)
{
    return operand1 + operand2;
}

/********************************************************************************
 * Define the Subtract function here.
 ********************************************************************************/
double Subtract(double operand1, double operand2)
{
    return operand1 - operand2;
}

/********************************************************************************
 * Define the Multiply function here.
 ********************************************************************************/
double Multiply(double operand1, double operand2)
{
    return operand1 * operand2;
}

/********************************************************************************
 * Define the Divide function here.
 ********************************************************************************/
double Divide(double operand1, double operand2)
{
    return operand1 / operand2;
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double AbsoluteValue(double operand)
{
    if (operand < 0) {
        operand *= -1;
    }
    return operand;
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double FahrenheitToCelsius(double operand)
{
    return (operand -32.0) * (5/9);
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double CelsiusToFahrenheit(double operand)
{
    return (operand * (9/5)) + 32.0;
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Average(double operand1, double operand2)
{
    return (operand1 + operand2) / 2.0;
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees.
 ********************************************************************************/
double Tangent(double operand)
{
    return tan (operand * PI / 180.0);
}

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
double Round(double operand)
{
    int integer = (int)operand;
    double reminder = operand - integer;
    if (reminder > 0.5 || reminder == 0.5) {           // reminder => [0.5, 1)
        return (double)integer + 1.0;
    } else if (reminder > 0 && reminder < 0.5) {       // reminder => (0, 0.5]
        return (double)integer;
    } else if (reminder < 0 && reminder > -0.5) {      // reminder => [-0.5, 0)
        return (double)integer;
    } else if (reminder < -0.5 || reminder == -0.5) {  // reminder => (-1, -0.5]
        return (double)integer - 1.0;
    }
    return (double)integer;
}