/*
 * parsecmd.h
 *
 *  Created on: May 14, 2015
 *      Author: brandon
 */

#ifndef PARSECMD_H_
#define PARSECMD_H_

#include <stdbool.h>
#include <stdlib.h>
#include "command.h"


cmd cmd_new(int _maxAgs);
bool parseCommand(char* input,cmd* command);
//int strEQ(char* a, char*b); //TODO remove if not used
bool strEQci(char* a, char*b);


#endif /* PARSECMD_H_ */
