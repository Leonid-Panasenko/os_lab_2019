#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void func1()
{
    pthread_mutex_lock(&lock2);
    sleep(1);
    pthread_mutex_lock(&lock1);
    printf("Func 1\n");
    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);
}

void func2()
{
    pthread_mutex_lock(&lock2);
    sleep(1);
    pthread_mutex_lock(&lock1);
    printf("Func 2\n");
    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);
}

int main()
{
    pthread_t thread1, thread2;

    if (pthread_create(&thread1, NULL, (void*)func1, NULL) != 0) {
        printf("Error: pthread_create failed!\n");
        exit(1);
    }

    if (pthread_create(&thread2, NULL, (void*)func2, NULL) != 0) {
        printf("Error: pthread_create failed!\n");
        exit(1);
    }

    if(pthread_join(thread1, NULL) != 0)
    {
        printf("Join error\n");
        exit(1);
    }

    if(pthread_join(thread2, NULL) != 0)
    {
        printf("Join error\n");
        exit(1);
    }

    return 0;
}