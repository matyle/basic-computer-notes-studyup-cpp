## 管道

- 管道是一种使用频繁的通信机制` |`

- 可以把一个命令的输出当做下一个命令的输入

- 类似于重定向`>>` ，可以将输出重定向到一个文本，还记得`tail`吗？

  ```sh
  ls -l /etc/init.d | more  //将ls的输出作为more的输入，就可以一页一页的显示了
  ```

  

## grep搜索文本

- grep是强大的**基于行** 文本搜索工具
- 与管道配合

```sh
grep [-ivnc] '需要匹配的字符' 文件名
-i 不区分大小写
-c 统计匹配的字符数
-n 输出行序号
-v 反向匹配 就是找不带的
matytan@ubuntu:~$ netstat | grep ssh 
指令解释: 将netstat的输出 作为grep指令的输入
tcp        0      0 ubuntu-linux.shared:ssh 10.211.55.2:51460       ESTABLISHED
tcp        0      0 ubuntu-linux.shared:ssh 10.211.55.2:56215       ESTABLISHED

matytan@ubuntu:~$ grep -i good test 打印带匹配字符的行
apple find good
good 

matytan@ubuntu:~$ grep -c  good test
2
matytan@ubuntu:~$ grep -n  good test 带行号
1:apple find good
3:good 
反向匹配
matytan@ubuntu:~$ grep -n -v  good test
2:deep fee free 
4:signal double
```



## sort排序

- 实际是对 **行排序** 

```sh
sort [-ntkr] 文件名
-n 数字排序
-t 指定分隔符
-k 指定第几列
-r 反向排序

matytan@ubuntu:~$ sort test
apple find good
bind funcition
deep fee free 
design
good 
signal double

matytan@ubuntu:~$ sort -r test
signal double
good 
deep fee free 
apple find good
```

## uniq删除重复内容

```sh
uniq [-ic]
-i 忽略大小写
-c 计算重复行数

uniq只对比相邻行，因此我们需要先排序
matytan@ubuntu:~$ cat uniq.txt | uniq
abc
123
abc
123

matytan@ubuntu:~$ cat uniq.txt | sort | uniq
123
abc

matytan@ubuntu:~$ cat uniq.txt | sort | uniq -c
      2 123
      2 abc
```





## cut截取文本

- 处理的对象是一行文本

  ```she
  cut -f 指定的列 -d '分隔符'
  
  打印系统所有用户
  
  cat /etc/passwd | cut -f 1 -d ':'
  打印系统所有用户登录的shell
  cat /etc/passwd | cut -f 1,6-7 -d ':'
  matytan@ubuntu:~$ cat /etc/passwd | cut -f 1,6-7 -d ':'
  root:/root:/bin/bash
  daemon:/usr/sbin:/usr/sbin/nologin
  bin:/bin:/usr/sbin/nologin
  sys:/dev:/usr/sbin/nologin
  ```

  

## tr文本转换

- 小写转大写

- 删除字符

  ```sh
  cat /etc/passwd | tr '[a-z]' '[A-Z]'
  matytan@ubuntu:~$ cat /etc/passwd | tr '[a-z]' '[A-Z]'
  ROOT:X:0:0:ROOT:/ROOT:/BIN/BASH
  DAEMON:X:1:1:DAEMON:/USR/SBIN:/USR/SBIN/NOLOGIN
  BIN:X:2:2:BIN:/BIN:/USR/SBIN/NOLOGIN
  
  matytan@ubuntu:~$ cat /etc/passwd | tr -d ':'
  rootx00root/root/bin/bash
  daemonx11daemon/usr/sbin/usr/sbin/nologin
  binx22bin/bin/usr/sbin/nologin
  sysx33sys/dev/usr/sbin/nologin
  ```

  

## paste文本合并



## split分割大文件

- 只能分割文本文件，按行分割
- 二进制文件没有行的概念，无法分割

