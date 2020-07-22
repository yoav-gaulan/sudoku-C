
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "parser.h"
#include "mainAux.h"

int main(){
    Game* currentGame = malloc(sizeof(Game));
    Command* command = malloc(sizeof(Command));

    printf("\nWelcome to our SUDOKU!\n");
    printf("This game was created by Yoav Gaulan and Karina Buchmannn.\n\n");
    printf("Enjoy! <3 \n\n");

    initStart(currentGame);
    srand(time(0));
    curGameMode = INIT;
    command->type = init;
    while(command->type != exiti){
        getCommandAndPlay(currentGame, command);
    }
    free(command);
    free(currentGame);
    return 0;




}
