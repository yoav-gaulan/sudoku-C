#ifndef FINAL_GAME_H
#define FINAL_GAME_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "doubleLinkedList.h"

/*
 * game: module summary:
 * contains all the functions and the structs for handle the board except solve it
 */

/*represent game mode*/

typedef enum  {INIT, SOLVE, EDIT} GameMode;

GameMode curGameMode;

/* represent cell in the sudoku board */

typedef struct SudukoCell{
    int value;
    int isFix;
    int isErroneous;
}Cell;

/* represent game of sudoku -
 * block height, block width, if the board is erroneous, bit for print the board with *,
 * number of cells that already filled, bit for - if the game is over
 * the sudoku board and the first ListNode(command), and the current command in the redo/undo list
 */

typedef struct SudokuGameStatus{
    int blockHeight, blockWidth;
    int N;
    int erroneousBoard;
    int printErroneous;
    int filledCells;
    int gameOver;
    Cell** gameBoard;
    ListNode* listMovesHead;
    ListNode* currentMove;
}Game;

/* these were all the structs*/

/*
 * returns a pointer to the cell in the (x,y) in the a board
 * Game* game - pointer to the game
 * int x, int y - indices of the cell in the board
 */

Cell * getCell(Game* game, int x, int y );

/*
 * returns the value of the cell in the (x,y) in the a board
 * Game* game - pointer to the game
 * int x, int y - indices of the cell in the board
 */

int getCellValue(Game* game, int x, int y );

/*
 * checks if the cell (x,y) contains a valid value
 * Game* game - pointer to current game
 * int x, int y - indices of the cell
 * returns 1 if it's valid, and 0 otherwise
 */

int isInvalidCell(Game* game, int x, int y);

/*
 * checks if the cell (x,y) is empty
 * Game* game - pointer to current game
 * int x, int y - indices of the cell
 * returns 1 if it's valid, and 0 otherwise
 */

int isEmptyCell(Game* game, int i , int j );

/*
 * checks if we can set the value in cell (x,y) to value z
 * Game* game- pointer to current game
 * int x, int y - indices of the cell
 * int z -  value
 * returns 1 if it's valid, and 0 otherwise
 */

int isValidValue(Game* game, int x, int y, int z);

/*
 * copy the game board values to another matrix.
 * Game* game- pointer to current game
 * int** matrix - pointer to the matrix
 * uninitialized - 1 if we didnt allocate memory for the array
 * return 1 if the process worked - 0 else
 */

int copyGameToArray(Game *game, int **matrix, int uninitialized);

/*
 * Copies the values in the matrix to the game board
 * Game* game- pointer to current game
 * int** matrix - pointer to the matrix
 */

void copyArrayToGame(Game* game, int** matrix);


/*
 * Frees the memory allocated to the board
 * Game* game- pointer to current game
 * matrix - pointer to the matrix
 */

void freeBoardArrCopy(int N, int** matrix);

/*
 * checks if the value z in cell (x,y) is valid
 * Game* game- pointer to current game
 * int x, int y - indices of the cell
 * int z -  value
 * returns 1 if it's valid, and 0 otherwise
 */

int invalidCellValue(Game* game, int x, int y, int z);

/* Counts the number of possible values in cell (x,y) in the current board, and save the possible moves in an array
 * Game* game- pointer to current game
 * int x, int y - indices of the cell
 * int* options - pointer to the values' array
 */

int countCellOptions(Game* game, int x, int y, int* options);

/* selects randomly 'tokeep' cells in the board and makes them fixed
 *  Game* game- pointer to current game
 *  int tokeep -  number of cells we will make them fix
 *  int N - the dim of board
 */

void keepRandCells(Game* game, int toKeep, int N);

/* checks if the board is solvable via GUROBI-
 * returns 1 if board is solvable, -2 if not, -1 if Gurobi failed.
 * Game* game- pointer to current game
 * int** boardCopy- pointer to a board for the GUROBI
 */

int validateBoard(Game *game);

/* set the cell (x,y) to z and updates the relevant fields of the game
 * Game* game- pointer to current game
 * int x, int y - indices of the cell
 * int z -  value
 * return 1 if its valid, -2 if the board unsolvable, -1 if error occurred
 */

void setCellAndUpdateBoard(Game* game, int x, int y, int z);

/* Marks the cells that are erroneous
 * Game* game- pointer to current game
 */

void remarkInvalidGameCells(Game *game);

/* handle the set command - set the cell (x,y) to z if its valid and updates the relevant fileds of the game
 * Game* game- pointer to current game
 * int x, int y - indices of the cell
 * int z -  value
 */

void setHandle(Game *game, int x, int y, int z);

/* Guess solution to the board, according to the threshold via gurobi LP
 * return 1 - if there is a solution, -1 if the gurobi failed , -2 if there is no solution
 * Game* game- pointer to current game
 * float threshold - between 0 to 1
 * int** oldBoard - pointer to the board before change
 */

int guessBoard(Game* game, float threshold ,int** oldBoard);

/* check if the cell (x,y) is fixed cell
 * return 1 - if true, 0 else.
 * Game* game- pointer to current game
 * int x, int y - indices of the cell
 */


int isFixCell(Game *game, int x, int y);

/* Fill to the board obvious values
 * Game* game- pointer to current game
 */

int autoFillBoard(Game* game);




/* check if the board filled
 * return 1 if true, 0 else
 * Game* game- pointer to current game
 */


int isFullBoard(Game* game);

/* check if the fix cells are not erroneous board
 * return 1 if true, 0 else
 * Game* game- pointer to current game
 */

int checkFixCells(Game* game);

/* Initializes all the fields to dipole values
 * Game* game- pointer to current game
 */

void initStart(Game* game);


#endif
