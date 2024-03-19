#include "defs.h"

/*
  Function: cleanUpAll(HouseType* house, GhostType* ghost)
  Purpose:  deallocates up all memory allocated during program execution
        in house: house to be cleaned up
        in ghost: ghost to be cleaned up

  return:   void, no return
*/
void cleanUpAll(HouseType* house, GhostType* ghost){
    cleanUpRoomsList(&house->rooms, C_FALSE);
    cleanUpEvidenceList(&house->sharedEvidenceList);
    cleanUpHunters(&house->hunters);
    free(ghost);
}

/*
  Function: cleanUpRoomsList(RoomsListType* roomsList, int onlyNodes)
  Purpose:  deallocates up memory of the passed list of rooms
      in roomsList: list of rooms to be cleaned up
      in onlyNodes: if true, only the nodes of the list will be deallocated, otherwise, the room data too
       
  return:   void, no return
*/
void cleanUpRoomsList(RoomsListType* roomsList, int onlyNodes){
    RoomsNodeType* currentRoomNode = roomsList->head;
    while(currentRoomNode!=NULL){
        RoomsNodeType* temp = currentRoomNode;
        currentRoomNode = currentRoomNode->next;
        if(onlyNodes==C_FALSE){
            cleanUpRoom(temp->room);
        }
        free(temp);
    }
}

/*
  Function: cleanUpRoom(RoomType* room)
  Purpose:  deallocates memory for a given room
        in room: room to be cleaned up

  return:   void, no return
*/
void cleanUpRoom(RoomType* room){
    cleanUpEvidenceList(&(room->evidenceInRoom));
    cleanUpRoomsList(&(room->connectedRooms), C_TRUE);
    free(room);
}

/*
  Function: cleanUpEvidenceList(EvidenceListType* evidenceList)
  Purpose:  deallocates memory for nodes of given list of evidence
      in evidenceList: list of evidence to be cleaned up
       
  return:   void, no return
*/
void cleanUpEvidenceList(EvidenceListType* evidenceList){
    EvidenceNodeType* currentEvidenceNode = evidenceList->head;
    while(currentEvidenceNode!=NULL){
        EvidenceNodeType* temp = currentEvidenceNode;
        currentEvidenceNode = currentEvidenceNode->next;
        free(temp);
    }
}

/*
  Function: cleanUpHunters(hunterArrayType* hunters)
  Purpose:  deallocates memory for all hunters in the given array
      in hunters: array of hunters to be cleaned up
       
  return:   void, no return
*/
void cleanUpHunters(hunterArrayType* hunters){
    for(int i=0; i<hunters->size; i++){
        cleanUpHunter(hunters->hunters[i]);
    }
}

/*
  Function: cleanUpHunter(HunterType* hunter)
  Purpose:  deallocates memory for a given hunter
      in hunter: hunter to be cleaned up
       
  return:   void, no return
  Note: while this function is not used in the code a lot, it might be useful in the future updates of the program
*/
void cleanUpHunter(HunterType* hunter){
    free(hunter);
}
