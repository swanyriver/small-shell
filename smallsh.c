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
size_t INBUFFSIZE = 512;  //changes when buffer realocated by getline
const int ORIGINAL_MAX_ARGUMENTS = 512;
const int NONSENSE = -5;
const int NAME_SIZE = 512;
const int DIR_NAME_SIZE = 512;

//used by status to describe last process
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

    //set up ignoring SIGINT
    struct sigaction action;
    action.sa_handler=SIG_IGN;
    sigaction(SIGINT,&action,NULL);

    //allocate space for string variables and create struct for storing commands
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


    ////////////////////////////////////////////////////////
    //begin prompt => input => verify => fork/execute loop//
    ////////////////////////////////////////////////////////
    do{
        //reap completed background tasks
        check_completedBG();

        if(showPrompt(&inputBuffer)
                && parseCommand(inputBuffer,&inputCommand))
        {
            //no errors on input, or parsing, and is not built in
            runcommand(&inputCommand, &lastP);

        } else if (inputCommand.builtin == STATUS) {
            //retrieve last endend tasks exit satus or signal
            if(lastP.exitnorm){
                printf("(%d) %s exited with status:%d\n",
                        lastP.pid,lastP.Pname,lastP.exit_status);
            } else {
                printf("(%d) %s ended with signal:%s\n",
                       lastP.pid,lastP.Pname,strsignal(lastP.signal));
            }
        } else if (inputCommand.builtin == CD) {
            //changes working director to supplied directory in args[1] or home if null
            changedir(inputCommand.args[1], dir_name);
        }


    } while (inputCommand.builtin!=EXIT);


    //free up heap allocated strings
    free(inputCommand.args);
    free(inputBuffer);
    free(dir_name);
    free(lastP.Pname);


    return 0;
}

void runcommand(cmd *command, process *proc){

    int spawnpid = NONSENSE;

    //begin separate processes
    spawnpid = fork();

    //Unlikely to be reached
    if(spawnpid == NONSENSE){
        fprintf(stderr,"%s","FORK ERROR: fork returned nonsense value");
        exit(1);
    }
    if(spawnpid == -1){
        fprintf(stderr,"%s","FORK ERROR: forking process failed");
        exit(1);
    }

    if (spawnpid == 0){
        ///////////////////////////
        ///////child process//////
        //////////////////////////

        //reset sigaction
        struct sigaction action;
        action.sa_handler=SIG_DFL;
        sigaction(SIGINT,&action,NULL);

        //redirect stdin and stdout
        prepare_redirects(command);

        if(execvp(command->cmd,command->args) == -1){
            //unable to execute, print errno string
            error_exit("EXEC ERROR");
        }else {  
            //should never arrive here  //child process should cease if sucsseful exec
            fprintf(stderr,"EXEC ERROR: %s\n", "undefined exec behavior");
            exit(1);
        }


    } else {
        ///////////////////////
        ////parent process/////
        ///////////////////////

        int status;

        if(command->bkgrnd){
            printf("%s started in background, PID:%d\n",command->cmd, spawnpid);

        } else {

            //save process name for status call
            strncpy(proc->Pname,command->cmd,NAME_SIZE);
            proc->pid=spawnpid;

            //halt shell until process completed
            waitpid(spawnpid,&status,0);

            //save exit status for calls to status
            if(WIFEXITED(status)){
                proc->exitnorm = true;
                proc->exit_status = WEXITSTATUS(status);
            } else if (WIFSIGNALED(status)){
                proc->exitnorm= false;
                proc->signal = WTERMSIG(status);

                //display signal information
                printf("%s exited due to signal: %s\n",
                        command->cmd, strsignal(proc->signal));
            }
        }
    }

}


//ran once before each time promt is shown
//caling waitpid(-1,..,WNOHANG) prevents zombies by waiting for finished children processes
void check_completedBG(){
    int status;
    int pid;

    do{

        //proceed imediatly if no finished children
        //return 0 if there are unfinished children
        pid = waitpid(-1,&status,WNOHANG);

        //pid of finished child returned
        if (pid > 0) {

            //use macros to get exit status or signal
            if (WIFSIGNALED(status)){
                printf("background process %d ended signal:%s\n",
                        pid, strsignal( WTERMSIG(status) ) );
            } else if(WIFEXITED(status)){
                printf("background process %d exited with status:%d\n",
                        pid, WEXITSTATUS(status));
            }
        }
    //reap all finished children
    }while(pid>0);
}


bool showPrompt(char** inputBuff){

    //show our prompt
    printf("%s","smallSH:");
    fflush(stdout);

    //get input using gnu extension getline, chosen for automatic realloc if input exceds INBUFFSIZE
    size_t readCount = getline(inputBuff,&INBUFFSIZE,stdin);

    //return to interactive mode when reached end of another file
    if (feof(stdin)) {
      if (!freopen("/dev/tty", "r", stdin)) {
        error_exit("STREAM ERROR:");
      }
      putc('\n',stdout);
      return false;
    } else if(readCount == 1){
        //user hit enter with no input
        return false;
    } else if(readCount == -1){ 
        //getline returned an error
        fprintf(stderr,"%s\n",
              "ERROR: there was an error reading your input, please try again");
        return false;
    }

    //remove trailing spaces
    --readCount;
    while((*inputBuff)[readCount-1]==' ') --readCount;
    (*inputBuff)[readCount]='\0';

    return true;
}

void changedir(char* path, char* cwd){

    //if no directory provided retrieve home directory
    if(!path){
        path = getenv("HOME");
    }

    //change directory and check for error
    if(chdir(path) != 0){
        const char* error = strerror(errno);
        fprintf(stderr, "ERROR:%s\n",error);
    } else {
        //place new directory name in cwd buffer and print
        getcwd(cwd, DIR_NAME_SIZE);
        printf("CWD changed to:%s\n",cwd);
    }

}
