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
#define INBUFFSIZE 512 // chat is requesting much longer that this TODO verify,
//is there a place to put this stream other than on the stack, read it pieces at a time?

//GLOBAL VARIABLES
char inputBuffer[INBUFFSIZE];
char** inBuffPointer = inputBuffer;

int showPrompt();

int main(void) {

	while(strcmp(inputBuffer,"exit") != 0){
		size_t rec = showPrompt();
		if(*inBuffPointer != inputBuffer) printf("%s", "input buffer has been malloced");
		printf("Received {%d} bytes => %s", rec, inputBuffer);
	}

	return 0;
}

int showPrompt(){
	printf("%s","\nsmallSH:");
	fflush(stdout);
	return getline(inBuffPointer,INBUFFSIZE,stdin);
}
