---
title: dockerfile详解
date: 2021-12-4 20:08:17
tags: docker
categories: 容器
keywords: docker
description: 
cover: https://gitee.com/matytan/tupic/raw/master/uPic/69358fdd0b7653e1268a26dd83168483.jpeg
---
## docker镜像

带代码，运行库，环境变量和配置文件的软件包



- 联合文件系统
- 镜像加载原理
  - bootfs
  - `rootfs`
- 镜像分层原理
  - `docker image inspect`



> `docker commit`
>
> 可选参数：
>
> -m=" 描述信息"
>
> -a="作者"



`cp -r 要拷贝的路径 目标路径`


理解概念，但是一定要实践，最后实践和理论结合

## dockerfile

### 初识dockerfile

dockerfile 构建docker镜像文件

```dockerfile
FROM centos
VOLUME []
CMD
```

### dockerfile指令

1. 关键字指令必须是大写字母
2. 从上到下执行
3. 注释#
4. 每一个指令都会创建一层

>dockerfile：构建文件，定义步骤
>
>dockerimages：通过dockerfile构建的镜像，最终发布的产品
>
>docker容器：镜像运行起来提供-al的服务器

```dockerflie
FROM       								# 基础镜像
MAINTAINER（maintainer）  #谁写的 姓名+邮箱
RUN 									# 构建时候需要运行的命令
ADD  file名 /usr/local								# COPY文件 添加内容自动解压 比如添加tomcat
WORKDIR     					# 工作目录
VOLUME								# 设置挂载的容器卷 挂载主机目录
EXPOSE								# 指定暴露端口
RUN										# 指定容器启动的工作
CMD 									# 指定容器启动时执行的命令 （只有最后一个会生效)

ENTERPOINT						# 指定容器启动时执行的命令，可追加

ONBUILD  							# 当构建一个被继承Dockerfile时候会运行ONBUILD指令
COPY									# 拷贝文件
ENV										# 设置环境变量
```



```dockerfile
FROM centos

WORKDIR MYPATH /usr/local
EXPOSE 80 1280
VOLUME ./data
RUN yum install -y vim
RUN yum install -y net-tools
CMD echo ---END---
CMD /bin/bash
```

```sh
docker build -f mydockerfile -t mycentos:0.1 .  # 
docker build -f dockerfile文件路径 -t 镜像名:版本号 .
docker build -t mytomcat .  (默认名称Dockerfile)
```



> `docker history 镜像id`#镜像构造过程
>
> `CMD ["ls","-a"]`
>
> `ENTRYPOINT ["ls","-a"]` #  追加命令
>
> Dockerfile 自动查找 不需要-f

掌握dockerfile的编写

1. 准备镜像文件

2. `Dockerfile` 官方命名

3. 编写Dockerfile

   ```dock
   FROM centos
   MAINTAINER  matytan<12300@qq.com>
   COPY readme.txt /usr/local/readme.txt
   ADD jdk1.8.tar.gz /usr/local/jdk1.8
   ADD tomcat.tar.gz /usr/local/tomcat #自动解压
   ENV MYPATH /usr/local
   WORKDIR MYPATH
   ENV JAVA_HOME /usr/local/jdk1.8
   ENV CLASSPATH_HOME $JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
   ENV CATALINA_HOME /usr/local/tomcat
   ENV CATALINA_BASE /usr/local/tomcat
   ENV PATH $PATH:$JAVA_HOME/bin:$CATALINA_HOME/lib:
   $CATALINA_HOME/bin
   EXPOSE 80
   CMD /usr/local/tomcat/bin/startup.sh && tail -F /usr/lcoal/tomcat/bin/logs/catalina.out
   
   
   #jdk下载 
   wget --no-check-certificate --no-cookies --header "Cookie: oraclelicense=accept-securebackup-cookie"  http://download.oracle.com/otn-pub/java/jdk/8u131-b11/d54c1d3a095b4ff2b6607d096fa80163/jdk-8u131-linux-x64.tar.gz
   
   ```

4. `docker build -t mytomcat .`

5. run镜像 生成容器
启动：
`docker run -it --name ubuntugo -p 2080:8080 -v /Users/matytan:/home/matytan ubuntu /bin/bash`
6. 测试发布
