#include <time.h>
#include "command.h"
#include "mainAux.h"
#include "game.h"
#include "solver.h"
#include "gurobi.h"


void MarkErrors(Game* game, GameMode mode, int input) {
    if (mode == SOLVE) {
        if (input == 0 || input == 1) {
            game->printErroneous = input;
            printf("Mark errors: setting mark error  to  %d. \n\n",input);
            return;
        } else {
            printf("Error: mark_errors parameter has to be 0 or 1.\n");
            return;

        }
    }
}


void Validate(Game* game){
    int grb;

    if(game->erroneousBoard){
        printf("Error: can not validate an erroneous board.\n");
        return;
    }
    grb = validateBoard(game);
    if(grb == 1){
        printf("The board is solvable! \n");
    }
    else {
        printf("The board is unsolvable! \n");
    }


}

void Guess(Game* game, GameMode mode, float threshold){
    int** boardCopy;
    int N = game->N;
    int grb;
    if(mode != SOLVE){
        printf("Error: this command only available in SOLVE mode.\n");
        return;
    }
    if(game->erroneousBoard){
        printf("Error: can not Guess an erroneous board.\n");
        return;
    }
    if(threshold < 0 || threshold > 1){
        printf("Error: guess threshold had to be in range [0,1].\n");
        return;

    }
    boardCopy = malloc(N* sizeof(int*));
    grb = guessBoard(game,threshold,boardCopy);

    if(grb == 1) {
        remarkInvalidGameCells(game);
        printBoard(game);
        if (isFullBoard(game)) {
            if (game->gameOver) {
                printf("Congratulations! you solved the board !\n");
                curGameMode = INIT;

            } else {
                printf("The sudoku board is erroneous.\n");
            }
        }

    }

    if(grb == -2){
        printf("Guess failed, can not Guess unsolvable board.\n");
    }

    if(grb == -1){
        printf("Guess failed. Gurobi error occurred.\n ");
    }

    freeBoardArrCopy(N,boardCopy);

}

void PrintBoard(Game* game){
    if(curGameMode == SOLVE || curGameMode == EDIT){
        printBoard(game);
        return;
    }
    printf("Error: print board command only available in SOLVE or EDIT mode.\n");

}

void Set(Game* game, int x, int y, int z){
    if(curGameMode == SOLVE || curGameMode == EDIT) {
        setHandle(game, x, y, z);
        if (isFullBoard(game)) {
            if (game->gameOver) {
                printf("Congratulations! you solved the board !\n");
                curGameMode = INIT;
            } else {
                printf("The sudoku board is erroneous.\n");
            }

        }
        return;
    }
    printf("Error: Set command only available in SOLVE or EDIT mode.\n");

}


/* helper function for generate
 * choosing 'XtoFill' empty cells in currnet board and fill them with random legal value
 * returns 1 if succeed
 * */

int fillRandCells(Game* game, int XtoFill){
    indexNode* emptyCellsArr;
    int** boardCopy;
    int* options,x,y,i,
    emptyCount,
    randIndex,
    failed = 0,
    optionsCnt = 0,
    N = game->N;

    emptyCount = countEmptyCells(game);
    if(emptyCount < XtoFill){
        return 0;
    }
    options = malloc(N* sizeof(int));
    boardCopy = malloc(N * sizeof(int*));
    emptyCellsArr = malloc(emptyCount * sizeof(indexNode));
    if(options == NULL || boardCopy == NULL || emptyCellsArr == NULL){
        printf("Error: memory error.\n");
        free(options);
        free(boardCopy);
        free(emptyCellsArr);
        return 0;
    }
    setEmptyCellsArr(game, emptyCount, emptyCellsArr); /*set arr of nodes to hold all <x,y> indexes of empty cells */
    copyGameToArray(game, boardCopy, 1);


    for(i = 0; i < XtoFill; i++){
        randIndex = rand()%emptyCount;
        x = emptyCellsArr[randIndex].x;
        y = emptyCellsArr[randIndex].y;
        popAndShift(emptyCount, randIndex, emptyCellsArr);
        emptyCount --;
        optionsCnt = countCellOptions(game,x,y,options);
        if(!optionsCnt) { /* failed to fill X  cells, restore origin board and break*/
            copyArrayToGame(game,boardCopy);
            failed = 1;
            break;
        }
        setCellAndUpdateBoard(game,x,y,options[rand()%optionsCnt]);
    }
    free(emptyCellsArr);
    free(options);
    freeBoardArrCopy(N,boardCopy);

    return (!failed);

}


void Generate(Game* game, int XtoFill, int YtoKeep){
    int** GRBBoardCopy;
    int** boardCopy;
    int grb,
    N = game->N,
    filledCells = game->filledCells,
    emptyCells = N*N - filledCells,
    counter = 0;
    if(curGameMode != EDIT){
        printf("Error: generate only available in EDIT mode.\n");
        return;
    }

    if(XtoFill > N*N || XtoFill < 0 || YtoKeep <= 0 || YtoKeep > N*N){
        printf("Error: invalid input parameters for generate command: the X's value "
               "has to be in range [0,%d], Y's value has to be in range [1,%d] \n",emptyCells, N*N );
        return;
    }

    if(XtoFill > emptyCells){
        printf("Error: X's value is too big, the board has only %d empty cells .\n",emptyCells);
        return;
    }

    if(validateBoard(game) != 1){
        printf("Error: board is unsolvable, can not generate such board.\n");
        return;
    }

    boardCopy = malloc(N* sizeof(int*));
    if (boardCopy == NULL){
        printf("Error: memory error.\n");
        return;
    }
    GRBBoardCopy = malloc(N* sizeof(int*));
    if(GRBBoardCopy == NULL){
        free(boardCopy);
        printf("Error: memory error.\n");
        return;
    }

    copyGameToArray(game,boardCopy,1);
    mallocIntsBoard(N,GRBBoardCopy);

    while(counter < 1000 ){
        counter ++;
        /* fill X empty cells */
        if(!fillRandCells(game, XtoFill)){
            continue; /*failed to fill, try another time */
        }
        if(game->filledCells == N*N){
            goto FINISH;
        }
        copyGameToArray(game, GRBBoardCopy, 0);
        grb = exeGurobi(game, 1, 0, 0, 0, 0, GRBBoardCopy);
        if(grb != 1){
            copyArrayToGame(game,boardCopy);
            counter++;

        }
        else{
            copyArrayToGame(game, GRBBoardCopy);
            keepRandCells(game,YtoKeep,N);
            addToMovesList(game,boardCopy);
            printBoard(game);
            freeBoardArrCopy(N, GRBBoardCopy);
            freeBoardArrCopy(N, boardCopy);
            return;

        }
    }

    if(counter >= 1000){
        copyArrayToGame(game,boardCopy);
        printf("Generate board failed: after 1000 tries.\n");
        freeBoardArrCopy(N, GRBBoardCopy);
        freeBoardArrCopy(N, boardCopy);
        return;
    }

    FINISH:
    keepRandCells(game,YtoKeep,N);
    addToMovesList(game,boardCopy);
    printBoard(game);
    freeBoardArrCopy(N, GRBBoardCopy);
    freeBoardArrCopy(N, boardCopy);
    return;



}

void UnDo(Game* game){
    int done;
    if(curGameMode == SOLVE || curGameMode == EDIT){
        done = redo_undo_move(game,1);
        if(done){
            remarkInvalidGameCells(game);
            printBoard(game);
            return;
        }
        else{
            printf("Error: there is not moves to undo from here.\n");
            return;
        }
    }
    else{
        printf("Error: undo command only available in SOLVE or EDIT mode.\n");
        return;
    }

}

void ReDo(Game* game){
    int done;
    if(curGameMode == SOLVE || curGameMode == EDIT){
        done = redo_undo_move(game,0);
        if(done){
            remarkInvalidGameCells(game);
            printBoard(game);
            return;
        }
        else{
            printf("Error: there is not moves to redo from here.\n");
            return;
        }
    }
    else{
        printf("Error: redo command only available in SOLVE or EDIT mode.\n");
        return;
    }

}

/*load a new Game from file 'path' , if 'path' exists and in the right format
 * helps edit and solve commands*/
int handleSolveOrEdit(Game* currentGame, char* path, int Edit,GameMode mode){
    Game* newGame = malloc(sizeof(Game));
    if (newGame == NULL){
        printf("Memory error\n");
        return 0;
    }
    initStart(newGame);
    if ((path == NULL) && (Edit)) { /* if there wasn't a path - initialize default board */
        if(!setDefaultBoard(newGame)) { /*initializing didnt work*/
            free(newGame);
            return 0;
        }
    }
    else{
        if(loadBoard(newGame,path,mode) == -1) { /*initializing didnt work*/
            free(newGame);
            return 0;
        }
    }
    if((!Edit) && (!checkFixCells(newGame))){/*MODE solve and the board is erroneous with only fix cells*/
        printf("Error: The Fixed cells that are defined in the file cause an erroneous board");
        freeGame(newGame);
        free(newGame);
        return 0;
    }
    /*everything is ok*/
    freeGame(currentGame);
    memcpy(currentGame, newGame, sizeof(Game)); /* if the new board is OK - replace the old with the new one */
    free(newGame);
    return 1;
}


void Solve (Game* currentGame, char* path,GameMode mode){
    if(handleSolveOrEdit(currentGame, path,0,mode) == 1){
        remarkInvalidGameCells(currentGame);
        curGameMode = SOLVE;
        printBoard(currentGame);
    }
}

void Edit (Game* currentGame, char* path,GameMode mode){
    if(handleSolveOrEdit(currentGame, path,1,mode) == 1){
        curGameMode = EDIT;
        printBoard(currentGame);
    }
}


void Save(Game* game, char* path){
   /* int** boardCopy;
    int N = game->N; */
    int valid;
    if(curGameMode == EDIT){
        if(game->erroneousBoard){
            printf("Error: can not save an erroneous game board in EDIT mode.\n");
            return;
        } else {
            valid = validateBoard(game);
            if(valid == 1){
                saveBoard(game,path);
            } else if (valid == -2){
                printf("Error: validation failed, the board is unsolvable. Can not save such board.\n");
            } else{
                printf("Error: validation failed, gurobi error. Can not save such board.\n");
            }
            return;
        }
    } else if (curGameMode == SOLVE){
        saveBoard(game,path);
        return;
    }

    else{
        printf("Error: save command only available in SOLVE or EDIT mode.\n");
        return;
    }
}

void Hint(Game* game, int x, int y){
    int grb;
    int N = game->N;
    if(curGameMode != SOLVE){
        printf("Error: hint command only available in SOLVE mode.\n");
        return;
    }
    if(game->erroneousBoard){
        printf("Error: hint command not available for erroneous board.\n");
        return;
    }
    if(x > N - 1 || x < 0 || y > N - 1 || y < 0 ) {
        printf("Error: invalid index parameters for hint, try with valid index between [1,%d].\n", N);
        return;
    }

    else if(isFixCell(game,x,y)){
        printf("Error: hint command not available for fix cell.\n");
        return;
    }
    else if (!isEmptyCell(game, x, y)){
        printf("Error: hint command not available for not empty cell.\n");
    }
    else if (validateBoard(game) != 1){
        printf("Error: hint failed - the board is unsolvable.\n");
        return;
    }
    else{
        grb = exeGurobi(game, 1, 1, x, y, 0, NULL);
         if (grb != 1 ){
            printf("Error: hint failed - the board is unsolvable.\n");
        }
        return;
    }
}

void GuessHint(Game* game, int x, int y){
    int grb;
    int N = game->N;
    if(curGameMode != SOLVE){
        printf("Error: guess hint command only available in SOLVE mode.\n");
        return;
    }
    if(game->erroneousBoard){
        printf("Error: guess hint command not available for erroneous board.\n");
        return;
    }

    if(x > N - 1 || x < 0 || y > N - 1 || y < 0 ){
        printf("Error: invalid index parameters for guess hint, try with valid index between [1,%d].\n",N);
        return;
    }

    else if(isFixCell(game,x,y)){
        printf("Error: guess hint command not available for fix cell.\n");
        return;
    }
    else if (!isEmptyCell(game, x, y)){
        printf("Error: guess hint command not available for non empty cell.\n");
    }
    else{
    grb = exeGurobi(game,0,1,x,y,0,NULL);
        if (grb != 1 ){
            printf("Error: guess hint failed - the board is unsolvable.\n");
        }

}

}


void Num_solutions(Game* game){
    int result;
    if(curGameMode == SOLVE || curGameMode == EDIT){
        if(game->erroneousBoard){
        printf("Error: the board is erroneous, can not solve it.\n");
            return;
        }
        result = solCount(game);
        printf("The current board has :  %d solutions.\n", result);
        return;
    }
    else{
        printf("Error: this command only available in SOLVE or EDIT mode.\n");
        return;
    }

    }

void Autofill(Game* game){
    int changed;
    int ** oldBoard;
    int N = game->N;
    if(curGameMode != SOLVE){
        printf("Error: autofill command is only available in SOLVE mode.\n");
        return;
    }

    if(game->erroneousBoard){
        printf("Error: can not auto fill an erroneous board.\n ");
        return;
    }
    oldBoard = malloc(N* sizeof(int*));
    if(oldBoard == NULL){
        printf("Memory error.\n");
    }

    copyGameToArray(game, oldBoard, 1);
    changed = autoFillBoard(game);
    addToMovesList(game,oldBoard);
    freeBoardArrCopy(N, oldBoard);
    if(changed) {
        printBoard(game);
    }
    if(isFullBoard(game)){
        if(game->gameOver){
            printf("Congratulations! you solved the board !\n");
            curGameMode = INIT;
           return;
        }
        else{
            printf("The sudoku board is erroneous.\n");
        }
    }

}


void Reset(Game* game){
    Move curMove;
    ListNode* node = game->currentMove;
    int len,i;
    if(curGameMode == INIT){
        printf("Error: reset command is only available in SOLVE or EDIT mode.\n");
        return;
    }
    if(node == NULL){
        printBoard(game);
        return;
    }
    while(node->len != -1) {
        len = node->len;
        for (i = 0; i < len; i++) {
            curMove = node->moves[i];
            setCellAndUpdateBoard(game, curMove.x, curMove.y, curMove.oldValue);
        }

        node = node->prev;
    }
    game->currentMove = node;
    printBoard(game);
}

void Exit(Game* game){
    printf("Exiting... \n Thank you for playing!\n");
    freeGame(game);
}
