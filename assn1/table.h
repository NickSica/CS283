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
tableNode *openTable(tableNode *table, size_t valsSize, size_t oneValSize, char *cmdFileName, char *dbDir, char *op);
int getFieldLoc(int length, char **tableFields, char *fieldName);
tableNode **findNode(tableNode *table, char *val, char *fieldName, int *numNodes);
void printNode(tableNode *node);
tableNode *allocTable(size_t valsSize, size_t oneValSize, size_t fileNameSize);
void freeTable(tableNode *node);

#endif
