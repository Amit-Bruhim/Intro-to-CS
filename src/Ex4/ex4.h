/**
 * @file ex_4.h
 * @brief Header file for ex_4.c
 */

#include <stdio.h>

// define global vars
#define COLS  7
#define NUM_ PLAYERS  2
#define CONNECT  4
#define ROWS  6

// declaring all the functions
void initBoard(char board[ROWS][COLS], int rows, int columns);
void printBoard(char board[ROWS][COLS], int rows, int columns);
int makeMove(char board[ROWS][COLS], int rows, int columns, int players, char player, int column);
int undoMove(char board[ROWS][COLS], int rows, int columns, int column);
int getStatus(char board[ROWS][COLS], int rows, int columns, int players, int connect);
char getWinner(char board[ROWS][COLS], int rows, int columns, int players, int connect);
int isValidBoard(char board[ROWS][COLS], int rows, int columns, int players, int connect);
void encode(const char board[ROWS][COLS], int rows, int cols, char *code);
void decode(const char *code, char board[ROWS][COLS]);