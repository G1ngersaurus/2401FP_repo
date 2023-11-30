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

void moveHunter(HunterType *hunter){
    int options = listSize(hunter->currRoom->adjacentRooms);
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
    addHunter(hunter->currRoom->hunters, hunter);
}

void* hunterBehaviour(void* arg){
    HunterType* hunter = (HunterType*) arg;
    while (1) {
        usleep(HUNTER_WAIT);
        if (hunter->currRoom->ghost != NULL){
            hunter->boredom = 0;
            hunter->fear++;
        }
        else {
            hunter->boredom++;
        }

        if (hunter->boredom >= BOREDOM_MAX || hunter->fear >= FEAR_MAX){
            return NULL;
        }

        int action = randInt(1, 3);
        switch(action){
            case 1:
                moveHunter(hunter);
            case 2:
                if(checkEvidenceMatch(hunter, hunter->currRoom)){
                    addEvidenceFind(hunter);
                }
            case 3:
                // Review evidence
                if (evidenceListSize(hunter->evidence) == 3){
                    return NULL;
                }

        }
        
    }
}