#include "defs.h"


/*
  Function: isInEvidenceList(EvidenceListType* evList, EvidenceType evidence)
  Purpose:  checks if the given evidence is in the passed list
      in evList: pointer to the list of evidence
      in evidence: evidence to be checked
        
  return:   C_TRUE if the evidence is in the list, C_FALSE otherwise
*/
int isInEvidenceList(EvidenceListType* evList, EvidenceType evidence){
    sem_wait(&(evList->blocked));
    EvidenceNodeType* currentEvidence = evList->head;
    while(currentEvidence!=NULL){
        if(currentEvidence->evidence == evidence){
            sem_post(&(evList->blocked));
            return C_TRUE;
        }
        currentEvidence = currentEvidence->next;
    }
    sem_post(&(evList->blocked));
    return C_FALSE;
}

/*
  Function: initEvidenceList(EvidenceListType* evidenceList)
  Purpose:  initializes the passed evidenceList with default values
      out roomsList: pointer to the list of evidence to be initialized
        
  return:   void, no return
*/
void initEvidenceList(EvidenceListType* evidenceList){
    evidenceList->head = NULL;
    evidenceList->tail = NULL;
    sem_init(&(evidenceList->blocked), 0, 1);
}

/*
  Function: addEvidenceToList(EvidenceListType* evList, EvidenceType type)
  Purpose:  adds the passed evidence to the end of the passed evidenceList
        in/out evList: pointer to the list of evidence to be added to
        in type: evidence to be added
        
  return:   void, no return
*/
void addEvidenceToList(EvidenceListType* evList, EvidenceType type){
    EvidenceNodeType* newEvidence = malloc(sizeof(EvidenceNodeType));
    newEvidence->previous = evList->tail;
    newEvidence->evidence = type;
    newEvidence->next = NULL;
    if(evList->head == NULL){
        evList->head = newEvidence;
        evList->tail = newEvidence;
    }else{
        evList->tail->next = newEvidence;
        evList->tail = newEvidence;
    }
}

/*
  Function: removeEvidenceFromList(EvidenceListType* evList, EvidenceNodeType** evNode)
  Purpose:  removes the passed evidence from the passed evidenceList
        in/out evList: pointer to the list of evidence to be removed from
        in/out evNode: pointer to the evidence to be removed
        
  return:   void, no return
*/
void removeEvidenceFromList(EvidenceListType* evList, EvidenceNodeType** evNode){
    if(evList->head == evList->tail){
        evList->head = NULL;
        evList->tail = NULL;
        free(*evNode);
        return;
    }
    if(evList->head == *evNode){
        evList->head = (*evNode)->next;
    }
    if(evList->tail == *evNode){
        evList->tail = (*evNode)->previous;
    }
    if((*evNode)->next == NULL){
        (*evNode)->previous->next = NULL;
    }else if((*evNode)->previous == NULL){
        (*evNode)->next->previous = NULL;
    }else{
        (*evNode)->previous->next = (*evNode)->next;
        (*evNode)->next->previous = (*evNode)->previous;
    }
    free(*evNode);
}

/*
  Function: printEvidenceList(EvidenceListType* evidenceList)
  Purpose:  prints elements of the passed evidenceList
      in evidenceList: pointer to the list of evidence to be printed
        
  return:   void, no return
*/
void printEvidenceList(EvidenceListType* evidenceList){
    EvidenceNodeType* currentEvidence = evidenceList->head;
    char* evidenceString = malloc(sizeof(char)*MAX_STR);
    while(currentEvidence!=NULL){
        evidenceToString(currentEvidence->evidence, evidenceString);
        printf("%s%s\n","*", evidenceString);
        currentEvidence = currentEvidence->next;
    }
    free(evidenceString);
}