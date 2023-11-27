#include "defs.h"

void initRoomList(RoomListType *list){
    list->head = NULL;
    list->tail = NULL;
}

RoomType* createRoom(char *newName){
    RoomType *newRoom = malloc(sizeof(RoomType));
    strcpy(newRoom->name, newName);

    RoomListType rList;
    initRoomList(&rList); 
    newRoom->adjacentRooms = &rList;

    EvidenceListType eList;
    initEvidenceList(&eList);
    newRoom->evidence = &eList;

    HunterListType hList;
    initHunterList(&hList);
    newRoom->hunters = &hList;

    sem_t mutexInit;
    
    if (sem_init(&mutexInit, 0, 1) < 0) {
    printf("semaphore initialization error\n");
    exit(1);
    }
    newRoom->mutex = mutexInit;

    return newRoom;
}

void addRoom(RoomListType *list, RoomType *r){
    RoomNodeType *currNode;
    RoomNodeType *prevNode;
    RoomNodeType *newNode;

    currNode = list->head;
    prevNode = NULL;

    while (currNode != NULL){
        prevNode = currNode;
        currNode = currNode->next;
    }

    newNode = malloc(sizeof(RoomNodeType));
    newNode->data = r;
    newNode->next = NULL;

    if (prevNode == NULL){
        list->head = newNode;
    }else{
        prevNode->next = newNode;
        list->tail = newNode;
    }

    newNode->next = currNode;
}

void connectRooms(RoomType *r1, RoomType *r2){
    addRoom(r1->adjacentRooms, r2);
    addRoom(r2->adjacentRooms, r1);
}