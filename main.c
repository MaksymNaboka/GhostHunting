#include "defs.h"

int main()
{
    pthread_t ghostThread, hunterThread1, hunterThread2, hunterThread3, hunterThread4;
    // Initialize the random number generator
    srand(time(NULL));

    HouseType house;
    initHouse(&house);
    populateRooms(&house);

    //create the ghost
    GhostType* ghost;
    initGhost(&ghost, &house.rooms);

    char name[MAX_STR]; 
    for(int i=0; i<NUM_HUNTERS; i++){
        printf("Enter hunter %d name: ", i+1);
        scanf("%s", name);
        initHunter(&(house.hunters.hunters[i]), name, i, &house);
    }

    pthread_create(&ghostThread, NULL, ghostMain, ghost);
    pthread_create(&hunterThread1, NULL, hunterMain, house.hunters.hunters[0]);
    pthread_create(&hunterThread2, NULL, hunterMain, house.hunters.hunters[1]);
    pthread_create(&hunterThread3, NULL, hunterMain, house.hunters.hunters[2]);
    pthread_create(&hunterThread4, NULL, hunterMain, house.hunters.hunters[3]);

    pthread_join(ghostThread, NULL);
    pthread_join(hunterThread1, NULL);
    pthread_join(hunterThread2, NULL);
    pthread_join(hunterThread3, NULL);
    pthread_join(hunterThread4, NULL);

    //print results
    calculateResult(&house);

    cleanUpAll(&house, ghost);

    return 0;
}