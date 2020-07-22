

#ifndef FINALPROJECT_PRINTUSER_H
#define FINALPROJECT_PRINTUSER_H

#endif
/*
 * print User module summary:
 * Responsible for printing errors and messages to user
 */

/* types of messages*/

typedef enum messageTypeH {Promot, tooLong, emptyCom, Invaild, tooManyArgs,
                           invaildSolveParam, invaildEditParam,ivalidMarkErrParam,invalidSetParam1,invalidSetParam2, invaildSaveParam,
                           invalidGuessParam1,  invalidGuessParam2, invalidGenerateParam1, invalidGenerateParam2,
                           invalidHintParam1,invalidHintParam2, invalidGuessHintParam1,invalidGuessHintParam2,
                           invalidInSolve, invalidInEdit,  invalidInInit } messageType;

/* The function responsible for printing*/

void printToUser(messageType message);
