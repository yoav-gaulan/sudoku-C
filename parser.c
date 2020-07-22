/* Created by Karina Buchmann on 3/14/2020.

/*/
#include "command.h"
#include "parser.h"
#include "game.h"
#include <ctype.h>


const char* comString[] = {"solve", "edit", "mark_errors", "print_board", "set",  "validate", "guess", "generate",
                          "undo", "redo", "save", "hint", "guess_hint", "num_solutions", "autofill", "reset", "exit" };

/*check if 'args' is a presentation of a float number
 * return 1 if TRUE, 0 else*/
int isValidfloat(char* arg){
    int count = 0;
    while(arg[count] != '\0'){
        if(isdigit(arg[count])){
            count++;
        }
        else if(arg[count] == '.'){
            count++;
            break;
        }
        else { /*not a float*/
            return 0;
        }
    }
    while(arg[count] != '\0'){
        if(isdigit(arg[count])){
            count++;
        }
        else { /*not a float*/
            return 0;
        }
    }
    return 1;
}



/* return 0 - the string not valid number, 1 - if the string contain int, 2- if the string contain float*/
int isValidNumber(char* arg){
    int valid = 0,
    flagDot = 0, i,
    len = strlen(arg);
    while (len > 0 && isspace(arg[len - 1])){
        len--;   }  /* skip white space */
    if (len > 0){
        valid = 1;
        for (i = 0; i < len; i++){
            if (!isdigit(arg[i])){
                valid = 0;
                if(arg[i] == '.' && flagDot == 0){
                    valid = 2;
                    flagDot = 1;
                }
            }
        }
    }
    return valid;
}

/*Executes the command after checking all parameters and entering all command parameters into command structure*/
void exeCommand(Command* command, Game* game){
    switch (command->type){
        case solve:
            Solve(game,command->path,SOLVE);
            break;
        case edit:
            Edit(game,command->path, EDIT);
            break;
        case make_errors:
            MarkErrors(game,curGameMode,command->x);
            break;
        case print_board:
            PrintBoard(game);
            break;
        case set:
            Set(game,command->x,command->y,command->z);
            break;
        case validate:
            Validate(game);
            break;
        case guess:
            Guess(game,curGameMode,command->threshold);
            break;
        case generate:
            Generate(game,command->x,command->y);
            break;
        case undo:
            UnDo(game);
            break;
        case redo:
            ReDo(game);
            break;
        case save:
            Save(game, command->path);
            break;
        case hint:
            Hint(game,command->x, command->y);
            break;
        case guess_hint:
            GuessHint(game,command->x,command->y);
            break;
        case num_solutions:
            Num_solutions(game);
            break;
        case autofill:
            Autofill(game);
            break;
        case reset:
            Reset(game);
            break;
        case exiti:
            Exit(game);
            break;
        case invalid:
        case init:
            break;
    }
}
/*Checks whether the given command is supported in current mode*/

int supported(Command* command){
    switch (command->type){
        case solve:
        case edit:
        case exiti:
        case invalid:
        case init:
            return 1;
        case print_board:
        case set:
        case validate:
        case undo:
        case redo:
        case save:
        case num_solutions:
        case reset:
            if(curGameMode == INIT){
                printToUser(invalidInInit);
                return 0;
            }
            return 1;
        case make_errors:
        case guess:
        case hint:
        case guess_hint:
        case autofill:
            if(curGameMode != SOLVE){
                curGameMode == EDIT ? printToUser(invalidInEdit):printToUser(invalidInInit);
                return 0;
            }
            return 1;
        case generate:
            if(curGameMode != EDIT){
                curGameMode == SOLVE ? printToUser(invalidInSolve) : printToUser(invalidInInit);
                return 0;
            }
            return 1;
    }
    return 1;
}

/*Checks whether the number of parameters matches the command type, and the parameters are correct*/

int handleParams(Command* command, int cnt, char* p1, char* p2 , char* p3) {
    switch (command->type) {
        case solve:
            if (cnt != 1) {
                printToUser(invaildSolveParam);
                return 0;
            } else {
                command->path = p1;
                return 1;
            }
        case edit:
            if (cnt > 1) {
                printToUser(invaildEditParam);
                return 0;
            }
            if (cnt < 1) {
                command->path = NULL;
            }
            if (cnt == 1) {
                command->path = p1;
            }
            return 1;
        case make_errors:
            if (cnt != 1) {
                printToUser(ivalidMarkErrParam);
                return 0;
            } else {
                if (isValidNumber(p1) == 1) {
                    command->x = atoi(p1);
                    return 1;
                }
            }
        case set:
            if (cnt != 3) {
                printToUser(invalidSetParam1);
                return 0;
            } else {
                if (isValidNumber(p1) == 1 && isValidNumber(p2) == 1 && isValidNumber(p3) == 1) {
                    command->y = atoi(p1) - 1;
                    command->x = atoi(p2) - 1;
                    command->z = atoi(p3);
                    return 1;
                } else {
                    printToUser(invalidSetParam2);
                    return 0;
                }
            }
        case save:
            if (cnt != 1) {
                printToUser(invaildSaveParam);
                return 0;
            }
            command->path = p1;
            return 1;
        case guess:
            if (cnt != 1) {
                printToUser(invalidGuessParam1);
                return 0;
            } else if (isValidfloat(p1) == 0) {
                printToUser(invalidGuessParam2);
                return 0;
            }
            command->threshold = atof(p1);
            return 1;
        case generate:
            if (cnt != 2) {
                printToUser(invalidGenerateParam1);
                return 0;
            } else if (isValidNumber(p1) != 1 || isValidNumber(p2) != 1) {
                printToUser(invalidGenerateParam2);
                return 0;
            }
            command->x = atoi(p1);
            command->y = atoi(p2);
            return 1;
        case hint:
            if (cnt != 2) {
                printToUser(invalidHintParam1);
                return 0;
            } else if (isValidNumber(p1) != 1 || isValidNumber(p2) != 1) {
                printToUser(invalidHintParam2);
                return 0;
            }
            command->y = atoi(p1) - 1;
            command->x = atoi(p2) - 1;
            return 1;
        case guess_hint:
            if (cnt != 2) {
                printToUser(invalidGuessHintParam1);
                return 0;
            } else if (isValidNumber(p1) != 1 || isValidNumber(p2) != 1) {
                printToUser(invalidGuessHintParam2);
                return 0;
            }
            command->y = atoi(p1) - 1;
            command->x = atoi(p2) - 1;
            return 1;
        case validate:
        case redo:
        case undo:
        case print_board:
        case num_solutions:
        case autofill:
        case reset:
        case exiti:
            if (cnt > 0) {
                printf("Error: that command should not get any parameter.\n");
                return 0;
            }
            return 1;
        case init:
        case invalid:
            return 1;
    }
    return 1;
}

/*Gets a command index, and puts the relevant command, according to the location in the 'comString' array*/
void setCommType(int i, Command* commi){
    switch(i){
        case 0:
            commi->type = solve;
            break;
        case 1:
            commi->type = edit;
            break;
        case 2:
            commi->type = make_errors;
            break;
        case 3:
            commi->type = print_board;
            break;
        case 4:
            commi->type = set;
            break;
        case 5:
            commi->type = validate;
            break;
        case 6:
            commi->type = guess;
            break;
        case 7:
            commi->type = generate;
            break;
        case 8:
            commi->type = undo;
            break;
        case 9:
            commi->type = redo;
            break;
        case 10:
            commi->type = save;
            break;
        case 11:
            commi->type = hint;
            break;
        case 12:
            commi->type = guess_hint;
            break;
        case 13:
            commi->type = num_solutions;
            break;
        case 14:
            commi->type = autofill;
            break;
        case 15:
            commi->type = reset;
            break;
        case 16:
            commi->type = exiti;
            break;
        default:
            commi->type = invalid;
    }
}


void getCommandAndPlay(Game* game, Command* command) {
    int gotCommand = 0;
    int valid = -1;
    int i, cnt = 0;
    char *token ;
    char *param1 = NULL , *param2 = NULL , *param3 = NULL;
    char newCommand[MAX_INPUT_LENGTH + 3] = {'\0'};
    while (!gotCommand) {  /*we didnt get a command type*/
        printf("Please, Enter a command:\n");
        if (fgets(newCommand, MAX_INPUT_LENGTH + 3, stdin)) {
            if (newCommand[MAX_INPUT_LENGTH] != '\0') {
                /* if the input is too long, clear the rest of it and ignore it */
                printf("Error: The command is too long, it has to be no longer than 256 characters.\n");
                /* clear the full buffer.. */
               if( scanf("%*[^\n]") > 0 ){}
               if(scanf("%*c") > 0 ) {}
                return;

            }
            token = strtok(newCommand, DELIMITERS);
            if (token != NULL) {
                gotCommand = 1;
                for (i = 0; i < 17; i++) {
                    if (strcmp(comString[i], token) == 0) {
                        valid = i;
                        break;
                    }
                }
                if (valid == -1) {
                    printToUser(Invaild);
                    break;
                }
                setCommType(valid, command);
                while (token ) {
                    token = strtok(NULL, DELIMITERS);
                    if(!token){ break;}
                    switch (cnt) {
                        case 0:
                            param1 = token;
                            break;
                        case 1:
                            param2 = token;
                            break;
                        case 2:
                            param3 = token;
                            break;
                        case 4:
                            printToUser(tooManyArgs);
                            break;
                        default:
                            break;
                    }
                    cnt++;
                }
                if (supported(command)) {
                    if (handleParams(command, cnt, param1, param2, param3)) {
                        exeCommand(command, game);
                        return;
                    }
                    else{
                        command->type = invalid;
                        continue;
                    }
                }
            }
        }
                else { /* we got EOF */
                    command->type = exiti;
                    Exit(game);
                    return;
                }

    }
}

