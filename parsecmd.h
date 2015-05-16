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

typedef enum { NOT_BUILT_IN, EXIT, CD, STATUS } built_in_type;

typedef struct {
    char* cmd;
    char** args;
    int maxArgs;
    bool redirIn, redirOut;
    char *inFILE, *outFILE;
    bool bkgrnd;
    built_in_type builtin;
} cmd;

cmd cmd_new(int _maxAgs);
bool parseCommand(char* input,cmd* command);
//int strEQ(char* a, char*b); //TODO remove if not used
bool strEQci(char* a, char*b);


#endif /* PARSECMD_H_ */
