## linux常用



centos 7版本

认识linux

基本命令（重点）可以跟着书 

软件安装和部署





LAMP

LNMP ： linux Nginx Mysql php

### 基本命令



```shell
shutdown -h now
shutdown -h 5

重启
shutdown -r now
reboot
halt
```





>系统目录结构
>
>1. 一切皆文件
>   - **/bin **  binary的缩写 存经常使用命令
>   - **/boot** 核心启动文件 连接文件 镜像文件
>   - /etc :这个目录用来存放所有的系统管理所需要的配置文件和子目录。
>   - /dev : dev是Device(设备)的缩写, 存放的是Linux的外部设备，在Linux中访问设备的方式和访问文件的方式是相同的。
>   - /home
>   - /lib  动态链接库
>   - /lost+found  非法关机后存放
>   - /media
>   - /mnt
>   - **/opt** 给主机额外安装所存放的目录
>   - /proc
>   - /root
>   - **/sbin** ：super user
>   - /srv
>   - /sys
>   - /tmp
>   - **/usr** 存放用户的应用程序和文件 类似于Windows的 program files
>   - /usr/bin 系统用户使用的程序
>   - /usr/src : 内核源代码默认存放位置
>   - /var ：存放不断扩充的东西，将经常修改的目录放在这个目录下，录入日志
>   - /run  存储系统启动以来的信息，重启时这个目录的内容会被删掉和清除
>   - /www



>LNMP  linux + ngnix + mysql + php
>
>LAMP
>
>
>
>





>mkdir
>
>参数： -p 创建多级目录
>
>rmdir -p 删除多级目录

> cp 原来的地方 新的地方



>rm

-f 强制删除 不会出现警告

-r  递归删除

-i 互动，询问是否删除

>mv 移动文件或者目录 或者重命名文件

-f 强制

-u





### 目录属性

chgrp

chown

chmod



rwx ： 4 2 1



### 文件内容查看

> cat
>
> tac



>more 空格翻页



>more
>
>less (/可以查询向下，？向上查询) 文件中查询





>ln 命令

硬链接 ：（和拷贝的区别） 硬链接文件占空间吗？ 指向一个inode

软连接



> touch
>
> echo >> 





### vim



Vimtutor



删除类

x 按字符删

d 

dd

dw



motion



> set nu #行号

> set nonu



### 账号管理

useradd:

>useradd -m matytan

本质：linux一切皆文件，即向文件中添加信息

`/etc/passwd`

参数说明：


> 删除用户：
>
> `userdel -r matytan`

> 修改用户：
>
> `usermod`
>
> 选项：
>   -c, --comment 注释            GECOS 字段的新值
>   -d, --home HOME_DIR           用户的新主目录
>   -e, --expiredate EXPIRE_DATE  设定帐户过期的日期为 EXPIRE_DATE
>   -f, --inactive INACTIVE       过期 INACTIVE 天数后，设定密码为失效状态
>   -g, --gid GROUP               强制使用 GROUP 为新主组
>   -G, --groups GROUPS           新的附加组列表 GROUPS
>   -a, --append GROUP            将用户追加至上边 -G 中提到的附加组中，
>                                 并不从其它组中删除此用户
>   -h, --help                    显示此帮助信息并推出
>   -l, --login LOGIN             新的登录名称
>   -L, --lock                    锁定用户帐号
>   -m, --move-home               将家目录内容移至新位置 (仅于 -d 一起使用)
>   -o, --non-unique              允许使用重复的(非唯一的) UID
>   -p, --password PASSWORD       将加密过的密码 (PASSWORD) 设为新密码
>   -R, --root CHROOT_DIR         chroot 到的目录
>   -P, --prefix PREFIX_DIR       prefix directory where are located the /etc/* files
>   -s, --shell SHELL             该用户帐号的新登录 shell
>   -u, --uid UID                 用户帐号的新 UID
>   -U, --unlock                  解锁用户帐号
>   -v, --add-subuids FIRST-LAST  add range of subordinate uids
>   -V, --del-subuids FIRST-LAST  remove range of subordinate uids
>   -w, --add-subgids FIRST-LAST  add range of subordinate gids
>   -W, --del-subgids FIRST-LAST  remove range of subordinate gids
>   -Z, --selinux-user  SEUSER       用户账户的新 SELinux 用户映射



> 切换用户：
>
> `su matytan`



> 修改用户密码：
>
> `passwd username`

冻结账号

> `passwd -l 用户名`
>
> `passwd -d 用户名 清密码`





### 用户组管理

/etc/group



### 磁盘管理

> df -h 按G查看 
>
> du  查看空间使用
>
> du -a 
>
> du -h
>
> Du -sm /*



> 挂载
>
> mount dev/u1 /mnt/u2
>
> unmount 



### 进程管理

> ps	

-a 显示终端所有信息

-u 用户的信息显示进程

-x 显示后台运行进程的参数

Ps -aux | grep redis

可以查看到父进程信息

Ps -ef | grep mysql

pstree -p 显示父id -u 显示用户组

> 结束进程
>
> kill -9 进程id

