Ivan Klemen 101275035, Logan Calvinho 101266052

Program purpose:
    ~ This program simulates a series of actions from four hunters and the ghost they are hunting inside of house.
      The actions the hunters have include: collecting evidence from rooms they are in, moving from room to room
      and exiting the house due to boredom, fear or successfully finding enough evidecne to identify the ghost.
      The actions the ghosts have include: leaving evidence in rooms they are in, moving from room to room
      and exiting the house due to boredom. (They also raise the fear of hunters when they are in the same room as them)
      Once this simulation is finished the program prints out the results in an organised format 

Files included:
    ~ defs.h
        ~ Header file containing all pre-definitions
    ~ evidence.c
        ~ A C file that contains all the functions needed to interact with evidence
    ~ ghost.c 
        ~ A C file that contains all the functions needed to interact with ghosts
    ~ house.c
        ~ A C file that contains all the functions needed to interact with house
    ~ hunter.c
        ~ A C file that contains all the functions needed to interact with hunters
    ~ logger.c
        ~ A C file that contains a series of functions that print certain kinds of information 
    ~ main.c
        ~ A C file that contains a series of funcitons that initailis the values in the function, the threads which then run the simulation
          It also frees all allocted memory after the simulation is complete  
    ~ room.c
        ~ A C file that contains all the functions needed to interact with rooms
    ~ utils.c
        ~ A C file that contains a series of helper functions that are used in the other files
    ~ Makefile
        ~ A file that facilitates the compilation of the program 

Instructions for compiling:
	~ In the terminal, make sure you are in the same directory as all of the
	  included files. Then, type the following command, and press enter:
		~ make all
    ~ To remove output/executable files, type the following command, and press enter:
        ~ make clean

To run:
	~ Staying in the same directory, enter the following command, then press
	  enter:
		~ ./fp

To use:
    ~ When the program runs the user will be prompted with a request to enter the name of the first hunter then the second etc.A
      The user has to enter any four names and then the simulation will run with no further input from the user.
