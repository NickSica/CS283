#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    char *operation;
    char **fileNames;
    char **fieldNames;
    char **vals;
    int lengths;
    unsigned long longestVal;
    unsigned long longestField;
    unsigned long longestFile;
} command;

void parseSQL(char *input, command *cmd);


#endif
