#include "solver.h"
#include "mainAux.h"





/* increase the value of the cell (x,y) in 1*/

void increaseCellValue(Game* game, int i, int j){
    getCell(game,i,j)->value += 1;
}
/*set the value in the cell (x,y) to 0 mean the cell empty*/

void setToZero(Game* game, int i, int j) {
    getCell(game, i, j)->value = 0;
}

/*if we got (x,y)=N-1, (max value) , set it to 0, and handle the next empty cell*/

void handleMaxValueForCell(Game* game , Stack* stack, int *x , int* y){
    indexNode top ;
    setToZero(game,*x, *y);
    pop(stack);
    if(!isEmptyStack(stack)) {
        top = getHead(stack);
        *x = top.x;
        *y = top.y;
        increaseCellValue(game, *x, *y);
    }

}

/*finds the next empty cell in the board*/

int nextEmptyCell(Game* game, int* i, int* j ){
    int x = *i;
    int y = *j;
    do {
        if(isEmptyCell(game, *i, *j)){
            return 1;
        }
    }while (getNextCellIndex(game->N, i, j));

    *i = x;  /*in case there in no next empty cell, return the pointers to origin */
    *j = y;
    return 0;
}


/* counts the number of solutions of the current board
 * Game* game- pointer to current game
 * int x, int y - indices of the first empty cell in the current board
 * maxLength - number of the empty cells
 * return counter if success, 0 else.
 */

int iterativeBacktrack(Game *game, int startX, int startY, int maxLength) {
    int x , y, N, cnt = 0;
    Stack* stack ;
    indexNode top;
    N = game->N;
    x = startX;
    y = startY;
    if(!maxLength){
        return 0;
    }
    stack = initStack(maxLength);
    if(stack == NULL){
        printf("Error : malloc error.\n");
        return 0;
    }
    push(stack, x, y);
    increaseCellValue(game, x, y);

    while(getCellValue(game,startX,startY) < N + 1){

        if(getCellValue(game,x,y) > N ){
            handleMaxValueForCell(game, stack , &x, &y);
            continue;
        }


        if(isInvalidCell(game, x, y)){
            increaseCellValue(game, x, y);
            continue;
        }

        if(!nextEmptyCell(game,&x,&y)){
            cnt += 1;
            increaseCellValue(game, x, y);
            continue;
        }
        push(stack, x, y);
        increaseCellValue(game, x, y);


    }
    top = getHead(stack);
    setToZero(game, top.x, top.y);
    deleteStack(stack);
    return cnt;
}

int solCount(Game* game){
    int solCnt = 0;
    int startX = 0;
    int startY = 0;
    int maxLength = 0;
    int N = game->N;
    int** boardCopy;
    if (!nextEmptyCell(game, &startX, &startY)){
        return 0;
    }
    maxLength = countEmptyCells(game);
    boardCopy =  malloc(N* sizeof(int*));
    if(boardCopy == NULL){
        printf("Error: memory error.\n");
        return 0;
    }

    if(copyGameToArray(game, boardCopy, 1)) {
        solCnt = iterativeBacktrack(game, startX, startY, maxLength);
        copyArrayToGame(game, boardCopy);
    }

    freeBoardArrCopy(N, boardCopy);
    return solCnt;

}
