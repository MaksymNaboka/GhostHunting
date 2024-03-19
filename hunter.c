#include "defs.h"



/*
  Function: hunterMain(void* arg)
  Purpose:  serves as main function for a hunter thread, choosing actions for hunter to do until it exits
      in arg: will be converted to hunterType* and used never used in calculations
        
  return:   void*, no return
*/
void* hunterMain(void* arg){
    HunterType* hunter = (HunterType*) arg;
    while(1){
        usleep(HUNTER_WAIT);
        if(hunter->boredom>BOREDOM_MAX){
            hunter->isHunting = C_FALSE;
            removeHunterFromRoom(hunter, hunter->currentRoom);
            l_hunterExit(hunter->name, LOG_BORED);
            hunter->exitReason = HUNTER_BORED;
            pthread_exit(0);
            // return;
        }
        if(hunter->fear>FEAR_MAX){
            hunter->isHunting = C_FALSE;
            removeHunterFromRoom(hunter, hunter->currentRoom);
            l_hunterExit(hunter->name, LOG_FEAR);
            hunter->exitReason = HUNTER_FEAR;
            pthread_exit(0);
            // return;
        }
        int action = randInt(0, 3);
        switch(action){
            case HUNTER_MOVE:
                hunterMove(hunter);
                break;
            case HUNTER_COLLECT:
                hunterCollect(hunter);
                break;
            case HUNTER_REVIEW:
                if(hunterReview(hunter)==C_TRUE){
                    hunter->isHunting = C_FALSE;
                    removeHunterFromRoom(hunter, hunter->currentRoom);
                    l_hunterExit(hunter->name, LOG_EVIDENCE);
                    hunter->exitReason = HUNTER_EVIDENCE;
                    pthread_exit(0);
                    // return;
                }
                break;
        }
        if(hunter->currentRoom->ghost == NULL){
            hunter->boredom++;
        }else{
            hunter->boredom = 0;
            hunter->fear++;
        }
    }
}

/*
  Function: hunterReview(HunterType* hunter)
  Purpose:  checks if the hunters have enough evidence to exit
      in hunter: the hunter to check it's list
        
  return:   C_TRUE if enough evidence, C_FALSE if not
*/
int hunterReview(HunterType* hunter){
    sem_wait(&(hunter->sharedEvidenceList->blocked));
    if(hunter->sharedEvidenceList->head != NULL){
        int counter = 0;
        EvidenceNodeType* currentEvidence = hunter->sharedEvidenceList->head;
        while(currentEvidence!=NULL){
            counter++;
            currentEvidence = currentEvidence->next;
        }
        if(counter==3){
            l_hunterReview(hunter->name, LOG_SUFFICIENT);
            sem_post(&(hunter->sharedEvidenceList->blocked));
            return C_TRUE;
        }
    }
    l_hunterReview(hunter->name, LOG_INSUFFICIENT);
    sem_post(&(hunter->sharedEvidenceList->blocked));
    return C_FALSE;
}

/*
  Function: hunterCollect(HunterType* hunter)
  Purpose:  collects evidence from the current room if the hunter has proper device
      in hunter: the hunter trying to collect evidence
        
  return:   void, no return
*/
void hunterCollect(HunterType* hunter){
    //block from race conditions
    sem_wait(&(hunter->currentRoom->evidenceInRoom.blocked));
    if(hunter->currentRoom->evidenceInRoom.head != NULL){
        EvidenceNodeType* currentEvidence = hunter->currentRoom->evidenceInRoom.head;
        while(currentEvidence!=NULL){
            EvidenceNodeType* tempNext = currentEvidence->next;
            if(currentEvidence->evidence == hunter->canCollect){
                //check if is already in evidence list
                if(isInEvidenceList(hunter->sharedEvidenceList, currentEvidence->evidence)==C_FALSE){
                    addEvidenceToList(hunter->sharedEvidenceList, currentEvidence->evidence);
                    l_hunterCollect(hunter->name, currentEvidence->evidence, hunter->currentRoom->name);
                }
                removeEvidenceFromList(&(hunter->currentRoom->evidenceInRoom), &(currentEvidence));
            }
            currentEvidence = tempNext;
        }
    }
    sem_post(&(hunter->currentRoom->evidenceInRoom.blocked));
}

/*
  Function: hunterMove(HunterType* hunter)
  Purpose:  moves the hunter to a random room connected to the current room
      in hunter: the hunter to move
        
  return:   void, no return
*/
void hunterMove(HunterType* hunter){
    RoomType* newRoom = chooseRoom(&(hunter->currentRoom->connectedRooms));
    int first = C_FALSE;
    if(&(hunter->currentRoom->blocked)<(&(newRoom->blocked))){
        first = C_TRUE;
        sem_wait(&(hunter->currentRoom->blocked));
        sem_wait(&(newRoom->blocked));
    }else{
        sem_wait(&(newRoom->blocked));
        sem_wait(&(hunter->currentRoom->blocked));
    }
    RoomType* prevRoom = hunter->currentRoom;
    removeHunterFromRoom(hunter, hunter->currentRoom);
    addHunterToRoom(hunter, newRoom);
    l_hunterMove(hunter->name, hunter->currentRoom->name);
    
    if(first == C_FALSE){
        sem_post(&(prevRoom->blocked));
        sem_post(&(newRoom->blocked));
    }else{
        sem_post(&(newRoom->blocked));
        sem_post(&(prevRoom->blocked));
    }
}

/*
  Function: removeHunterFromRoom(HunterType* hunter, RoomType* room)
    Purpose:  removes given hunter from a given room
        in hunter: the hunter to remove
        in/out room: the room to remove the hunter from
    
    return:   void, no return
*/
void removeHunterFromRoom(HunterType* hunter, RoomType* room){
    for(int i=0; i<room->huntersInRoom.size; i++){
        if(strcmp(room->huntersInRoom.hunters[i]->name, hunter->name)==0){
            for(int j=i; j<room->huntersInRoom.size-1; j++){
                room->huntersInRoom.hunters[j] = room->huntersInRoom.hunters[j+1];
            }
            hunter->currentRoom = NULL;
        }
    }
    room->huntersInRoom.size--;
}

/*
  Function: addHunterToRoom(HunterType* hunter, RoomType* room)
    Purpose:  adds given hunter to a given room
        in hunter: the hunter to add
        in/out room: the room to add the hunter to
    
    return:   void, no return
*/
void addHunterToRoom(HunterType* hunter, RoomType* room){
    // sem_wait(&(room->blocked));
    room->huntersInRoom.hunters[room->huntersInRoom.size] = hunter;
    hunter->currentRoom = room;
    room->huntersInRoom.size++;
    // sem_post(&(room->blocked));
}

/*
  Function: initHunter(HunterType** hunter, char* name, int Device, HouseType* house)
    Purpose:  initializes a hunter and all of its fields
        in/out hunter: pointer to the hunter to initialize
        in name: name of the hunter
        in Device: device the hunter has
        in/out house: the main house structure

    return:   void, no return
*/
void initHunter(HunterType** hunter, char* name, int Device, HouseType* house) {
    *hunter = malloc(sizeof(HunterType));
    strcpy((*hunter)->name, name);
    (*hunter)->currentRoom = house->rooms.head->room;
    addHunterToRoom(*hunter, house->rooms.head->room);
    (*hunter)->canCollect = Device;
    (*hunter)->sharedEvidenceList = &house->sharedEvidenceList;
    (*hunter)->fear = 0;
    (*hunter)->boredom = 0;
    (*hunter)->exitReason = 0;
    (*hunter)->isHunting = C_TRUE;
    house->hunters.size++;
    l_hunterInit((*hunter)->name, (*hunter)->canCollect);
}

/*
  Function: initHunterArray(hunterArrayType *arr)
    Purpose:  initializes a hunter array and all of its fields
        in/out arr: pointer to the hunter array to initialize

    return:   void, no return
*/
void initHunterArray(hunterArrayType *arr){
    arr->size=0;
}