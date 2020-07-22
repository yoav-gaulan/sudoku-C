#include "game.h"
#include "solver.h"

/* Gurobi module:
 * using GUROBI library to solve sudoku boards using ILP/LP optimization
 * params:
 * game - pointer to currnet game
 * int ilp: ilp = 0 for run groubi NP, else run ILP
 * int hint: boolean 1/0 , if hint = 1: we using gurobi for hint (or guess_hint) functions
 * Xhint, Yhint index for hint, optional parameters in case we looking for hint
 * thershold - optional parameters in case we use Guess function
 * copyBoard - optional parameter, 2d int array in case we run Generate function, for holding the board solution.
 *
 * summary parameters for functions:
 * validate : ilp = 2, hint = 0 , xHint = 0, yHint = 0, threshold = 0, copyBoard = NULL
 * generate: ilp = 1 , hint = 0 , xHint = 0, yHint = 0, threshold = 0, copyBoard = [N][N] int array
 * hint : ilp = 1, hint = 1 , xHint = x, yHint = y, threshold = 0, copyBoard = NULL
 * guess: ilp = 0  , hint = 0 , xHint = 0, yHint = 0, threshold = x in range [0,1] , copyBoard = NULL
 * guess_hint: ilp = 0, hint = 1 , xHint = x , yHint = y, threshold = 0, copyBoard = NULL
 *
 * return value:
 *           1: if gurobi find a solution for the given  board
 *          -1: if gurobi failed
 *          -2: if the board unsolvable

*/
int exeGurobi(Game *game, int ilp, int hint, int xHint, int yHint, float threshold, int **copyBoard);

