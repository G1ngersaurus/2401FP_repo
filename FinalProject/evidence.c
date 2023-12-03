#include "defs.h"

void initEvidenceList(EvidenceListType *list){
    list->head = NULL;
    list->tail = NULL;

    sem_t mutexInit;
    
    if (sem_init(&mutexInit, 0, 1) < 0) {
        printf("semaphore initialization error\n");
    }
    list->mutex = mutexInit;
}

void addEvidenceLeave(RoomType *room, EvidenceType evidence){
    EvidenceNodeType *currNode;
    EvidenceNodeType *prevNode;
    EvidenceNodeType *newNode;

    currNode = room->evidence.head;
    prevNode = NULL;

    while (currNode != NULL){
        prevNode = currNode;
        currNode = currNode->next;
    }

    newNode = malloc(sizeof(EvidenceNodeType));
    newNode->data = &evidence;
    newNode->next = NULL;

    if (prevNode == NULL){
        room->evidence.head = newNode;
    }else{
        prevNode->next = newNode;
        room->evidence.tail = newNode;
    }

    newNode->next = currNode;
    l_ghostEvidence(evidence, room->name);
}

int evidenceListSize(EvidenceListType *list){
    int size = 0;
    EvidenceNodeType *currNode;
    //EvidenceNodeType *prevNode;

    currNode = list->head;
    //prevNode = NULL;

    while (currNode != NULL){
        //prevNode = currNode;
        currNode = currNode->next;
        size++;
    }
    return size;
}

void addEvidenceFind(HunterType *h, EvidenceType evidence){
    EvidenceNodeType *currNode;
    EvidenceNodeType *prevNode;
    EvidenceNodeType *newNode;

    currNode = h->currRoom->evidence.head;
    prevNode = NULL;
    int size = evidenceListSize(&h->currRoom->evidence);
    int index = 0;
    int uniqueCount = 0;

    while (currNode != NULL && index < size){
        prevNode = currNode;
        currNode = currNode->next;
        if (!checkEvidenceMatch(h, h->currRoom)){
            uniqueCount++;
        }
        index++;
    }
    if (uniqueCount == size){
        newNode = malloc(sizeof(EvidenceNodeType));
        newNode->data = &evidence;
        newNode->next = NULL;

        if (prevNode == NULL){
            h->evidence->head = newNode;
        }else{
            prevNode->next = newNode;
            h->evidence->tail = newNode;
        } 

        newNode->next = currNode;
        evidence = *currNode->data;
    }
    
}

int checkEvidenceMatch(HunterType *h, RoomType *r){
    EvidenceNodeType *currNode;
    //EvidenceNodeType *prevNode;

    currNode = r->evidence.head;
    //prevNode = NULL;

    while (currNode != NULL){
        //prevNode = currNode;
        if(currNode->data == h->device){
            return C_TRUE;
        }
        currNode = currNode->next;
    }

    return C_FALSE;
    
}
void freeEvidenceData(EvidenceListType *list){
    EvidenceNodeType *currNode;
    EvidenceNodeType *nextNode;
    
    currNode = list->head;

    while (currNode != NULL) 
    {
        nextNode = currNode->next;
        free(currNode->data);
        currNode = nextNode;
    }
}  
void freeEvidenceList(EvidenceListType *list){
    EvidenceNodeType *currNode;
    EvidenceNodeType *nextNode;
    
    currNode = list->head;

    while (currNode != NULL) 
    {
        nextNode = currNode->next;
        free(currNode);
        currNode = nextNode;
    }
}