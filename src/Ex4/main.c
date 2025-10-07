/**
 * @file main.c
 * @brief Main file for Connect-N
 */

#include "ex4.h"
#include "../utils/utils.h"

// define global vars
#define RESET "\033[0m"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"

// declaring all the functions
int getPlayerMove(int currentPlayer);
void handleMove(char board[ROWS][COLS], int currentPlayer, int column);
int checkGameEnd(char board[ROWS][COLS], int currentPlayer);

/**
 * @brief - main function that runs the Connect-N game
 *
 * @return - 0 if the program runs successfully
 */
int main()
{
    // declare variables
    char board[ROWS][COLS];
    int currentPlayer = 0, gameStatus = -1;

    // initialize and print the board
    initBoard(board, ROWS, COLS);
    printBoard(board, ROWS, COLS);

    // run the game, until the board if full or one of the palyers won.
    while (1)
    {
        // makes a move
        int column = getPlayerMove(currentPlayer);
        handleMove(board, currentPlayer, column);
        printBoard(board, ROWS, COLS);

        // check the status of the game
        gameStatus = checkGameEnd(board, currentPlayer);
        if (gameStatus != -1)
            break;

        // Switch to the next player (cycles between 0 and NUM_PLAYERS-1)
        currentPlayer = (currentPlayer + 1) % NUM_PLAYERS;
    }
}

/**
 * @brief Asks the player for their next move.
 *
 * @return the column the user choose.
 */
int getPlayerMove(int currentPlayer)
{
    int column;
    printf(GREEN "Player %c, choose a column (0-%d): " RESET, 'A' + currentPlayer, COLS - 1);
    while (scanf("%d", &column) != 1 || column < 0 || column >= COLS)
    {
        printf(RED "Invalid input. Please enter a column between 0 and %d: " RESET, COLS - 1);
        while (getchar() != '\n')
            ; // clear input buffer
    }
    return column;
}

/**
 * @brief Executes a move for the given player, re-asking if invalid.
 *
 * @return void.
 */
void handleMove(char board[ROWS][COLS], int currentPlayer, int column)
{
    while (!makeMove(board, ROWS, COLS, NUM_PLAYERS, 'A' + currentPlayer, column))
    {
        printError("Column full or invalid. Try again.\n");
        column = getPlayerMove(currentPlayer);
    }
}

/**
 * @brief Checks if the game has ended (win or draw), and prints the result.
 *
 * @return the status of the game.
 */
int checkGameEnd(char board[ROWS][COLS], int currentPlayer)
{
    // check the status of the game
    int status = getStatus(board, ROWS, COLS, NUM_PLAYERS, CONNECT);

    // handle winner
    if (status == 1)
    {
        char winner = getWinner(board, ROWS, COLS, NUM_PLAYERS, CONNECT);
        printf("\n🎉 Player %c wins! 🎉\n", winner);
        return 1;
    }
    // handle full board
    else if (status == 0)
    {
        printf("\nIt's a draw!\n");
        return 0;
    }
    // the game is still running
    return -1;
}
