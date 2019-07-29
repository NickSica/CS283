#include "Array.h"

int *add(int *arr, int *len, int *maxLen, int newElement, int loc)
{
	if(loc >= *maxLen)
	{
		arr = (int *)realloc(arr, sizeof(int)*loc);
		arr[loc] = newElement;
		*maxLen = loc;
	}
	else
	{
        if(*len == *maxLen)
        {
            arr = (int *)realloc(arr, sizeof(int)*(*maxLen+1));
            *maxLen = *maxLen + 1;
            *len = *maxLen;
        }

		int i;
        int temp = newElement;
		for(i = loc; i < *len; i++)
		{
            int temp2 = temp;
            temp = arr[i];
            arr[i] = temp2;
		}
    }
    return arr;
}

void arrRemove(int *arr, int *len, int loc)
{
	if(loc < *len-1)
	{
		int i;
		for(i = 0; i < *len-1; i++)
		{
			if(i >= loc)
			{
				arr[i] = arr[i+1];
			}
		}
	}
    *len -= 1;
}

int get(int *arr, int *len, int loc)
{
	if(loc < *len)
	{
		return arr[loc];
	}
	else
	{
		perror("Index out of bound");
		return -1;
	}
}

void printArray(int *arr, int len)
{
    int i;
    for(i = 0; i < len; i++)
    {
        printf("Element %d: %d\n", i, arr[i]);
    }
    printf("\n");
}









