#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include "parsecmd.h"
#include "prepare.h"
#include "command.h"


//GLOBAL VARIABLES
size_t INBUFFSIZE = 512;
const int ORIGINAL_MAX_ARGUMENTS = 512;
const int NONSENSE = -5;
const int NAME_SIZE = 512;

typedef struct {
    int pid;
    char* Pname;
    int exit_status;
} process;


//function prototypes
bool showPrompt(char** inputBuff);
void changedir(char* path);
void runcommand(cmd *command, process *proc);
void deadChild(int signum);


int main(void) {

    //set up signal handling
    struct sigaction interuptAction;
    interuptAction.sa_handler=SIG_IGN;
    sigaction(SIGINT,&interuptAction,NULL);
    sigaction(SIGCHLD,&interuptAction,NULL);

	char *inputBuffer = malloc(INBUFFSIZE * sizeof(char));
	cmd inputCommand = cmd_new(ORIGINAL_MAX_ARGUMENTS);

	//set up process exit status recording for status command
	process lastP = {.pid=0,
	        .Pname = malloc(sizeof(char)*NAME_SIZE),
	        .exit_status =0};
	if (!lastP.Pname){
	    const char* error = strerror(errno);
        fprintf(stderr, "MEMORY ERROR:%s",error);
        exit(1);
	} //todo do a null check on all mallocs

	//begin input output loop
	do{
		if(showPrompt(&inputBuffer)
		        && parseCommand(inputBuffer,&inputCommand)
		){
		    runcommand(&inputCommand, &lastP);
		} else if (inputCommand.builtin == STATUS) {
			printf("(%d) %s exited with status:%d\n",
			        lastP.pid,lastP.Pname,lastP.exit_status);  //todo check for signal
		} else if (inputCommand.builtin == CD) {
		    changedir(inputCommand.args[1]);
		}


	} while (inputCommand.builtin!=EXIT);

	free(inputCommand.args);
	free(inputBuffer);
	return 0;
}

void runcommand(cmd *command, process *proc){

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

        if(execvp(command->cmd,command->args) == -1){
            const char* error = strerror(errno);
            fprintf(stderr,"EXEC ERROR: %s", error);
            exit(1);
        }else {
            //should never arrive here
            fprintf(stderr,"EXEC ERROR: %s", "undefined exec behavior");
            exit(1);
        }


    } else {
        //parent process
        int status;

        if(!command->bkgrnd){

            strncpy(proc->Pname,command->cmd,NAME_SIZE);
            proc->pid=spawnpid;

            waitpid(spawnpid,&status,0);

            if(WIFEXITED(status)){
                proc->exit_status = WEXITSTATUS(status);
            } else if (WIFSIGNALED(status)){
                //todo check for signal
            }
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
        char* cwd = (char*) get_current_dir_name();  //todo change to pre-alloced getcwd()
        printf("CWD changed to:%s",cwd);
        free(cwd);
    }

}
