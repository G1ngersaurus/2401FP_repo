#include "defs.h"

void initRoomList(RoomListType *list){
    list->head = NULL;
    list->tail = NULL;
}

void initRoom(char *newName, RoomType **room){
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

    *room = newRoom;
}