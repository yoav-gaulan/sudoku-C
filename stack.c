#include <stdlib.h>
#include <stdint.h>
#include "stack.h"


void push(Stack* stack, int x, int y){
    int ind = stack->top + 1;
    if(ind == stack->maxLength){
        return;
    }
    stack->nodes[ind].x = x;
    stack->nodes[ind].y = y;
    stack->top = ind;

}


int isEmptyStack(Stack* stack){
    return stack->top == - 1 ? 1 : 0;
}

int pop(Stack *stack) {
    int top = stack->top;
    if (top == -1) {
        return 0;
    }
    stack->top --;
    return 1;
}

Stack* initStack(int len){
    Stack* stack = malloc(sizeof(Stack));
    indexNode* nodes = malloc(len * sizeof(indexNode));
    if(nodes == NULL || stack == NULL){
        free(nodes);
        free(stack);
        return NULL;
    }
    stack->maxLength = len;
    stack->nodes = nodes;
    stack->top = -1;

    return stack;
}

void deleteStack(Stack* stack){
    if(stack == NULL){
        return;
    }
    free(stack->nodes);
    free(stack);
}

indexNode getHead(Stack *stack) {
    int ind = stack->top;
    indexNode top = stack->nodes[0];
    if(ind > -1 ){
        top = stack->nodes[ind];
    }
    return top;
}





