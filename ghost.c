#include "defs.h"

/*
  Function: ghostMain(void* arg)
  Purpose:  serves as main function for a ghost thread, choosing actions for ghost to do until it exits
      in arg: will be converted to ghostType* and used never used in calculations
       
  return:   void*, no return
*/
void* ghostMain(void* arg){
    GhostType* ghost = (GhostType*) arg;
    while(1){
        usleep(GHOST_WAIT);
        if(ghost->boredom>BOREDOM_MAX){
            ghost->currentRoom->ghost = NULL;
            // printf("exit");
            l_ghostExit(LOG_BORED);
            pthread_exit(0);
            // return;
        }
        
        int action = randInt(0, 3);
        switch (action){
            case GHOST_NOTHING:
                //NOTHING
                break;
            case GHOST_LEAVEEVIDENCE:
                leaveEvidence(ghost);
                break;
                
            case GHOST_MOVE:
                if(ghost->currentRoom->huntersInRoom.size != 0){
                    int ALTaction = randInt(0, 2);
                    switch(ALTaction){
                        case GHOST_NOTHING:
                            //NOTHING
                            break;
                        case GHOST_LEAVEEVIDENCE:
                            leaveEvidence(ghost);
                            break;
                    }
                }
                else
                {
                    ghostMove(ghost);
                }
                
                break;
        }
        if(ghost->currentRoom->huntersInRoom.size > 0){
            ghost->boredom=0;
        }else{
            ghost->boredom++;
        }
    }
}

/*
  Function: leaveEvidence(GhostType* ghost)
  Purpose:  leaves evidence in the current room
      in ghost: pointer to the ghost leaving evidence
       
  return:   void, no return
*/
void leaveEvidence(GhostType* ghost){
    int ghClass = ghost->Ghostclass;
    int evType = chooseEvidenceToLeave(ghClass);
    //block evidence list here
    sem_wait(&(ghost->currentRoom->evidenceInRoom.blocked));
    addEvidenceToList(&(ghost->currentRoom->evidenceInRoom), evType);
    l_ghostEvidence(evType, ghost->currentRoom->name);
    sem_post(&(ghost->currentRoom->evidenceInRoom.blocked));
    //unblock
}

/*
  Function: chooseEvidenceToLeave(int class)
  Purpose:  initializes the house and all necessary components
      in class: the class of the ghost to determine evidence type to leave
       
  return:   int (enum) of the evidence type to leave
*/
int chooseEvidenceToLeave(int class){
    int type;
    switch(class){
        case POLTERGEIST:
            switch(randInt(0,3)){
                case 0:
                    type = EMF;
                    break;
                case 1:
                    type = TEMPERATURE;
                    break;
                case 2:
                    type = FINGERPRINTS;
                    break;
            }
            break;

        case BANSHEE:
            switch(randInt(0,3)){
                case 0:
                    type = EMF;
                    break;
                case 1:
                    type = TEMPERATURE;
                    break;
                case 2:
                    type = SOUND;
                    break;
            }
            break;

        case BULLIES:
            switch(randInt(0,3)){
                case 0:
                    type = EMF;
                    break;
                case 1:
                    type = FINGERPRINTS;
                    break;
                case 2:
                    type = FINGERPRINTS;
                    break;
            }
            break;

        case PHANTOM:
            switch(randInt(0,3)){
                case 0:
                    type = TEMPERATURE;
                    break;
                case 1:
                    type = FINGERPRINTS;
                    break;
                case 2:
                    type = SOUND;
                    break;
            }
            break;
    }
    return type;
}

/*
  Function: ghostMove(GhostType* ghost)
  Purpose:  moves ghost to a random connected room
      in/out ghost: pointer to the ghost to move
       
  return:   void, no return
*/
void ghostMove(GhostType* ghost){
    //if semaphores blocked here, may be case that all connected rooms have hunters, ghost cannot move, and hunters cannot exit 
    RoomType* roomToMove = chooseRoom(&(ghost->currentRoom->connectedRooms));
    //block semaphores here
    int first = C_FALSE;
    if(&(ghost->currentRoom->blocked)<&(roomToMove->blocked)){
        first = C_TRUE;
        sem_wait(&(ghost->currentRoom->blocked));
        sem_wait(&(roomToMove->blocked));
    }else{
        sem_wait(&(roomToMove->blocked));
        sem_wait(&(ghost->currentRoom->blocked));
    }
    RoomType* prevRoom = ghost->currentRoom;
    ghost->currentRoom->ghost = NULL;
    ghost->currentRoom = roomToMove;
    ghost->currentRoom->ghost = ghost;
    l_ghostMove(ghost->currentRoom->name);

    if(first == C_FALSE){
        sem_post(&(ghost->currentRoom->blocked));
        sem_post(&(prevRoom->blocked));
    }else{
        sem_post(&(prevRoom->blocked));
        sem_post(&(ghost->currentRoom->blocked));
    }
}

/*
  Function: initGhost(GhostType** ghost, RoomsListType* rooms)
  Purpose:  initializes the ghost and all necessary components/fields
        in/out ghost: double pointer to the ghost to initialize
        in rooms: pointer to the rooms list to choose a starting room from
  return:   void, no return
*/
void initGhost(GhostType** ghost, RoomsListType* rooms){
    *ghost = malloc(sizeof(GhostType));
    (*ghost)->Ghostclass = randomGhost();
    (*ghost)->currentRoom = GhostStartRoom(rooms);

    (*ghost)->currentRoom->ghost = *ghost;
    
    (*ghost)->boredom = 0;
    l_ghostInit((*ghost)->Ghostclass, (*ghost)->currentRoom->name);
}

/* 
    Returns a random enum GhostClass.
*/
enum GhostClass randomGhost() {
    return (enum GhostClass) randInt(0, 4);
}

/*
  Function: GhostStartRoom(RoomsListType* rooms)
  Purpose:  choses a random room from the list for ghost to start in
      in rooms: pointer to the list of rooms in house
       
  return:   pointer to room that the ghost will start in
*/
RoomType* GhostStartRoom(RoomsListType* rooms){
    RoomType* startRoom = chooseRoom(rooms);
    return startRoom;
}

/*
  Function: determineGhost(EvidenceListType* evList)
  Purpose:  determines the ghost class based on what is in the evidence list
      in evList: pointer to the list of evidence 
       
  return:   int (enum) of the ghost class
*/
int determineGhost(EvidenceListType* evList){
    int EMFcheck = isInEvidenceList(evList, EMF);
    int FINGERPRINTScheck = isInEvidenceList(evList, FINGERPRINTS);
    int TEMPERATUREcheck = isInEvidenceList(evList, TEMPERATURE); 
    if(EMFcheck==C_TRUE){
        if(TEMPERATUREcheck==C_TRUE){
            if(FINGERPRINTScheck==C_TRUE){
                return POLTERGEIST;
            }
            else{
                return BANSHEE;
            }
        }
        else{
            return BULLIES;
        }
    }
    else{
        return PHANTOM;
    }
}