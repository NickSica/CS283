#ifndef _TABLE_H_
#define _TABLE_H_

#include <stdio.h>
#include <stdlib.h>

#include "csapp.h"

struct tableNode;
typedef struct tableNode tableNode;

struct tableNode {
    char **vals;
    char *fileName;
    tableNode *next;
    tableNode *prev;
    int length;
};

void writeTable(tableNode *table, char *dbDir);
tableNode *openTable(tableNode *table, char *cmdTableName, char *dbDir);
int getFieldLoc(int length, char **tableFields, char *fieldName);
tableNode **findNode(tableNode *table, char *val, char *fieldName);
void printNode(tableNode *node);
void freeTable(tableNode *node);

#endif
