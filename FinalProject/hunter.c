#include "defs.h"

void initHunterList(HunterListType *list){
    list->head = NULL;
    list->tail = NULL;
}

void initHunter(char *name, HunterType **hunter, enum EvidenceType equipment, RoomType *room, EvidenceListType *list){
    HunterType *newHunter = malloc(sizeof(HunterType));
    strcpy(newHunter->name, name);
    newHunter->boredom = 0;
    newHunter->fear = 0;
    newHunter->currRoom = room;
    newHunter->device = equipment;
    newHunter->evidence = list;

    *hunter = newHunter;
}

void addHunter(HunterListType *list, HunterType *h){
    HunterNodeType *currNode;
    HunterNodeType *prevNode;
    HunterNodeType *newNode;

    currNode = list->head;
    prevNode = NULL;

    while (currNode != NULL){
        prevNode = currNode;
        currNode = currNode->next;
    }

    newNode = malloc(sizeof(HunterNodeType));
    newNode->data = h;
    newNode->next = NULL;

    if (prevNode == NULL){
        list->head = newNode;
    }else{
        prevNode->next = newNode;
        list->tail = newNode;
    }

    newNode->next = currNode;
}