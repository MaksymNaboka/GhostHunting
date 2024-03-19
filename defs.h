#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

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

#define HUNTER_FEAR     501
#define HUNTER_BORED    502
#define HUNTER_EVIDENCE 503

typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;
typedef struct House   HouseType;
typedef struct Room    RoomType;
typedef struct RoomsList RoomsListType;
typedef struct RoomsNode RoomsNodeType;
typedef struct EvidenceList EvidenceListType;
typedef struct EvidenceNode EvidenceNodeType;
typedef struct Hunter HunterType;
typedef struct Ghost GhostType;
typedef struct hunterArray hunterArrayType;

enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND};
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };
enum HunterAction { HUNTER_MOVE, HUNTER_COLLECT, HUNTER_REVIEW, HUNTER_EXIT};
enum GhostAction { GHOST_NOTHING, GHOST_LEAVEEVIDENCE, GHOST_MOVE, GHOST_EXIT};

struct Ghost{
    GhostClass Ghostclass;
    RoomType* currentRoom;
    int boredom;
};

struct EvidenceList{
    EvidenceNodeType* head;
    EvidenceNodeType* tail;
    sem_t blocked;
};

struct EvidenceNode{
    EvidenceNodeType* previous;
    EvidenceType evidence;
    EvidenceNodeType* next;
};

struct Hunter{
    RoomType* currentRoom;
    EvidenceType canCollect;
    char name[MAX_STR];
    EvidenceListType* sharedEvidenceList;
    int fear;
    int boredom;
    int isHunting;
    int exitReason;
};

struct RoomsList{
    RoomsNodeType* head;
    RoomsNodeType* tail;
};

struct RoomsNode{
    RoomType* room;
    RoomsNodeType* next;
};

struct hunterArray{
    HunterType* hunters[NUM_HUNTERS];
    int size;
};

struct Room{
    GhostType* ghost;
    char name[MAX_STR];
    RoomsListType connectedRooms;
    EvidenceListType evidenceInRoom;
    hunterArrayType huntersInRoom;
    sem_t blocked;
};

struct House {
  RoomsListType rooms;
  hunterArrayType hunters;
  EvidenceListType sharedEvidenceList;
};


// Helper Utilies
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter
void calculateResult(HouseType* house);
void printReasonToLeave(hunterArrayType* hunters);
int foundGhost(hunterArrayType* hunters);

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

// House
void initHouse(HouseType* house);
void populateRooms(HouseType* house);

//hunter
void initHunter(HunterType** hunter, char* name, int Device, HouseType* house);
void* hunterMain(void* arg);
int hunterReview(HunterType* hunter);
void hunterCollect(HunterType* hunter);
void hunterMove(HunterType* hunter);
void removeHunterFromRoom(HunterType* hunter, RoomType* room);
void addHunterToRoom(HunterType* hunter, RoomType* room);
void initHunterArray(hunterArrayType* hunterArray);

//ghost
void initGhost(GhostType** ghost, RoomsListType* rooms);
void* ghostMain(void* arg);
void leaveEvidence(GhostType* ghost);
int chooseEvidenceToLeave(int class);
void ghostMove(GhostType* ghost);
RoomType* GhostStartRoom(RoomsListType* rooms);
int determineGhost(EvidenceListType* evList);

//room
void initRoom(RoomType** room, char* initName);
RoomType* createRoom(char* initName);
void addRoomToList(RoomsListType* rooms, RoomType* room);
void connectRooms(RoomType* room1, RoomType* room2);
RoomType* chooseRoom(RoomsListType* rooms);
void initRoomsList(RoomsListType* roomsList);

//evidence
void initEvidenceList(EvidenceListType* evidenceList);
void addEvidenceToList(EvidenceListType* evList, EvidenceType type);
void removeEvidenceFromList(EvidenceListType* evList, EvidenceNodeType** evNode);
void printEvidenceList(EvidenceListType* evidenceList);
int isInEvidenceList(EvidenceListType* evList, EvidenceType evidence);

//cleanup
void cleanUpAll(HouseType* house, GhostType* ghost);
void cleanUpRoomsList(RoomsListType* roomsList, int onlyNodes);
void cleanUpRoom(RoomType* room);
void cleanUpEvidenceList(EvidenceListType* evidenceList);
void cleanUpHunters(hunterArrayType* hunters);
void cleanUpHunter(HunterType* hunter);