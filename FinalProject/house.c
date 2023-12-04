#include "defs.h"

//This function initailises the three lists RoomListType, EvidenceListType and HunterListType all found in a HouseType object 
void initHouse(HouseType *h){
    
    
    RoomListType rList;
    initRoomList(&rList); 
    h->rooms = rList;

    EvidenceListType eList;
    initEvidenceList(&eList);
    h->evidence = eList;

    HunterListType hList;
    initHunterList(&hList);
    h->hunters = hList;
    
}

//This function frees all the allocated memory in the program using a series of free functions that can be found in the program
void cleanupHouse(HouseType *house){
    RoomNodeType *currNode;
    RoomNodeType *nextNode;
    
    currNode = house->rooms.head;

    while (currNode != NULL){
        nextNode = currNode->next;
        freeRoomList(&currNode->data->adjacentRooms);
        freeHunterList(&currNode->data->hunters);
        freeEvidenceList(&currNode->data->evidence);
        currNode = nextNode;
    }
    freeRoomData(&house->rooms);
    freeRoomList(&house->rooms);
    freeEvidenceList(&house->evidence);
    freeHunterList(&house->hunters);
}

//This function prints the final results of the program inluding: 
//Who won, what kind of ghost it is, which hunters ran away in fear or got bored and the evidence that was successfully collected 
void printResults(HouseType *house, GhostType *ghost){
    HunterNodeType *currNode;
    HunterNodeType *nextNode;
    
    currNode = house->hunters.head;
    int count = 0;
    printf("=================================================\n");
    while (currNode != NULL) 
    {
        nextNode = currNode->next;
        if (currNode->data->boredom >= BOREDOM_MAX){
            printf("Hunter %s left because there was nothing to do\n", currNode->data->name);
            count++;
        }
        if (currNode->data->fear >= FEAR_MAX){
            count++;
            printf("Hunter %s ran away in fear\n", currNode->data->name);
        }
        currNode = nextNode;
    }
    printf("=================================================\n");
    if (count == 4){
        printf("The ghost won\n");
    }
    else{
        printf("The hunters won\n");
    }
    printf("=================================================\n");
    char type[MAX_STR];
    ghostToString(ghost->type, type);
    printf("Ghost was a type %s\n", type);
    printf("=================================================\n");
    printf("The hunters found the following evidence:\n");
    printUniques(&house->evidence);
    printf("=================================================\n");
    if (count != 4){
        printf("They determined it was a %s\n", type);
    }
    else{
        printf("They did not determine the type\n");
    }
    

}

/*
    Dynamically allocates several rooms and populates the provided house.
    Note: You may modify this as long as room names and connections are maintained.
        out: house - the house to populate with rooms. Assumes house has been initialized.
*/
void populateRooms(HouseType* house) {
    // First, create each room

    // createRoom assumes that we dynamically allocate a room, initializes the values, and returns a RoomType*
    // create functions are pretty typical, but it means errors are harder to return aside from NULL
    RoomType* van                = createRoom("Van");
    RoomType* hallway            = createRoom("Hallway");
    RoomType* master_bedroom     = createRoom("Master Bedroom");
    RoomType* boys_bedroom       = createRoom("Boy's Bedroom");
    RoomType* bathroom           = createRoom("Bathroom");
    RoomType* basement           = createRoom("Basement");
    RoomType* basement_hallway   = createRoom("Basement Hallway");
    RoomType* right_storage_room = createRoom("Right Storage Room");
    RoomType* left_storage_room  = createRoom("Left Storage Room");
    RoomType* kitchen            = createRoom("Kitchen");
    RoomType* living_room        = createRoom("Living Room");
    RoomType* garage             = createRoom("Garage");
    RoomType* utility_room       = createRoom("Utility Room");

    // This adds each room to each other's room lists
    // All rooms are two-way connections
    connectRooms(van, hallway);
    //Hallway adds van / van adds hallway
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);

    // Add each room to the house's room list
    addRoom(&house->rooms, van);
    addRoom(&house->rooms, hallway);
    addRoom(&house->rooms, master_bedroom);
    addRoom(&house->rooms, boys_bedroom);
    addRoom(&house->rooms, bathroom);
    addRoom(&house->rooms, basement);
    addRoom(&house->rooms, basement_hallway);
    addRoom(&house->rooms, right_storage_room);
    addRoom(&house->rooms, left_storage_room);
    addRoom(&house->rooms, kitchen);
    addRoom(&house->rooms, living_room);
    addRoom(&house->rooms, garage);
    addRoom(&house->rooms, utility_room);
}