

#include "mainAux.h"
#include <stdio.h>
#include <ctype.h>

/* Keeps indexes of empty cells in the current game in the array 'arr'
 * helps to generate command*/
void setEmptyCellsArr(Game* game, int len, indexNode* arr){
    int N = game->N,
    i,j, index = 0;
    for(i = 0; i < N ; i++){
        for (j = 0; j < N ; j++) {
            if(isEmptyCell(game, i, j)){
                arr[index].x = i;
                arr[index].y = j;
                index++;
            }
            if(index == len){
                return;
            }

        }
    }

}

/* pop the value of indexNode in index in the arr-
 * helps to generate command*/
void popAndShift(int len, int index, indexNode* arr){
    if(len <= 0 || index >= len){
        return;
    }
    arr[index].x = arr[len - 1].x;
    arr[index].y = arr[len - 1].y;
}

/* counts the empty cells in the board
 * helps generate command and num_solutions command*/
int countEmptyCells(Game* game){
    int N = game->N,
    i,j, count = 0;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            if(isEmptyCell(game, i, j)){
                count += 1;
            }
        }
    }
    return count;
}

/* gets next cell's indexes, current (*i,*j)
 * helps function loadboard*/
int getNextCellIndex(int N, int* i, int* j) {
    if (*j < (N - 1)) {
        *j += 1;
        return 1;
    } else if (*i < (N - 1)) {
        *j = 0;
        *i += 1;
        return 1;
    }
    return 0;
}

/*allocate space for N*N board of ints*/

int mallocIntsBoard(int N, int** arr){
    int i,j;
    for(i = 0; i < N ; i++ ){
        arr[i] = malloc(N* sizeof(int));
        if(arr[i] == NULL){
            for(j = 0; j < i; j++){
                free(arr[j]);
            }
            printf("Memory error.\n");
            return 0;
        }
    }
    return 1;
}

/*allocates Dynamic space N*N*sizeof(cell) in heap for the all board
 * return 1 if success ,0 else
 */

int mallocBoard(int N, Game* game){
int i,j;
    game->gameBoard = malloc(N* sizeof(Cell*));
    if(game->gameBoard == NULL){
        printf("Memory error\n");
        return 0;
    }
    for(i = 0; i < N; i++){
        game->gameBoard[i] = malloc(N* sizeof(Cell));
        if(game->gameBoard[i] == NULL){
            for(j = 0; j < 0; j++){
                free(game->gameBoard[j]);
            }
            free(game->gameBoard);
            printf("Memory error\n");
            return 0;
        }
    }
    return 1;
}



void addSingleMoveNode(Game *game, int x, int y, int newVal, int oldVal) {
    Move *move = (Move *) malloc(sizeof(Move));
    if (move == NULL) {
        printf("Memory error");
        return;
    }
    move->oldValue = oldVal;
    move->currentValue = newVal;
    move->x = x;
    move->y = y;
    move->index = 0;

    game->currentMove = addNode(game->currentMove, move,1);
    if(game->listMovesHead == NULL){
        game->listMovesHead = game->currentMove->prev;
    }

}

int countChanges(Game* game, int** oldBoard){
    int i,j;
    int cnt = 0;
    int N = game->N;
    for(i=0; i < N ; i++){
        for(j=0; j< N; j++){
            if(getCellValue(game,i,j) != oldBoard[i][j]){
                cnt++;
            }
        }
    }
    return cnt;
}

int addToMovesList(Game* game, int** oldBoard) {

    Move* moves = NULL;
    int x, y, i = 0;
    int N = game->N;
    int len = countChanges(game, oldBoard);
    if (len == 0) {
        return 0;
    }
    moves = (Move *) malloc(len * sizeof(Move));

    for (x = 0; x < N; x++) {
        for (y = 0; y < N; y++) {
            if (getCellValue(game, x, y) != oldBoard[x][y]) {
                moves[i].currentValue = getCellValue(game, x, y);
                moves[i].oldValue = oldBoard[x][y];
                moves[i].x = x;
                moves[i].y = y;
                moves[i].index = i;
                i++;
            }
        }
    }
    game->currentMove = addNode(game->currentMove,moves,len);
    if(game->listMovesHead == NULL){
        game->listMovesHead = game->currentMove->prev;
    }
    return 1;


}





/*set cell and mark fix if needed and update filled cells*/
void setLoadedCell(Game* game, int x , int y, int z , int tofix, GameMode mode){
    game->gameBoard[x][y].value = z;
    game->gameBoard[x][y].isErroneous = 0;
    game->gameBoard[x][y].isFix = 0;
    if(mode == SOLVE && tofix){
        game->gameBoard[x][y].isFix = 1;
    }
    if(z){
        game->filledCells++;
    }


}

void handleLoadError(int err){
    switch(err){
        case 3:
            printf("Error: the first line in file must contain two valid integers for blocks dimension.\n");
            break;
        case -1:
            printf("Error: invalid file format.\n");
            break;
        case -2:
            printf("Error: invalid block dimensions.\n");
            break;
        case -3:
            printf("Error: invalid value found in file - out of range, all values have to match the correct range derived from dimensions declared in it first line.\n");
            break;
        case -4:
            printf("Error: invalid file, all the values in the file have to be integers and in range that match the dimensions declared in it first line. .\n");
            break;
        case -5:
            printf("Error: invalid file - must be space after fix value.\n");
            break;
        case -6:
            printf("Error: invalid file - can not fix an empty cell.\n");
            break;
        case -7:
            printf("Error: allocate memory failed");
            break;
        case -8:
            printf("Error: file contains too many characters, numbers of values must match the dimensions declared in it first line. \n");
            break;
        case -9:
            printf("Error: file does not contain enough values, numbers of values must match the dimensions declared in it first line.\n");
            break;
    }
}

void freeBoard(Game* game){
    int i;
    for (i = 0; i < game->N; i++) {
        free(game->gameBoard[i]);
    }
    free(game->gameBoard);
}

void initGameFields(Game* game, int width, int height, int N){
    game->N = N;
    game->blockWidth = width;
    game->blockHeight = height;
    game->filledCells = 0;
    game->printErroneous = 1;
    game->erroneousBoard = 0;
    game->gameOver = 0;
}

int isLastCell(Game* game, int i, int j){
    if((i == game->N -1) && (j == game->N -1)){
        return 1;
    }
    return 0;
}

/* 0 means success , 3 error reading , -2 not valid size */
int loadBlockSizeError(FILE* file, int* width, int* height){
    int status;
    status = fscanf(file, "%d %d", height, width);
    if(status != 2){
        return 3;
    }
    if((*width)*(*height) > 99 || (*width)*(*height) <= 0){
        return -2;
    }

    return 0;
}


int loadValueCell(FILE* file, Game* game, int x, int y, GameMode mode) {
    char c, charFix;
    int val, intFix = 0;
    int input;
    int N = game->N;

    input = fscanf(file, "%d%c", &val, &charFix);
    if ((input == 1 && isLastCell(game, x, y)) || input == 2) {
        if (val < 0 || val > N) {
            return -3;                   /*invalid range value*/
        } else if (charFix != '.' && (!isspace(charFix))) {
            return -4;                  /*  not number nor dot !!! */
        } else if (charFix == '.') {
            intFix = 1;
            if (val != 0) {
                if (!isLastCell(game, x, y)) {
                    if (fscanf(file, "%c", &c) != 1 || (!isspace(c))) {
                        return -5;           /*after dot with need to separate!!!!*/
                    }
                }
            }

            if (val == 0) {
                return -6;           /*cant fix empty cell*/
            }
        }
        setLoadedCell(game, x, y, val, intFix, mode);
        return 0;
    }
    if((input == 1 && (!isLastCell(game, x, y)))){
        return -9;
    }

    return -4; /* invalid file - inculde invalid char*/
}


int loadBoard(Game* newGame, char* path, GameMode mode){
    int width, height, error, N,
            i = 0,
            j = 0,
            counter = 0;
    FILE* file;
    char c;
    file = fopen(path,"r");
    if(file == NULL){
        printf("Error: invalid file path.\n");
        return -1;
    }
    error = loadBlockSizeError(file, &width, &height);
    if(error){
        fclose(file);
        handleLoadError(error);
        return -1;
    }
    N = width*height;
    if(!mallocBoard(N,newGame)){
        fclose(file);
        handleLoadError(-7);
        return -1;
    }

    initGameFields(newGame,width,height,N);
    while (!feof(file) && counter < N*N){
        error = loadValueCell(file,newGame,i,j,mode);
        if(error){
            handleLoadError(error);
            freeBoard(newGame);
            fclose(file);
            return -1;
        }
        getNextCellIndex(N,&i,&j);
        counter ++;
    }


    while(!feof(file)){
        if(fscanf(file,"%c",&c) == 1 && (!isspace(c))){
            handleLoadError(-8);    /*too many symbols*/
            fclose(file);
            freeBoard(newGame);
            return -1;
        }
    }

    fclose(file);
    remarkInvalidGameCells(newGame);
    return 1;
}


/* 0 means fails, 1 means success*/
int setDefaultBoard(Game* newGame){
    int i,j;
    if(!mallocBoard(9,newGame)){
        return 0;
    }
    initGameFields(newGame,3,3,9);
    for(i=0; i<9 ; i++){
        for(j=0 ; j<9; j++){
            newGame->gameBoard[i][j].value = 0;
            newGame->gameBoard[i][j].isErroneous = 0;
            newGame->gameBoard[i][j].isFix = 0;
        }
    }
    return 1;
}

void printBoard(Game* game){
        Cell curCell;
        int i,j,
        rows = game->blockHeight,
        cols = game->blockWidth,
        N = game->N,
        printError = game->printErroneous,
        sparateConst = 4 * N + rows + 1;

        for(i=0; i < sparateConst ; i++){
            printf("-");
        }
        printf("\n");
        for(j = 0; j < N; j++){
            for(i = 0; i < N ;i++){
                curCell = game->gameBoard[j][i];
                if(i == 0){
                    printf("|");
                }
                if(curCell.value == 0){
                    printf("   ");}
                else{
                    printf(" %2d", curCell.value);}
                if(curGameMode == EDIT || printError){
                    if(curGameMode == SOLVE && curCell.isFix){
                        printf(".");}
                    else if(curCell.isErroneous){
                        printf("*");
                    }
                    else{
                        printf(" ");
                    }

                }else{
                    if(curCell.isFix){
                        printf(".");
                    }
                    else{
                        printf(" ");
                    }
                }
                if((i+1)%(cols) == 0){
                    printf("|");
                }
            }
            printf("\n");
            if((j+1)%rows == 0){
                for(i = 0; i < sparateConst; i++){
                    printf("-");
                }
                printf("\n");

            }
        }

    }

void saveBoard(Game* game, char* path){
    int rows = game->blockHeight,
    cols = game->blockWidth,
    N = game->N,
    i,j;
    Cell* curCell;
    FILE* newFile = fopen(path, "w");
    if(newFile == NULL){
        printf("Error: there is problem to save to the path - %s",path);
        return ;
    }
    fprintf(newFile, "%d %d\n",rows,cols);
    for(i = 0; i < N; i++ ){
        for(j = 0; j < N; j++){
            curCell = getCell(game,i,j);
            if(curCell->isFix || (curGameMode == EDIT && curCell->value != 0)){
                fprintf(newFile, "%d. ", getCellValue(game,i,j));
            }
            else{
                fprintf(newFile, "%d ", getCellValue(game,i,j));
            }

        }
        fprintf(newFile, "\n");
    }
    fclose(newFile);
    printf("Game saved to %s.\n",path);

}



/* undo = 1 for undo move, else redo*/
int redo_undo_move(Game* game, int undo){
    Move curMove;
    ListNode* node = game->currentMove;
    int len,i;
    if(node == NULL){
        return 0;
    }
    if(undo && node->len == -1){
        return 0;
    }
    if(!undo && node->next == NULL ){
        return 0;
    }
    if(undo){
        len = node->len;
        for(i = 0; i < len; i++){
            curMove = node->moves[i];
            printf("Undo move: set the value of cell <%d,%d> to %d.\n",curMove.y+1,curMove.x+1,curMove.oldValue);
            setCellAndUpdateBoard(game,curMove.x,curMove.y,curMove.oldValue);
        }
        game->currentMove = game->currentMove->prev;
        return 1;
    }
    else{
        node = node->next;
        len = node->len;
        for(i = 0; i < len; i++){
            curMove = node->moves[i];
            printf("Redo move: set the value of cell <%d,%d> to %d.\n",curMove.y+1,curMove.x+1,curMove.currentValue);
            setCellAndUpdateBoard(game,curMove.x,curMove.y,curMove.currentValue);
        }

        game->currentMove = game->currentMove->next;
        return 1;

    }
}

void freeGame(Game* game){
    int i,
    N = game->N;
    if(game->listMovesHead != NULL) {
        deleteList(&game->currentMove);
    }
    for(i = 0 ; i< N; i++){
            free(game->gameBoard[i]);
    }
        free(game->gameBoard);
    }



