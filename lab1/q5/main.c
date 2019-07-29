#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Array.h"

int main()
{
    srand(time(0));
    int maxLen = 10;
    int len = maxLen;
    int *intArr = malloc(sizeof(int)*maxLen);
    int i = 0;
    for(i = 0; i < maxLen; i++)
    {
        intArr[i] = rand();
    }
    printArray(intArr, maxLen);
    
    int index = rand() % maxLen;
    printf("Index to add at: %d\n", index);
    intArr = add(intArr, &len, &maxLen, 20, index);
    printArray(intArr, maxLen);
    

    index = rand() % maxLen;
    printf("Index to remove: %d\n", index);
    arrRemove(intArr, &maxLen, index);
    printArray(intArr, maxLen);

    index = rand() % maxLen;
    printf("Index to get: %d\n", index);
    printf("Value: %d\n", get(intArr, &maxLen, index));

    free(intArr);
}