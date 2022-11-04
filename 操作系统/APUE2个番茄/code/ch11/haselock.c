#include <apue.h>
#include <pthread.h>

// 散列表
#define HASHID 29
#define Hash(id) ((unsigned long)id)%HASHID)

struct foo* fh[HASHID];

pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;


//foo对象定义
struct foo
{
    int f_id;
    int f_count;
    pthread_mutex_t f_lock;
    struct foo *f_next;
    //...


};

//给分配对象内存 建立哈希散列表
struct foo* alloc_foo(int id)
{
    

}

//foo_hold


//foo_rele
