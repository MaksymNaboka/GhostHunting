#include "defs.h"

/*
  Function: foundGhost(hunterArrayType* hunters)
  Purpose:  checks if any of the hunters in the passed array have exited due to finding evidence(confirms that the ghost was found)
      in hunters: pointer to the array of hunters
        
  return:   C_TRUE if at least one hunter has found enough evidence, C_FALSE otherwise
*/
int foundGhost(hunterArrayType* hunters){
    for(int i=0; i<hunters->size; i++){
        if(hunters->hunters[i]->exitReason==HUNTER_EVIDENCE){
            return C_TRUE;
        }
    }
    return C_FALSE;
}

/*
  Function: calculateResult(HouseType* house)
  Purpose:  summarizes the results of the hunt, prints all necessary information to the screen
      in house: pointer to the house where the hunt took place
        
  return:   void, no return
*/
void calculateResult(HouseType* house){
    printf("%s","*-------------------------* \n");
    printf("%s","Results: \n");
    if(foundGhost(&(house->hunters))==C_TRUE){
        printf("%s","The ghost was found, hunters won! \n");
        GhostClass resultClass;
        resultClass = determineGhost(&(house->sharedEvidenceList));
        char* resultGhost = malloc(sizeof(char)*MAX_STR);
        ghostToString(resultClass, resultGhost);
        printf("%s%s \n","The ghost was a ", resultGhost);
        free(resultGhost);
    }
    else{
        printf("%s","The ghost was not found and won! \n");
    }
    printf("%s","Hunters' reasons to leave: \n");
    printReasonToLeave(&(house->hunters));

    printf("%s","Collected evidence: \n");
    printEvidenceList(&(house->sharedEvidenceList));
}

/*
  Function: printReasonToLeave(hunterArrayType* hunters)
  Purpose:  prints the reason why each hunter left the hunt
      in hunters: pointer to the array of hunters
        
  return:   void, no return
*/
void printReasonToLeave(hunterArrayType* hunters){
    for(int i=0; i<hunters->size; i++){
        int reason = hunters->hunters[i]->exitReason;
        if(reason==HUNTER_BORED){
            printf("%s%s%s \n","Hunter ",hunters->hunters[i]->name," left because they were bored.");
        }else if(reason==HUNTER_FEAR){
            printf("%s%s%s \n","Hunter ",hunters->hunters[i]->name," left because they were afraid.");
        }else{
            printf("%s%s%s \n","Hunter ",hunters->hunters[i]->name," left because they collected enough evidence.");
        }
    }
}





/*
    Returns a pseudo randomly generated number, in the range min to (max - 1), inclusively
        in:   lower end of the range of the generated number
        in:   upper end of the range of the generated number
    return:   randomly generated integer in the range [0, max-1) 
*/
int randInt(int min, int max)
{
    return (int) randFloat(min, max);
}

/*
    Returns a pseudo randomly generated floating point number.
    A few tricks to make this thread safe, just to reduce any chance of issues using random
        in:   lower end of the range of the generated number
        in:   upper end of the range of the generated number
    return:   randomly generated floating point number in the range [min, max)
*/
float randFloat(float min, float max) {
    static __thread unsigned int seed = 0;
    if (seed == 0) {
        seed = (unsigned int)time(NULL) ^ (unsigned int)pthread_self();
    }

    float random = ((float) rand_r(&seed)) / (float) RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}

/*
    Returns the string representation of the given enum EvidenceType.
        in: type - the enum EvidenceType to convert
        out: str - the string representation of the given enum EvidenceType, minimum 16 characters
*/
void evidenceToString(enum EvidenceType type, char* str) {
    switch (type) {
        case EMF:
            strcpy(str, "EMF");
            break;
        case TEMPERATURE:
            strcpy(str, "TEMPERATURE");
            break;
        case FINGERPRINTS:
            strcpy(str, "FINGERPRINTS");
            break;
        case SOUND:
            strcpy(str, "SOUND");
            break;
        default:
            strcpy(str, "UNKNOWN");
            break;
    }
}

/* 
    Returns the string representation of the given enum GhostClass.
        in: ghost - the enum GhostClass to convert
        out: buffer - the string representation of the given enum GhostClass, minimum 16 characters
*/
void ghostToString(enum GhostClass ghost, char* buffer) {
    switch(ghost) {
        case BANSHEE:
            strcpy(buffer, "Banshee");
            break;
        case BULLIES:
            strcpy(buffer, "Bullies");
            break;
        case PHANTOM:
            strcpy(buffer, "Phantom");
            break;
        case POLTERGEIST:
            strcpy(buffer, "Poltergeist");
            break;
        default:
            strcpy(buffer, "Unknown");
            break;
        
    }
}
