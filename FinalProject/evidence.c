#include "defs.h"

//This function initailises a list of evidence which includes a head, tail and a mutex which will be used several times in the program
void initEvidenceList(EvidenceListType *list){
    list->head = NULL;
    list->tail = NULL;

    sem_t mutexInit;
    
    if (sem_init(&mutexInit, 0, 1) < 0) {
        printf("semaphore initialization error\n");
    }
    
    list->mutex = mutexInit;
}

//This function is used by the ghost to add evidence to the evidencelist of the room it is currently in 
//It also malloc space for the newNode
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
    newNode->data = evidence;
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

//This function calculates the size of an evidencelist and returns that value 
int evidenceListSize(EvidenceListType *list){
    int size = 0;
    EvidenceNodeType *currNode;

    currNode = list->head;

    while (currNode != NULL){
        currNode = currNode->next;
        size++;
    }
    return size;
}

//This function checks to see if the shared evidencelist contains 3 unique kinds of evidence
//If it does it returns C_TRUE if not it returns C_FALSE
int checkUniques(EvidenceListType *list){
    EvidenceNodeType *currNode;
    EvidenceType secondOne = EV_UNKNOWN;
    currNode = list->head;
    int index = 0;
    int index2 = 0;

    while (currNode != NULL){
        if (index > 0 && index2 == 0){
            if (list->head->data != currNode->data){
                secondOne = currNode->data;
                index2++;
            }
        }
        if (index2 > 0){
            if (list->head->data != currNode->data && secondOne != currNode->data){
                return C_TRUE;
            }
        }
        currNode = currNode->next;
        index++;
    }

    return C_FALSE;
}

//This fucntion prints all the unique evidence values in an evidencelist 
void printUniques(EvidenceListType *list){
    EvidenceNodeType *currNode;
    EvidenceType secondOne = EV_UNKNOWN;
    currNode = list->head;
    int index = 0;
    int index2 = 0;
    char first[MAX_STR];
    evidenceToString(currNode->data, first);
    printf("%s\n", first);

    while (currNode != NULL){
        
        if (index > 0 && index2 == 0){
            if (list->head->data != currNode->data){
                secondOne = currNode->data;
                char second[MAX_STR];
                evidenceToString(currNode->data, second);
                printf("%s\n", second);
                index2++;
            }
        }
        if (index2 > 0){
            if (list->head->data != currNode->data && secondOne != currNode->data){
                char third[MAX_STR];
                evidenceToString(currNode->data, third);
                printf("%s\n", third);
                break;
            }
        }
        currNode = currNode->next;
        index++;
    }

}

//This function is used by hunters to add evidence to the shared evidencelist when the successfully collect evidence
void addEvidenceFind(HunterType *h, EvidenceType evidence){
    EvidenceNodeType *currNode;
    EvidenceNodeType *prevNode;
    EvidenceNodeType *newNode;

    currNode = h->evidence->head;
    prevNode = NULL;

    while (currNode != NULL){
        prevNode = currNode;
        currNode = currNode->next;
    }

    newNode = malloc(sizeof(RoomNodeType));
    newNode->data = evidence;
    newNode->next = NULL;

    if (prevNode == NULL){
        h->evidence->head = newNode;
    }else{
        prevNode->next = newNode;
        h->evidence->tail = newNode;
    }

    newNode->next = currNode;
    
}

//This funtion compares two peices of evidence and returns C_TRUE if they match and C_FALSE if they don't 
int checkEvidenceMatch(HunterType *h, RoomType *r){
    EvidenceNodeType *currNode;

    currNode = r->evidence.head;

    while (currNode != NULL){
        if(currNode->data == h->device){
            return C_TRUE;
        }
        currNode = currNode->next;
    }

    return C_FALSE;
    
}

//This function frees the evidence data of an evidencelist 
void freeEvidenceData(EvidenceListType *list){
    EvidenceNodeType *currNode;
    EvidenceNodeType *nextNode;
    
    currNode = list->head;

    while (currNode != NULL) 
    {
        nextNode = currNode->next;
        free(&currNode->data);
        currNode = nextNode;
    }
}  

//This fucntion frees the Nodes of an evidencelist
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