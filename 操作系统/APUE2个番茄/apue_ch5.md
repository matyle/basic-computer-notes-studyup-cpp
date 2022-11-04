
### 标准io库

#### 5.2流和file对象
- 使用一个标准库打开或者创建一个文件时，我们已经使一个流与一个文件相关联
- 流的定向：
  - 在未定向的流上使用多字节的io函数，则试图将该流设置为宽定向
  - 在未定向的流上使用但字节的io函数，则试图将该流设置为字节定向
  - mode<0表示为字节定向，mode>0表示宽定向，若mode=0,不设置定向，返回标识该流定向的值。
  - 不改变已经定向的流

    ```c
    #include<stdio.h>
    #include<wchar.h>
    int fwide(FILE *fp,int mode);//设置流的定向
    ```

#### 5.3 标准输入，输出，出错
-STDIN_FILENO,STDOUT_FILENO,STDERR_FILENO

#### 5.4 缓冲
- 全缓冲：填满io缓冲区之后才进行实际io操作
  - flush（冲洗）将缓冲区的内容写到磁盘，在终端驱动方法，flush的意思是丢弃已存储在缓冲区的内容
- 行缓冲：在输入输出时，遇到换行符时才进行标准io库执行io操作
  - 超过缓冲区长度，即使没有换行符也会进行io操作

- 不带缓冲：

    ```c
    //更改缓冲类型
    #include<stdio.h>
    void setbuf(FILE *restrict fp,char *restrict buf);//buf为NULL则为关闭缓冲
    void setvbuf(FILE *restrict fp,char *restrict buf,int mode);
    ```
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-16-17-31-46.png
width=500px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

- 强制冲洗一个流
  ```c
  #include<stdio.h>
  int fflush(FILE *fp);
  ```

#### 5.5 打开流
```c
#include<stdio.h>
FILE *fopen(const char *restrict pathname,const char *restrict type);
FILE *freopen(const char *restrict pathname,const char *restrict type,FILE *restrict fp);
FILE *fdopen(int filedes,const char *type);
```
- 三个函数区别：
  - fopen指定打开的文件
  - freopen用于指定文件打开为一个定义的流：标准输入，输出，出错
  - fdopen获取一个现有的文件描述符：通常用于创建管道和网络通信通道函数返回的描述符
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-16-17-39-48.png
width=500px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

#### 5.6 读和写流
- 每次一个字符的io
    ```c
    #include<stdio.h>
    int getc(FILE *fp);
    int fgetc(FILE *fp);
    int getchar(void);
    ```
- 每次一行的io：fgets ，fputs
```c
#include<stdio.h>
char *fgets(char *restrict buf,int n,FILE *restrict fp);
char *gets(char *buf);//不推荐使用，不能指定长度，造成缓冲区溢出，导致返回值被修改

int fputs(char *restrict buf,FILE *restrict fp);
int puts(const char *str);
```
- 直接io：fread，fwrite


#### 标准io的效率
- 每次读一行的效果比单个字符读更快，因为循环数量更少
```c

```

#### 二进制io
- 二进制io操作，我们更愿意一次读写整个结构
```c
#include<stdio.h>
size_t fread(void *restrict ptr,size_t size,size_t nobj,FILE *restrict fp);
size_t fwrite(void *restrict ptr,size_t size,size_t nobj,FILE *restrict fp);
//nboj表示要写或者读的对象数量
//读写一个二进制数组
float data[10];
if(fwrite(&data[2],sizeof(float),4,fp)!=4)
    err_sys("fwrite error");

//读写一个结构
struct {
    short count;
    long total; //4字节
    char num[NAMESIZE];
}item;

if(fwrite(&item,sizeof(item),1,fp)!=1)
    err_sys("fwrite error");
```

#### 定位流
  - ftell和fseek函数
    - whence参数：SEEK_SET,SEEK_CUR,SEEK_END
  - ftello,fseeko
  - fgetpos,fsetpos
  ```c    
    #include<stdio.h>
    long ftell(FILE *fp);
    int fseek(FILE *fp,long offset,int whence);
    /*
    whence : SEEK_SET,SEEK_CUR,SEEK_END
    */
    void rewind(FILE *fp); //将一个流设置到文件的起始位置


    long ftello(FILE *fp);
    int fseeko(FILE *fp,off_t offset,int whence);

    long fgetpos(FILE restrict *fp,fpos_t *restrict pos);
    int fseeko(FILE *fp,cosnt fpos_t *pos);

    //restict 解决指针别名无法辨认；
    /*
  总括而言，restrict 是为了告诉编译器额外信息(两个指针不指向同一数据)，从而生成更优化的机
  器码。注意，编译器是无法自行在编译期检测两个指针是否alias。如使用restrict, 程序员也要遵
  守契约才能得出正确的代码(指针不 能指向相同数据).
  个人经验而言，编写代码时通常会忽略pointer aliasing的问题。更常见是在性能剖测时，通过反
  编译看到很多冗余的读取指令，才会想到加入restrict关键字来提升性能。
    */
    #include<stdio.h>
    int foo(int *a,int *b){ //未用restrict修饰
        *a = 5;
        *b = 6;
        return *a+*b;
    }
    int main(){
        int i = 0;
        int *a = &i;
        int *b = &i;
        printf("%d",foo(a,b));

  }
  ```

#### 格式化io
 - `%[flags][fldwidth][precision][lenmodifier]convtype`
 - [lenmodifier]
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=
  width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">
    图片描述</div>
  </center>
  
```c
#include<stdio.h>
int printf(const char* restrict format,...);
int fprintf(FILE *restrict fp,const char* restrict format,...);
int sprintf(char *restrict buf,const char* restrict format,...);
int snprintf(char *restrict buf,size_t n ,const char* restrict format,...);

//%[flags][fldwidth][precision][lenmodifier]convtype
```