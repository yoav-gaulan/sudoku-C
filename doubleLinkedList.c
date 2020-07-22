#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "doubleLinkedList.h"
#include <stdlib.h>

void deleteAllAfter(ListNode** after)
{
    ListNode* tmp;
    ListNode* toDel = (*after)->next;
    if(toDel == NULL){
        return;
    }
    while(toDel != NULL){
        tmp = toDel->next;
        free(toDel->moves);
        free(toDel);
        toDel = tmp;

    }
    (*after)->next = NULL;
}

ListNode* addNode(ListNode* node, Move* move , int len){
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    if(newNode == NULL) {
        printf("error: memory error");
        return NULL;
    }
    if(node == NULL){  /* init new head to list */
        node = (ListNode*) malloc(sizeof(ListNode));
        node->len = -1;
        node->moves = NULL;
        node->prev = NULL;
        node->next = NULL;
    }
    newNode->len = len;
    newNode->moves = move;
    newNode->prev = node;
    newNode->next = NULL;
        if(node->next != NULL) {
            deleteAllAfter(&node);
        }


    node->next = newNode;
    return newNode;

}

void deleteList(ListNode** node){
    while((*node)->len != -1){
        *node = (*node)->prev;
    }

    deleteAllAfter(node);
    free((*node)->moves);
    free(*node);
}


