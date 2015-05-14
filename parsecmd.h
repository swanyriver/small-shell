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

typedef struct {
    char* cmd;
    char** args;
    int maxArgs;
    bool redirIn, redirOut;
    char* inFILE, outFILE;
    bool bkgrnd;
} cmd;

cmd cmd_new(int _maxAgs);
void parseCommand(char* input,cmd* command);


#endif /* PARSECMD_H_ */
