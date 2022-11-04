 #include "mycrt.h"
 typedef struct _heap_header
  {
      enum{
          HEAP_BLOCK_FREE = 0xABABABAB,//空闲块的魔数
          HEAP_BLOCK_USED = 0xCDCDCDCD,//已用块的魔数
      }type;

      unsigned size; //包含头的大小 注意不是直接空闲块的大小
      struct _heap_header* next;
      struct _heap_header* prev;
  }heap_header;

  #define ADDR_ADD(a,o) (((char*)(a))+(o))// 地址加o char* 指针设置宽度为1
  #define HEADER_SIZE (sizeof(heap_header))

  static heap_header*  header_list = NULL; //静态成员


  static int brk(void* end_data_segment)
  {
      int ret =0;
      //brk系统调用45
      asm(
          "movl $45 ,%%eax \n\t"
          "movl %1, %%ebx \n\t"
          "int $0x80  \n\t"
          "movl %%eax, %0 \n\t" //把eax传入
          :"=r"(ret):"m"(end_data_segment) //保存在寄存器中的地址
      );
      return ret;
  }
  //初始化 32M的堆头 实际就一个结点
  int heap_init()
  {
      void* base = NULL;
      heap_header* header = NULL;

      //分配空间
      unsigned heap_size = 1024*1024*32;

      
      //liunx
      base = (void*)(brk(0));//起始地址 brk是建立进程数据段边界，sbrk可以移动进程的数据段边界
      void* end = ADDR_ADD(base,heap_size); //从base开始分配不是从0
      
      end = (void*)brk(end);//分配结束地址i
      if(!end) return 0;
      

      //建立链表 堆头
      header = (heap_header*)base;//

      header->size = heap_size;
      header->type = HEAP_BLOCK_FREE;
      header->next = NULL;
      header->prev = NULL;
      
      header_list = header;
      return 1;
      
  }
  //分配
  void *malloc(unsigned size)
  {
      //size比堆大小小 分块
      heap_header *header; //堆头
      if (size==0) return NULL;

      header = header_list;// 链表

      while(header!=0)
      {
          //第一种情况
          if(HEAP_BLOCK_USED==header->type) //被使用
          {
              header = header->next;
              continue;
          }
            //header->size - HEADER_SIZE为空闲块大小 大于申请块大小，但是空闲块大小又不能容纳size加一个头大小，因此不能分块
          if(header->size - HEADER_SIZE >size && header->size-HEADER_SIZE<=size + HEADER_SIZE)
          {
              header->type = HEAP_BLOCK_USED;
              return ADDR_ADD(header,HEADER_SIZE); //从头结点后面开始
          }

          //分块
          if(header->size-HEADER_SIZE > size + HEADER_SIZE)
          {
              heap_header *next = (heap_header*)ADDR_ADD(header,size + HEADER_SIZE);//下一块要占一个头和刚刚分配的空间,将这块空间转为heap_header

              next->prev = header;
              next->next = header->next;

              next->type = HEAP_BLOCK_FREE;

              next->size = header->size - (size - HEADER_SIZE);//新的空闲块大小
                //修改原来header的属性
              header->next = next;
              header->size = size + HEADER_SIZE;
              header->type = HEAP_BLOCK_USED; //header使用
              return  ADDR_ADD(header,HEADER_SIZE); //返回地址
              //
          }
          header = header->next;//大小不足
      }
      return NULL;


  }
  //释放 ptr指向的是空闲块 不是空闲块头、
  //优先排除错误条件
  void free(void* ptr)
  {
      heap_header* header = (heap_header*)ADDR_ADD(ptr,-HEADER_SIZE);
      if(header->type!=HEAP_BLOCK_USED) //如果本身是空闲的直接return
      {
          return;
      }
      header->type = HEAP_BLOCK_FREE;
      //看前后有没有空闲 合并
      if(header->prev&&(header->prev->type==HEAP_BLOCK_FREE))
      {
          //
          header->prev->size  += header->size;
          //处理head的prev和next
          if(header->next)
          {
            header->prev->next = header->next;
            header->next->prev = header->prev; //指向header prev
          }
          header = header->prev;
      }
      //header->next不空
      if((header->next!=NULL)&&(header->next->type==HEAP_BLOCK_FREE))
      {
          header->size += header->next->size;
          //处理header信息

          
          header->next = header->next->next;
          if(header->next)
          {
              header->next->prev = header;
          }
          
      }

  }