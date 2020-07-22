#ifndef STACK_H
#define STACK_H


/*
 * stack module summary:
 * Contains functions for handling the stack class, which is used for solving the board by the backtracking algorithm.
 */

/* represent a location of a board cell*/

typedef struct indexNode  {
    int x;
    int y;

}indexNode;

/*Represents the stack : The number of nodes it now contains, maximum number of nodes it can contain, and the nodes*/

typedef struct Stack{
    int maxLength;
    int top;
    indexNode* nodes;

}Stack;


/* these were all the structs*/

/*Adds a new ListNode with the values x, y to the top of the stack*/

void push(Stack* stack, int x, int y);

/*Removes the first ListNode in the stack*/

int pop(Stack *stack);

/*return the top node
 *stack - pointer to the stack*/

indexNode getHead(Stack *stack);

/* checks whether the stack is empty
 * stack - pointer to the stack*/

int isEmptyStack(Stack* stack);

/* delete the stack, and free the memory of it
 * stack - pointer to the stack*/

void deleteStack(Stack* stack);

/*Return an empty stack, with maximum capacity 'len'
 *int len - yhe maximum capacity*/

Stack* initStack(int len);


#endif
