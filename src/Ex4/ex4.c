#include "ex4.h"

// define const for the array that containing the players.
#define MAX_PLAYERS 27

// declaring some of the functions

int checkWin(char board[ROWS][COLS], int rows, int columns, char player, int players, int connect);
int levelsAmount(char board[ROWS][COLS], int rows, int columns);
int checkMoves(char board[ROWS][COLS], int rows, int columns, char player, int players, int levels);
char getLastPlayer(char board[ROWS][COLS], int rows, int columns, int players);

/**
 * @brief function that prints a given array
 *
 * @param arr the given array
 * @param size the size of the array
 */
void printArr(char arr[], int size) {
    // printing the first value in the array
    if (size) {
        printf("%c", arr[0]);
    }

    // printing all the other values
    for (int i = 1; i < size; i++) {
        // stops when getting to NULL
        if (arr[i] == '\0') {
            break;
        }

        printf("%c", arr[i]);
    }

    // printing new line
    printf("\n");
}

/**
 * @brief function that match a letter to a number
 *
 * @param player the given player
 *
 * @return the matched char
 */
char makeLetter(int player) {
    // declaring the diffrence between the letters at the ascii table to the decimal
    // value of the letters (A=1 and so on)
    int diff = 'A' - 1;
    // returnig the matched char
    return (player + diff);
}

/**
 * @brief function that puts spaces at all the places on the board
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 */
void initBoard(char board[ROWS][COLS], int rows, int columns) {
    // running throuh all the board
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            // set them to be space
            board[i][j] = ' ';
        }
    }
}

/**
 * @brief function that prints the board
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 */
void printBoard(char board[ROWS][COLS], int rows, int columns) {
    // prints ~ at all the first line
    for (int i = 0; i < 2 * columns + 1; i++) {
        printf("~");
    }

    // printing new line
    printf("\n");
    // running through all the board
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            // prints |
            printf("|%c", board[i][j]);
        }
        // printing new line
        printf("|\n");
    }

    // prints ~ at all the last line
    for (int i = 0; i < 2 * columns + 1; i++) {
        printf("~");
    }
    // printing new line
    printf("\n");
}

/**
 * @brief function that "drops" the letter that represent a given player in a given column
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 * @param players the amount of players in the board that plays at the game
 * @param player the player that this is his turn to play
 * @param column the column that the player chose to "drop" his letter in
 *
 * @return 1 if the move was made, 0 if the move isn't possible
 */
int makeMove(char board[ROWS][COLS], int rows, int columns, int players, char player, int column) {
    // edge cases
    if (column >= columns || column < 0) {
        return 0;
    }
    if (player > makeLetter(players) || player < 'A') {
        return 0;
    }
    // declaring the last row
    int i = rows - 1;
    // running until reaching the first space place on the board
    while (i >= 0 && board[i][column] != ' ') {
        i--;
    }

    // edge case- when the column is full
    if (i == -1) {
        return 0;
    }

    // set that first space place on the board to be the char that represent the player
    board[i][column] = player;
    return 1;
}

/**
 * @brief function that cancles the last turn that was made in a given column
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 * @param column the specific column that need to be cancled
 *
 * @return 1 if the move was cancled, 0 if the move could not be cancled
 */
int undoMove(char board[ROWS][COLS], int rows, int columns, int column) {
    // edge cases
    if (columns < 0 || column < 0 || column >= columns) {
        return 0;
    }

    // declaring the first row
    int i = 0;
    // running until reaching the first non-space place on the board
    while (i < rows && board[i][column] == ' ') {
        i++;
    }

    // edge cases - when the column is empty
    if (i == rows) {
        return 0;
    }

    // removing the char from that place
    board[i][column] = ' ';
    return 1;
}

/**
 * @brief function that checks if there was a win (by row) in a given board
 *
 * @param board the given board
 * @param columns the amount of columns in the board
 * @param tempchar1 the temporary char, that starts the counting - in order to check if there are enough consecutive
 * identical chars at the same row
 * @param tempchar2 the temporary char, that is compared to tempchar1
 * @param connect the number of consecutive identical chars that is necessary in order to declare a viktory
 * @param k the line of tempchar1
 * @param j the column of tempchar1
 *
 * @return 1 if there was a winning, 0 else.
 */
int checkWinRow(char board[ROWS][COLS], int columns, char tempChar1, char tempChar2, int counter1, int connect, int k,
                int j) {
    // running until the next char isn't the identical to the first one, or there was a connect already, or there aren't
    // enough columns at that row
    while ((tempChar2 == tempChar1) && (counter1 < connect) && ((j + counter1) < columns)) {
        // checks that the column is valid
        if ((j + counter1 + 1) < columns) {
            // set the next char on the row
            tempChar2 = board[k][j + counter1 + 1];
        }
        // counting the amount of consecutive identical chars that is necessary in order to declare a viktory
        counter1++;
    }

    // returning 1 if there was a connect
    if (counter1 == connect) {
        return 1;
    }

    // returning 0 if there wasn't a connect
    return 0;
}

/**
 * @brief function that checks if there was a win (by column) in a given board
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param tempchar1 the temporary char, that starts the counting - in order to check if there are enough consecutive
 * identical chars at the same column
 * @param tempchar2 the temporary char, that is compared to tempchar1
 * @param counter1 counting the amount of consecutive identical chars that is necessary in order to declare a viktory
 * @param connect the number of consecutive identical chars that is necessary in order to declare a viktory
 * @param k the line of tempchar1
 * @param j the column of tempchar1
 *
 * @return 1 if there was a winning, 0 else.
 */
int checkWinColumn(char board[ROWS][COLS], int rows, char tempChar1, char tempChar2, int counter1, int connect, int k,
                   int j) {
    // running until the next char isn't the identical to the first one, or there was a connect already, or there aren't
    // enough rows at that column
    while ((tempChar2 == tempChar1) && (counter1 < connect) && (k + counter1 < rows)) {
        // checks that the row is valid
        if ((k + 1 + counter1) < rows) {
            // set the next char on the column
            tempChar2 = board[k + 1 + counter1][j];
        }
        // counting the amount of consecutive identical chars that is necessary in order to declare a viktory
        counter1++;
    }

    // returning 1 if there was a connect
    if (counter1 == connect) {
        return 1;
    }

    // returning 0 if there wasn't a connect
    return 0;
}

/**
 * @brief function that checks if there was a win (by secondary digonal) in a given board
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 * @param tempchar1 the temporary char, that starts the counting - in order to check if there are enough consecutive
 * identical chars at the same column
 * @param tempchar2 the temporary char, that is compared to tempchar1
 * @param counter1 counting the amount of consecutive identical chars that is necessary in order to declare a viktory
 * @param connect the number of consecutive identical chars that is necessary in order to declare a viktory
 * @param k the line of tempchar1
 * @param j the column of tempchar1
 *
 * @return 1 if there was a winning, 0 else.
 */
int checkWinCross1(char board[ROWS][COLS], int rows, int columns, char tempChar1, char tempChar2, int counter1,
                   int connect, int k, int j) {
    // edge cases
    if (rows < 0) {
        return 0;
    }

    // running until the next char isn't the identical to the first one, or there was a connect already, or there aren't
    // enough places on the diagonal
    while ((tempChar2 == tempChar1) && (counter1 < connect) && (k - counter1 >= 0) && ((j + counter1) < columns)) {
        // checks that the rows and columns are valid
        if ((j + counter1 + 1) < columns && (k - 1 - counter1) > 0) {
            // set the next char on the column
            tempChar2 = board[k - counter1 - 1][j + counter1 + 1];
        }
        // counting the amount of consecutive identical chars that is necessary in order to declare a viktory
        counter1++;
    }

    // returning 1 if there was a connect
    if (counter1 == connect) {
        return 1;
    }

    // returning 0 if there wasn't a connect
    return 0;
}

/**
 * @brief function that checks if there was a win (by main digonal) in a given board
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 * @param tempchar1 the temporary char, that starts the counting - in order to check if there are enough consecutive
 * identical chars at the same column
 * @param tempchar2 the temporary char, that is compared to tempchar1
 * @param counter1 counting the amount of consecutive identical chars that is necessary in order to declare a viktory
 * @param connect the number of consecutive identical chars that is necessary in order to declare a viktory
 * @param k the line of tempchar1
 * @param j the column of tempchar1
 *
 * @return 1 if there was a winning, 0 else.
 */
int checkWinCross2(char board[ROWS][COLS], int rows, int columns, char tempChar1, char tempChar2, int counter1,
                   int connect, int k, int j) {
    // running until the next char isn't the identical to the first one, or there was a connect already, or there aren't
    // enough places on the diagonal
    while ((tempChar2 == tempChar1) && (counter1 < connect) && (k + counter1 < rows) && ((j + counter1) < columns)) {
        // checks that the rows and columns are valid
        if ((j + counter1 + 1) < columns && (k + 1 + counter1) < rows) {
            // set the next char on the column
            tempChar2 = board[k + counter1 + 1][j + counter1 + 1];
        }
        // counting the amount of consecutive identical chars that is necessary in order to declare a viktory
        counter1++;
    }

    // returning 1 if there was a connect
    if (counter1 == connect) {
        return 1;
    }

    // returning 0 if there wasn't a connect
    return 0;
}

/**
 * @brief function that checks if there was a win in a given board
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 * @param tempchar1 the temporary char, that starts the counting - in order to check if there are enough consecutive
 * identical chars at the same column
 * @param tempchar2 the temporary char, that is compared to tempchar1
 * @param counter1 counting the amount of consecutive identical chars that is necessary in order to declare a viktory
 * @param connect the number of consecutive identical chars that is necessary in order to declare a viktory
 * @param k the line of tempchar1
 * @param j the column of tempchar1
 *
 * @return 1 if there was a winning, 0 else.
 */
int checkWinning(char board[ROWS][COLS], int rows, int columns, char tempChar1, char tempChar2, int counter1,
                 int connect, int k, int j) {
    // returning 1 if there was a win by row
    if (checkWinRow(board, columns, tempChar1, tempChar2, counter1, connect, k, j) == 1) {
        return 1;
    }

    // returning 1 if there was a win by column
    if (checkWinColumn(board, rows, tempChar1, tempChar2, counter1, connect, k, j) == 1) {
        return 1;
    }

    // returning 1 if there was a win by main digonal
    if (checkWinCross2(board, rows, columns, tempChar1, tempChar2, counter1, connect, k, j) == 1) {
        return 1;
    }

    // returning 1 if there was a win by secondary digonal
    if (checkWinCross1(board, rows, columns, tempChar1, tempChar2, counter1, connect, k, j) == 1) {
        return 1;
    }

    // returning 0 if there wasn't a viktory
    return 0;
}

/**
 * @brief function checks if a given board is full or not
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 *
 * @return 0 if the board is full, -1 else.
 */
int checkFullBoard(char board[ROWS][COLS], int rows, int columns) {
    // running through all the places on the board
    for (int k = 0; k < rows; k++) {
        for (int j = 0; j < columns; j++) {
            // returning -1 if one of the cells is empty
            if (board[k][j] == ' ') {
                return -1;
            }
        }
    }
    // returning 0 if all the cells are full
    return 0;
}

/**
 * @brief function checks who is the winner (if exist) at a given board
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 * @param players the amount of players in the board that plays at the game
 * @param connect the number of consecutive identical chars that is necessary in order to declare a viktory

 *
 * @return -1 if there was not any winner, else- return the winner.
 */
char getWinner(char board[ROWS][COLS], int rows, int columns, int players, int connect) {
    // running through all the players
    for (int i = 1; i < players + 1; i++) {
        // running through all the cells on the board
        for (int k = 0; k < rows; k++) {
            for (int j = 0; j < columns; j++) {
                // checks if the cell contains the letter that represent the player
                if (board[k][j] == makeLetter(i)) {
                    // setting the first char and another char that will be compared to it
                    char tempChar1 = board[k][j], tempChar2 = board[k][j];
                    // initializes the counter that counts the identical chars
                    int counter1 = 0;
                    // returning the winner char if exist
                    if (checkWinning(board, rows, columns, tempChar1, tempChar2, counter1, connect, k, j) == 1) {
                        return makeLetter(i);
                    }
                }
            }
        }
    }
    // returning -1 if there isn't any winner
    return -1;
}

/**
 * @brief function checks the "status" (viktory, tie or still running) of a given board
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 * @param players the amount of players in the board that plays at the game
 * @param connect the number of consecutive identical chars that is necessary in order to declare a viktory
 *
 * @return -1 if the game still running, 0 if it's a tie and 1 if there is a winner
 */
int getStatus(char board[ROWS][COLS], int rows, int columns, int players, int connect) {
    // running through all the players
    for (int i = 1; i < players + 1; i++) {
        // running through all the cells on the board
        for (int k = 0; k < rows; k++) {
            for (int j = 0; j < columns; j++) {
                // checks if the cell contains the letter that represent the player
                if (board[k][j] == makeLetter(i)) {
                    // setting the first char and another char that will be compared to it
                    char tempChar1 = board[k][j], tempChar2 = board[k][j];
                    // initializes the counter that counts the identical chars
                    int counter1 = 0;
                    // return 1 if there was a winner
                    if (checkWinning(board, rows, columns, tempChar1, tempChar2, counter1, connect, k, j) == 1) {
                        return 1;
                    }
                }
            }
        }
    }
    // returning -1 if the game is still running
    if (checkFullBoard(board, rows, columns) == -1) {
        return -1;
        // returning 0 if the game over without a winner
    } else if (checkFullBoard(board, rows, columns) == 0) {
        return 0;
    }
    return 0;
}

/**
 * @brief function that converts a char from 64 base to it's decimal value
 *
 * @param num the char that's converted
 *
 * @return the decimal value of the char
 */
int converFrom64(char num) {
    // declaring consts
    const int LETTERS_NUMBERS = 26;
    const int PLUS = 62;
    const int SLASH = 63;

    // checks if the char is a capital letter
    if (num >= 'A' && num <= 'Z') {
        return (num - 'A');
        // checks if the char isn't a capital letter
    } else if (num >= 'a' && num <= 'z') {
        return (num - 'a' + LETTERS_NUMBERS);
        // checks if the char is a number
    } else if (num >= '0' && num <= '9') {
        return (num - '0' + LETTERS_NUMBERS * 2);
        // checks if the char is special (not digit or letter)
    } else if (num == '+') {
        return (PLUS);
    } else if (num == '/') {
        return (SLASH);
    }
    return 0;
}

/**
 * @brief function that converts a number from decimal base to a char at 64 base
 *
 * @param num the number that's converted
 *
 * @return the char at 64 base
 */
char convertBase64(int num) {
    // declaring consts
    const int LETTERS_NUMBERS = 26;
    const int DIGITS_NUMBERS = 10;
    const int PLUS = 62;
    const int SLASH = 63;

    // checks if the matched char is a capital letter
    if (num < LETTERS_NUMBERS) {
        return (num + 'A');
        // checks if the matched char isn't a capital letter
    } else if (num < (LETTERS_NUMBERS * 2)) {
        return (num - LETTERS_NUMBERS + 'a');
        // checks if the matched char is a digit
    } else if (num < ((LETTERS_NUMBERS * 2) + DIGITS_NUMBERS)) {
        return (num - (LETTERS_NUMBERS * 2) + '0');
        // checks if the matched char is special (not digit or letter)
    } else if (num == PLUS) {
        return ('+');
    } else if (num == SLASH) {
        return ('/');
    }
    return '0';
}

/**
 * @brief function that checks if thare are "floating" letters (letters that has space below them)
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 *
 * @return 1 if the board is valid (there aren't any floating letters), 0 else.
 */
int checkSpaces(char board[ROWS][COLS], int rows, int columns) {
    // running through all the cells on the board
    for (int j = 0; j < columns; j++) {
        int k = 0;
        // sets the first value and initialize the counter
        char tempValue = board[k][j];
        int counter = 0;
        // running until reaching the first letter (or the rows went out)
        while (tempValue == ' ' && k < rows) {
            // set the next char and counting the it's row's index
            k++;
            counter++;
            tempValue = board[k][j];
        }

        // running until the rows went out
        while (counter < rows) {
            // setting the next char
            char tempChar = board[counter][j];
            // returning 0 if thare is a floating space
            if (tempChar == ' ') {
                return 0;
            }

            // increment the row
            counter++;
        }
    }
    // returning 1 if there aren't any floating spaces
    return 1;
}

/**
 * @brief function that checks if thare are any illegal chars at the board- chars that aren't letters or spaces,
 * or letters that comes after the letter that represent the last player to play.
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 * @param players the amount of players in the board that plays at the game
 *
 * @return 1 if the board is valid (there aren't any illegal chars), 0 else.
 */
int checkChars(char board[ROWS][COLS], int rows, int columns, int players) {
    // running through all the cells on the board
    for (int k = 0; k < rows; k++) {
        for (int j = 0; j < columns; j++) {
            // initializes the flag
            int checkValidChar = 0;
            // running through all the players
            for (int i = 1; i < players + 1; i++) {
                // checking that the char is valid (one of the players or space)
                if (board[k][j] == makeLetter(i) || board[k][j] == ' ') {
                    // increment the flag and breaks the loop of the players
                    checkValidChar = 1;
                    break;
                }
            }

            // returning 0 if one of the chars isn't valid
            if (checkValidChar == 0) {
                return 0;
            }
        }
    }
    // returning 1 if all the chars are valid
    return 1;
}

/**
 * @brief function that checks there is a player that played more (or less) turns than he should.
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 * @param players the amount of players in the board that plays at the game
 *
 * @return 1 if the board is valid (there aren't any illegal turns), 0 else.
 */
int checkAmount(char board[ROWS][COLS], int rows, int columns, int players) {
    // declaring an array that will contains the amount of cells that contains any player
    char playersArray[MAX_PLAYERS];
    // running through all the players
    for (int i = 1; i < players + 1; i++) {
        // initializes the counter
        int counter = 0;
        // running through all the cells on the board
        for (int k = 0; k < rows; k++) {
            for (int j = 0; j < columns; j++) {
                // counting the amount of identical chars
                if (board[k][j] == makeLetter(i)) {
                    counter++;
                }
            }
        }
        // places that amount in the array
        playersArray[i] = counter;
    }
    // running through all the cells at the array
    for (int i = 1; i < players + 1; i++) {
        for (int j = 1; j < players + 1; j++) {
            // checks that the column is valid
            if (i + j < players + 1) {
                // return 0 if one of the players played more turns than another player that plays before him
                if (playersArray[i] < playersArray[i + j]) {
                    return 0;
                }
                /*return 0 if one of the players played more than one turn more than one of the players that
                plays after him */
                if ((playersArray[i] - playersArray[i + j]) > 1) {
                    return 0;
                }
            }
        }
    }
    // return 1 if every player played exactly the right amount of turns
    return 1;
}

/**
 * @brief function that checks what is the first row (from the top) that contains a letter at a given column
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param column the given column
 *
 * @return the first row at the column that contains a letter
 */
int firstInColumn(char board[ROWS][COLS], int rows, int column) {
    // setting the first char at the column
    char firstChar = board[0][column];
    int i = 0;
    // running through all the cells at the row
    while (firstChar == ' ' && i + 1 < rows) {
        firstChar = board[i + 1][column];
        i++;
    }

    // edge case
    if (firstChar == ' ') {
        return -1;
    }

    return i;
}

/**
 * @brief function that counts the amount of letters in the board
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 *
 * @return the amount of letters in the board
 */
int levelsAmount(char board[ROWS][COLS], int rows, int columns) {
    // initializes the counter
    int counter = 0;
    // running through all the cells on the board
    for (int k = 0; k < rows; k++) {
        for (int j = 0; j < columns; j++) {
            // counting all the full cells
            if (board[k][j] != ' ') {
                counter++;
            }
        }
    }
    // returning the amount of full cells
    return counter;
    return 0;
}

/**
 * @brief function that checks who is the next player that needs to play
 *
 * @param current the current player that is playing
 * @param players the amount of players in the board that plays at the game
 *
 * @return the the next player that needs to play
 */
char nextTurn(char current, int players) {
    // if it's not the last player return the next letter
    if (current < makeLetter(players)) {
        return (current + 1);
        // if it's the last player- start the round again by returning A
    } else if (current == makeLetter(players)) {
        return 'A';
    }
    return '0';
}

/**
 * @brief function that checks who is the previous player that played
 *
 * @param current the current player that is playing
 * @param players the amount of players in the board that plays at the game
 *
 * @return the previous player that played
 */
char previousTurn(char current, int players) {
    // if it's not the last player return the next letter
    if (current != 'A') {
        return (current - 1);
        // if it's the last player- start the round again by returning A
    } else {
        return makeLetter(players);
    }
    return '0';
}

/**
 * @brief function that checks if 2 boards are identical
 *
 * @param board the given board
 * @param checkboard the boared that is compared to the original board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 *
 * @return 1 if the boards are identical, 0 else.
 */
int boardCompare(char board[ROWS][COLS], char checkboard[ROWS][COLS], int rows, int columns) {
    // running through all the cells on the board
    for (int k = 0; k < rows; k++) {
        for (int j = 0; j < columns; j++) {
            // return 0 if one of the cells are not identical
            if (board[k][j] != checkboard[k][j]) {
                return 0;
            }
        }
    }

    // return 1 if those are the same boards
    return 1;
}

/**
 * @brief function that checks if a given board is vaild, in terms of the lows of the game.
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 * @param players the amount of players in the board that plays at the game
 * @param connect the number of consecutive identical chars that is necessary in order to declare a viktory
 *
 * @return 1 if the board is valid, 0 else.
 */
int isValidBoard(char board[ROWS][COLS], int rows, int columns, int players, int connect) {
    // return 0 if there are "floating" letters
    if (checkSpaces(board, rows, columns) == 0) {
        return 0;
        // return 0 if there are invalid chars
    } else if (checkChars(board, rows, columns, players) == 0) {
        return 0;
        // return 0 if a player played more turns than he should
    } else if (checkAmount(board, rows, columns, players) == 0) {
        return 0;
        // return 0 if one of the moves was illegal
    } else if (checkMoves(board, rows, columns, getLastPlayer(board, rows, columns, players), players,
                          levelsAmount(board, rows, columns)) == 0) {
        return 0;
        // return 0 if there was a play after a viktory
    } else if (getStatus(board, rows, columns, players, connect) == 1) {
        if (checkWin(board, rows, columns, getLastPlayer(board, rows, columns, players), players, connect) == 0) {
            return 0;
        }
    }
    // return 1 if all the tests passed Successfully- the board is valid
    return 1;
}

/**
 * @brief function that checks if all the plays at a given board were made by the right order.
 * that is fine by Recursion, by removing the last player to play each time.
 * the stopping condition of the function is when all the plays were removed.
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 * @param player the last player that played
 * @param players the amount of players in the board that plays at the game
 * @param connect the number of turn that still left (to empty the board)
 *
 * @return 1 if the board is valid, 0 else.
 */
int checkMoves(char board[ROWS][COLS], int rows, int columns, char player, int players, int levels) {
    // return 1 when all the letters removed Successfully
    if (levels == 0) {
        return 1;
    } else {
        // running through the columns
        for (int i = 0; i < columns; i++) {
            // assigning the index of the first letter at the column
            int firstRaw = firstInColumn(board, rows, i);
            // checks if the the first letter from the top is the last player to play
            if (board[firstRaw][i] == player) {
                // removing it
                undoMove(board, rows, columns, i);
                // starts the Recursion again with the new board and the previous player
                int result = checkMoves(board, rows, columns, previousTurn(player, players), players, levels - 1);
                // returning the letter to it's place
                makeMove(board, rows, columns, players, player, i);
                // return 1 if all the letters removed Successfully
                if (result == 1) {
                    return 1;
                }
            }
        }
    }
    // return 0 if can't remove the last player
    return 0;
}

/**
 * @brief function that checks if the last play was the winning play or there was another play
 * that was played after the viktory.
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 * @param player the last player that played
 * @param players the amount of players in the board that plays at the game
 * @param connect the number of turn that still left (to empty the board)
 *
 * @return 1 if the board is valid, 0 else.
 */
int checkWin(char board[ROWS][COLS], int rows, int columns, char player, int players, int connect) {
    // running through the columns
    for (int j = 0; j < columns; j++) {
        // assigning the first letter at the column
        char firstPlayer = board[firstInColumn(board, rows, j)][j];
        // remove the letter if it's the winnig letter
        if (player == firstPlayer) {
            undoMove(board, rows, columns, j);
            // return 1 if after the removing there wasn't a viktory (that means there wasn't any other plays after)
            if (getStatus(board, rows, columns, players, connect) == -1) {
                // returning the letter to it's place
                makeMove(board, rows, columns, players, player, j);
                return 1;
                // return 1 if after the removing there was a viktory (that means there was other play after)
            } else if (getStatus(board, rows, columns, players, connect == 1)) {
                makeMove(board, rows, columns, players, player, j);
                continue;
            }
        }
    }
    return 0;
}

/**
 * @brief function that checks who was the last player to play. that's done by checking
 * the reminder of the dividing of the amount of plays by the number of the plays.
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 * @param players the amount of players in the board that plays at the game
 *
 * @return the last player to play.
 */
char getLastPlayer(char board[ROWS][COLS], int rows, int columns, int players) {
    // assigning the amount of plays at the board
    int levels = levelsAmount(board, rows, columns);
    // assigning the reminder of the dividing of the amount of plays by the number of the plays.
    int lastPlayer = levels % players;
    // return the last player to play.
    return (lastPlayer == 0) ? makeLetter(players) : makeLetter(lastPlayer);
}

/**
 * @brief function that encodes a board (by specific rules)
 *
 * @param board the given board
 * @param rows the amount of rows in the board
 * @param columns the amount of columns in the board
 * @param code pointer that represent an array that saves the Encryption.
 */
void encode(const char board[ROWS][COLS], int rows, int columns, char *code) {
    int counter2 = 0;
    // running through the rows
    for (int k = 0; k < rows; k++) {
        int j = 0;
        int counter = 0;
        // running through the columns
        while (j < columns) {
            counter = 0;
            // initializes the first char in the sequence
            char tempChar1 = board[k][j];
            char tempChar2 = board[k][j];
            // running until the next char isn't identical to the previous one
            while (tempChar1 == tempChar2 && (counter + j) < columns) {
                // setting the next char and counting the amount of consecutive identical chars
                tempChar2 = board[k][1 + j + counter];
                counter++;
            }
            // sets the next uniqe char
            j += counter;
            if (k == 2) {
            }
            // insert the amount of chars (on 64 base) and the char itself to the array
            code[counter2] = convertBase64(counter);
            code[counter2 + 1] = tempChar1;
            // increment the counter to the next place for the next char
            counter2 += 2;
        }
        // adding '/' at the end of every row
        code[counter2] = '/';
        counter2++;
    }
    // adding NULL to the end of the array
    code[counter2] = '\0';
}

/**
 * @brief function that decodes a string to a board (by specific rules)
 *
 * @param board the given board
 * @param code pointer that represent an array that containing the Encryption.
 */
void decode(const char *code, char board[ROWS][COLS]) {
    // declaring the indexes for the array and the line and column of the chars.
    int line = 0;
    int column = 0;
    int counter = 0;
    int index1 = 0;
    int index2 = 1;
    // running until the end of the string
    while (code[index1] != '\0' && code[index2] != '\0') {
        column = 0;
        // running until the end of the line
        while (code[index1] != '/') {
            // filling the boards with the char - as many times as the code says
            for (int i = 0; i < converFrom64(code[index1]); i++) {
                board[line][i + column] = code[index2];
                counter = i;
            }
            // increments the indexes by 2 and also the column to the next column that doesn't contain the
            // previous char
            column += (counter + 1);
            index1 += 2;
            index2 += 2;
        }
        // increments the indexes and jumping to the next line
        line++;
        index1++;
        index2++;
    }
}
