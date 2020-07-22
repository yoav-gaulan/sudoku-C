#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c.h"
#include "gurobi.h"
#include "game.h"





/*allocate memory for our 3D matrix, hold the board variables,
 * count the number of variables for currnet board and returns thar value */

int setMatrix(Game *game, int ***matrix) {
    int i, j, z, cnt;
    int N = game->N;

    /* memory allocate */
    for (i = 0; i < N; ++i){
        matrix[i] = (int **) malloc(sizeof(int *) * N);
    }

    for(i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            matrix[i][j] = (int *) malloc(sizeof(int) * N);
            for(z = 0; z < N ; z++){
                matrix[i][j][z] = -1;
            }
        }
    }

    cnt=0;
    for(i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            for (z = 0; z < N ; z++){
                if(isEmptyCell(game,i,j)  && isValidValue(game, i, j, z+1)) {
                    matrix[i][j][z] = cnt;
                    cnt++;
                }
            }
        }
    }
    return cnt;
}





/*buliding gurobi model , for LP random obj function and continuous varibles. ILP binary variable and constant obj function  */

int buildModel(GRBmodel *model, int N , int varCnt, int ilp, double* obj, char* vtype , double* lb){
    int i,  error = 0;
    srand(time(NULL));
    for(i = 0 ; i < varCnt; i++){
        obj[i] = ilp ? 0 : (rand()%N*N)+1;
        vtype[i] = ilp ? GRB_BINARY : GRB_CONTINUOUS;
        lb[i] = 1;
    }

    /* adding variables */
    if(ilp){
        error = GRBaddvars(model, varCnt, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
    }

    else{
        error = GRBaddvars(model, varCnt, 0, NULL, NULL, NULL, obj, NULL, lb, vtype, NULL);

    }
    return error;
}

/*free the memory that allocated for the 3D matrix */
void freeMatrix(int N, int*** matrix){
    int i,j;
    for(i = 0; i < N ; i++){
        for(j = 0; j < N ; j++){
            free(matrix[i][j]);
        }
        free(matrix[i]);
    }
    free(matrix);
}

/*free the memory that allocated for GUROBI: matrix, model environment, variables, and etc.   */
void freeAll(GRBmodel* model, GRBenv   *env, int N, int*** matrix,	int* ind, double* val, char* vtype ,double* sol , double* obj, double* lb){
    freeMatrix(N, matrix);
    GRBfreemodel(model);
    GRBfreeenv(env);
    free(ind);
    free(val);
    free(obj);
    free(vtype);
    free(sol);
    free(lb);
}

/* Build constrains of the following type: each cell must have value  */
int constrain1(int N , int*** matrix, GRBmodel* model, int* ind, double* val ){
    int i,j, cnt,z, error = 0;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            cnt=0;
            for (z = 0; z < N ; z++) {
                if(matrix[i][j][z] != -1){     /*passing all the legal value for cell <i,j> */
                    ind[cnt] = matrix[i][j][z];
                    val[cnt++] = 1.0;
                }
            }
            if(cnt){
            error = GRBaddconstr(model, cnt, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                return error;
                }
            }
        }

    }

    return error;
}


/*Build constrains of the following type: each value must be in every row */
int constrain2(int N , int*** matrix, GRBmodel* model, int* ind, double* val ) {
    int i, j, cnt, z, error = 0;
    for (z = 0; z < N ; z++) {
        for (j = 0; j < N; j++) {
            cnt = 0;
            for (i = 0; i < N; i++) {
                if (matrix[i][j][z] != -1) {
                    ind[cnt] = matrix[i][j][z];
                    val[cnt++] = 1.0;
                }
            }
            if (cnt) {
                error = GRBaddconstr(model, cnt, ind, val, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    return error;
                }
            }
        }
    }
    return error;
}

/*Build constrains of the following type: each value must be in every column */
int constrain3(int N , int*** matrix, GRBmodel* model, int* ind, double* val) {
    int i, j, cnt, z, error = 0;
    for (z = 0; z < N ; z++) {
        for (i = 0; i < N; i++) {
            cnt = 0;
            for (j = 0; j < N; j++) {
                if (matrix[i][j][z] != -1) {
                    ind[cnt] = matrix[i][j][z];
                    val[cnt++] = 1.0;
                }
            }
            if (cnt) {
                error = GRBaddconstr(model, cnt, ind, val, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    return error;
                }
            }

        }
    }
    return error;
}

/*Build constrains of the following type: each value must be in every block */
int constrain4(Game* game, int N, int*** matrix, GRBmodel* model, int* ind, double* val) {
    int i, j, z, i_b, j_b, rows, cols, cnt;
    int error = 0;
    rows = game->blockHeight;
    cols = game->blockWidth;
    for (z = 0; z < N ; z++) {
        for (i_b = 0; i_b < cols; i_b++) {
            for (j_b = 0; j_b < rows; j_b++) {
                cnt = 0;
                for (i = i_b * rows; i < (i_b + 1) * rows ; i++) {
                    for (j = j_b * cols; j < (j_b+1) * cols; j++) {
                        if (matrix[i][j][z] != -1) {
                            ind[cnt] = matrix[i][j][z];
                            val[cnt++] = 1.0;
                        }
                    }
                }
                if(cnt) {
                    error = GRBaddconstr(model, cnt, ind, val, GRB_EQUAL, 1.0, NULL);
                    if (error) {
                        return error;
                    }
                }
            }
        }
    }
    return error;
}


/*copy the solution of a borad to 2D Matrix, helps generate function*/
void copyILPSolution(Game* game, int N, int*** matrix, double* sol, int** solToFill){
    int i,j,v;
    for(i = 0; i < N; i++){
        for(j = 0 ; j < N; j++){
            if(isEmptyCell(game,i,j)) {
                for (v = 0; v < N ; v++) {
                    if (matrix[i][j][v] != -1) {
                        if (sol[matrix[i][j][v]] == 1) {
                            solToFill[i][j] = v + 1;
                        }
                    }
                }
            }
        }

    }

}

/*Using the 3D matrix , based on GUROBI solution,  print a hint for the (x,y) cell
 *helps hint command*/
void printHint(int N, int xHint, int yHint, int*** matrix, double* sol){
    int v;
    for (v = 0; v < N; v++) {
        if (matrix[xHint][yHint][v] != -1) {

            if (sol[matrix[xHint][yHint][v]] == 1) {

                printf("Hint: the value for cell <%d,%d> is %d.\n", yHint + 1 , xHint + 1, v + 1);
            }
        }
    }
}



/* helper function for guess
 * gets arrary of weights for all possible values for cell,
 * returns random index + 1 , as random value
 **/

int getRandomVal(double* weights, int N){
    int index;
    double randV;
    double sum = 0.0;
    for (index = 0; index < N; index++) {
        sum += weights[index];
    }
    /* change the current weight according to the sum of weights */
    for (index = 0; index < N; index++) {
        if (sum != 0.0) {
            weights[index] /= sum;
        } else { /* if the threshold = 0 */
            weights[index] = (double)1 / N;
        }
    }

    randV = (double)rand() / (double )((unsigned)RAND_MAX + 1); /*get random value in [0,1] */


    sum = 0.0;
    for (index = 0; index < N; index++) {
        sum += weights[index];
        if (sum > randV) {
            break;
        }
    }


    return (index + 1);
}


/* for handle guess X command - choosing random legal values with score above threshold and set them*/
void setGuessedVals(Game* game, int N, int*** matrix, double* sol, float threshold) {
    int i, j, k, var, isOK = 0;

    double *weights = (double *) malloc(sizeof(double) * N);

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                var = matrix[i][j][k];
                if ((var == -1) || (sol[var] < threshold) || invalidCellValue(game, i, j, k+1)) {
                    weights[k] = 0.0;
                } else {
                    isOK = 1;
                    weights[k] = sol[var];
                }

            }
            if (isOK) {
                setCellAndUpdateBoard(game, i, j, getRandomVal(weights, N));
            }
            isOK = 0;
        }

    }

    free(weights);
}


/* prints all legal value for given cell index and their score */
void guessHintOutput(int N, int*** matrix, double* sol, int x, int y){
    int val;
    printf("Guess Hint: The legal values of cell <%d,%d> are:  \n",y+1,x+1);
    for(val = 1 ; val < N+1 ; val++){
        if(matrix[x][y][val-1] != -1 && sol[matrix[x][y][val-1]] > 0.0 ){
            printf("   Value: %d , score: %f \n", val, sol[matrix[x][y][val-1]]);
        }
    }
}


/* ILP = 0 means exe LP , else IPL.
 * return value: -1 = gurobi error, -2 = unsolavble, 1 = valid/success*/
int exeGurobi(Game* game, int ilp, int hint, int xHint, int yHint, float threshold, int** copyBoard){
    int*** matrix = NULL;
    int N = game->N;
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;
    int      *ind ;
    double   *val;
    char     *vtype;
    double   *sol;
    double   *obj;
    double   *lb;

    int state = -1;
    int       optimstatus;
    double    objval;
    int       varCount ;
    int       error = 0;


    matrix = (int***) malloc(N * sizeof(int**));
    varCount = setMatrix(game, matrix);
    if(varCount == 0 ){
        freeMatrix(N,matrix);
        return -2;
    }


    ind = (int*)malloc(N*sizeof(int));
    val = (double*)malloc(N*sizeof(double));
    obj = (double*)malloc(varCount*sizeof(double));
    vtype = (char*)malloc(varCount*sizeof(char));
    sol = (double*)malloc(varCount*sizeof(double));
    lb = (double*)malloc(varCount * sizeof(double));

    /*Loading Gurobi environment*/
    error = GRBloadenv(&env, "mip1.log");
    if (error) {
        printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
        state = -1;
        goto Quit;
    }
    /*Setting the parameters for the gurobi*/
    error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (error) {
        printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
        state = -1;
        goto Quit;
    }

    /* Create an empty model named "mip1" */
    error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
    if (error) {
        printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
        state = -1;
        goto Quit;
    }

    error = buildModel(model, N, varCount , ilp, obj, vtype, lb );
    if(error){
        printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
        state = -1;
        goto Quit;
    }

    /* Change objective sense to maximization */
    error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
    if (error) {
        printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
        state = -1;
        goto Quit;
    }

    error = GRBupdatemodel(model);
    if (error) {
        printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
        state = -1;
        goto Quit;
    }

    error = constrain1(N,matrix,model,ind,val);
    if(error){
        printf("ERROR %d 1nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
        state = -1;
        goto Quit;
    }

    error = constrain2(N,matrix,model,ind,val);
    if(error){
        printf("ERROR %d 2nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
        state = -1;
        goto Quit;
    }

    error = constrain3(N, matrix,model,ind,val);
    if(error){
        printf("ERROR %d 3nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
        state = -1;
        goto Quit;

    }

    error = constrain4(game,N,matrix,model,ind,val);
    if(error){
        printf("ERROR %d 4nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
        state = -1;
        goto Quit;
    }

    /* Optimize model - need to call this before calculation */
    error = GRBoptimize(model);
    if (error) {
        printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
        state = -1;
        goto Quit;
    }

    /* Write model to 'mip1.lp' */
    error = GRBwrite(model, "mip1.lp");
    if (error) {
        printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
        state = -1;
        goto Quit;
    }

    /* Get solution information */

    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
        state = -1;
        goto Quit;
    }


    /* solution found */
    if (optimstatus == GRB_OPTIMAL) {


        /* get the objective -- the optimal result of the function */
        error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
        if (error) {
            printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
            state = -1;
            goto Quit;
        }

        /* get the solution - the assignment to each variable */
        error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, varCount, sol);
        if (error) {
            printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
            state = -1;
            goto Quit;
        }

        switch (ilp) {
            case 1:     /*  ILP- for hint or generate */
                if (hint) { printHint(N, xHint, yHint, matrix, sol); }
                else { copyILPSolution(game, N, matrix, sol, copyBoard); }
                break;
            case 0:    /* LP - for guess_hint or guess */
                if (hint) { guessHintOutput(N, matrix, sol, xHint, yHint); }
                else { setGuessedVals(game, N, matrix, sol, threshold); }
                break;
            case 2:    /* in case we only need to validate the board (also used ILP) */
                break;
        }
        freeAll(model, env, N, matrix, ind, val, vtype, sol, obj, lb);
        return 1;

    }

    else if (optimstatus == GRB_INF_OR_UNBD) {
        printf("The board is unsolvable\n");
        state = -2;
        goto Quit;

    }
    Quit:
    freeAll(model, env, N, matrix, ind, val, vtype, sol, obj, lb);
    return state;





}





