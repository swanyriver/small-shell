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

//compares strings character by character, converting each to lower case
//used for checking for built in commands "cd", "status", "exit"
bool strEQci(char* a, char*b){
    while(*a && *b){
        if ( ((*a++)|32) != ((*b++)|32))  return false;
    }
    if (*a != *b) return false;
    return true;
}

//check input against built in commands
int _isBuiltIn(char* cmd){

    if (strEQci(cmd,"exit")) return EXIT;
    if (strEQci(cmd,"status")) return STATUS;
    if (strEQci(cmd,"cd")) return CD;
    return 0;
}

//initialize new command struct and allocate memory
cmd cmd_new(int _maxAgs){
    cmd my_cmd;

    my_cmd.maxArgs = _maxAgs;
    my_cmd.args=malloc(_maxAgs * sizeof(char*));

    //check for bad memory allocation
    if(!my_cmd.args) error_exit("MEMORY ERROR");

    return my_cmd;
}

//called after each input, to reset reused struct
void _resetCMD(cmd* command){
    command->redirIn=false;
    command->redirOut=false;
    command->bkgrnd=false;
    command->builtin=NOT_BUILT_IN;
}

//checks for single character shell syntax < > &
bool _isSyntax(char* arg){
    if(arg[1]!='\0') return false;
    if(arg[0]=='<') return true;
    if(arg[0]=='>') return true;
    if(arg[0]=='&') return true;
    return false;
}

//return true if syntax is valid,
bool parseCommand(char* input,cmd* command){

    //checking for comment or blank line
    if(input[0] == '#' || input[0] == '\0' || input[0] == '\n'){
        return false;
    }

    _resetCMD(command);

    int endOfArgs = 0;
    int inFileINDX = 0;
    int outFileINDX = 0;
    int bkgroundINDX = 0;
    //will become pointer to first 'word' after strtok runs
    command->cmd=input;
    command->args[0]=input;
    /*use strtok to create an array of pointers the of each space separated word,
     * replacing ' ' with '/0'*/
    int numWords = 0;
    strtok( input , " " );
    do {
        //capture pointer to begining of next arg
        command->args[++numWords] = strtok( NULL , " " );

        //reallocate arg pointer array if necessary
        if(numWords>=command->maxArgs){
            command->maxArgs *=2;
            char** tmp = realloc(command->args,command->maxArgs * sizeof(char*));
            if(tmp) command->args = tmp;
            else {
                error_exit("MEMMORY ERROR:");
            }
        }

        //set redir flags and record location of syntax marker
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

    //check for built in commands
    command->builtin = _isBuiltIn(command->cmd);
    if(command->builtin) return false;

    //syntax checking and parsing of redirect and background commands
    if(endOfArgs){

        //ensure & is the last argument
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
