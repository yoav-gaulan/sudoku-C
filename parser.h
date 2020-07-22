
#include "printUser.h"

#define MAX_INPUT_LENGTH 256
#define DELIMITERS " \t\r\n"


/*
 * parser: module summary:
 * contains all the functions and the structs for parsing input from the user.
 */

/*type used to store type of user's command*/

typedef enum commandTypeH {solve, edit, make_errors, print_board, set, validate, guess, generate,
    undo, redo, save, hint, guess_hint, num_solutions,
    autofill, reset, exiti, invalid, init} commandType;

/* represent command and its params */

typedef struct CommandAndParams{
    commandType type;
    int x;
    int y;
    int z;
    char *path;
    float threshold;
} Command;

/*
 * gets and parses the next command.
 * check if the command is valid
 * check if the parameters have the right type and the right amount.
 * stores the correct command
 * Game* game- pointer to current game
 * Command* command pointer to the current command
 */

void getCommandAndPlay(Game* game, Command* command);


