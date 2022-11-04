#include "csapp.h"
#include "memlib.c"
#define WSIZE 4 //字大小
#define DSIZE 8 //双字
#define CHUNKSIZE (1<<12) //初始化堆大小
#define OVERHEAD 8 //头和脚部开销

#define MAX(x,y) ((x)>(y) ? (x) : (y));

#define PACK(size,alloc) ((size) | (alloc)) //并集

//读写地址p的元素
#define GET(p)  (*(size_t *)(p))

#define PUT(p,val) (*(size_t *)(p) = (val))

//从地址p读取大小和分配位
#define GETSIZE(p) (GET(p) & ~0x7) //把后三位置0 剩下的就是头部大小
#define GET_ALLOC(p) (GET(p) & 0x1) //读取是否分配位

//通过块指针bp 计算他的头部和脚部地址
#define HDRP(bp) ((char*)(bp) - WSIZE) //bp是指向当前块的有效载荷的第一个字节
#define FTRP(bp) ((char*)(bp) + GET_SIZE(HDRP(bp))-DSIZE)

//通过bp 计算下一个块的地址和前一个块的地址

#define NEXT_BLKP(bp) ((char*)(bp) + GET_SIZE(((char*)(bp)-WSIZE)))//bp-wsize 获得头部的地址

#define PREV_BLKP(bp) ((char*)(bp) - GET_SIZE(((char*)(bp)-DSIZE)))//bp-dsize  获取尾部地址

//扩展堆
static void *extend_heap(size_t words){
    char* bp;
    size_t size;

    //分配偶数个字双字对齐
    size  = (words % 2)? (words+1)*WSIZE : words * WSIZE;
    if((int)(bp = mem_sbrk(size))<0) return NULL;

    PUT(HDRP(bp),PACK(size,0)); //空闲块块头部

    //初始化空闲块头部和脚部和序言块
    PUT(HDRP(bp),PACK(size,0));
    PUT(FTRP(bp),PACK(size,0)); //空闲脚部
    PUT(NEXT_BLKP(bp),PACK(0,1));

    //如果前一个块空闲 就合并块
    return coalesce(bp);

}



//创建初始空闲链表 heap_listp指向序言块 初始化后+DSIZE
static char* heap_listp;
int mm_init(void){
    //创建一个初始空堆
    if((heap_listp = mem_sbrk(4*WSIZE))==NULL){
        return -1;
    }

    PUT(heap_listp, 0);//对齐
    PUT(heap_listp+WSIZE, PACK(OVERHEAD,1));//序言块
    PUT(heap_listp+DSIZE,PACK(OVERHEAD,1));//序言块脚部
    PUT(heap_listp+WSIZE+DSIZE,PACK(0,1));//结尾块头部
    heap_listp += DSIZE; //全局堆链表指针 8 字节 指向序言块第一个有效字节（也就是脚部）

    if(extend_heap(CHUNKSIZE/WSIZE)==NULL) return -1;

    return 0;

}

//释放和合并块，利用边界标记方法合并块
void mm_free(void *bp){
    size_t size = GETSIZE(HDRP(bp));

    PUT(HDRP(bp),PACK(size,0));
    PUT(FTRP(bp),PACK(size,0));//标记为空闲

    coalesce(bp);//合并块
}

static void* coalesce(void *bp){ //脚部在前面计算前一个块有重要作用！！！
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));//上一个块是否分配
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GETSIZE(HDRP(bp));//当前块大小

    //四种情况
    if(prev_alloc&&next_alloc){
        return bp;
    }
    else if(prev_alloc&&!next_alloc){
        //情况2 前一块已分配，后一块空闲
        size += GETSIZE(HDRP(NEXT_BLKP(bp)));//把后一块的大小加到当前块
        PUT(HDRP(bp),PACK(size,0));
        PUT(FTRP(bp),(size,0)); //修改脚部，因为计算脚部时，已经算了整个新的大小了。。
        return bp;
    }
    else if(!prev_alloc&&next_alloc){
        //情况3
        size += GETSIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp),(size,0));
        PUT(HDRP(PREV_BLKP(bp)),(size,0));
        return(PREV_BLKP(bp));
    }
    else{
        size += GETSIZE(HDRP(NEXT_BLKP(bp)))+GETSIZE(HDRP(PREV_BLKP(bp)));

        PUT(HDRP(PREV_BLKP(bp)),(size,0));
        PUT(FTRP(NEXT_BLKP(bp)),(size,0));

        return (PREV_BLKP(bp));
    }
}



//分配块
void *mm_malloc(size_t size){
    size_t asize; //调整堆的大小
    size_t extendsize;//如果没有合适的空闲块 扩展堆
    char *bp;
    if(size<=0){
        return NULL;
    }
    //调整堆大小包括开销和对齐
    if(size<DSIZE){
        asize = DSIZE + OVERHEAD; //满足对齐和放头部和脚部
    }
    else{
        asize = DSIZE *((size+(OVERHEAD)+(DSIZE-1))/DSIZE);
        //向上取8的整数倍大小 （size+(OVERHEAD)）/DSIZE+1，然后乘以DSIZE
    }
    //搜索空闲链表第一个合适的大小
    if((bp=find_fit(asize))!=NULL){
        place(bp,asize);
        return bp;
    }
    //如果没有合适的空间在空闲链表
    extendsize = MAX(asize,CHUNKSIZE);//
    if((bp=extend_heap(extendsize/WSIZE))==NULL)
        return NULL;
    
    place(bp,asize);
    return bp;
}

static void *find_fit(size_t asize){
    //从堆的第一个指针
    char* bp = heap_listp ; //指向有效载荷第一个字节

    while (bp!=mem_max_addr)
    {
        /* code */
        if(!GET_ALLOC(HDRP(bp))&&asize <= GET_SIZE(HDRP(bp))){
            return bp;
        }
        bp = NEXT_BLKP(bp);
    }
    return NULL;
}

static void place(void *bp,size_t size){
    size_t csize = GET_SIZE(HDRP(bp))-size;
    if(csize>=(DSIZE+OVERHEAD)){ //剩下的大于等于最小块才能分割
        PUT(HDRP(bp),PACK(size,1));
        PUT(FTRP(bp),PACK(size,1));

        bp = NEXT_BLKP(bp);

        PUT(HDRP(bp),PACK(csize,0));
        PUT(FTRP(bp),PACK(csize,0));

    }
    else{
        PUT(HDRP(bp),PACK(GET_SIZE(HDRP(bp)),1));
        PUT(FTRP(bp),PACK(GET_SIZE(HDRP(bp)),1));
    }

}