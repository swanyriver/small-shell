/*
 * error.c
 *
 *  Created on: May 18, 2015
 *      Author: brandon
 */


#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"

//prints errno message and exits with failure status
void error_exit(char* header){
    const char* error = strerror(errno);
    fprintf(stderr, "%s:%s\n",header,error);
    exit(1);
}
