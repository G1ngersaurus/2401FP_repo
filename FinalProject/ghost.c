#include "defs.h"

//This function initailises the values of a ghostType
//This includes their type and their intial boredom (it also mallocs space for it)
void initGhost(GhostType **ghost){
    GhostType *newGhost = malloc(sizeof(GhostType));
    int choice = randomGhost();
    newGhost->type = choice;
    newGhost->boredom = 0;
    *ghost = newGhost;
}

//This function picks the ghosts starting room and places it inside of that room
void pickStartingRoom(RoomListType *list, GhostType *g){

    int room = randInt(2, 12);

    RoomNodeType *currNode;
    RoomNodeType *prevNode;
    RoomNodeType *nextNode;

    int i = 0;

    currNode = list->head;
    prevNode = NULL;
    nextNode = NULL;
    
    while (currNode != NULL && i < room){
        nextNode = currNode->next;
        prevNode = currNode;
        currNode = nextNode;
        
        i++;
    }
    prevNode->data->ghost = g;
    g->currRoom = prevNode->data;
}

//This function picks the kind of evidence that the ghost will be leaving based of the type of ghost 
EvidenceType pickEvidenceToLeave(GhostType *ghost){
    
    int evidenceleave = randInt(1, 4);
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

//This function allows the ghost to leave evidence in the rooms its in 
void leaveEvidence(GhostType *ghost){
    EvidenceType toLeave = pickEvidenceToLeave(ghost);
    addEvidenceLeave(ghost->currRoom, toLeave);
}

//This fucntion allows that ghost to switch rooms by checking the adjeacent rooms list 
RoomType* pickRoomGhost(GhostType *ghost){
    int options = roomListSize(&ghost->currRoom->adjacentRooms);
    int choice = randInt(1, options + 1);

    int index = 0;
    RoomNodeType *currNode;
    RoomNodeType *prevNode;

    currNode = ghost->currRoom->adjacentRooms.head;
    prevNode = NULL;

    while (currNode != NULL && index != choice){
        prevNode = currNode;
        currNode = currNode->next;
        index++;
    }

    return prevNode->data;
}

//This function moves the ghost room one room to another 
void moveGhost(GhostType *ghost, RoomType *room){
    
    ghost->currRoom->ghost = NULL;
    ghost->currRoom = NULL;
    ghost->currRoom = room;
    ghost->currRoom->ghost = ghost;

    l_ghostMove(room->name);
}
/*
    This function dictates the ghost actions based on several variables
    If the ghosts boredom has reached a value of BOREDOM_MAX the ghosts pthread_exit(NULL); is used 
    The ghost has a 1/3 chance to do nothing. a 1/3 chance to leave evidence in the rooms its in 
    and a 1/3 chance to move rooms only if the hunter is not in that room. 
    If that happens the ghost instead has a 1/2 chance of doing nothing and a 1/2 chance of leaving evidence
*/
void* ghostBehaviour(void* arg){
    GhostType* ghost = ((GhostType*) arg);

    while(1){
        usleep(GHOST_WAIT);

        if (ghost->boredom >= BOREDOM_MAX){
            l_ghostExit(LOG_BORED);
            pthread_exit(NULL);
        }
        
        if (sem_wait(&ghost->currRoom->mutex) < 0){
            return NULL;
        }

        if (ghost->currRoom->hunters.head != NULL){
            ghost->boredom = 0;

            int action = randInt(1, 3);
            switch(action){
                case 1:
                    if (sem_post(&ghost->currRoom->mutex) < 0){
                        return NULL;
                    }
                    continue;
                case 2:
                    if (sem_wait(&ghost->currRoom->evidence.mutex) < 0){
                        return NULL;
                    }
                    leaveEvidence(ghost);
                    if (sem_post(&ghost->currRoom->evidence.mutex) < 0){
                        return NULL;
                    }
                    if (sem_post(&ghost->currRoom->mutex) < 0){
                        return NULL;
                    }
                    continue;
            }
        }
        else{
            ghost->boredom++;
            int action = randInt(1, 4);
            switch(action){
                case 1:
                    if (sem_post(&ghost->currRoom->mutex) < 0){
                        return NULL;
                    }
                    if (ghost->boredom >= BOREDOM_MAX){
                        l_ghostExit(LOG_BORED);
                        pthread_exit(NULL);
        }
                    continue;
                case 2:
                    if (sem_wait(&ghost->currRoom->evidence.mutex) < 0){
                        return NULL;
                    }
                    leaveEvidence(ghost);
                    if (sem_post(&ghost->currRoom->evidence.mutex) < 0){
                        return NULL;
                    }
                    if (sem_post(&ghost->currRoom->mutex) < 0){
                        return NULL;
                    }
                    continue;
                case 3:
                    RoomType* room = pickRoomGhost(ghost);
                    if (sem_wait(&room->mutex) < 0){
                        return NULL;
                    }
                    RoomType* original = ghost->currRoom;
                    moveGhost(ghost, room);
                    if (sem_post(&room->mutex) < 0){
                        return NULL;
                    }
                    if (sem_post(&original->mutex) < 0){
                        return NULL;
                    }
                    continue;
            }
        }
    }
}