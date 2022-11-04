#ifndef __MYCRT_H
#define __MYCRT_H

#ifdef __cplusplus //if定义了
extern "C"{
#endif

//malloc
#ifndef NULL
#define NULL (0)
#endif
int heap_init();
void free(void* ptr);
void* malloc(unsigned size);
static int brk(void* end_data_segment);

//字符串
char* itoa(char* str,int n,int radix);
int strcmp(const char* src,const char *dst);
char *strcpy(char* dest,const char* scr);
unsigned strlen(const char *str);


//文件与IO
typedef int FILE;
#define EOF (-1)
#ifdef WIN32
#define stdin ((FILE*)(GetStdHanle(STD_INPUT_HANDLE)))
#define stdout ((FILE*)(GetStdHanle(STD_OUTPUT_HANDLE)))
#define stderr ((FILE*)(GetStdHanle(STD_ERROR_HANDLE)))
#else
#define stdin ((FILE*)0)
#define stdout ((FILE*)1)
#define stderr ((FILE*)2)

#define O_RDONLY 00
#define O_WRONLY 01
#define O_RDWR 02
#define O_CREAT 0100
#define O_TRUNC 01000
#define O_APPEND 02000
#endif


int mini_crt_io();
FILE* fopen(const char* filename,const char *mode);
int fread(void* buffer,int size,int count,FILE* stream);
int fwrite(const void* buffer,int size,int count,FILE* stream);
int fclose(FILE* fp);
int fseek(FILE* fp,int offset,int set);


//printf
int fputc(int c,FILE* stream);

int printf(const char* format,...);
int fprintf(FILE* stream,const char* format,...);
int fputs(const char *str,FILE* stream);

void do_global_ctors();
void mycrt_call_exit_routine();

//atexit
typedef void (*atexit_func_t) (void); //atexit_func_t指针类型返回值为void，参数void
int atexit(atexit_func_t func);
#ifdef __cplusplus
}
#endif



#endif //__MYCRT_H