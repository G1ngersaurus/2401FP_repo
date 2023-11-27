#include "defs.h"

void initGhost(GhostClass class, GhostType **ghost){
    GhostType *newGhost = malloc(sizeof(GhostType));
    newGhost->type = class;
    newGhost->boredom = 0;

    *ghost = newGhost;
}

void pickStartingRoom(RoomListType *list, GhostType *g){

    int room = randInt(1, 12);

    RoomNodeType *currNode;
    RoomNodeType *prevNode;

    int i = 0;

    currNode = list->head;
    prevNode = NULL;

    //Might be i <= room
    while (currNode != NULL && i < room){
        prevNode = currNode;
        currNode = currNode->next;
        i++;
    }
    currNode->data->ghost = g;
}