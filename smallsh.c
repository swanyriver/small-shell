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
size_t INBUFFSIZE = 512;

//TODO can have global char** to local input buffer
//      unassigned here, assigned in main



//function prototypes
int showPrompt(char** inputBuff);

int main(void) {

	char *inputBuffer = malloc(INBUFFSIZE * sizeof(char));
	printf("heapstring:%p\n",inputBuffer);

	while(strcmp(inputBuffer,"exit") != 0){
	    //todo, should i realloc downward on buffer increase
		int readCount = showPrompt(&inputBuffer);
		printf("Received {%d} bytes => %s", readCount, inputBuffer);
	}

	return 0;
}

int showPrompt(char** inputBuff){

    //TODO clear input buffer

	printf("%s","\nsmallSH:");
	fflush(stdout);
	int readCount = (int) getline(inputBuff,&INBUFFSIZE,stdin);

	printf("heapstringPointer:%p\n",inputBuff);
    printf("heapstring:%p\n",*inputBuff);
    printf("heapstringchar:%c\n",**inputBuff);
    printf("heapstringchar:%c\n",*inputBuff[0]);

    (*inputBuff)[readCount-1]='\0';



	return --readCount;

	//TODO check for 0 or -1
}
