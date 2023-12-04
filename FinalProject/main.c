#include "defs.h"

int main()
{
    printf("Welcome to the Carleton Ghost Hunter Club!\n\n");
    //Initialize ghost and house
    GhostType* g;
    HouseType house;
    initHouse(&house);
    //Adds rooms to the house
    populateRooms(&house);
    initGhost(&g);
    //picks the ghosts starting room 
    pickStartingRoom(&house.rooms, g);
    l_ghostInit((g->type), g->currRoom->name);

    //declare the hunters 
    HunterType* h1;
    HunterType* h2;
    HunterType* h3;
    HunterType* h4;

    //declare char's that will be the hunters names 
    char h1Name[MAX_STR];
    char h2Name[MAX_STR];
    char h3Name[MAX_STR];
    char h4Name[MAX_STR];

    // Asking user for hunter names, initializing them
    printf("Enter name of hunter 1: ");
    scanf("%s", h1Name);
    while (getchar() != '\n');
    initHunter(h1Name, &h1, EMF, house.rooms.head->data, &house.evidence);
    printf("Enter name of hunter 2: ");
    scanf("%s", h2Name);
    while (getchar() != '\n');
    initHunter(h2Name, &h2, TEMPERATURE, house.rooms.head->data, &house.evidence);
    printf("Enter name of hunter 3: ");
    scanf("%s", h3Name);
    while (getchar() != '\n');
    initHunter(h3Name, &h3, FINGERPRINTS, house.rooms.head->data, &house.evidence);
    printf("Enter name of hunter 4: ");
    scanf("%s", h4Name);
    while (getchar() != '\n');
    initHunter(h4Name, &h4, SOUND, house.rooms.head->data, &house.evidence);

    //Adds the hunters to the house's HunterListType
    addHunter(&house.hunters, h1);
    addHunter(&house.hunters, h2);
    addHunter(&house.hunters, h3);
    addHunter(&house.hunters, h4);

    //Adds the hunters to the rooms's HunterListType
    addHunter(&house.rooms.head->data->hunters, h1);
    addHunter(&house.rooms.head->data->hunters, h2);
    addHunter(&house.rooms.head->data->hunters, h3);
    addHunter(&house.rooms.head->data->hunters, h4);

    // Initialize the random number generator
    srand(time(NULL));

    //declares the threads for the ghost and hunters 
    pthread_t ghostThread, hT1, hT2, hT3, hT4;

    //creates the threads for the ghost and hunters 
    pthread_create(&ghostThread, NULL, ghostBehaviour, g);
    pthread_create(&hT1, NULL, hunterBehaviour, h1);
    pthread_create(&hT2, NULL, hunterBehaviour, h2);
    pthread_create(&hT3, NULL, hunterBehaviour, h3);
    pthread_create(&hT4, NULL, hunterBehaviour, h4);

    //joins the threads for the ghost and hunters 
    pthread_join(ghostThread, NULL);
    pthread_join(hT1, NULL);
    pthread_join(hT2, NULL);
    pthread_join(hT3, NULL);
    pthread_join(hT4, NULL);

    //Prints the final results of the program 
    printResults(&house, g);

    //These functions free all the allocated memory in the program 
    cleanupHouse(&house);
    free(g);
    free(h1);
    free(h2);
    free(h3);
    free(h4);
    return 0;
}

