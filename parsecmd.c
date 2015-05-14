/*
 * parsecmd.c
 *
 *  Created on: May 14, 2015
 *      Author: brandon
 */
#include "parsecmd.h"
#include <string.h>

#include <stdio.h>

int strEQ(char* a, char*b){return strcmp(a,b) == 0;}

cmd cmd_new(int _maxAgs){
    cmd my_cmd;

    my_cmd.maxArgs = _maxAgs;
    my_cmd.args=malloc(_maxAgs * sizeof(char*));
    //todo raise if args==0 malloc failed

    return my_cmd;
}

void _resetCMD(cmd* command){
    command->redirIn=false;
    command->redirOut=false;
    command->bkgrnd=false;
}

bool _isSyntax(char* arg){
    if(arg[1]!='\0') return false;
    if(arg[0]=='<') return true;
    if(arg[0]=='>') return true;
    if(arg[0]=='&') return true;
    return false;
}

//return true if syntax is valid
//prints own error message, todo maybe pass msg back to controller
bool parseCommand(char* input,cmd* command){

    if(input[0] == '#' || input[0] == '\0'){
        puts("comment line");
        return false;
    }

    _resetCMD(command);

    command->cmd=input;
    command->args[0]=input;
    int endOfArgs = 0;
    int inFileINDX = 0;
    int outFileINDX = 0;
    int bkgroundINDX = 0;
    /*use strtok to create an array of pointers the of each space separated word,
     * replacing ' ' with '/0'*/
    int numWords = 0;
    strtok( input , " " );
    do {
        command->args[++numWords] = strtok( NULL , " " );

        //printf("next=>%s\n",command->args[numWords]); //todo remove debug print

        if(numWords>=command->maxArgs){
            command->maxArgs *=2;
            char** tmp = realloc(command->args,command->maxArgs * sizeof(char*));
            if(tmp) command->args = tmp;
            else exit(1);  //todo raise()
        }


        if(command->args[numWords] && _isSyntax(command->args[numWords])){
            if (!endOfArgs)endOfArgs = numWords;

            if( command->args[numWords][0] == '>' ) {
                outFileINDX = numWords;
                command->redirIn = true;
            }
            if( command->args[numWords][0] == '<' ){
                inFileINDX = numWords;
                command->redirOut = true;
            }
            if( command->args[numWords][0] == '&' ) {
                bkgroundINDX = numWords;
                command->bkgrnd = true;
            }
        }

    } while ( command->args[numWords] );


    if(endOfArgs){

        if(command->bkgrnd && bkgroundINDX < numWords-1){
            printf("%s","SYNTAX ERROR: no commands belong after & symbol");
        }

        /*if (numWords-endOfArgs >
            command->redirIn*2 + command->redirOut + command->bkgrnd ){
            //i suspect there are too many
        }*/

        //todo check for out of order input output redirect
        //todo check for not enough or too many argumetns after > <

        if(command->redirIn) command->inFILE = command->args[inFileINDX+1];
        if(command->redirOut) command->outFILE = command->args[inFileINDX+1];

        command->args[endOfArgs]=NULL;
    }

    return true;

}
