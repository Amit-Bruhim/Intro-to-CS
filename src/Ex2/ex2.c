#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	// takes the size of the hourglass from the user
	int size;
	const int MINIMUMSIZE = 3;
	printf("Enter size:\n");
	scanf("%d", &size);
	// edge case
	char clean;
	if (size < MINIMUMSIZE)
	{
		scanf("%c", &clean);
		printf(RED "Invalid paramaters\n" RESET);
		return;
	}

	/* let the user to decide if the sand will be at the top or the
	 * buttom of the hour glass
	 */
	const int SANDUP = 1, SANDDOWN = 0;
	int flag;
	printf("Enter flag:\n");
	scanf("%d", &flag);
	// edge case
	if ((flag != SANDDOWN) && (flag != SANDUP))
	{
		printf(RED "Invalid paramaters\n" RESET);
		scanf("%c", &clean);
		return;
	}

	// printing the upper triangle of the hourglass
	for (int a = 0; a < size; a++)
	{
		printf("-");
	}

	printf("\n");
	for (int j = 0; j < size / 2; j++)
	{
		for (int k = 0; k < j; k++)
		{
			printf(" ");
		}

		printf("\\");
		for (int i = 0; i < size - 2 - j * 2; i++)
		{
			// choosing the place of the sand according to the user will
			if (flag == SANDUP)
			{
				printf(".");
			}
			else
			{
				printf(" ");
			}
		}
		printf("/\n");
	}

	// adding X in the middle of the hourglass in case the size is not even
	if (size % 2 == 1)
	{
		for (int p = 0; p < size / 2; p++)
		{
			printf(" ");
		}

		printf("X\n");
	}

	// printing the lower triangle of the hourglass
	for (int t = size / 2; t > 0; t--)
	{
		for (int q = 0; q < t - 1; q++)
		{
			printf(" ");
		}

		printf("/");
		for (int w = 0; w <= size - 1 - t * 2; w++)
		{
			// choosing the place of the sand according to the user will
			if (flag == SANDUP)
			{
				printf(" ");
			}
			else
			{
				printf(".");
			}
		}

		printf("\\\n");
	}

	for (int b = 0; b < size; b++)
	{
		printf("-");
	}

	printf("\n");
	return;
}

/**
 * @brief - function that runs Reverse bits option
 *
 * @return - void
 */
void reverseBits()
{
	// takes number from the user
	printf("Enter a number:\n");
	int num, revers = 0;
	scanf("%d", &num);
	// reverse the bits of the user number
	const int SIZE = sizeof(int) * 8;
	for (int i = 0; i < SIZE; i++)
	{
		revers = revers << 1;
		revers = revers | (1 & num);
		num = num >> 1;
	}

	// printing the result
	printf("The reversed number is %d\n", revers);
	return;
}

/**
 * @brief - function that runs Reverse digits option
 *
 * @return - void
 */
void reverseDigits()
{
	// taking the number from the user
	int num;
	printf("Enter a number:\n");
	scanf(" %d", &num);
	// edge case
	char clean;
	if (num == 0)
	{
		scanf("%c", &clean);
		printf("The reversed number is 0\n");
		return;
	}

	// reverse the number until it ran out of digits
	const int BASE = 10;
	int digit = num % BASE;
	while (digit == 0)
	{
		num = num / BASE;
		digit = num % BASE;
	}

	// print the reversed number
	printf("The reversed number is ");
	while (num != 0)
	{
		printf("%d", digit);
		num = num / BASE;
		digit = num % BASE;
	}

	printf("\n");
	return;
}

/**
 * @brief - function that runs Convert to Decimal option
 *
 * @return - void
 */
void convertToDecimal()
{
	int base;
	char num;
	// takes the base from the user
	printf("Enter base:\n");
	scanf("%d", &base);
	char clean;
	scanf("%c", &clean);
	const int MINBASE = 2, MAXBASE = 9;
	if ((base < MINBASE) || (base > MAXBASE))
	{
		printf(RED "Invalid base\n" RESET);
		return;
	}

	// takes the number from the user
	printf("Enter a number:\n");
	scanf("%c", &num);
	int sum = 0, error = 0;
	// runs until the user press enter
	while (num != '\n')
	{
		// edge cases
		if ((num < '0' || (num >= base + '0')))
		{
			printf(RED "Invalid character %c in base %d\n" RESET, num, base);
			error++;
			break;
		}

		// calculates the result
		sum = (num - '0') + sum * base;
		scanf("%c", &num);
	}

	// edge cases - cleans the buffer
	if (error)
	{
		while (num != '\n')
		{
			scanf("%c", &num);
		}
		return;
	}

	// printing the result
	printf("The converted number is %d\n", sum);
	return;
}

/**
 * @brief - function that runs Base64 to Decimal option
 *
 * @return - void
 */
void base64ToDecimal()
{
	// mission45++;
	const int BASE = 64;
	const int NUMBERSOFLETTERS = 26, DIFF = ('a' - NUMBERSOFLETTERS),
			  ZERO64 = 52;
	const int DIFF2 = (ZERO64 - '0');
	int tempDigit = 0, error = 0;
	long long int sum = 0;
	// taking the number from the user
	printf("Enter a number:\n");
	char number;
	char clean;
	scanf("%c", &clean);
	scanf("%c", &number);
	// runs until the user press enter
	while (number != '\n')
	{
		// convert the digit to decimal
		if (number >= '0' && number <= '0' + 9)
		{
			tempDigit = number + DIFF2;
		}
		else if (number >= 'A' && number < 'A' + NUMBERSOFLETTERS)
		{
			tempDigit = number - 'A';
		}
		else if (number >= 'a' && number < 'a' + NUMBERSOFLETTERS)
		{
			tempDigit = number - DIFF;
		}
		else if (number == '+')
		{
			tempDigit = 62;
		}
		else if (number == '/')
		{
			tempDigit = 63;
		}

		// edge cases
		else
		{
			printf(RED "Invalid character %c in base 64 \n" RESET, number);
			// mission45 = 0;
			error++;
			break;
		}

		// calculates the result and taking the next digit
		sum = sum * BASE + (tempDigit);
		scanf("%c", &number);
	}

	// printing the result
	if (error == 0)
	{
		printf("The converted number is %lld\n", sum);
	}

	return;
}

/**
 * @brief - function that runs Swap bits option
 *
 * @return - void
 */
void swapBits()
{
	// taking the number and the bits
	int index1, index2;
	unsigned int number, result = 0;
	printf("Enter a number:\n");
	scanf("%d,", &number);
	printf("Enter indexes:\n");
	scanf("%d", &index1);
	scanf("%d", &index2);
	result = number;
	int bitIndex1 = 0, bitIndex2 = 0;
	// edge cases
	const int SIZE = sizeof(int) * 8;
	char clean;
	if (index1 < 0 || index2 < 0 || index1 >= SIZE || index2 >= SIZE)
	{
		printf(RED "Invalid indexes\n" RESET);
		scanf("%c", &clean);
		return;
	}

	// checks if the bit is 0 or 1
	if (number & (1 << index1))
	{
		bitIndex1 = 1;
	}

	if (number & (1 << index2))
	{
		bitIndex2 = 1;
	}

	// swiping the bits
	if (bitIndex1 != bitIndex2)
	{
		if (bitIndex1 == 1)
		{
			result = (number & (~(1 << index1)));
			result = (result | (1 << index2));
			printf("the result is %u\n", result);
		}
		else
		{
			result = (number & (~(1 << index2)));
			result = (result | (1 << index1));
			printf("The result is %u\n", result);
		}

		// printing the same number if those specific bits are the same
	}
	else
	{
		printf("The result is %u\n", result);
	}

	return;
}
