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
#include "prepare.h"
#include "command.h"


//GLOBAL VARIABLES
size_t INBUFFSIZE = 512;
const int ORIGINAL_MAX_ARGUMENTS = 512;


//TODO can have global char** to local input buffer
//      unassigned here, assigned in main


//function prototypes
bool showPrompt(char** inputBuff);

int main(void) {

	char *inputBuffer = malloc(INBUFFSIZE * sizeof(char));
	cmd inputCommand = cmd_new(ORIGINAL_MAX_ARGUMENTS);

	do{
		if(showPrompt(&inputBuffer)
		        && parseCommand(inputBuffer,&inputCommand)
				&& preprared_to_exec(&inputCommand)
				&& redirects_ready(&inputCommand)
		){
			//EXECUTE COMMAND

			printf("command ready:%s",inputCommand.cmd);

		} else if (inputCommand.builtin == STATUS) {
			printf("status:%d",inputCommand.builtin);
		} else if (inputCommand.builtin == CD) {
			printf("cd:%d",inputCommand.builtin);
		}


	} while (inputCommand.builtin!=EXIT);


/*
	while(!exit){
	    //todo, should i realloc downward on buffer increase
		int readCount = showPrompt(&inputBuffer);

		//returns true on properly formated and not commented, otherwise not executed
		if(parseCommand(inputBuffer,&inputCommand)){
			//command is valid syntac
		    if(preprared_to_exec(&inputCommand)){
		    	//checked for built in, and read/writable files
		    	if(inputCommand.builtin){
		    		printf("built in #%d:%s,", inputCommand.builtin, inputCommand.cmd);
		    		if(inputCommand.builtin==EXIT){
		    			exit=true;
		    		}
		    	} else {
		    		//do appropriate redirection
		    		//forking and exec

		    	}
		    //end of Prepared to execute
		    }
		// end of parse command
		} //ELSE, bad command, re-prompt

	//end of input loop
	} //exit command issued to exit
*/

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
