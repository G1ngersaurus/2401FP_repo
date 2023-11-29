#include "defs.h"

void initGhost(GhostClass class, GhostType **ghost){
    GhostType *newGhost = malloc(sizeof(GhostType));
    newGhost->type = class;
    newGhost->boredom = 0;

    *ghost = newGhost;
}

void pickStartingRoom(RoomListType *list, GhostType *g){

    int room = randInt(1, 12);

    RoomNodeType *currNode;
    RoomNodeType *prevNode;

    int i = 0;

    currNode = list->head;
    prevNode = NULL;

    //Might be i <= room
    while (currNode != NULL && i < room){
        prevNode = currNode;
        currNode = currNode->next;
        i++;
    }
    currNode->data->ghost = g;
    g->currRoom = currNode->data;
}

//EMF, TEMPERATURE, FINGERPRINTS, SOUND
// PO = 0, 1, 2
// BA = 0, 1, 3
// BU = 0, 2, 3
// PH = 1, 2, 3 

EvidenceType pickEvidenceToLeave(GhostType *ghost){
    int evidenceleave = randInt(1, 3);
    switch (ghost->type) {
        
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
}

void leaveEvidence(GhostType *ghost){
    EvidenceType toLeave = pickEvidenceToLeave(ghost);
    addEvidenceLeave(ghost->currRoom, toLeave);
}

void moveGhost(GhostType *ghost){
    int options = listSize(ghost->currRoom->adjacentRooms);
    int choice = randInt(1, options);

    int index = 0;
    RoomNodeType *currNode;
    RoomNodeType *prevNode;

    currNode = ghost->currRoom->adjacentRooms->head;
    prevNode = NULL;

    while (currNode != NULL && index != choice){
        prevNode = currNode;
        currNode = currNode->next;
        index++;
    }
    ghost->currRoom->ghost = NULL;
    currNode->data->ghost = ghost;
}

// STILL SOME BEHAVIOUR TO MAKE, MOSTLY DONE
void* ghostBehaviour(void* arg){
    GhostType* ghost = (GhostType*) arg;
    while (1){
        usleep(GHOST_WAIT);
        if (ghost->currRoom->hunters->head != NULL) {
            ghost->boredom = 0;
            int action = randInt(1, 2);
            switch(action){
                case 1:
                    //Ghost does nothing
                case 2:
                    leaveEvidence(ghost);
            }   
        }
        else {
            ghost->boredom++;
            int action = randInt(1, 3);
            switch(action){
                case 1:
                    //Ghost does nothing
                case 2:
                    leaveEvidence(ghost);
                case 3:
                    moveGhost(ghost);
            }
        }
        if (ghost->boredom >= BOREDOM_MAX){
            return NULL;
        }
    }
    //leave thread
}   