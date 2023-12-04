#include "defs.h"

////This function initailises a list of evidence which includes a head and a tail
void initRoomList(RoomListType *list){
    list->head = NULL;
    list->tail = NULL;
}

//This function creates a roomType object which includes:
//initialising the three lists RoomListType, EvidenceListType and HunterListType all found in a Roomtype object
//It also allocates space for the room in memory and initailises a mutex 
RoomType* createRoom(char *newName){
    RoomType *newRoom = malloc(sizeof(RoomType));
    strcpy(newRoom->name, newName);

    RoomListType rList;
    initRoomList(&rList); 
    newRoom->adjacentRooms = rList;

    EvidenceListType eList;
    initEvidenceList(&eList);
    newRoom->evidence = eList;

    HunterListType hList;
    initHunterList(&hList);
    newRoom->hunters = hList;

    sem_t mutexInit;
    
    if (sem_init(&mutexInit, 0, 1) < 0) {
        printf("semaphore initialization error\n");
    }
    
    newRoom->mutex = mutexInit;

    return newRoom;
}

//This function is used to add rooms to a houseType object   
//It also mallocs space for the newNode
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

//This function adds a room to another rooms adjacentRooms list and vice versa 
void connectRooms(RoomType *r1, RoomType *r2){
    addRoom(&r1->adjacentRooms, r2);
    addRoom(&r2->adjacentRooms, r1);
}

void freeRoomList(RoomListType *list){
    RoomNodeType *currNode;
    RoomNodeType *nextNode;
    
    currNode = list->head;

    while (currNode != NULL) 
    {
        nextNode = currNode->next;
        free(currNode);
        currNode = nextNode;
    }
}

//This function frees the data in RoomListType object 
void freeRoomData(RoomListType *list){
    RoomNodeType *currNode;
    RoomNodeType *nextNode;
    
    currNode = list->head;

    while (currNode != NULL) 
    {
        nextNode = currNode->next;
        free(currNode->data);
        currNode = nextNode;
    }
}

//This functions calculates the size of a RoomListType and returns it 
int roomListSize(RoomListType *list){
    int size = 0;
    RoomNodeType *currNode;

    currNode = list->head;

    while (currNode != NULL){
        currNode = currNode->next;
        size++;
    }
    return size;
}