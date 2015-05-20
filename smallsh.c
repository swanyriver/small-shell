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
#include "error.h"


//GLOBAL VARIABLES
size_t INBUFFSIZE = 512;
const int ORIGINAL_MAX_ARGUMENTS = 512;
const int NONSENSE = -5;
const int NAME_SIZE = 512;
const int DIR_NAME_SIZE = 512;

typedef struct {
    int pid;
    char* Pname;
    bool exitnorm;
    int exit_status;
    int signal;
} process;


//function prototypes
bool showPrompt(char** inputBuff);
void changedir(char* path, char* cwd);
void runcommand(cmd *command, process *proc);
void check_completedBG();

int main(void) {

    //set up signal handling
    struct sigaction action;
    action.sa_handler=SIG_IGN;
    sigaction(SIGINT,&action,NULL);

    char *dir_name = malloc(DIR_NAME_SIZE * sizeof(char));
    if (!dir_name) error_exit("MEMORY ERROR");
	char *inputBuffer = malloc(INBUFFSIZE * sizeof(char));
	if (!inputBuffer) error_exit("MEMORY ERROR");
	cmd inputCommand = cmd_new(ORIGINAL_MAX_ARGUMENTS);

	//set up process exit status recording for status command
	process lastP = {.pid=0,
	        .Pname = malloc(sizeof(char)*NAME_SIZE),
	        .exit_status =0};
	if (!lastP.Pname){
	    error_exit("MEMORY ERROR");
	}



	//begin input output loop
	do{
	    check_completedBG();
	    if(showPrompt(&inputBuffer)
		        && parseCommand(inputBuffer,&inputCommand))
	    {

		    runcommand(&inputCommand, &lastP);

	    } else if (inputCommand.builtin == STATUS) {
		    if(lastP.exitnorm){
		        printf("(%d) %s exited with status:%d\n",
		                lastP.pid,lastP.Pname,lastP.exit_status);
		    } else {
		        printf("(%d) %s ended with signal:%s\n",
		               lastP.pid,lastP.Pname,strsignal(lastP.signal));
		    }
		} else if (inputCommand.builtin == CD) {
		    changedir(inputCommand.args[1], dir_name);
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
            fprintf(stderr,"EXEC ERROR: %s\n", error);
            exit(1);
        }else {
            //should never arrive here
            fprintf(stderr,"EXEC ERROR: %s\n", "undefined exec behavior");
            exit(1);
        }


    } else {
        //parent process
        int status;

        if(command->bkgrnd){
            printf("%s started in background, PID:%d\n",command->cmd, spawnpid);

        } else {

            strncpy(proc->Pname,command->cmd,NAME_SIZE);
            proc->pid=spawnpid;

            waitpid(spawnpid,&status,0);

            if(WIFEXITED(status)){
                proc->exitnorm = true;
                proc->exit_status = WEXITSTATUS(status);
            } else if (WIFSIGNALED(status)){
                proc->exitnorm= false;
                proc->signal = WTERMSIG(status);
                printf("%s exited due to signal: %s\n",
                        command->cmd, strsignal(proc->signal));
            }
        }
    }


}

void check_completedBG(){
    int status;
    int pid;

    do{
        pid = waitpid(-1,&status,WNOHANG);

        if (pid > 0) {
            if (WIFSIGNALED(status)){
                printf("background process %d ended signal:%s\n",
                        pid, strsignal( WTERMSIG(status) ) );
            } else if(WIFEXITED(status)){
                printf("background process %d exited with status:%d\n",
                        pid, WEXITSTATUS(status));
            }
        }

    }while(pid>0);
}


bool showPrompt(char** inputBuff){

	printf("%s","smallSH:");
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
        fprintf(stderr,"%s\n",
              "ERROR: there was an error reading your input, please try again");
        return false;
    }

    //return to interactive mode when reached end of another file
    if (feof(stdin)) {
      if (!freopen("/dev/tty", "r", stdin)) {
        error_exit("STREAM ERROR:");
      }
    }

    return true;
}

void changedir(char* path, char* cwd){
    if(!path){
        path = getenv("HOME");
    }

    if(chdir(path) != 0){
        const char* error = strerror(errno);
        fprintf(stderr, "ERROR:%s\n",error);
    } else {
        getcwd(cwd, DIR_NAME_SIZE);
        printf("CWD changed to:%s\n",cwd);
    }

}
