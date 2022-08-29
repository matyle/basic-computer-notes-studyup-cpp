#### 前提说明：

这里的命令默认在apue.3e解压文件夹目录下运行。

------

#### 报告权限不够：

```c
make[1]: 进入目录“/home/cxs/code/apue.3e/lib”
make[1]: execvp: ../systype.sh: 权限不够
Makefile:6: ../Make.defines.: 没有那个文件或目录
make[1]: *** 没有规则可制作目标“../Make.defines.”。 停止。
make[1]: 离开目录“/home/cxs/code/apue.3e/lib”
make: *** [Makefile:6：all] 错误 1

123456
```

解决办法：`chmod a+x systype.sh`

```c
./fixup.awk rot13a.c >xlate
/bin/sh: 1: ./fixup.awk: Permission denied
make[1]: *** [Makefile:18：rot13c2.c] 错误 126
make[1]: 离开目录“/home/cxs/code/apue.3e/advio”
make: *** [Makefile:6：all] 错误 1

12345
```

解决办法：`chmod a+x ./advio/fixup.awk`

------

#### undefined reference to `major’

```c
/usr/bin/ld: /tmp/ccmTGRc3.o: in function `main':
devrdev.c:(.text+0xc5): undefined reference to `minor'
/usr/bin/ld: devrdev.c:(.text+0xdb): undefined reference to `major'
/usr/bin/ld: devrdev.c:(.text+0x128): undefined reference to `minor'
/usr/bin/ld: devrdev.c:(.text+0x13e): undefined reference to `major'
collect2: error: ld returned 1 exit status
make[1]: *** [Makefile:18：devrdev] 错误 1
make[1]: 离开目录“/home/cxs/code/apue.3e/filedir”
make: *** [Makefile:6：all] 错误 1

```

解决办法：添加`#include <sys/types.h> #include <sys/sysmacros.h>`

------

#### 提示 error: ‘FILE’ {aka ‘struct _IO_FILE’} has no member named ‘_flag’; did you mean ‘_flags’?

```c
gcc -ansi -I../include -Wall -DLINUX -D_GNU_SOURCE  buf.c -o buf  -L../lib -lapue 
buf.c: In function ‘is_linebuffered’:
buf.c:90:15: error: ‘FILE’ {aka ‘struct _IO_FILE’} has no member named ‘__pad’; did you mean ‘__pad5’?
   90 | #define _flag __pad[4]
      |               ^~~~~
buf.c:104:13: note: in expansion of macro ‘_flag’
  104 |  return(fp->_flag & _IOLBF);
      |             ^~~~~
buf.c:105:1: warning: control reaches end of non-void function [-Wreturn-type]
  105 | }
      | ^
make[1]: *** [Makefile:16：buf] 错误 1
make[1]: 离开目录“/home/cxs/code/apue.3e/stdio”
make: *** [Makefile:6：all] 错误 1



gcc -ansi -I../include -Wall -DLINUX -D_GNU_SOURCE  buf.c -o buf  -L../lib -lapue 
buf.c: In function ‘is_linebuffered’:
buf.c:99:13: error: ‘FILE’ {aka ‘struct _IO_FILE’} has no member named ‘_flag’; did you mean ‘_flags’?
   99 |  return(fp->_flag & _IOLBF);
      |             ^~~~~
      |             _flags
buf.c:100:1: warning: control reaches end of non-void function [-Wreturn-type]
  100 | }
      | ^
make[1]: *** [Makefile:16：buf] 错误 1
make[1]: 离开目录“/home/cxs/code/apue.3e/stdio”
make: *** [Makefile:6：all] 错误 1

123456789101112
```

解决办法：将`./stdio/buf.c`文件的

```c
#ifdef _LP64
#define _flag __pad[4]
#define _ptr __pad[1]
#define _base __pad[2]
#endif

12345
```

删掉，同时将下面的flag全部换成flags：

```c
int
is_unbuffered(FILE *fp)
{
    return(fp->_flags & _IONBF);
}

int
is_linebuffered(FILE *fp)
{
	return(fp->_flag & _IOLBF);
}

int
buffer_size(FILE *fp)
{
#ifdef _LP64
    return(fp->_IO_buf_end - fp->_IO_buf_base);
#else
	return(BUFSIZ);	/* just a guess */
#endif
}

#else

#error unknown stdio implementation!

#endif

```

------

#### 提示：/usr/bin/ld: 找不到 -lbsd

```bash
gcc -ansi -I../include -Wall -DLINUX -D_GNU_SOURCE  badexit2.c -o badexit2  -L../lib -lapue -pthread -lrt -lbsd
/usr/bin/ld: 找不到 -lbsd
collect2: error: ld returned 1 exit status
make[1]: *** [Makefile:31：badexit2] 错误 1
make[1]: 离开目录“/home/cxs/code/apue.3e/threads”
make: *** [Makefile:6：all] 错误 1

```

解决办法：`sudo apt-get install libbsd-dev`， 安装所需要的文件库







最后

```sh
matytan@ubuntu:~/study/apue.3e$ sudo cp ./include/apue.h /usr/include/
matytan@ubuntu:~/study/apue.3e$ sudo cp ./lib/libapue.a /usr/local/lib/
```

