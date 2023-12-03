#include "defs.h"

void initGhost(GhostType **ghost){
    GhostType *newGhost = malloc(sizeof(GhostType));
    int choice = pickRandType();
    newGhost->type = choice;
    newGhost->boredom = 0;
    *ghost = newGhost;
    //printf("%d\n", *ghost->type);
}

enum GhostClass pickRandType(){
    int type = randInt(0, 4);
    switch(type){
        case 0:
            return POLTERGEIST;
            break;
        case 1:
            return BANSHEE;
            break;
        case 2:
            return BULLIES;
            break;
        case 3:
            return PHANTOM;
            break;
    }
}

void pickStartingRoom(RoomListType *list, GhostType *g){

    int room = randInt(1, 12);

    RoomNodeType *currNode;
    RoomNodeType *prevNode;
    RoomNodeType *nextNode;

    int i = 0;

    currNode = list->head;
    prevNode = NULL;
    nextNode = NULL;
    //printf("List size: %d\n", roomListSize(list));
    printf("Before loop\n");

    //while (currNode != NULL){
    //    prevNode = currNode;
    //    currNode = currNode->next;
    //}

    //Might be i <= room
    while (currNode != NULL && i < room){
        printf("%d\n", i);
        printf("%d\n", room);
        printf("Before currNode operation\n");
        nextNode = currNode->next;
        prevNode = currNode;
        printf("After currNode operation\n");
        currNode = nextNode;
        
        i++;
    }
    printf("After loop\n");
    prevNode->data->ghost = g;
    g->currRoom = prevNode->data;
    printf("After loop 2\n");
}

EvidenceType pickEvidenceToLeave(GhostType *ghost){
    
    int evidenceleave = randInt(1, 3);
    switch ((ghost->type)) {
        
        case POLTERGEIST:
            switch(evidenceleave){
                case 1:
                    return EMF;
                    break;
                case 2:
                    return TEMPERATURE;
                    break;
                case 3:
                    return FINGERPRINTS;
                    break;
            } 
            break;
        case BANSHEE:
            switch(evidenceleave){
                case 1:
                    return EMF;
                    break;
                case 2:
                    return TEMPERATURE;
                    break;
                case 3:
                    return SOUND;
                    break;
            }
            break;
        case BULLIES:
                switch(evidenceleave){
                case 1:
                    return EMF;
                    break;
                case 2:
                    return FINGERPRINTS;
                    break;
                case 3:
                    return SOUND;
                    break;
            }
            break;
        case PHANTOM:
            switch(evidenceleave){
                case 1:
                    return TEMPERATURE;
                    break;
                case 2:
                    return FINGERPRINTS;
                    break;
                case 3:
                    return SOUND;
                    break;
            }
            break;
        default:
            return EV_UNKNOWN;
            break;
    }
    return EV_UNKNOWN;
}

void leaveEvidence(GhostType *ghost){
    EvidenceType toLeave = pickEvidenceToLeave(ghost);
    addEvidenceLeave(ghost->currRoom, toLeave);
}

void moveGhost(GhostType *ghost){
    int options = roomListSize(&ghost->currRoom->adjacentRooms);
    int choice = randInt(1, options + 1);

    int index = 0;
    RoomNodeType *currNode;
    //RoomNodeType *prevNode;

    currNode = ghost->currRoom->adjacentRooms.head;
    //prevNode = NULL;

    while (currNode != NULL && index != choice){
        //prevNode = currNode;
        currNode = currNode->next;
        index++;
    }
    ghost->currRoom->ghost = NULL;
    ghost->currRoom = currNode->data;
    currNode->data->ghost = ghost;
    l_ghostMove(currNode->data->name);
}

void* ghostBehaviour(void* arg){
    GhostType* ghost = (GhostType*) arg;
    while (1){
        usleep(GHOST_WAIT);
        if (sem_wait(&ghost->currRoom->mutex) < 0){
            printf("Error on semaphore wait\n");
            return NULL;
            break;
        }

        if (ghost->currRoom->hunters.head != NULL) {
            ghost->boredom = 0;
            int action = randInt(1, 3);
            switch(action){
                case 1:
                    continue;
                case 2:
                    leaveEvidence(ghost);
                    continue;
            }   
        }
        else {
            ghost->boredom++;
            int action = randInt(1, 4);
            switch(action){
                case 1:
                    continue;
                case 2:
                    leaveEvidence(ghost);
                    continue;
                case 3:
                    moveGhost(ghost);
                    continue;
            }
        }
        if (sem_post(&ghost->currRoom->mutex) < 0){
            printf("Error on semaphore post\n");
            return NULL;
        }

        if (ghost->boredom >= BOREDOM_MAX){
            l_ghostExit(LOG_BORED);
            return NULL;
        }
    }

}   