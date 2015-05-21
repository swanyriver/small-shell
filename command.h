/*
 * command.h
 *
 *  Created on: May 18, 2015
 *      Author: brandon
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <stdbool.h>

typedef enum { NOT_BUILT_IN, EXIT, CD, STATUS } built_in_type;

//used for parsing input
typedef struct {
    char* cmd;
    char** args;
    int maxArgs;
    bool redirIn, redirOut;
    char *inFILE, *outFILE;
    bool bkgrnd;
    built_in_type builtin;
} cmd;

#endif /* COMMAND_H_ */
