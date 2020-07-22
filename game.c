

#include "game.h"
#include "solver.h"
#include "mainAux.h"



Cell * getCell(Game* game, int x, int y ){
    return &(game->gameBoard[x][y]);
}

int getCellValue(Game* game, int x, int y ){
    return (game->gameBoard[x][y].value);
}



int checkGameOver(Game* game){
    int N = game->N;
    remarkInvalidGameCells(game);
    if(game->filledCells == N*N && (!game->erroneousBoard) && curGameMode == SOLVE){
        return 1;
    }
    return 0;
}

int isFullBoard(Game* game){
    if(game->filledCells == game->N * game->N){
        return 1;
    }
    return 0;
}

void setCellAndUpdateBoard(Game* game, int x, int y, int z) {
    int oldVal = getCellValue(game, x, y);
    getCell(game, x, y)->value = z;

    if (z == 0 && oldVal != 0) {
        game->filledCells--;
        getCell(game, x, y)->isErroneous = 0;
        getCell(game, x, y)->isFix = 0;

    } else if (z != 0) {
        if (oldVal == 0) {
            game->filledCells++;
            game->gameOver = checkGameOver(game);
        }
        if (isInvalidCell(game, x, y)) {
            game->erroneousBoard = 1;
            getCell(game, x, y)->isErroneous = 1;
        }
    }
    remarkInvalidGameCells(game);
}



int isEmptyCell(Game *game, int i, int j) {
        if (game->gameBoard[i][j].value == 0) {
            return 1;
        }
        return 0;}


void freeBoardArrCopy(int N, int** matrix){
    int i;
    for (i = 0; i < N ; i++){
        free(matrix[i]);
    }
    free(matrix);
}





int copyGameToArray(Game *game, int **matrix, int uninitialized) {
    int i, j;
    int N = game->N;

    if(uninitialized) {
        /*malloc arrays */
        for (i = 0; i < N; i++) {
            matrix[i] = malloc(N * sizeof(int));
            if (matrix[i] == NULL) {
                for (j = 0 ; j < i; j++) {
                    free(matrix[j]);
                }
                printf("Memory error.\n");
                return 0;
            }
        }
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            matrix[i][j] = getCellValue(game, i, j);
        }
    }

        return 1;
    }


void copyArrayToGame(Game *game, int **matrix) {
        int i, j;
        int N = game->N;
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                setCellAndUpdateBoard(game, i, j, matrix[i][j]);
            }
        }

}



int checkValidRow(Game *game, int x, int y , int z) {
        int i;
        for (i = 0; i < (game->N); i++) {
            if (game->gameBoard[x][i].value == z && i != y) {
                return 0;
            }
        }
        return 1;
    }

int checkValidCol(Game *game, int x ,int y, int z) {
        int i;
        for (i = 0; i < (game->N); i++) {
            if (getCellValue(game, i, y) == z && i != x) {
                return 0;
            }
        }
        return 1;
    }

int checkValidBlock(Game *game, int x, int y, int z) {
    int blockRow = x / game->blockHeight;
    int blockCol = y / game->blockWidth;
    int rows = game->blockHeight,
    cols = game->blockWidth;
    int i,j;
    for (i = blockRow * rows; i < (blockRow + 1) * rows; i++) {
        for (j = blockCol * cols; j < (blockCol + 1) * cols; j++) {
            if ((i != x || j != y ) && getCellValue(game, i, j) == z) {
                return 0;
            }
        }
    }
    return 1;
}




int isValidValue(Game *game, int x, int y, int z) {
        if (checkValidRow(game, x,y, z) && checkValidCol(game,x, y, z) && checkValidBlock(game, x, y, z)) {
            return 1;
        }
        return 0;
    }

int invalidCellValue(Game *game, int x, int y, int z) {
        return !isValidValue(game, x, y, z);
    }

int isInvalidCell(Game *game, int x, int y) {
        int z = getCellValue(game, x, y);
        return !isValidValue(game, x, y, z);
    }

int isFixCell(Game *game, int x, int y) {
        return (getCell(game, x, y)->isFix);
    }

int invalidSetParam(int N, int x, int y, int z) {

        if (x > N - 1 || y > N - 1 || x  < 0 || y < 0) {
            printf("Error:  Invalid index for Set command , both x and y have to be between [1,%d].\n", N);
            return 1;
        }

        if (z > N || z < 0) {
            printf("Error: Invalid value for Set command, z has to be between [1,%d].\n", N);
            return 1;
        }
        return 0;

    }


/* if cell <x,y> has only 1 valit value return that value, else return 0; */

int getObviousValue(Game* game, int x, int y){
    int  i , val = 0;
    for(i = 1; i < game-> N+1 ; i++){
        if(isValidValue(game,x,y,i)){
            if(val > 0){
                return 0;
            }
            val = i;
        }
    }
    return val;
}


void setObviousValuesArray(Game* game, int** obsVals){
    int i,j,
    N = game->N;
    for(i = 0; i < N ; i++){
        for(j = 0; j < N ; j++){
            if(isEmptyCell(game, i, j)){
                obsVals[i][j] = getObviousValue(game,i,j);
            }
            else{
                obsVals[i][j] = 0;
            }
        }
    }

}

/* @pre: game is not erroneousBoard */

int autoFillBoard(Game* game){
    int N = game->N,
    i,j;
    int changed = 0;
    int** obiousVals = malloc(N * sizeof(int*));
    if(obiousVals == NULL){
        printf("Memory error.\n");
        return 0;
    }
    if(!mallocIntsBoard(N,obiousVals)){ /*allocation didnt work*/
        free(obiousVals);
        printf("Memory error.\n");
        return 0;
    }
    setObviousValuesArray(game,obiousVals);
    for(i = 0 ; i < N; i++){
        for(j = 0; j < N ; j++ ){
            if(obiousVals[i][j]){
                changed = 1;
                setCellAndUpdateBoard(game,i,j,obiousVals[i][j]);
                printf("Auto fill: the value of cell <%d,%d> set to %d. \n",j+1, i+1, obiousVals[i][j]);
            }
        }
    }

    if(!changed){
        printf("Auto fill: sorry, there is no cells that could be autofilled.\n");

    }


    freeBoardArrCopy(N,obiousVals);
    return changed;



}

/*autofill board without any prints.
 * inner function fot improve groubi by set obvious values.

 */


int autofillGRB(Game *game){
    int changed = 0;
    int N = game->N,
            i,j;
    int** obiousVals = malloc(N * sizeof(int*));
    if(obiousVals == NULL){
        return 0;
    }
    if(!mallocIntsBoard(N,obiousVals)){ /*allocation didnt work*/
        free(obiousVals);
        return 0;
    }

    do {
        changed = 0;
        setObviousValuesArray(game, obiousVals);
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                if (obiousVals[i][j]) {
                    changed = 1;
                    setCellAndUpdateBoard(game, i, j, obiousVals[i][j]);
                }
            }
        }
    }while (changed > 0);
    freeBoardArrCopy(N,obiousVals);
    return changed;

}


int countCellOptions(Game* game, int x, int y, int* options){
    int N = game->N,
    count = 0,
    v;
    for(v = 1; v < N + 1; v++){
        if(isValidValue(game,x,y,v)){
            options[count] = v;
            count ++;
        }
    }
    return count;

}


void remarkInvalidGameCells(Game *game) {
        int i, j, isErroneous = 0;
        int N = game->N;
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                if (getCellValue(game, i, j) == 0) {
                    getCell(game, i, j)->isErroneous = 0;
                } else {
                    if (isInvalidCell(game, i, j)) {
                        getCell(game, i, j)->isErroneous = 1;
                        isErroneous = 1;
                    } else {
                        getCell(game, i, j)->isErroneous = 0;
                    }
                }
            }
            game->erroneousBoard = isErroneous;

        }
    }


void unFixAllCells(Game* game){
    int N = game->N,
    i,j;
    for (i = 0; i < N ; i++) {
        for(j = 0; j < N; j++){
            game->gameBoard[i][j].isFix = 0;
        }

    }
}


void setHandle(Game *game, int x, int y, int z) {

        int newVal = z;
        int oldVal ;

        if (invalidSetParam(game->N, x, y, z)) {
            /*the func. invalidSetParam prints the error to user*/
            return;
        }

        if (curGameMode == SOLVE && isFixCell(game, x, y)) {
            printf("Error : cant change fix cell in SOLVE mode.\n");
            return;
        }
        oldVal = getCellValue(game, x, y);
        addSingleMoveNode(game, x, y, newVal, oldVal);
        setCellAndUpdateBoard(game, x, y, z);
        remarkInvalidGameCells(game);
        printBoard(game);
}



/* @pre : game->gameBoard is not erroneous */
int validateBoard(Game *game) {
    int grb;
    int N = game->N;
    int** curBoard;
    curBoard =  malloc(N* sizeof(int*));
    copyGameToArray(game,curBoard,1);
    autofillGRB(game);
    if(game->erroneousBoard){
        grb =  -2;

    } else if (game->filledCells == N*N){
        grb = 1;
    } else {
        grb = exeGurobi(game, 2, 0, 0, 0, 0, NULL);
    }
    copyArrayToGame(game,curBoard);
    freeBoardArrCopy(N,curBoard);
    return grb;
}



/* @pre : game->gameBoard is not erroneous */

int guessBoard(Game* game, float threshold ,int** oldBoard){
    int grb;
    copyGameToArray(game, oldBoard,1);
    grb = exeGurobi(game, 0, 0, 0, 0, threshold, NULL);
    if(grb == 1){
        addToMovesList(game,oldBoard);
        return 1;
    }
    else{
        return grb;
    }

}

void keepRandCells(Game* game, int toKeep, int N){
    int i,j, cnt;
    unFixAllCells(game);

    i = rand()%N;
    j = rand()%N;
    for(cnt = 0 ; cnt < toKeep; cnt++){
    while(isFixCell(game,i,j)) {
        i = rand() % N;
        j = rand() % N;
    }
    game->gameBoard[i][j].isFix = 1;
        }

    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            if(!isFixCell(game,i,j)){
                setCellAndUpdateBoard(game,i,j,0);
            }
        }
    }

    game->filledCells = toKeep;
}
/*checks if the fixed cell (x,y) = z is legal value
 *return 1 if TRUE, 0 else
 * helps checkFixCells function , in SOLVE mode*/
int isValidValueArray(int** arr, int x, int y, int z, int width, int height, int N){
    int i,j;
    int blockRow = x / height;
    int blockCol = y / width;
    int rows = height,
    cols = width;
    if(z == 0){
        return 1;
    }
    for (i = 0; i < N; i++) {
        if (arr[x][i] == z  && i != y) {
            return 0;
        }
    }

    for (i = 0; i < N; i++) {
        if (arr[i][y] == z && i != x ) {
            return 0;
        }
    }

    for (i = blockRow * rows; i < (blockRow + 1) * rows; i++) {
        for (j = blockCol * cols; j < (blockCol + 1) * cols; j++) {
            if ((i != x || j != y ) && arr[i][j] == z) {
                return 0;
            }
        }
    }
    return 1;

}


int checkFixCells(Game* game){
    int i,j,
    width = game->blockWidth,
    height = game->blockHeight,
    N = game->N;
    int** copyTheFix = malloc(N*sizeof(int*));
    if(copyTheFix == NULL){
        printf("Memory error\n");
        return 0;
    }
    if(!mallocIntsBoard(N,copyTheFix)){
        free(copyTheFix);
        return 0;
    }

    for(i = 0; i < N; i++){
        for (j = 0; j < N ; j++) {
                if(isFixCell(game,i,j)){
                    copyTheFix[i][j] = getCellValue(game,i,j);
                }
                else{
                    copyTheFix[i][j] = 0;
                }

        }
    }

    for(i = 0; i < N; i++){
        for (j = 0; j < N ; j++) {
            if(!isValidValueArray(copyTheFix,i,j,copyTheFix[i][j],width,height,N)){
                freeBoardArrCopy(N, copyTheFix);
                return 0;
            }
        }
    }

    freeBoardArrCopy(N, copyTheFix);
    return 1;

}

void initStart(Game* game){
    game->currentMove = NULL;
    game->gameBoard = NULL;
    game->N = 0;
    game->gameOver = 0;
    game->filledCells = 0;
    game->erroneousBoard = 0;
    game->listMovesHead = NULL;
    game->printErroneous = 1;
    game->blockWidth = 0;
    game->blockHeight = 0;


}


