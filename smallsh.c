/*
 ============================================================================
 Name        : smallsh.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parsecmd.h"


//GLOBAL VARIABLES
size_t INBUFFSIZE = 512;

//todo change to enum
const int EXIT = 1;
const int STATUS = 2;
const int CD = 3;


//todo maybe move to a seperate file
int isBuiltIn(char* cmd){

    if (strEQci(cmd,"exit")) return EXIT;
    if (strEQci(cmd,"status")) return STATUS;
    if (strEQci(cmd,"cd")) return CD;
    return 0;
}

//TODO can have global char** to local input buffer
//      unassigned here, assigned in main


//function prototypes
int showPrompt(char** inputBuff);

int main(void) {

	char *inputBuffer = malloc(INBUFFSIZE * sizeof(char));
	cmd inputCommand = cmd_new(512);  //todo define a constant
	bool exit = false;

	while(!exit){
	    //todo, should i realloc downward on buffer increase
		int readCount = showPrompt(&inputBuffer);

		//returns true on properly formated and not commented, otherwise not executed
		if(parseCommand(inputBuffer,&inputCommand)){
		    int command = isBuiltIn(inputCommand.cmd);
		    if (command==EXIT) exit=true;
		}


	}

	return 0;
}

int showPrompt(char** inputBuff){

	printf("%s","\nsmallSH:");
	fflush(stdout);
	int readCount = (int) getline(inputBuff,&INBUFFSIZE,stdin);
	--readCount;
	while((*inputBuff)[readCount-1]==' ') --readCount;
    (*inputBuff)[readCount]='\0';
	return readCount;

	//TODO check for 0 or -1
}
