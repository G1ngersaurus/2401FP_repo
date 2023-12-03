#include "defs.h"

int main()
{
    GhostType* g;
    HouseType house;
    initHouse(&house);
    populateRooms(&house);

    initGhost(&g);
    pickStartingRoom(&house.rooms, g);
    l_ghostInit((g->type), g->currRoom->name);

    HunterType* h1;
    HunterType* h2;
    HunterType* h3;
    HunterType* h4;
    
    pthread_t ghostThread, hT1, hT2, hT3, hT4;

    char h1Name[MAX_STR];
    char h2Name[MAX_STR];
    char h3Name[MAX_STR];
    char h4Name[MAX_STR];

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

    addHunter(&house.rooms.head->data->hunters, h1);
    addHunter(&house.rooms.head->data->hunters, h2);
    addHunter(&house.rooms.head->data->hunters, h3);
    addHunter(&house.rooms.head->data->hunters, h4);

    // Initialize the random number generator
    srand(time(NULL));

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

    printf("Got to cleanup\n");
    cleanupHouse(&house);
    free(g);
    return 0;
}

