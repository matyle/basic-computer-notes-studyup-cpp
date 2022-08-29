## 文件和目录管理

> ls 命令
>
> -l 显示所有属性
>
> -a 显示所有文件

```sh
matytan@ubuntu:/$ ls -l
bin   cdrom  etc   lib    lib64   lost+found  mnt  proc  run   snap  swapfile  tmp  var
boot  dev    home  lib32  libx32  media       opt  root  sbin  srv   sys       usr

lrwxrwxrwx   1(链接数) root root          7 Apr 17 14:39 bin -> usr/bin 常见用户指令
lrwxrwxrwx   1 root root          7 Apr 17 14:39 lib -> usr/lib
lrwxrwxrwx   1 root root          9 Apr 17 14:39 lib32 -> usr/lib32
lrwxrwxrwx   1 root root          9 Apr 17 14:39 lib64 -> usr/lib64
lrwxrwxrwx   1 root root         10 Apr 17 14:39 libx32 -> usr/libx32
drwxr-xr-x  34 root root       1080 Jul 30 12:41 run
lrwxrwxrwx   1 root root          8 Apr 17 14:39 sbin -> usr/sbin
drwxr-xr-x  14 root root       4096 Feb 10 02:48 usr
drwxr-xr-x  14 root root       4096 Feb 10 02:56 var
```



> 当前目录` pwd`

```sh
matytan@ubuntu:~$ pwd
/home/matytan
```

- 绝对路径：以`/`开头的路径
- 相对路径：相对于当前目录的路径 `./`
- 特殊目录 `.`和`..`

### 文件的相关操作

> 创建文件：`touch`

touch已经存在的文件会更新之前文件的时间戳，不会修改其内容

> 删除文件：`rm`

`rm test.txt`



> 移动或者重命名：`mv`

```sh
mv test /tmp //移动到了tmp文件夹

mv test test2 //重命名
```



> 查看文件：`cat`,concatenate的简写

`cat test -n `可以显示行号 等价于nl

> 倒序查看文件` tac `

> 查看文件头：`head`默认10 可加参数n控制行数

> 查看文件尾：`tail`默认10行 可加参数n控制行数

`tail`==更实用的功能是动态查看文件尾== 参数 `-f`



为什么：vim编辑保存之后不能实时显示？

原因是：vim编辑保存一个文件之后，vim创建了一个新的文件，旧文件被删除，tail就无法跟踪
在tail执行的过程中，跟踪的是t.txt

可以使用重定向：`echo new >>test`

7. 文件格式转换dos2unix 将DOS格式的文本文件转变成unix下的文本文件



> more可翻页查看命令

> less 可上下翻页，同时可以使用 / 和？ 查找字符串

### 目录的相关操作

> 进入目录 `cd`,当前目录`pwd`

> 创建目录 `mkdir`,创建文件`touch`

重要参数：`-p `一次性创建所有目录 `mkdir -p test/test` 递归创建

用绝对路径创建 `mkdir -p /root/home/mat/test`

> 删除时 `rm -r test`

3. 文件和目录复制 `cp`第一个参数是源文件/目录，第二个参数是目标目录或者复制后的文件名
   1. 复制目录加` -r`

### 文件时间戳

可以touch一个目录，因为目录本身就是一个文件，touch会更新时间戳，而不会修改其内容

```sh
mkdir touch_dir1
touch touch_file1
//

两分钟后touch
touch touch_file1,touch_dir1
```



## 文件目录的权限

### 查看文件或目录的权限

- `ls -al`
  - 第一列 是文件类型和权限 d(目录) rwxr-xr-x l开头表示是链接
  - 第二列 链接数 

```sh
total 2097248
drwxr-xr-x  20 root root       4096 Apr 17 14:41 .
drwxr-xr-x  20 root root       4096 Apr 17 14:41 ..
lrwxrwxrwx   1 root root          7 Apr 17 14:39 bin -> usr/bin
drwxr-xr-x   4 root root       4096 Jul 24 09:56 boot
drwxr-xr-x  20 root root       4280 Jul 31 08:38 dev
drwxr-xr-x 135 root root      12288 Jul 31 08:38 etc
dr-xr-xr-x  13 root root          0 Jul 31 08:38 sys
drwxrwxrwt  18 root root      12288 Jul 31 15:07 tmp
drwxr-xr-x  14 root root       4096 Feb 10 02:48 usr
drwxr-xr-x  14 root root       4096 Feb 10 02:56 var
```

### 文件隐藏属性

`lsattr`查看隐藏属性

`chattr`设置文件隐藏属性

```sh
matytan@ubuntu:~$ lsattr
--------------e----- ./snap
--------------e----- ./touch_file1
--------------e----- ./study
```



### 修改文件权限

> `chmod`指令

文件拥有者 user (u)

拥有组 group (g)

其他人 other (o)

`r =4, w=2, x=1` rwx （421）

754 就代表 rwx（拥有者)  r-x（拥有组权限） r(其他用户只读)

```sh
给文件添加用户执行权限 chmod u+x file
给文件移除用户执行权限 chmod u-x file

给文件添加用户读权限 chmod u+r file
给文件移除用户读权限 chmod u-r file

拥有组是g，其他用户是o
```



> 改变文件拥有者`chown`

> 改变文件所有组`chgrp`







### 特殊属性

`SUID`：用户组有个s，说明有SUID 普通用户可以使用root身份执行这个命令

该命令只能用于 ==二进制文件==

添加SUID权限方法 `chmod u+s filename`

```sh
-rwsr-xr-x 1 root root 68208 May 28  2020 /usr/bin/passwd*

```



`GUID `设置了s权限之后，文件的用户组中所有用户都能以该文件的用户身份运行命令

 `chmod g+s filename`



`Sticky` 其他用户权限(t)设置在目录（只能设置在目录）之后：该目录任何用户可以修改创建文件，但是只有文件创建者和root可以删除此文件。

例如/tmp/文件目录 ,other其他权限是`t`

权限添加 `chmod o + t dirname`

```sh
root@ubuntu:~# ll -d /tmp
drwxrwxrwt 19 root root 12288 Aug  1 10:35 /tmp/
```

### 默认权限和umask

`r =4, w=2, x=1`

```sh
root用户创建
root@ubuntu:/tmp# ll fli1 
-rw-r--r-- 1 root root 0 Aug  1 11:24 fli1 权限644

root@ubuntu:/tmp# ll -d dir1/
drwxr-xr-x 2 root root 4096 Aug  1 11:25 dir1// 权限755


普通用户创建 文件默认权限664 目录权限775
```



- umask ：遮罩 
  - linux下目录默认权限值是umask遮罩777后的权限
  - 文件默认权限值是umask遮罩666的权限





文件默认权限 666  rw-rw-rw-

目录默认权限值777 rwxrwxrwx 

root和普通用户分别用022和002 umask





### 查看文件类型 file

```sh
root@ubuntu:/tmp# file /root
/root: directory
```





文件操作2

## 查找文件

### 一般查找 find

`find PATH -name FILENAME`

```sh
root@ubuntu:/tmp# find / -name httpd.conf
find: ‘/run/user/1000/gvfs’: Permission denied
find: ‘/run/user/125/gvfs’: Permission denied

root@ubuntu:/tmp# find /etc -name httpd.conf
```



模糊匹配\* `find /etc -name *.conf`

```sh
root@ubuntu:/tmp# find /etc -name *.conf
/etc/sysctl.d/10-console-messages.conf
/etc/sysctl.d/99-sysctl.conf
/etc/sysctl.d/10-kernel-hardening.conf
/etc/sysctl.d/10-network-security.conf
/etc/sysctl.d/10-ptrace.conf
/etc/sysctl.d/10-magic-sysrq.conf
```





### 数据库查找 locate

```shell
root@ubuntu:~# find / -name test_local
find: ‘/run/user/1000/gvfs’: Permission denied
find: ‘/run/user/125/gvfs’: Permission denied
/root/test_local
locate test_local
matytan@ubuntu:~$ locate test_locate
/home/matytan/test_locate

updatedb


```

`locate`

`updatedb`



### which/whereis

`which`用于从PATH变量定义的目录中查找可执行文件的绝对路径



`Whereis` 不但能找到二进制文件，也能找到相关man文件







## 文件压缩和打包

- `gzip/gunzip`

  gzip/gunzip是用来压缩和解压缩单个文件的工具





- `tar`命令

打包文件：`tar -zcvf boot.tgz /boot`

z代表用gzip压缩，c为创建文件，v显示当前被压缩的文件，f使用文件名 boot.tgz



解压文件` tar -zxvf boot boot.tgz-C /tmp`



- `cpio`指令

```sh
matytan@ubuntu:~$ sudo find /etc -name *.conf | cpio -cov > /tmp/conf.cpio
/etc/sysctl.d/10-console-messages.conf
/etc/sysctl.d/99-sysctl.conf

```

