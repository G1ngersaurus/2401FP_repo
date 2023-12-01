#include "defs.h"

void initGhost(GhostClass class, GhostType **ghost){
    GhostType *newGhost = malloc(sizeof(GhostType));
    newGhost->type = &class;
    newGhost->boredom = 0;

    *ghost = newGhost;
}

void pickStartingRoom(RoomListType *list, GhostType *g){

    int room = randInt(1, 12);

    RoomNodeType *currNode;
    //RoomNodeType *prevNode;

    int i = 0;

    currNode = list->head;
    //prevNode = NULL;

    //Might be i <= room
    while (currNode != NULL && i < room){
        //prevNode = currNode;
        currNode = currNode->next;
        i++;
    }
    currNode->data->ghost = g;
    g->currRoom = currNode->data;
}

EvidenceType pickEvidenceToLeave(GhostType *ghost){
    
    int evidenceleave = randInt(1, 3);
    switch (*ghost->type) {
        
        case POLTERGEIST:
            switch(evidenceleave){
                case 1:
                    return EMF;
                case 2:
                    return TEMPERATURE;
                case 3:
                    return FINGERPRINTS;
            } 
        
        case BANSHEE:
            switch(evidenceleave){
                case 1:
                    return EMF;
                case 2:
                    return TEMPERATURE;
                case 3:
                    return SOUND;
            }
        
        case BULLIES:
                switch(evidenceleave){
                case 1:
                    return EMF;
                case 2:
                    return FINGERPRINTS;
                case 3:
                    return SOUND;
            }
        
        case PHANTOM:
            switch(evidenceleave){
                case 1:
                    return TEMPERATURE;
                case 2:
                    return FINGERPRINTS;
                case 3:
                    return SOUND;
            }
    }
    return GH_UNKNOWN;
}

void leaveEvidence(GhostType *ghost){
    EvidenceType toLeave = pickEvidenceToLeave(ghost);
    addEvidenceLeave(ghost->currRoom, toLeave);
}

void moveGhost(GhostType *ghost){
    int options = roomListSize(ghost->currRoom->adjacentRooms);
    int choice = randInt(1, options);

    int index = 0;
    RoomNodeType *currNode;
    //RoomNodeType *prevNode;

    currNode = ghost->currRoom->adjacentRooms->head;
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
        }

        if (ghost->currRoom->hunters->head != NULL) {
            ghost->boredom = 0;
            int action = randInt(1, 2);
            switch(action){
                case 1:

                case 2:
                    leaveEvidence(ghost);
            }   
        }
        else {
            ghost->boredom++;
            int action = randInt(1, 3);
            switch(action){
                case 1:

                case 2:
                    leaveEvidence(ghost);
                case 3:
                    moveGhost(ghost);
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