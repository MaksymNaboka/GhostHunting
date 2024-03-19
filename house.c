#include "defs.h"

/*
  Function: initHouse(HouseType* house)
  Purpose:  initializes the house and all necessary components
      in/out house: pointer to the house to initialize
       
  return:   void, no return
*/
void initHouse(HouseType* house) {
    hunterArrayType hunterArray;
    initHunterArray(&hunterArray);
    house->hunters = hunterArray;

    RoomsListType roomsList;
    initRoomsList(&roomsList);
    house->rooms = roomsList;

    EvidenceListType evidenceList;
    initEvidenceList(&evidenceList);
    house->sharedEvidenceList = evidenceList;
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
    struct Room* van                = createRoom("Van");
    struct Room* hallway            = createRoom("Hallway");
    struct Room* master_bedroom     = createRoom("Master Bedroom");
    struct Room* boys_bedroom       = createRoom("Boy's Bedroom");
    struct Room* bathroom           = createRoom("Bathroom");
    struct Room* basement           = createRoom("Basement");
    struct Room* basement_hallway   = createRoom("Basement Hallway");
    struct Room* right_storage_room = createRoom("Right Storage Room");
    struct Room* left_storage_room  = createRoom("Left Storage Room");
    struct Room* kitchen            = createRoom("Kitchen");
    struct Room* living_room        = createRoom("Living Room");
    struct Room* garage             = createRoom("Garage");
    struct Room* utility_room       = createRoom("Utility Room");

    // This adds each room to each other's room lists
    // All rooms are two-way connections
    connectRooms(van, hallway);
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
    addRoomToList(&house->rooms, van);
    addRoomToList(&house->rooms, hallway);
    addRoomToList(&house->rooms, master_bedroom);
    addRoomToList(&house->rooms, boys_bedroom);
    addRoomToList(&house->rooms, bathroom);
    addRoomToList(&house->rooms, basement);
    addRoomToList(&house->rooms, basement_hallway);
    addRoomToList(&house->rooms, right_storage_room);
    addRoomToList(&house->rooms, left_storage_room);
    addRoomToList(&house->rooms, kitchen);
    addRoomToList(&house->rooms, living_room);
    addRoomToList(&house->rooms, garage);
    addRoomToList(&house->rooms, utility_room);
}