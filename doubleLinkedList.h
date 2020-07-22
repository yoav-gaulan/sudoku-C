
/*
 * DoublyLinkedList module summary:
 *Contains functions for handling the double Linked List class, which is used for the our redo/undo list.
 * Each node represents a user's command, and hold Moves array of all changes done in the same command
 * The command is one of the following: "set", "autofill", "generate", or "guess")
 */

/* represent a command in the redo/undo list, contains all the moves that the command makes */

typedef struct ListNode{
        struct ListNode *next;
        struct ListNode *prev;
        struct Move *moves;
        int len;
} ListNode ;

/* represent one change in the board game */

typedef struct Move{
    int x;
    int y;
    int currentValue;
    int oldValue;
    int index;

}Move;

/* these were all the structs*/

/*adds a new ListNode with the given list of moves after the given node.*/

ListNode* addNode(ListNode* node, Move* move, int len);

/* Deletes all of the following nodes, and the moves in the nodes, after the given node.*/

void deleteAllAfter(ListNode** after);

/* Deletes all nodes in the list that the given node is part of.*/

void deleteList(ListNode** node);



