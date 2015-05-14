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

//TODO can have global char** to local input buffer
//      unassigned here, assigned in main


//function prototypes
int showPrompt(char** inputBuff);

int main(void) {

	char *inputBuffer = malloc(INBUFFSIZE * sizeof(char));
	cmd inputCommand = cmd_new(512);  //todo define a constant

	while(!strEQ(inputBuffer,"exit")){
	    //todo, should i realloc downward on buffer increase
		int readCount = showPrompt(&inputBuffer);

		//printf("Received {%d/%d} bytes => %s", readCount, INBUFFSIZE, inputBuffer);
		bool success = parseCommand(inputBuffer,&inputCommand);

		int i=0;
		while(inputCommand.args[i]) printf("%s/",inputCommand.args[i++]);

	}

	return 0;
}

int showPrompt(char** inputBuff){

    //TODO clear input buffer

	printf("%s","\nsmallSH:");
	fflush(stdout);
	int readCount = (int) getline(inputBuff,&INBUFFSIZE,stdin);
	--readCount;
	while((*inputBuff)[readCount-1]==' ') --readCount;
    (*inputBuff)[readCount]='\0';
	return readCount;

	//TODO check for 0 or -1
}
