THE PROGRAM WILL MOST LIKELY WORK ONLY ONLINUX OS BECAUSE IT RELIES ON SPECIFIC SOFTWARE AND ARCHITECTURE VERSIONS

## Program's Purpose

This program allows for the playing of a multithreaded simulation of 4 hunters trying to find a ghost in a house. The hunters can each only collect one type of evidence, and the ghost can leave up to 3 different types of evidence as hints. The game ends when all the hunters leave the house, and the results of the game are printed to the screen. The ghost wins if it it not identified, otherwise, it loses.

## List of Files

- main.c: Contains the main() function, which takes in input for hunters' names and creates/joins the threads for each hunter and the ghost

- ghost.c: Contains the initGhost(), ghostMain(), leaveEvidence(), ghostMove(), and determineGhost() functions, with helper functions to choose what evidence to leave and where to spawn the ghost

- hunter.c: Contains the initHunter(), hunterMain(), hunterReview(), hunterCollect(), hunterMove(), removeHunterFromRoom(), addHunterToRoom(), and initHunterArray() functions

- room.c: Contains the initRoom(), createRoom(), initRoomsList(), addRoomToList(), connectRoom() and chooseRoom() functions

- house.c: Contains the initHouse() and populateRooms() functions

- evidence.c: Contains the initEvidenceList(), addEvidenceToList(), removeEvidenceFromList(), and printEvidenceList() functions, along with a helper function to determine if a piece of evidence is already in the list

- cleanup.c: Contains all functions for deallocation of dynamically allocated memory, including the cleanUpAll(), cleanUpRoomsList(), cleanUpRoom(), cleanUpEvidenceList(), cleanUpHunters(), and cleanUpHunter() functions

- utils.c: Contains all utility/other helper functions, including given functions and calculateResult(), printReasonToLeave(), and foundGhost() functions

- logger.c: Unmodified logger file from skeleton code to work with partial automation of grading

- defs.h: Header file, containing forward declarations, type/structure definitions, and constant definitions

- Makefile: Makefile containing instructions for compilation and linking of the source files

- README.md: This file, containing description of the program

## Compiling and Executing

1. Open a terminal and navigate to folder containing source code, making sure all files listed above are in folder
2. Using Makefile: type "make", which will compile and link all necessary files into executable called "project"
3. Use "valgrind --leak-check=full ./project" in the folder with executable file to run program and check for memory leaks - Note: Current make does not include "-fsanitize=thread" in order to make code work with valgrind
4. Use "make clean" afterwards to get rid of object and executable files if desired

## Using the Program

1. Follow instructions given on display, and please enter names that do not have spaces in between them. The simulation will then run with the names given.
