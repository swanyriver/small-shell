/*
 * prepare.c
 *
 *  Created on: May 15, 2015
 *      Author: brandon
 */

#include <stdbool.h>
#include "prepare.h"

int _isBuiltIn(char* cmd){

    if (strEQci(cmd,"exit")) return EXIT;
    if (strEQci(cmd,"status")) return STATUS;
    if (strEQci(cmd,"cd")) return CD;
    return 0;
}

bool preprared_to_exec(cmd *command){

	command->builtin = _isBuiltIn(command->cmd);
	if(command->builtin) return false;

	if(command->redirIn){
		//open file, return false if unable
	}

	if(command->redirOut){
		//open file, return false if unable
	}

	return true;
}

bool redirects_ready(cmd *command){
	if(!(command->redirIn || command->redirOut)) return true;

	return true;
}


