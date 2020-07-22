
#include "game.h"
#include "gurobi.h"
#include "stack.h"


/*
 * mainAux: module summary:
 * Contains all functions to support : UNDO REDO operations , solver operations, and auxiliary function .
 */

/* add to the REDO UNDO list node that contains one change in cell (x,y)
 * Game* game- pointer to current game
 * int x, int y - indices of the cell
 *  int newVal, int oldval - the old and new values of cell (x,y)
 */

void addSingleMoveNode(Game *game, int x, int y, int newVal, int oldVal);

/* Counts the changed cells from the previous board to the current board
 * Game* game -  pointer to current game
 * int** oldBoard - pointer to previous board
 * return counter
 */

int countChanges(Game* game, int** oldBoard);

/* add to the REDO UNDO list node that contains all changes in board
 * Game* game- pointer to current game
 * int** oldBoard - pointer to previous board
 * return counter
 */

int addToMovesList(Game* game, int** oldBoard);

/* Gets a path to a file, checks if file is valid,
 * load the board if its valid
 * return 1 if everything is ok, -1 else
 * Game* game- pointer to current game
 * char* path - pointer to the file's path
 * GameMode mode - new mode for the game
 */

int loadBoard(Game* newGame, char* path, GameMode mode);

/* sets the game board to empty 9*9 board with wight = 3, height = 3
 * Game* game- pointer to current game
 */

int setDefaultBoard(Game* newGame);

/* prints current game board
 * Game* game- pointer to current game
 */

void printBoard(Game* game);

/* Saves the board to a file in path, if the path is valid, Overrides an existing file with the same path if exists
 * Game* game- pointer to current game
 * char* path - pointer for the file's path
 */

void saveBoard(Game* game, char* path);

/* counts the empty cells in the board
 * Game* game- pointer to current game
 * return counter
 */

int countEmptyCells(Game* game);

/* handle a redo or an undo command
 * Game* game- pointer to current game
 * int undo - 1 if its undo, 0 if its redo
 * return 1 if we change the board, 0 else
 */

int redo_undo_move(Game* game, int undo);

/* frees all resources in the game - REDO UNDO list, board
 * Game* game- pointer to current game
 */

void freeGame(Game* game);

/* init game field accept board
 * Game* game- pointer to current game
 * int width, int height, int N - the width, height and N = width*height
 */

void initGameFields(Game* game, int width, int height, int N);

/* allocate space for N*N board of cells
 * return 1 if success, 0 else
 * Game* game- pointer to current game
 */

int mallocBoard(int N, Game* game);

/* @pre: arr = N*(int *))
 * allocate space for N*N board of ints
 * return 1 if success, 0 else
 * int** arr - the arr we will make a board
 * int N - the dim of the board
 */

int mallocIntsBoard(int N, int** arr);

/* frees all resources in the game board
 * Game* game- pointer to current game
 */

void freeBoard(Game* game);

/* gets next cell's indexes, current (*i,*j)
 * int N - dim of the board
 * int* i, int* j- pointer to current cell's indexes
 * return 1 if there is next cell, 0 if the current is the last
 */

int getNextCellIndex(int N, int* i, int* j);

/* Keeps indexes of empty cells in the current game in the array arr
 * Game* game- pointer to current game
 * int len - number of empty cells
 * indexNode* arr - pointer to the array we will keep the indexes
 */

void setEmptyCellsArr(Game* game, int len, indexNode* arr);

/* pop the value of indexNode in index in the arr
 * int len - the array length
 * int index - the index of the indexNode to pop
 * indexNode* arr -pointer to the array
 */

void popAndShift(int len, int index, indexNode* arr);


