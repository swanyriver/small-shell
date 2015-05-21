/*
 * prepare.c
 *
 *  Created on: May 15, 2015
 *      Author: brandon
 */

#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prepare.h"
#include "error.h"


//used to print errno msg and close files before exiting child process
void _safe_exit(int FDout, int FDin, const char* errorMsg){

    fprintf(stderr,"FILE ERROR:%s\n",errorMsg);
    fflush(stderr);

    if(FDin) close(FDin);
    if(FDout) close(FDout);
    exit(1);
}

//open files and attach pipes
//exit with error message on any failure
void prepare_redirects(cmd *command){

    int FDin = 0;
    int FDout = 0;

    if(command->redirIn){
        //open user specified file for reading
        FDin = open(command->inFILE, O_RDONLY);

        //check for error in opening
        if(FDin == 0){
            _safe_exit(FDin,FDout,"File for input not opened");
        } else if (FDin == -1){
            _safe_exit(FDin,FDout,strerror(errno));
        }

        //file opened succesfully, redirect stdin
        int redir = dup2(FDin,0);
        //check for error in redirecting
        if(redir == -1){
            _safe_exit(FDin,FDout,strerror(errno));
        }

        close(FDin);
    }

    //redirect stdin for background tasks
    else if(command->bkgrnd){
        //open user dev/null file for reading
        FDin = open("/dev/null", O_RDONLY);

        //check for error in opening
        if (FDin == -1){
            _safe_exit(FDin,FDout,strerror(errno));
        }

        //file opened succesfully, redirect stdin
        int redir = dup2(FDin,0);
        //check for error in redirecting
        if(redir == -1){
            _safe_exit(FDin,FDout,strerror(errno));
        }

        close(FDin);
    }

    if(command->redirOut){
        //open user specified file for writing, create if not existent
        FDout = open(command->outFILE, O_WRONLY|O_CREAT|O_TRUNC, 0644);

        //check for error in opening
        if(FDout == 0){
            _safe_exit(FDin,FDout,"File for output not opened");
        } else if (FDout == -1){
            _safe_exit(FDin,FDout,strerror(errno));
        }

        //file opened succesfully, redirect stdout
        int redir = dup2(FDout,1);

        //chec for error in redirecting
        if(redir == -1){
            _safe_exit(FDin,FDout,strerror(errno));
        }

        close(FDout);
    }

}



