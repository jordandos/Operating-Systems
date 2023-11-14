#include <stdio.h>
#include <stdlib.h>

/* Function prototypes */
int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
int divide(int a, int b);

int main(void)
{
    /* Array of function pointers */
    int (*operations[4])(int, int) = {add, subtract, multiply, divide};

    int a = 10, b = 5;
    char userInput;

    while (1)
    {
        printf("Enter operation ('0' for add, '1' for subtract, '2' for multiply, '3' for divide, '4' to exit): ");
        scanf(" %c", &userInput);

        /* Exit the program if '4' is entered */
        if (userInput == '4')
        {
            printf("Exiting program.\n");
            break;
        }

        /* Ensure the input is a valid character ('0', '1', '2', '3') */
        if (userInput >= '0' && userInput <= '3')
        {
            int result = operations[userInput - '0'](a, b);
            printf("Result: %d\n", result);
        }
        else
        {
            printf("Invalid input. Please enter '0', '1', '2', '3', or '4'.\n");
        }
    }

    return 0;
}

/* Function definitions */
int add(int a, int b)
{
    printf("Adding %d and %d\n", a, b);
    return a + b;
}

int subtract(int a, int b)
{
    printf("Subtracting %d from %d\n", b, a);
    return a - b;
}

int multiply(int a, int b)
{
    printf("Multiplying %d and %d\n", a, b);
    return a * b;
}

int divide(int a, int b)
{
    if (b == 0)
    {
        printf("Error: Division by zero.\n");
        return 0;
    }

    printf("Dividing %d by %d\n", a, b);
    return a / b;
}
