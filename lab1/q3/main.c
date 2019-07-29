#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Sort.h"

void printArray(int *arr, int len)
{
    int i;
    for(i = 0; i < len; i++)
    {
        printf("Element %d: %d\n", i, arr[i]);
    }
    printf("\n");
}

int main()
{
    srand(time(0));
    int len = 10;
    int *intArr = malloc(sizeof(int)*len);
    int i = 0;
    for(i = 0; i < len; i++)
    {
        intArr[i] = rand();
    }
    printf("Unsorted:\n");
    printArray(intArr, len);

    sort(intArr, len);
    printf("Sorted:\n");
    printArray(intArr, len);

    free(intArr);
}