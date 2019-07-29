#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printArray(char **arr, int len)
{
    int i;
    for(i = 0; i < len; i++)
    {
        printf("%s\n", arr[i]);
    }
}

int main()
{  
    int len = 10;
    char **charPtr = malloc(sizeof(char*)*len);
    int i = 0;
    for(i = 0; i < len; i++)
    {
        charPtr[i] = (char *)malloc(sizeof(char)*15);
    }
    strncpy(charPtr[0], "hey", 15);
    strncpy(charPtr[1], "superfluous", 15);
    strncpy(charPtr[2], "never", 15);
    strncpy(charPtr[3], "country", 15);
    strncpy(charPtr[4], "honest", 15);
    strncpy(charPtr[5], "garden", 15);
    strncpy(charPtr[6], "life", 15);
    strncpy(charPtr[7], "earplugs", 15);
    strncpy(charPtr[8], "suitcase", 15);
    strncpy(charPtr[9], "watch", 15);

    printArray(charPtr, len);

    for(i = 0; i < len; i++)
    {
        free(charPtr[i]);
    }
    free(charPtr);
}