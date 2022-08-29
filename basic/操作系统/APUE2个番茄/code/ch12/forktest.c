#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t mutexA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexB = PTHREAD_MUTEX_INITIALIZER;

void* func(void* arg)
{
    pthread_mutex_lock(&mutexA);
    printf("thread 1 lock mutexA\n");
    sleep(5);
    printf("thread 1\n");
    pthread_mutex_unlock(&mutexA);
    printf("thread 1 unlock mutexA\n");

    return NULL;
}
void prepare()
{
    printf("prepare lock mutexA\n");
    pthread_mutex_lock(&mutexA);
    
}

void parent()
{
    printf("parent unlock mutexA\n");
    pthread_mutex_unlock(&mutexA);
    
}


void child()
{
    printf("child fork unlock mutexA\n");
    pthread_mutex_unlock(&mutexA);
    
}
int main()
{
    pid_t pid;
    pthread_t tid;
    pthread_create(&tid,NULL,func,NULL);
    
    pthread_atfork(prepare,parent,child);
    sleep(2);
    printf("about to fork\n");
    pid = fork();
    if(pid==0)
    {
        //调用child
        printf("child want to get mutexA\n");
        pthread_mutex_lock(&mutexA);
        printf("child get mutexA\n");
        pthread_mutex_unlock(&mutexA);
        printf("child unlock mutexA\n");
        //exit(0);
    }
    else
    {
        //调用parent
        printf("parent return\n");
    }
    
    // pthread_mutex_unlock(&mutexA);
    // printf("parent return\n");
    exit(0);
}