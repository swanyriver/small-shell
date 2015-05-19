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
const int NONSENSE = -5;


//TODO can have global char** to local input buffer
//      unassigned here, assigned in main


//function prototypes
bool showPrompt(char** inputBuff);
void changedir(char* path);
void status();
void runcommand(cmd *command);

int main(void) {

	char *inputBuffer = malloc(INBUFFSIZE * sizeof(char));
	cmd inputCommand = cmd_new(ORIGINAL_MAX_ARGUMENTS);

	do{
		if(showPrompt(&inputBuffer)
		        && parseCommand(inputBuffer,&inputCommand)
		){
		    runcommand(&inputCommand);
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

void runcommand(cmd *command){

    int spawnpid = NONSENSE;

    spawnpid = fork();

    if(spawnpid == NONSENSE){
        fprintf(stderr,"%s","FORK ERROR: fork returned nonsense value");
        exit(1);
    }
    if(spawnpid == -1){
        fprintf(stderr,"%s","FORK ERROR: forking process failed");
        exit(1);
    }

    if (spawnpid == 0){
        //child process

        //redirect in and out
        prepare_redirects(command);

        execvp(command->cmd,command->args);

    } else {
        int status;
        waitpid(spawnpid,&status,0);

        if(WIFEXITED(status)){
            //puts(" sucsessfully");
        } else {
            //puts (" unsucsessful");
        }
    }


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
        fprintf(stderr,"%s",
              "ERROR: there was an error reading your input, please try again");
        return false;
    }

    return true;
}

void changedir(char* path){
    if(!path){
        fprintf(stderr,"%s","ERROR: must provide directory name/path");
        return;
    }

    if(chdir(path) != 0){
        const char* error = strerror(errno);
        fprintf(stderr, "ERROR:%s",error);
    } else {
        char* cwd = get_current_dir_name();  //todo change to pre-alloced getcwd()
        printf("CWD changed to:%s",cwd);
        free(cwd);
    }

}
