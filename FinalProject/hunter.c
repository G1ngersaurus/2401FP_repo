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

