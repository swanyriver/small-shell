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
#include "prepare.h"



void _safe_exit(int FDout, int FDin, char* errorMsg){

    fprintf(stderr,"FILE ERROR:%s\n",errorMsg);

    if(FDin) close(FDin);
    if(FDout) close(FDout);
    exit(1);
}

//open files and attach pipes
//exit with error message on any failure
void preprare_redirects(cmd *command){

    int FDin = 0;
    int FDout = 0;

	if(command->redirIn){

	    printf("opening input redirect:%s\n",command->inFILE);

	    FDin = open(command->inFILE, O_RDONLY);

	    if(FDin == 0){
	        _safe_exit(FDin,FDout,"File for input not opened");
	    } else if (FDin == -1){
	        _safe_exit(FDin,FDout,(char*) strerror(errno));
	    }

	    //file opened succesfully, redirect stdin


	}

	if(command->redirOut){
	    printf("opening output redirect:%s\n",command->outFILE);
	    FDout = open(command->outFILE, O_WRONLY|O_CREAT|O_TRUNC, 0644);


	}

	//todo, ask brewster, who takes care of closing these file descriptors

}



