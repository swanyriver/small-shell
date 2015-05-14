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

//GLOBAL CONSTANTS
//#define INBUFFSIZE 512 // chat is requesting much longer that this TODO verify,
//is there a place to put this stream other than on the stack, read it pieces at a time?

//GLOBAL VARIABLES
const int INBUFFSIZE = 512;

//TODO can have global char** to local input buffer
//      unassigned here, assigned in main



//function prototypes
int showPrompt();

int main(void) {

	char *inputBuffer = malloc(INBUFFSIZE * sizeof(char));

	strncpy(inputBuffer,"hello",10);

	//printf("addr:%d, *str:%d, &str:%d, first(%d):%c, str:%s",inputBuffer, *inputBuffer, &inputBuffer, *inputBuffer,*(inputBuffer+1), inputBuffer);

	char** strpp = &inputBuffer;
	printf("firstchar %p,   bufferaddr: %p", inputBuffer, strpp);

	exit(1);

	while(strcmp(inputBuffer,"exit") != 0){
		int rec = showPrompt();
		//if(*inBuffPointer != inputBuffer) printf("%s", "input buffer has been malloced");
		//printf("Received {%d} bytes => %s", rec, inputBuffer);
	}

	return 0;
}

int showPrompt(){
	printf("%s","\nsmallSH:");
	fflush(stdout);
	//return getline(inBuffPointer,INBUFFSIZE,stdin);
	return 0;
}
