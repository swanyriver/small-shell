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
#include <errno.h>
#include "parsecmd.h"
#include "prepare.h"
#include "command.h"


//GLOBAL VARIABLES
size_t INBUFFSIZE = 512;
const int ORIGINAL_MAX_ARGUMENTS = 512;


//TODO can have global char** to local input buffer
//      unassigned here, assigned in main


//function prototypes
bool showPrompt(char** inputBuff);
void changedir(char* path);
void status();

int main(void) {

	char *inputBuffer = malloc(INBUFFSIZE * sizeof(char));
	cmd inputCommand = cmd_new(ORIGINAL_MAX_ARGUMENTS);

	do{
		if(showPrompt(&inputBuffer)
		        && parseCommand(inputBuffer,&inputCommand)
		){
			//EXECUTE COMMAND

			printf("command ready:%s",inputCommand.cmd);

		} else if (inputCommand.builtin == STATUS) {
			printf("status:%d",inputCommand.builtin);  //todo call status
		} else if (inputCommand.builtin == CD) {
		    changedir(inputCommand.args[1]);
		}


	} while (inputCommand.builtin!=EXIT);

	free(inputCommand.args);
	free(inputBuffer);
	return 0;
}

bool showPrompt(char** inputBuff){

	printf("%s","\nsmallSH:");
	fflush(stdout);
	size_t readCount = getline(inputBuff,&INBUFFSIZE,stdin);
	--readCount;
	while((*inputBuff)[readCount-1]==' ') --readCount;
    (*inputBuff)[readCount]='\0';


    //user hit enter with no input
    if(readCount == 0){
        return false;
    }

    if(readCount == -1){
        printf("%s",
              "ERROR: there was an error reading your input, please try again");
        return false;
    }

    return true;
}

void changedir(char* path){
    if(!path){
        fprintf(stderr,"%s","ERROR: must provide directory name/path"); //TODO change all errors to stderr
        return;
    }

    if(chdir(path) != 0){
        const char* error = strerror(errno);
        fprintf(stderr, "ERROR:%s",error);
    } else {
        char* cwd = get_current_dir_name();
        printf("CWD changed to:%s",cwd);
        free(cwd);
    }

}
