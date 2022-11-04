### shell脚本调试

1. 使用echo
2. -x 参数观察脚本运行情况
3. bashdb调试软件



## shell内建命令

内建命令即bash自身提供的命令，而不是文件系统的某个可执行文件

```sh
matytan@ubuntu:/sys/devices/virtual$ which cd
可以发现more是一个可执行文件，在bin目录
matytan@ubuntu:/sys/devices/virtual$ which more
/usr/bin/more
```



1. 确定内建命令：type

   ```shell
   matytan@ubuntu:/sys/devices/virtual$ type cd
   cd is a shell builtin
   matytan@ubuntu:/sys/devices/virtual$ type more
   more is /usr/bin/more
   ```

   

2. 执行程序 "." 点号

3. 别名：alias

   创建命令的别名

4. 删除别名:

5. 任务前后台切换 bg,fg,jobs

   1. 后台运行 cltr+z ,fg,&符号使用

   2. Cltr+z使前台程序暂停

   3. jobs显示暂停的任务

   4. bg 将暂停的程序放置后台

   5. fg将后台程序调至前台

      ```sh
      matytan@ubuntu:~$ tar -zcf usr.tgz /usr
      tar: Removing leading `/' from member names
      ^Z ctrl+z快捷键
      [1]+  Stopped                 tar -zcf usr.tgz /usr
      matytan@ubuntu:~$ jobs 查看暂停任务
      [1]+  Stopped                 tar -zcf usr.tgz /usr
      matytan@ubuntu:~$ bg 1
      [1]+ tar -zcf usr.tgz /usr &
      matytan@ubuntu:~$ fg 1
      
      ```

      

6. cd

   ### 声明变量 declear ,typeset

   - $符号获取变量
   - -i 声明整型变量
   - -r 只读
   - -a声明数组
   - -F显示脚本中定义的函数
   - -f显示脚本中定义的函数体

   ```sh
   matytan@ubuntu:~$ declare -r ro=100
   matytan@ubuntu:~$ ro=200
   -bash: ro: readonly variable
   matytan@ubuntu:~$ echo $ro  
   100
   
   matytan@ubuntu:~$ declare -a arr='([0]='a' [1]='b')'
   matytan@ubuntu:~$ echo $arr
   a
   matytan@ubuntu:~$ echo ${arr[1]}
   b
   matyta
   
   
   matytan@ubuntu:~$ ./fun.sh
   declare -F：
   declare -f func_1
   declare -f func_2
   declare -f
   func_1 () 
   { 
       echo "func 1"
   }
   func_2 () 
   { 
       echo "func 2"
   }
   ```


打印字符 echo

- 默认因此-e参数（禁止解释打印反斜杠转义字符）

跳出循环break

- break
- break2

```shell
#!/bin/bash
for I in A B C D
do
        echo -n "$I:"
        #$符号表示变量
        for J in $(seq 10)
        do
                if [ $J -eq 5 ]; # 注意两端的空格！！！！！
                then
                break
                fi
                echo -n "$J"
        done
        echo
done
echo


matytan@ubuntu:~/shell$ bash break_01.sh 
A:1234
B:1234
C:1234
D:1234
```



循环控制 continue







11. `eval `将所跟的参数作为shell的输入，并执行命令

    ```sh
    matytan@ubuntu:~/shell$ cmd="ls -l /etc/passwd"
    matytan@ubuntu:~/shell$ eval $cmd
    -rw-r--r-- 1 root root 2975 Jul 29 14:48 /etc/passwd
    ```

12. `exec`执行新的程序替代shell
13. 退出shell exit

14. export变量