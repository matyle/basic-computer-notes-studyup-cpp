

## linux用户和用户组

### UID和GID

linux是一个多任务分时系统

- UID
- GID

涉及指令

`id`

`groups`

`Who`

### /etc/shadow /etc/passwd



```sh
cat /etc/passwd
root:x:0:0:root:/root:/bin/bash
mysql:x:127:133:MySQL Server,,,:/nonexistent:/bin/false
matytan:x:1000:1000:Maty Tan,,,:/home/matytan:/bin/bash
....

sudo cat /etc/shadow
matytan:$6$oK5E1aCZxFjIjjUB$OQxds1eNAA47YJdGzwCzE8sFQYg0/3vvVdpE0Yxl9YIau/eef28i76.5zEtQUAsfBsOzKeqANqmgQKuGRS23d/:18734:0:99999:7:::
```



- /etc/passwd
- /etc/shadow



## linux账号管理



`useradd username` 添加用户，其中步骤

- 记录 分配UID

- 创建home目录(Ubuntu不会创建，需要加-m)

- 复制/etc/skel隐藏文件

- 创建同名用户组

  ```sh
  matytan@ubuntu:~/study$ ls -a -l /etc/skel
  total 28
  drwxr-xr-x   2 root root  4096 Feb 10 02:48 .
  drwxr-xr-x 135 root root 12288 Jul 27 06:44 ..
  -rw-r--r--   1 root root   220 Feb 25  2020 .bash_logout
  -rw-r--r--   1 root root  3771 Feb 25  2020 .bashrc
  -rw-r--r--   1 root root   807 Feb 25  2020 .profile
  
  useradd-u 555 user1 //指定uid
  useradd-g user1 user2 指定所属User1组 GID
  ```



修改密码 passwd



修改用户 usermod ：修改/etc/shadow 和/etc/passwd文件内容

```sh
usermod-d /home/maty1_new-m maty
```



- -m 表示不存在则新建目录

删除用户



### 新增和删除用户组



### 检查用户信息

检查用户信息的四个命令

`users`,`who`,`w`,`finger`



## 切换用户

`su`命令：切换用户的意思

用其他用户的身份执行命令 `sudo`

##  例行任务管理

at：单一时刻执行一次任务

atq 查看定时任务

### 周期性执行任务 cron

`service crond start`



- `crontab -e` 编辑模式

  ```sh
  * * * * * service httpd restart //每分钟重启http进程
  */1 * * * * service httpd restart //每分钟重启http进程
  * */1 * * * service httpd restart //每小时
  
  * 23-3/1 * * *  service httpd restart //23-3点 每小时重启一次
  
  30 23 1 * * //每月第一天晚上23：30重启
  
  30 23 1 1 * //每年1月1日23：30重启
  
  30 23 * * 0 //每周日23：30重启http
  ```
  
  - 第一个\*是分钟1-59
  - 第二个\*是小时0-23
  - 第三个\*是日期 1-31
  - 第四个\*是月 1-12
  - 第五个\*是星期 0-6

crontab -l查看设置任务

crontab -r删除设置任务

root用户查看其他用户的任务使用-u 如`crontab -u matytan -l`

查看crontab的内容

```sh
matytan@ubuntu:~$ cat /etc/crontab
# /etc/crontab: system-wide crontab
# Unlike any other crontab you don't have to run the `crontab'
# command to install the new version when you edit this file
# and files in /etc/cron.d. These files also have username fields,
# that none of the other crontabs do.

SHELL=/bin/sh
PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin

# Example of job definition:
# .---------------- minute (0 - 59)
# |  .------------- hour (0 - 23)
# |  |  .---------- day of month (1 - 31)
# |  |  |  .------- month (1 - 12) OR jan,feb,mar,apr ...
# |  |  |  |  .---- day of week (0 - 6) (Sunday=0 or 7) OR sun,mon,tue,wed,thu,fri,sat
# |  |  |  |  |
# *  *  *  *  * user-name command to be executed
17 *    * * *   root    cd / && run-parts --report /etc/cron.hourly
25 6    * * *   root    test -x /usr/sbin/anacron || ( cd / && run-parts --report /etc/cron.daily )
47 6    * * 7   root    test -x /usr/sbin/anacron || ( cd / && run-parts --report /etc/cron.weekly )
52 6    1 * *   root    test -x /usr/sbin/anacron || ( cd / && run-parts --report /etc/cron.monthly )
#
```





