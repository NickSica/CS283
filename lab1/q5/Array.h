#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdio.h>
#include <stdlib.h>

int *add(int *arr, int *len, int *maxLen, int newElement, int loc);
void arrRemove(int *arr, int *len, int loc);
int get(int *arr, int *len, int loc);
void printArray(int *arr, int len);

#endif
