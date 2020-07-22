
#include "printUser.h"
#include <stdio.h>



void printToUser(messageType message){
    switch(message){
        case Promot:
            printf("Please enter a command:");
            break;
        case tooLong:
            printf("You have entered too many characters into the command. A command can contain up to 256 characters.\n");
            break;
        case emptyCom:
            printf("You have entered too many characters into the command. A command can contain up to 256 characters.\n");
            break;
        case Invaild:
            printf("Error: invalid command.\n");
            break;
        case tooManyArgs:
            printf("Error: you have entered too many arguments into the command.\n");
            break;
        case invaildSolveParam:
            printf("Error: Solve command should get exactly one parameter - path to file.\n");
            break;
        case invaildEditParam:
            printf("Error: Edit command should not get more then one parameter.\n");
            break;
        case ivalidMarkErrParam:
            printf("Error: mark_error command should get exactly one parameter.\n");
            break;
        case invalidSetParam1:
            printf("Error: Set command should get exactly 3 parameters.\n");
            break;
        case invalidSetParam2:
            printf("Error: all Set command parameters have to be positive integers.\n");
            break;
        case invaildSaveParam:
            printf("Error: Save command  should get exactly 1 parameters.\n");
            break;
        case invalidGuessParam1:
            printf("Error: Guess command  should get exactly 1 parameters.\n");
            break;
        case invalidGuessParam2:
            printf("Error: Guess command parameter have to be a number in range [0,1].\n");
            break;
        case invalidGenerateParam1:
            printf("Error: Generate command should get exactly 2 parameters.\n");
            break;
        case invalidGenerateParam2:
            printf("Error: Generate parameters have to be positive integers.\n");
            break;
        case invalidHintParam1:
            printf("Error: Hint command should get exactly 2 parameters.\n");
            break;
        case invalidHintParam2:
            printf("Error: Hint parameters have to be  positive integers.\n");
            break;
        case invalidGuessHintParam1:
            printf("Error: Guess hint command should get exactly 2 parameters.\n");
            break;
        case invalidGuessHintParam2:
            printf("Error: Guess hint parameters have to be positive integers.\n");
            break;
        case invalidInEdit:
            printf("Error: that command is not supported in Edit mode.\n");
            break;
        case invalidInSolve:
            printf("Error: that command is not supported in Solve mode.\n");
            break;
        case invalidInInit:
            printf("Error: that command is not supported in Init mode.\n");
            break;
    }
}
