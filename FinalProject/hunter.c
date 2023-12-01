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
    newHunter->device = &equipment;
    newHunter->evidence = list;

    *hunter = newHunter;
    l_hunterInit(name, equipment);
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

void removeHunter(HunterListType *list, HunterType *hunter){
    HunterNodeType *currNode;
    HunterNodeType *prevNode;

    currNode = list->head;
    prevNode = NULL;

    while (currNode != NULL) {
        if (strcmp(currNode->data->name, hunter->name) == 0)
        break;

        prevNode = currNode;
        currNode = currNode->next;
    }

    if (prevNode == NULL){
        list->head = currNode->next;
    } else {
        prevNode->next = currNode->next;        
    }

}

void moveHunter(HunterType *hunter){
    int options = roomListSize(hunter->currRoom->adjacentRooms);
    int choice = randInt(1, options);

    int index = 0;
    RoomNodeType *currNode;
    RoomNodeType *prevNode;

    currNode = hunter->currRoom->adjacentRooms->head;
    prevNode = NULL;

    while (currNode != NULL && index != choice){
        prevNode = currNode;
        currNode = currNode->next;
        index++;
    }
    hunter->currRoom->ghost = NULL;
    addHunter(prevNode->data->hunters, hunter);
    l_hunterMove(hunter->name, prevNode->data->name);
}

void freeHunterList(HunterListType *list){
    HunterNodeType *currNode;
    HunterNodeType *nextNode;
    
    currNode = list->head;

    while (currNode != NULL) 
    {
        nextNode = currNode->next;
        free(currNode);
        currNode = nextNode;
    }
}

void freeHunterData(HunterListType *list){
    HunterNodeType *currNode;
    HunterNodeType *nextNode;
    
    currNode = list->head;

    while (currNode != NULL) 
    {
        nextNode = currNode->next;
        free(currNode->data);
        currNode = nextNode;
    }
}

void* hunterBehaviour(void* arg){
    HunterType* hunter = (HunterType*) arg;
    int check = C_FALSE;
    while (1) {
        usleep(HUNTER_WAIT);
        if (sem_wait(&hunter->currRoom->mutex) < 0){
            printf("Error on semaphore wait\n");
            return NULL;
        }
        if (hunter->currRoom->ghost != NULL){
            hunter->boredom = 0;
            hunter->fear++;
        }
        else {
            hunter->boredom++;
        }
        if (sem_post(&hunter->currRoom->mutex) < 0){
            printf("Error on semaphore post\n");
            return NULL;
        }
        if (hunter->boredom >= BOREDOM_MAX){
            l_hunterExit(hunter->name, LOG_BORED);
            return NULL;
        }
        if (hunter->fear >= FEAR_MAX){
            l_hunterExit(hunter->name, LOG_FEAR);
            return NULL;
        }
        if (sem_wait(&hunter->currRoom->mutex) < 0){
            printf("Error on semaphore wait\n");
            return NULL;
        }
        if (sem_wait(&hunter->evidence->mutex) < 0){
            printf("Error on semaphore wait\n");
            return NULL;
        }
        int action = randInt(1, 3);
        switch(action){
            case 1:
                moveHunter(hunter);
            case 2:
                if(checkEvidenceMatch(hunter, hunter->currRoom)){
                    EvidenceType ev;
                    addEvidenceFind(hunter, ev);
                    l_hunterCollect(hunter->name, ev, hunter->currRoom->name);
                }
            case 3:
                // Review evidence
                if (evidenceListSize(hunter->evidence) == 3){
                    check = C_TRUE;
                    l_hunterReview(hunter->name, LOG_SUFFICIENT);
                }
                else {
                    l_hunterReview(hunter->name, LOG_INSUFFICIENT);
                }

        }
        if (sem_post(&hunter->currRoom->mutex) < 0){
            printf("Error on semaphore post\n");
            return NULL;
        }
        if (sem_post(&hunter->evidence->mutex) < 0){
            printf("Error on semaphore post\n");
            return NULL;
        }
        if (check == C_TRUE){
            l_hunterExit(hunter->name, LOG_EVIDENCE);
            return NULL;
        }
        
    }
}