/*
 * parsecmd.c
 *
 *  Created on: May 14, 2015
 *      Author: brandon
 */
#include "parsecmd.h"
#include <string.h>

#include <stdio.h>

cmd cmd_new(int _maxAgs){
    cmd my_cmd;

    my_cmd.maxArgs = _maxAgs;
    my_cmd.args=malloc(_maxAgs * sizeof(char*));
    //todo raise if args==0 malloc failed

    return my_cmd;
}

void parseCommand(char* input,cmd* command){
    command->cmd=input;

    /*use strtok to create an array of pointers the of each space separated word,
     * replacing ' ' with '/0'*/
    int numWords = -1;
    strtok( input , " " );

    printf("first=>%s\n",command->cmd);


    do {
        command->args[++numWords] = strtok( NULL , " " );
        printf("next=>%s\n",command->args[numWords]);

        //if(numWords)

    } while ( command->args[numWords] );



}
