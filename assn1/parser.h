#ifndef _PARSER_H_
#define _PARSER_H_

#include <string.h>

typedef struct
{
    char *operation;
    char **fileNames;
    char **fieldNames;
    char **vals;
    int lengths;
} command;

void parseSQL(char *input, command *cmd);


#endif
