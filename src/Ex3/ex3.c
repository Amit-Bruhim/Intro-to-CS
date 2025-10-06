#include "../utils/utils.h"
#include <stdio.h>

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
void evaluator();
void binaryStrings();
void excitingNumber();
void coolNumber();
void countPaths();

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
void exitProgram() {
    printf("Exiting program...\n");
    exit(0);
}

/**
 * @brief - function that runs Evaluator option
 *
 * @return - void
 */
void evaluator() {
    printf("Evaluator function selected!\n");
}

/**
 * @brief - function that runs Binary strings option
 *
 * @return - void
 */
void binaryStrings() {
    printf("Binary strings function selected!\n");
}

/**
 * @brief - function that runs Exciting number option
 *
 * @return - void
 */
void excitingNumber() {
    printf("Exciting number function selected!\n");
}

/**
 * @brief - function that runs Cool number option
 *
 * @return - void
 */
void coolNumber() {
    printf("Cool number function selected!\n");
}

/**
 * @brief - function that runs Count paths option
 *
 * @return - void
 */
void countPaths() {
    printf("Count paths function selected!\n");
}
