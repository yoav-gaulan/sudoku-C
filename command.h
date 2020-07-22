#ifndef FINAL_COMMAND_H
#define FINAL_COMMAND_H

#include "game.h"
#include "solver.h"
#include "gurobi.h"
#include "stack.h"


/*
 * command: module summary:
 * contains all the functions that execute the all possible game commands, after verify the number and type of parameters.
 */

/* set mark_errors to input if input = 1 or input = 0
 * Game* game- pointer to current game
 * GameMode mode - current mode
 * int input - number for setting mark_errors
 * if input = 1: errors will mark in printboard, else: not
 */

void MarkErrors(Game* game, GameMode mode, int input);

/* validates the board
 * Game* game- pointer to current game
 */

void Validate(Game *game);

/* Guess solution for the current board with threshold ,
 * threshold - float between 0 to 1
 * GameMode mode - current mode
 * Game* game- pointer to current game
 * filled the board we guessed values with score over the given threshold via Gurobi LP
 */

void Guess(Game* game, GameMode mode, float threshold);

/* prints the current game board
 * Game* game- pointer to current game
 */

void PrintBoard(Game* game);

/* set (x,y) to z if possible
 * Game* game- pointer to current game
 * int x, int y - indices of the cell
 * int z - value for the cell
 */

void Set(Game* game, int x, int y, int z);

/* randomly filling 'XtoFill' empty cells with legal values, then solve the board via Gurobi ILP, and keeps 'YtoKeep' cells
 * the function tries to generate such board within 1000 iterations, else failed.
 * Game* game- pointer to current game
 * int XtoFill, int YtoKeep - numbers as explained up
 */

void Generate(Game* game, int XtoFill, int YtoKeep);

/* UnDo the last command if possible
 * Game* game- pointer to current game
 */

void UnDo(Game* game);

/* ReDo the last command if possible
 * Game* game- pointer to current game
 */

void ReDo(Game* game);

/* loaded game of file in path (if the file exists) , start with SOLVE mode
 * Game* game- pointer to current game
 * GameMode mode - current mode
 * char* path - file's path
 */

void Solve (Game* currentGame, char* path,GameMode mode);

/* loaded game of file in path (if the file exists) , if path = NULL, open new default 9x9 plain board
 * start with EDIT mode
 * Game* game- pointer to current game
 * GameMode mode - current mode
 * char* path - file's path
 */

void Edit (Game* currentGame, char* path,GameMode mode);

/* save current game to file with name - path, overrides a file with the same file if exists
 * Game* game- pointer to current game
 * char* path - file's path
 */

void Save(Game* game, char* path);

/* prints a possible value for cell (x,y) in the current board via Gurobi ILP.
 * Game* game- pointer to current game
 * int x, int y - indices of the cell
 */

void Hint(Game* game, int x, int y);

/* prints values for cell (x,y) and their probability , in the current board via Gurobi LP
 * Game* game- pointer to current game
 * int x, int y - indices of the cell
 */

void GuessHint(Game* game, int x, int y);


/* prints the number of solutions the current board has via iterative backtrack algorithm
 * Game* game- pointer to current game
 */

void Num_solutions(Game* game);

/* fills all obvious cells values of the current board - cells that have only one possible for a legal value
 * Game* game- pointer to current game
 */

void Autofill(Game* game);

/* reset the current game, UnDo all the moves
 * Game* game- pointer to current game
 */

void Reset(Game* game);

/* exit the program and free all the resources in use
 * Game* game- pointer to current game
 */
void Exit(Game* game);



#endif
