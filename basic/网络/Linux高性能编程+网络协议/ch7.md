### Linux服务器程序规范

#### 7.1 日志

##### Linux系统日志
- syslogd和rsyslogd
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src= img/2021-05-12-14-52-50.png
width=500px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

- syslog函数
```c
#include<syslog.h>
void syslog(int priority,const char* message,...);

#define LOG_EMERG 0 //系统不可用
#define LOG_ALERT 1
#define LOG_CRIT 2
...


void openlog(const char* ident, int logopt,int facility);

int setlogmask(int maskpri);

viod closelog();
```


#### 7.2 用户信息

##### 7.2.1  UID,EUID,GID,EGID

```c
#include <sys/types.h>
#inculde <unistd.h>

uid_t getuid();
uid_t geteuid()//有效用户id
gid_t getegid()//真实组id
gid_t getegid()//有效组id

int setuid(uid_t uid) //设置真实用户id
int seteuid(uid_t euid) //设置真实用户id
```


##### 7.2.2  切换用户



#### 7.3 进程间关系
 
##### 7.3.1 进程组
- 每个进程除了有一个PID信息之外，还有一个进程组ID PGID
- 每个进程组都有个首领进程，其pgid和pid相同
```c
#include<unistd.h>
pid_t getpgid(pid_t pid); //获取指定进程的pgid

int setpgid(pid_t pid,pid_t pgid);
```

##### 7.3.2 会话
- 一些有关联的进程组形成一个会话(session)。
```c
include<unistd.h>
pid_t setsid(void);

pid_t getsid(pid_t pid);
```

##### 7.3.3 ps命令

- `ps -o pid,ppid,pgid,sid,comm|less`
- ppid表示父进程
```c
    PID    PPID    PGID     SID COMMAND
   2868    2792    2868    2868 bash
  39636    2868   39636    2868 ps
  39637    2868   39636    2868 less
```
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-12-15-46-56.png
width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

#### 7.4 系统资源限制

- 物理资源限制，系统策略限制，具体实现限制（文件长度等）
- resource 参数指定资源限制类型
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-12-16-14-28.png
width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

```c
include<sys/resource.h>
int getrlimit(int resource,struct rlimit *rlim);
int getrlimit(int resource,const struct rlimit *rlim);


struct rilimit{
    rlim_t rlim_cur;
    rlim_t rlim_max;
}
```

#### 7.5 改变工作目录和根目录
```c
#include<unistd.h>
char* getcwd(char* buf,size_t size); //获取进程当前目录 buf用于存储分配好的地址 如果buf为NULL则函数返回的动态内存必须要我们释放
int chdir(const char* path); //改变进程工作目录

int chroot(const char* path);//path参数指定要切换到的目标根目录
```

#### 7.6 服务器程序后台化
- 守护进程
- 代码 
```c
#include<unistd.h>
int daemon(int nochdir,int noclose);//nochdir是否改变工作目录，
```

