#include <pthread.h>
#include <stdio.h>

volatile int count = 0;

void *counter()
{
    for(int i = 0; i < 999; i++)
	count++;
    return NULL;
}


int main()
{
    pthread_t tid[100];
    for(int i = 0; i < 99; i++)
	pthread_create(&tid[i], NULL, counter, NULL);

    for(int i = 0; i < 99; i++)
	pthread_join(tid[i], NULL);

    printf("%d", count);
    return 0;
}
