#include "Sort.h"

void sort(int *a, int size)
{
    int i = 0;
    while(i < size)
    {
        int j;
        for(j = i + 1; j < size; j++)
        {
            if(*(a+i) > *(a+j))
            {
                swap(a, i, j);
            }
        }
        i++;
    }
}

void swap(int *a, int i, int j)
{
    int tempVal = *(a+i);
    *(a+i) = *(a+j);
    *(a+j) = tempVal;
}
