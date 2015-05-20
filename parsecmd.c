/*
 * parsecmd.c
 *
 *  Created on: May 14, 2015
 *      Author: brandon
 */
#include "parsecmd.h"
#include "error.h"
#include <string.h>
#include <stdio.h>

bool strEQci(char* a, char*b){
    while(*a && *b){
        if ( ((*a++)|32) != ((*b++)|32))  return false;
    }
    if (*a != *b) return false;
    return true;
}

int _isBuiltIn(char* cmd){

    if (strEQci(cmd,"exit")) return EXIT;
    if (strEQci(cmd,"status")) return STATUS;
    if (strEQci(cmd,"cd")) return CD;
    return 0;
}


cmd cmd_new(int _maxAgs){
    cmd my_cmd;

    my_cmd.maxArgs = _maxAgs;
    my_cmd.args=malloc(_maxAgs * sizeof(char*));
    if(!my_cmd.args) error_exit("MEMORY ERROR");

    return my_cmd;
}

void _resetCMD(cmd* command){
    command->redirIn=false;
    command->redirOut=false;
    command->bkgrnd=false;
    command->builtin=NOT_BUILT_IN;
}

bool _isSyntax(char* arg){
    if(arg[1]!='\0') return false;
    if(arg[0]=='<') return true;
    if(arg[0]=='>') return true;
    if(arg[0]=='&') return true;
    return false;
}

//return true if syntax is valid,
bool parseCommand(char* input,cmd* command){

    if(input[0] == '#' || input[0] == '\0' || input[0] == '\n'){
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

        if(numWords>=command->maxArgs){
            command->maxArgs *=2;
            char** tmp = realloc(command->args,command->maxArgs * sizeof(char*));
            if(tmp) command->args = tmp;
            else {
                error_exit("MEMMORY ERROR:");
            }
        }


        if(command->args[numWords] && _isSyntax(command->args[numWords])){
            if (!endOfArgs)endOfArgs = numWords;

            if( command->args[numWords][0] == '>' ) {
                outFileINDX = numWords;
                command->redirOut = true;
            }
            if( command->args[numWords][0] == '<' ){
                inFileINDX = numWords;
                command->redirIn = true;
            }
            if( command->args[numWords][0] == '&' ) {
                bkgroundINDX = numWords;
                command->bkgrnd = true;
            }
        }

    } while ( command->args[numWords] );

    command->builtin = _isBuiltIn(command->cmd);
    if(command->builtin) return false;

    if(endOfArgs){

        if(command->bkgrnd && bkgroundINDX < numWords-1){
            fprintf(stderr,"%s",
                    "SYNTAX ERROR: no commands belong after & symbol\n");
            return false;
        }

        //syntax checking in and out redirect
        if(command->redirIn){

            if(!command->args[inFileINDX+1] ||
                    _isSyntax(command->args[inFileINDX+1])){
                fprintf(stderr,"%s",
                        "SYNTAX ERROR: please provide input file\n");
                return false;
            }

            if(command->args[inFileINDX+2] &&
                    ! _isSyntax(command->args[inFileINDX+2] ) ){
                fprintf(stderr,"%s",
                        "SYNTAX ERROR: please provide only one input file\n");
                return false;
            }

            command->inFILE = command->args[inFileINDX+1];
        }

        if(command->redirOut){

            if(!command->args[outFileINDX+1] ||
                    _isSyntax(command->args[outFileINDX+1])){
                fprintf(stderr,"%s",
                        "SYNTAX ERROR: please provide output file\n");
                return false;
            }

            if(command->args[outFileINDX+2] &&
                    ! _isSyntax(command->args[outFileINDX+2] ) ){
                fprintf(stderr,"%s",
                        "SYNTAX ERROR: please provide only one output file\n");
                return false;
            }

            command->outFILE = command->args[outFileINDX+1];
        }

        //preparing args array for execv call
        command->args[endOfArgs]=NULL;
    }

    return true;

}
