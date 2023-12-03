#include "defs.h"

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

void cleanupHouse(HouseType *house){
    RoomNodeType *currNode;
    RoomNodeType *nextNode;
    
    currNode = house->rooms.head;

    while (currNode != NULL){
        nextNode = currNode->next;
        freeRoomList(&currNode->data->adjacentRooms);
        freeHunterList(&currNode->data->hunters);
        currNode = nextNode;
    }
    freeRoomData(&house->rooms);
    freeRoomList(&house->rooms);
    freeEvidenceData(&house->evidence);
    freeEvidenceList(&house->evidence);
    freeHunterData(&house->hunters);
    freeHunterList(&house->hunters);
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