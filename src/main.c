#include <stdio.h>
#include <stdlib.h>
#include "./utils/utils.h"

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
void runMenu(MenuItem *menu, int menuSize);
void printMenu(MenuItem *menu, int size);
void runEx2();
void runEx3();

/**
 * @brief - Displays the main menu with several user options
 * and executes the corresponding function based on the user's choice.
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
        {"Ex2 - Loops and Conditions", runEx2},
        {"Ex3 - Recursion", runEx3}};

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
			printf(RED "%s" RESET, invalidOptionString);
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
			printf(RED "%s" RESET, invalidOptionString);
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
 * @brief - runs ex2
 *
 * @return void
 */
void runEx2() {
    system("./Ex2/a.out");
}

/**
 * @brief - runs ex3
 *
 * @return void
 */
void runEx3() {
    system("./Ex3/a.out");
}