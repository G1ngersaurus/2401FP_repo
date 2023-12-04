#include "defs.h"

//This function initailises a list of hunters which includes a head and a tail
void initHunterList(HunterListType *list){
    list->head = NULL;
    list->tail = NULL;
}

//This function initailises all the values found in a HunterType object 
void initHunter(char *name, HunterType **hunter, enum EvidenceType equipment, RoomType *room, EvidenceListType *list){
    HunterType *newHunter = malloc(sizeof(HunterType));
    strcpy(newHunter->name, name);
    newHunter->boredom = 0;
    newHunter->fear = 0;
    newHunter->currRoom = room;
    newHunter->device = equipment;
    newHunter->evidence = list;

    *hunter = newHunter;
    l_hunterInit(name, equipment);
}

//This function is used by a hunter to add evidence to the shared evidencelist  
//It also mallocs space for the newNode
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

//This function removes a specified hunter from a list of hunters 
void removeHunter(HunterListType *list, HunterType *hunter){
    HunterNodeType *currNode;
    HunterNodeType *prevNode;

    currNode = list->head;
    prevNode = NULL;

    while (currNode != NULL) {
        if (strcmp(currNode->data->name, hunter->name) == 0){
            break;
        }
        
        prevNode = currNode;
        currNode = currNode->next;
    }
    if (prevNode == NULL){
        list->head = currNode->next;
    } else {
        prevNode->next = currNode->next;      
    }

    free(currNode);

}

//This function picks the room that a hunter will move to using the adjacentRooms list 
RoomType* pickRoomToMove(HunterType *hunter){
    int options = roomListSize(&hunter->currRoom->adjacentRooms);
    int choice = randInt(1, options + 1);

    int index = 0;
    RoomNodeType *currNode;
    RoomNodeType *prevNode;

    currNode = hunter->currRoom->adjacentRooms.head;
    prevNode = NULL;

    while (currNode != NULL && index != choice){
        prevNode = currNode;
        currNode = currNode->next;
        index++;
    }

    return prevNode->data;

}

//This function moves a hunter from one to room to another 
void moveHunter(HunterType *hunter, RoomType* room){
    removeHunter(&hunter->currRoom->hunters, hunter);
    addHunter(&room->hunters, hunter);
    hunter->currRoom = room;
    l_hunterMove(hunter->name, room->name);
}

//This function frees the allocted memory from a hunterlist 
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

//This function frees the data from a hunterlist 
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

//This function dictates what action a hunter will take
//If their boredom reaches BOREDOM_MAX of if their fear reaches FEAR_MAX they will be removed from currRoom list and activate pthread_exit(NULL);
//The other actions are a 1/3 to move rooms, a 1/3 to try to collect eivdence from the room their in 
//and a 1/3 chance to review evidence and see if they have enough to figure out what kind of ghost is in the house 
//Also if a hunter is in the same room as a ghost their fear increases and their boredom is set to 0
//If their not in the same room as the ghost boredom is increased by 1 
void* hunterBehaviour(void* arg){
    HunterType* hunter = ((HunterType*) arg);
    while (1){
        usleep(HUNTER_WAIT);
        
        if (hunter->boredom >= BOREDOM_MAX){
            l_hunterExit(hunter->name, LOG_BORED);
            removeHunter(&hunter->currRoom->hunters, hunter);
            pthread_exit(NULL);
        }

        if (hunter->fear >= FEAR_MAX){
            l_hunterExit(hunter->name, LOG_FEAR);
            removeHunter(&hunter->currRoom->hunters, hunter);
            pthread_exit(NULL);
        }

        if (sem_wait(&hunter->currRoom->mutex) < 0){
            return NULL;
        }

        if (hunter->currRoom->ghost != NULL){
            hunter->fear++;
            hunter->boredom = 0;
        } 
        else {
            hunter->boredom++;
        }

        int action = randInt(1, 4);
        switch(action){
            case 1:
                if (sem_wait(&hunter->currRoom->evidence.mutex) < 0){
                    return NULL;
                }
                if (sem_wait(&hunter->evidence->mutex) < 0){
                    return NULL;
                }
                
                if (checkEvidenceMatch(hunter, hunter->currRoom)){
                    EvidenceType ev = hunter->device;
                    addEvidenceFind(hunter, ev);
                    l_hunterCollect(hunter->name, ev, hunter->currRoom->name);
                }
                if (sem_post(&hunter->evidence->mutex) < 0){
                    return NULL;
                }
                if (sem_post(&hunter->currRoom->evidence.mutex) < 0){
                    return NULL;
                }
                if (sem_post(&hunter->currRoom->mutex) < 0){
                    return NULL;
                }
                continue;
            case 2:
                RoomType* room = pickRoomToMove(hunter);
                if (sem_wait(&room->mutex) < 0){
                    return NULL;
                }
                RoomType* original = hunter->currRoom;
                moveHunter(hunter, room);
                if (sem_post(&room->mutex) < 0){
                    return NULL;
                }
                if (sem_post(&original->mutex) < 0){
                    return NULL;
                }
                continue;
            case 3:
                if (sem_wait(&hunter->evidence->mutex) < 0){
                    return NULL;
                }
                if (checkUniques(hunter->evidence) == C_TRUE){
                    l_hunterReview(hunter->name, LOG_SUFFICIENT);
                    if (sem_post(&hunter->evidence->mutex) < 0){
                        return NULL;
                    }
                    if (sem_post(&hunter->currRoom->mutex) < 0){
                        return NULL;
                    }
                    l_hunterExit(hunter->name, LOG_EVIDENCE);
                    removeHunter(&hunter->currRoom->hunters, hunter);
                    pthread_exit(NULL);
                }
                else {
                    l_hunterReview(hunter->name, LOG_INSUFFICIENT);
                }
                if (sem_post(&hunter->evidence->mutex) < 0){
                    return NULL;
                }
                if (sem_post(&hunter->currRoom->mutex) < 0){
                    return NULL;
                }
                continue;

        }
    }
}