#include "defs.h"

/*
  Function: initRoom(RoomType** room, char* initName)
  Purpose:  initialize a room with a name and empty lists
      in/out room: pointer to the room to be initialized
      in initName: name of the room
        
  return:   void, no return
*/
void initRoom(RoomType** room, char *initName){
    *room = malloc(sizeof(RoomType));
    strcpy((*room)->name, initName);
    (*room)->ghost = NULL;
    
    RoomsListType connectedRooms;
    initRoomsList(&connectedRooms);
    (*room)->connectedRooms = connectedRooms;

    EvidenceListType evidenceInRoom;
    initEvidenceList(&evidenceInRoom);
    (*room)->evidenceInRoom = evidenceInRoom;

    hunterArrayType huntersInRoom;
    initHunterArray(&huntersInRoom);
    (*room)->huntersInRoom = huntersInRoom;
    sem_init(&((*room)->blocked), 0, 1);
}

/*
  Function: createRoom(char* name)
  Purpose:  create and return a new room
      in name:   name of the room
        
  return:   pointer to newly created room
*/
RoomType* createRoom(char* name){
    RoomType* tempRoom;
    initRoom(&tempRoom, name);
    return tempRoom;
}

/*
  Function: connectRooms(RoomType* room1, RoomType* room2)
  Purpose:  given two rooms, connect them to each other by adding them to each other's connectedRooms list
      in/out room1: pointer to the first room to be connected
      in/out room2: pointer to the second room to be connected
        
  return:   void, no return
*/
void connectRooms(RoomType* room1, RoomType* room2){
    addRoomToList(&(room1->connectedRooms), room2);
    addRoomToList(&(room2->connectedRooms), room1);
}

/*
  Function: addRoomToList(RoomsListType* roomsList, RoomType* room)
  Purpose:  adds passed room to the end of the passed roomsList
      in/out roomsList: pointer to the list of rooms
      in/out room: pointer to the room to be added
        
  return:   void, no return
*/
void addRoomToList(RoomsListType* roomsList, RoomType* room){
    RoomsNodeType* newRoom = malloc(sizeof(RoomsNodeType));
    newRoom->room = room;
    newRoom->next = NULL;
    if(roomsList->head == NULL){
        roomsList->head = newRoom;
        roomsList->tail = newRoom;
    }else{
        roomsList->tail->next = newRoom;
        roomsList->tail = newRoom;
    }
}

/*
  Function: initRoomsList(RoomsListType* roomsList)
  Purpose:  initializes the passed roomsList with default values
      in/out roomsList: pointer to the list of rooms to be initialized
        
  return:   void, no return
*/
void initRoomsList(RoomsListType* roomsList){
    roomsList->head = NULL;
    roomsList->tail = NULL;
}

/*
  Function: chooseRoom(RoomsListType* rooms)
  Purpose:  randomly chooses a room from the passed roomsList
      in rooms: pointer to the list of rooms to choose from
        
  return:   pointer to the chosen room
*/
RoomType* chooseRoom(RoomsListType* rooms){
    RoomType* chosenRoom = NULL;
    RoomsNodeType* currentRoomNode = rooms->head;
    while(chosenRoom == NULL){
        if(strcmp(currentRoomNode->room->name, "Van")==0){
            currentRoomNode = currentRoomNode->next;
            continue;
        }
        int rand = randInt(0, 4);
        if(rand == 2){
            chosenRoom = currentRoomNode->room;
            break;
        }
        currentRoomNode = currentRoomNode->next;
        if(currentRoomNode == NULL){
            currentRoomNode = rooms->head;
        }
    }
    return chosenRoom;
}