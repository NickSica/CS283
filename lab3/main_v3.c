#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

volatile unsigned int count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *counter()
{
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < 1000; i++)
    {
	count++;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int v1()
{
    pthread_t tid[100];
    for(int i = 0; i < 100; i++)
	pthread_create(&tid[i], NULL, counter, NULL);

    for(int i = 0; i < 100; i++)
	pthread_join(tid[i], NULL);

    printf("%d\n", count);
    return 0;
}

int main()
{
    struct timespec before_ts;
    struct timespec after_ts;
    clock_gettime(CLOCK_REALTIME, &before_ts);
    for(int i = 0; i < 9; i++)
    {
	count = 0;
	v1();
    }
    clock_gettime(CLOCK_REALTIME, &after_ts);

//    float average = (float)(t - time(NULL));
//    average = average / (float)10;
    time_t avg_sec = after_ts.tv_sec - before_ts.tv_sec;
    time_t avg_msec = round((after_ts.tv_nsec - before_ts.tv_nsec) / 1.0e6);
    
    printf("Average in milliseconds: %lu\n", avg_msec);
//    printf("Average Time: %f", average);
}







