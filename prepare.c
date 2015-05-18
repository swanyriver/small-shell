/*
 * prepare.c
 *
 *  Created on: May 15, 2015
 *      Author: brandon
 */

#include <stdbool.h>
#include "prepare.h"


//open files and attach pipes
//exit with error message on any failure
void preprare_redirects(cmd *command){

	if(command->redirIn){
		//open file, return false if unable
	    //set close on exec
	}

	if(command->redirOut){
		//open file, return false if unable
	}

}



