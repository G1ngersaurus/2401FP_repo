#include "defs.h"

void initEvidenceList(EvidenceListType *list){
    list->head = NULL;
    list->tail = NULL;
}

void addEvidenceLeave(RoomType *room, EvidenceType evidence){
    EvidenceNodeType *currNode;
    EvidenceNodeType *prevNode;
    EvidenceNodeType *newNode;

    currNode = room->evidence->head;
    prevNode = NULL;

    while (currNode != NULL){
        prevNode = currNode;
        currNode = currNode->next;
    }

    newNode = malloc(sizeof(EvidenceNodeType));
    newNode->data = evidence;
    newNode->next = NULL;

    if (prevNode == NULL){
        room->evidence->head = newNode;
    }else{
        prevNode->next = newNode;
        room->evidence->tail = newNode;
    }

    newNode->next = currNode;
}

void addEvidenceFind(EvidenceType evidence, HouseType *house){

}