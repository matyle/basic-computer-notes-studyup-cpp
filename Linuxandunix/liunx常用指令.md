## 常见命令



### tail命令

```sh
tail -n 20 filename
说明：显示filename最后20行。

Linux下tail命令的使用方法。
linux tail命令用途是依照要求将指定的文件的最后部分输出到标准设备，通常是终端，通俗讲来，就是把某个档案文件的最后几行显示到终端上，假设该档案有更新，tail会自己主动刷新，确保你看到最新的档案内容。
一、tail命令语法
```





### head

```sh
head -n 20 filename
head - output the first part of files


-c, --bytes=[-]NUM
print the first NUM bytes of each file; with the leading '-', print all but the last NUM bytes of each file

-n, --lines=[-]NUM
print the first NUM lines instead of the first 10; with the leading '-', print all but the last NUM lines of each file

-q, --quiet, --silent
never print headers giving file names

-v, --verbose
always print headers giving file names

-z, --zero-terminated
line delimiter is NUL, not newline
```





### 显示文件的其他命令

```
cat 从第一行开始显示档案内容。
tac 从最后一行开始显示档案内容。
more 分页显示档案内容。
less 与 more 相似，但支持向前翻页
head 仅仅显示前面几行
tail 仅仅显示后面几行
n 带行号显示档案内容
od 以二进制方式显示档案内容
```





### 关闭图形界面

```sh
sudo systemctl isolate multi-user.target
sudo systemctl set-default multi-user.target
```





### netstat 查看网络

```sh
netstat -ntlp   //查看当前所有tcp端口·
netstat -ntulp |grep 80   //查看所有80端口使用情况·
netstat -an | grep 3306   //查看所有3306端口使用情况·
netstat命令参数：
　　-t : 指明显示TCP端口
　　-u : 指明显示UDP端口
　　-l : 仅显示监听套接字(所谓套接字就是使应用程序能够读写与收发通讯协议(protocol)与资料的程序)
　　-p : 显示进程标识符和程序名称，每一个套接字/端口都属于一个程序。
　　-n : 不进行DNS轮询，显示IP(可以加速操作)
```





### 文件权限

r ---4

w---2

x----1 

777 即 所有者rwx，同组用户rwx，其他用户rwx

755 即 所有者rwx，同组用户r_x，其他用户r_x

644 所有者rx_ 同组用户r__ ，其他用户r__
