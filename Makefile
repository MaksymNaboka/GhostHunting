#######################################################################################
#
#
#
#
#######################################################################################



OBJ = main.o utils.o hunter.o ghost.o house.o logger.o room.o evidence.o cleanup.o
allOBJDependencies = main utils ghost hunter house logger room evidence cleanup

all: $(allOBJDependencies) 
	gcc -g -pthread -o project -Wall -Wextra $(OBJ)
main: main.c defs.h
	gcc -c main.c
utils: utils.c defs.h
	gcc -c utils.c
hunter: hunter.c defs.h
	gcc -c hunter.c
ghost: ghost.c defs.h
	gcc -c ghost.c
house: house.c defs.h
	gcc -c house.c
logger: logger.c defs.h
	gcc -c logger.c
room: room.c defs.h
	gcc -c room.c
evidence: evidence.c defs.h
	gcc -c evidence.c
cleanup: cleanup.c defs.h
	gcc -c cleanup.c

clean:
	rm -f $(OBJ) project