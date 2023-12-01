#include "defs.h"

int main()
{
    // Declare 4 hunters and a ghost
    HunterType* h1;
    HunterType* h2;
    HunterType* h3;
    HunterType* h4;
    GhostType* g;

    pthread_t ghostThread, hT1, hT2, hT3, hT4;

    char h1Name[MAX_STR];
    char h2Name[MAX_STR];
    char h3Name[MAX_STR];
    char h4Name[MAX_STR];

    printf("Enter name of hunter 1: ");
    scanf("%s", h1Name);
    while (getchar() != '\n');
    printf("Enter name of hunter 2: ");
    scanf("%s", h2Name);
    while (getchar() != '\n');
    printf("Enter name of hunter 3: ");
    scanf("%s", h3Name);
    while (getchar() != '\n');
    printf("Enter name of hunter 4: ");
    scanf("%s", h4Name);
    while (getchar() != '\n');

    // Initialize the random number generator
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    HouseType house;
    initHouse(&house);
    populateRooms(&house);
    
    initHunter(h1Name, &h1, EMF, &house.rooms->head->data, house.evidence);
    initHunter(h2Name, &h2, TEMPERATURE, &house.rooms->head->data, house.evidence);
    initHunter(h3Name, &h3, FINGERPRINTS, &house.rooms->head->data, house.evidence);
    initHunter(h4Name, &h4, SOUND, &house.rooms->head->data, house.evidence);

    addHunter(house.rooms->head->data->hunters, &h1);
    addHunter(house.rooms->head->data->hunters, &h2);
    addHunter(house.rooms->head->data->hunters, &h3);
    addHunter(house.rooms->head->data->hunters, &h4);

    initGhost(randInt(0, 3), &g);
    pickStartingRoom(house.rooms, &g);
    l_ghostInit(g->type, g->currRoom);

    pthread_create(&ghostThread, NULL, ghostBehaviour, &g);
    pthread_create(&hT1, NULL, hunterBehaviour, &h1);
    pthread_create(&hT2, NULL, hunterBehaviour, &h2);
    pthread_create(&hT3, NULL, hunterBehaviour, &h3);
    pthread_create(&hT4, NULL, hunterBehaviour, &h4);

    pthread_join(ghostThread, NULL);
    pthread_join(hT1, NULL);
    pthread_join(hT2, NULL);
    pthread_join(hT3, NULL);
    pthread_join(hT4, NULL);


    cleanupHouse(&house);
    return 0;
}

