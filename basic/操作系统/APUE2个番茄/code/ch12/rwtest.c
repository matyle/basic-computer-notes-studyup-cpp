#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
int cnt = 0;
pthread_rwlock_t rwlock;
void* fn1() //读
{
    pthread_rwlock_rdlock(&rwlock);
    printf("fn1() cnt : %d\n",cnt);
    pthread_rwlock_unlock(&rwlock);
    printf("fn1() rd unlock\n");
    //pthread_exit(NULL);
    return NULL;

}

void* fn2() //读
{
    sleep(5);
    printf("fn2() wait rdlock\n");
    pthread_rwlock_rdlock(&rwlock);
    printf("fn2() cnt : %d\n",cnt);
    pthread_rwlock_unlock(&rwlock);
    //pthread_exit(NULL);
    return NULL;

}


int main()
{
    // pthread_spinlock_t splock;
    // pthread_barrier_t barrier;

    pthread_t tid1,tid2;
    pthread_rwlock_init(&rwlock,NULL);

    pthread_create(&tid1,NULL,fn1,NULL); //这是几个过程，很有可能tid还没赋值

    pthread_create(&tid2,NULL,fn2,NULL); //tid
    sleep(2);
    pthread_rwlock_wrlock(&rwlock); //写锁阻塞
        cnt++;
        printf("write lock\n");
        sleep(10);
    //pthread_rwlock_unlock(&rwlock);
    printf("write unlock\n");
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    
    pthread_rwlock_destroy(&rwlock);

    //后面的读锁必须 等待写锁。否则会造成饥饿
}