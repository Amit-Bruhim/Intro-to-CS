#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void hourglass();
void reverseBits();
void reverseDigits();
void convertToDecimal();
void base64ToDecimal();
void swapBits();
void printMenu(MenuItem *menu, int size);
void runMenu(MenuItem *menu, int menuSize);
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
			printf("Invalid input!\n");
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
			printf("Invalid choice!\n");
		}
	}
}

/**
 * @brief - print the menu options to the terminal
 *
 * @return - void
 */
void printMenu(MenuItem *menu, int size)
{
	// print the menu
	printf("Choose an option:\n");
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
	// create the nenu
	static MenuItem menu[] = {
		{"Exit", exitProgram},
		{"Hourglass", hourglass},
		{"Reverse bits", reverseBits},
		{"Reverse digits", reverseDigits},
		{"Convert to Decimal", convertToDecimal},
		{"Base64 to Decimal", base64ToDecimal},
		{"Swap bits", swapBits}};

	// update the size and return
	*size = sizeof(menu) / sizeof(menu[0]);
	return menu;
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
 * @brief - function that runs Hourglass option
 *
 * @return - void
 */
void hourglass()
{
	printf("Hourglass function selected!\n");
}

/**
 * @brief - function that runs Reverse bits option
 *
 * @return - void
 */
void reverseBits()
{
	printf("Reverse bits function selected!\n");
}

/**
 * @brief - function that runs Reverse digits option
 *
 * @return - void
 */
void reverseDigits()
{
	printf("Reverse digits function selected!\n");
}

/**
 * @brief - function that runs Convert to Decimal option
 *
 * @return - void
 */
void convertToDecimal()
{
	printf("Convert to Decimal function selected!\n");
}

/**
 * @brief - function that runs Base64 to Decimal option
 *
 * @return - void
 */
void base64ToDecimal()
{
	printf("Base64 to Decimal function selected!\n");
}

/**
 * @brief - function that runs Swap bits option
 *
 * @return - void
 */
void swapBits()
{
	printf("Swap bits function selected!\n");
}
