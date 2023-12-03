#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

//Makefile line example
////gcc -Wall -std=c99 -o t05 t05.c t05util.c -g

#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
#define NUM_HUNTERS     4
#define FEAR_MAX        10
#define LOGGING         C_TRUE

typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;

typedef struct House HouseType;
typedef struct Room RoomType;
typedef struct Ghost GhostType;
typedef struct Hunter HunterType;
typedef struct RoomNode RoomNodeType;
typedef struct HunterNode HunterNodeType;
typedef struct EvidenceNode EvidenceNodeType;

typedef struct RoomList RoomListType;
typedef struct EvidenceList EvidenceListType;
typedef struct HunterList HunterListType;

struct RoomList {
  RoomNodeType *head;
  RoomNodeType *tail;
};

struct EvidenceList {
  EvidenceNodeType *head;
  EvidenceNodeType *tail;
  sem_t mutex;
};

struct HunterList {
  HunterNodeType *head;
  HunterNodeType *tail;
};

struct RoomNode {
  RoomType* data;
  RoomNodeType* next;
};

struct HunterNode {
  HunterType* data;
  struct HunterNode* next;
};

struct EvidenceNode {
  EvidenceType* data;
  struct EvidenceNode* next;
};

struct House {
    RoomListType rooms;
    EvidenceListType evidence;
    HunterListType hunters;
};

struct Room {
    char name[MAX_STR];
    RoomListType adjacentRooms;
    EvidenceListType evidence;
    HunterListType hunters;
    GhostType *ghost;
    sem_t mutex;
    // SORTING BY ADDRESS JUST MEANS DOING IF/ELSE TO DETERMINE ORDER IN WHICH YOU WAIT AND POST
};

struct Hunter {
    char name[MAX_STR];
    RoomType *currRoom;
    enum EvidenceType *device;
    EvidenceListType *evidence;
    int fear;
    int boredom;
};




enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };


struct Ghost {
    enum GhostClass type;
    RoomType *currRoom;
    int boredom;
};

// Object initialization functions
void initHouse(HouseType *h);
RoomType* createRoom(char *name);
void initHunter(char *name, HunterType **h, enum EvidenceType equipment, RoomType *r, EvidenceListType *sharedList);
void initGhost(GhostType **g);
void populateRooms(HouseType *h);

// List initialization functions
void initRoomList(RoomListType *list);
void initEvidenceList(EvidenceListType *list);
void initHunterList(HunterListType *list);

// Helper Utilies
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter

// List operations
void addRoom(RoomListType *l, RoomType *r);
void connectRooms(RoomType *r1, RoomType *r2);
void addHunter(HunterListType *l, HunterType *h);
void addEvidenceLeave(RoomType *r, EvidenceType e);
void addEvidenceFind(HunterType *h, EvidenceType e);
void removeHunter(HunterListType *l, HunterType *h);
void freeHunterList(HunterListType *l);
void freeEvidenceList(EvidenceListType *l);
void freeRoomList(RoomListType *l);
void freeHunterData(HunterListType *l);
void freeEvidenceData(EvidenceListType *l);
void freeRoomData(RoomListType *l);
void cleanupHouse(HouseType *h);

// Behaviour functions
void* ghostBehaviour(void* arg);
void* hunterBehaviour(void* arg);
void leaveEvidence(GhostType *g);
int checkEvidenceMatch(HunterType *h, RoomType *r);
//int reviewEvidence(HunterType *h);

// Helper functions
void pickStartingRoom(RoomListType* l, GhostType *g);
EvidenceType pickEvidenceToLeave(GhostType *g);
void moveGhost(GhostType *g);
int roomListSize(RoomListType *r);
int evidenceListSize(EvidenceListType *l);
void moveHunter(HunterType *h);
void printResults(HouseType *h, GhostType *g);
enum GhostClass pickRandType();

// Logging Utilities
void l_hunterInit(char* name, enum EvidenceType equipment);
void l_hunterMove(char* name, char* room);
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
void l_hunterExit(char* name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char* room);
void l_ghostMove(char* room);
void l_ghostEvidence(enum EvidenceType evidence, char* room);
void l_ghostExit(enum LoggerDetails reason);
