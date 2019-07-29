#include <stdlib.h>
#include <stdio.h>

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
    int *intPtr = malloc(sizeof(int)*10);
    int i;
    for(i = 0; i < 10; i++)
    {
        intPtr[i] = i * 2 + 7;
    }
    printArray(intPtr, 10);
    
    free(intPtr);
}
