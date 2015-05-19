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
	    FDin = open(command->inFILE, O_RDONLY);

	    if(FDin == 0){
	        _safe_exit(FDin,FDout,"File for input not opened");
	    } else if (FDin == -1){
	        _safe_exit(FDin,FDout,strerror(errno));
	    }

	    //file opened succesfully, redirect stdin
	    int redir = dup2(FDin,0);
	    if(redir == -1){
	        _safe_exit(FDin,FDout,strerror(errno));
	    }

	    close(FDin);
	}
	//redirect stdin for background tasks
	else if(command->bkgrnd){
	    FDin = open("/dev/null", O_RDONLY);

	    if (FDin == -1){
            _safe_exit(FDin,FDout,strerror(errno));
        }

	    int redir = dup2(FDin,0);

	    if(redir == -1){
            _safe_exit(FDin,FDout,strerror(errno));
        }

        close(FDin);
	}

	if(command->redirOut){
	    FDout = open(command->outFILE, O_WRONLY|O_CREAT|O_TRUNC, 0644);

	    if(FDout == 0){
            _safe_exit(FDin,FDout,"File for input not opened");
        } else if (FDout == -1){
            _safe_exit(FDin,FDout,strerror(errno));
        }

        //file opened succesfully, redirect stdout
        int redir = dup2(FDout,1);
        if(redir == -1){
            _safe_exit(FDin,FDout,strerror(errno));
        }

        close(FDout);
	}

}



