#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>

// define global vars
const char *invalidOptionString = "Invalid option\n";
#define RESET "\033[0m"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"

// define handler function
typedef void (*Handler)(void);

// define struct for menu item
typedef struct
{
    const char *name;
    Handler handler;
} MenuItem;

// declaring all the functions
MenuItem *createMenu(int *size);
void exitProgram();
void binaryStrings();
void excitingNumber();
void coolNumber();
void countPaths();
void runMenu(MenuItem *menu, int menuSize);
void printMenu(MenuItem *menu, int size);
void makeLegalBin(int num, int sequence, int digitsAmount);
void printBin(int num, int countSteps, int digitsAmount);
int countDigits(int number);
int reverse(int number, int base);
int checkingExiting(unsigned int number, int sum, int pow, int base);
int power(int base, int pow);

/**
 * @brief - main function that gives the user 6 options of functions
 *
 * @return - 0 if the program runs successfully
 */
int main()
{
    int size;
    MenuItem *menu = createMenu(&size);
    runMenu(menu, size);
    return 0;
}

/**
 * @brief - print the menu options to the terminal
 *
 * @return - void
 */
void printMenu(MenuItem *menu, int size)
{
    // print the menu
    printf(GREEN "Choose an option:\n" RESET);
    for (int i = 0; i < size; i++)
    {
        printf("%d. %s\n", i, menu[i].name);
    }
}

/**
 * @brief - function that creates the menu
 *
 * @return - a new menu
 */
MenuItem *createMenu(int *size)
{
    // create the menu
    static MenuItem menu[] = {
        {"Exit", exitProgram},
        {"Binary strings", binaryStrings},
        {"Exciting number", excitingNumber},
        {"Cool number", coolNumber},
        {"Count paths", countPaths}};

    // update the size and return
    *size = sizeof(menu) / sizeof(menu[0]);
    return menu;
}

/**
 * @brief - runs the menu loop, prints the menu and executes handler according to user choice
 *
 * @param menu - array of MenuItem
 * @param menuSize - number of items in the menu
 * @return void
 */
void runMenu(MenuItem menu[], int menuSize)
{
    // sets the choice to be diffrent from 0
    int choice = 1;
    // let the user choose an option, until he finished
    while (1)
    {
        // print the menu
        printMenu(menu, menuSize);
        // Try to read an integer choice from the user.
        // If input is invalid (not a number), clear the input buffer, show an error, and prompt again.
        if (scanf("%d", &choice) != 1)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            printError(invalidOptionString);
            continue;
        }

        // If the choice is within the valid range, call the corresponding handler function.
        // Otherwise, notify the user that the choice is invalid.
        if (choice >= 0 && choice < menuSize)
        {
            menu[choice].handler();
        }
        else
        {
            printError(invalidOptionString);
        }
    }
}

/**
 * @brief - function that exits the program
 *
 * @return - void
 */
void exitProgram()
{
    printf("Exiting program...\n");
    exit(0);
}

/**
 * @brief - function that runs Binary strings option
 *
 * @return - void
 */
void binaryStrings()
{
    // takes the number from the user
    printf("Enter length:\n");
    long unsigned int num;
    scanf("%lu", &num);
    // edge cases
    if (num < 1 || num > sizeof(int) * 8)
    {
        printError("Invalid length\n");
        return;
    }
    // activates the function that prints the appropriate binary numbers.
    makeLegalBin(num, 0, num);
    return;
}

/**
 * @brief function that creates legal (without 2 consecutive '1')
 * binary numbers
 * the function calls itself enough times, until the binary sequence is
 * in the length that the user chose.
 *
 * @param num The number from the user.
 * @param sequence the current sequence of the binary number.
 * @param digitsamount the length of the numbers (that the user chose).
 *
 * @return
 */
void makeLegalBin(int num, int sequence, int digitsAmount)
{
    // stops and prints the number in binary base, after the number is "ready".
    if (num == 0)
    {
        printBin(sequence, 0, digitsAmount);
        printf("\n");
    }
    /* adds only 0 when the previous digit was 1,
    and adds both 1 and 0 when the previous digit was 0. */
    else if (sequence % 2 == 0)
    {
        makeLegalBin(num - 1, sequence << 1, digitsAmount);
        makeLegalBin(num - 1, (sequence << 1) + 1, digitsAmount);
    }
    else if (sequence % 2 == 1)
    {
        makeLegalBin(num - 1, sequence << 1, digitsAmount);
    }
}

/**
 * @brief prints decimal number as a binary number,
 * in the length of the user's choise.
 *
 * @param num The decimal number.
 * @param countsteps counts the current amount of digits in the number.
 * @param digitsamount the length of the numbers (that the user chose).
 *
 * @return
 */
void printBin(int num, int countSteps, int digitsAmount)
{
    // stopping condition when the number ran out of digits.
    if (num != 0)
    {
        // activates the function again after deviding by 2.
        printBin(num >> 1, countSteps + 1, digitsAmount);
        /* prints the remainder after dividing the number by 2
        (and therefore, prints 1 or 0 at the right place at the binary number). */
        printf("%d", num % 2);
    }
    else
    {
        // fills the number with zeroes from left if nessessory.
        if (countSteps < digitsAmount)
        {
            printf("0");
            printBin(0, countSteps + 1, digitsAmount);
        }
    }
}

/**
 * @brief - function that runs Exciting number option
 *
 * @return - void
 */
void excitingNumber()
{
    // let the user choose the number
    printf("Enter a natural number:\n");
    unsigned int number;
    int sumOfDigits;
    unsigned int check;
    scanf("%d", &number);
    // edge cases
    if (number == 0)
    {
        printf("The number is exciting\n");
        return;
    }
    sumOfDigits = countDigits(number);
    // checks if his number is exiting and prints suitable massage
    check = checkingExiting(number, 0, sumOfDigits, 10);
    if (check == number)
    {
        printf("The number is exciting\n");
    }
    else
    {
        printf("The number is not exciting\n");
    }
    return;
}

/**
 * @brief Calculates every digit of the number powered by
 * the amount of digits in the number, and then sum it.
 *
 * @param number The number from the user.
 * @param sum The current sum of the digits (after the power)
 * @param pow The exponent
 * @param base the decimal base
 *
 * @return The sum of all of the digits after the power and
 * the sum (according to the definition of exiting number).
 */
int checkingExiting(unsigned int number, int sum, int pow, int base)
{
    int tempDigit, newSum = 0;
    // takes the first digit and powered it by the exponent. than sum it with the
    // previous value.
    tempDigit = number % base;
    newSum = sum + power(tempDigit, pow);
    // stopping condition when the the number ran out of digits.
    if (number == 0)
    {
        return newSum;
    }
    // devides the number by 10 and activates the recoursion once again.
    number = number / base;
    return checkingExiting(number, newSum, pow, base);
}

/**
 * @brief calculates a number powered by another number.
 * doing that by multiple the base by itself once every time.
 * the stopping condition is when the number was multiplied by itself
 * for the exponent's time.
 *
 * @param base the base of the number.
 * @param pow the exponent of the number.
 *
 * @return the result of the calculation.
 */
int power(int base, int pow) {
    // calculates the number powered by the exponent
    if (pow != 0) {
        return (base * power(base, pow - 1));
    } else {
        return 1;
    }
}

/**
 * @brief Counting the numbers of digits in a number
 *
 * @param number The number from the user.
 *
 * @return The amount of digits at the number.
 */
int countDigits(int number)
{
    /* the stoping condition, when the number was devided enough times,
    that all the digits were counted. */
    if (number == 0)
    {
        return 0;
    }
    // devides the number by 10 and runs the function again.
    number = number / 10;
    return (1 + countDigits(number));
}

/**
 * @brief function that reverse the digits in a number
 *
 * @param number The number from the user.
 *
 * @return The reversed number.
 */
int reverse(int number, int base)
{
    int newNum = 0;
    int temporary;
    // runs until all the digits were reversed.
    while (number != 0)
    {
        // moves the new number one digit to the left.
        newNum = newNum * base;
        // gets the first digit
        temporary = number % base;
        // puts the first digit in the new number
        newNum = newNum + temporary;
        // deviding the number by 10 in order to get the next number.
        number = number / base;
    }
    // return the reversed number.
    return newNum;
}

/**
 * @brief - function that runs Cool number option
 *
 * @return - void
 */
void coolNumber()
{
    printf("Cool number function selected!\n");
}

/**
 * @brief - function that runs Count paths option
 *
 * @return - void
 */
void countPaths()
{
    printf("Count paths function selected!\n");
}
