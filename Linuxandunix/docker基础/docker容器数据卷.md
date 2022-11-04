---
title: docker容器数据卷
date: 2021-12-4 20:05:17
tags: docker
categories: 容器
keywords: docker
description: 
cover: https://gitee.com/matytan/tupic/raw/master/uPic/69358fdd0b7653e1268a26dd83168483.jpeg
---

##容器数据卷

### 同步机制

> `docker run 可选参数  镜像名`
>
> -d  后台运行
>
> -p 端口映射 主机：容器
>
> --name 容器名
>
> -e 环境配置
>
> 方式一：命令 `-v` 主机目录：容器目录

**挂载之后**

主机修改自动同步

容器修改也会自动同步到宿主机

绑定的方式

>        "Mounts": [
>            {
>                "Type": "bind",
>                "Source": "/home/centos01/home",
>                "Destination": "/home",
>                "Mode": "",
>                "RW": true,
>                "Propagation": "rprivate"
>            }



于是可以把自己的容器打包为镜像，然后设置文件映射呀！！

### mysql数据同步

mysql 端口3306

利用dockerhub的帮助文档

同时使用-v 主机目录:容器目录 挂载卷

### 具名挂载 匿名挂载

不指定容器外的名字，只指定容器目录 ：匿名挂载

> 匿名挂载
>
> `docker run -v 容器内路径`  # 匿名挂载

>具名挂载 没有指定主机目录
>
>`docker run -v 卷名：/home centos:ro` # 只读
>
>`docker run -v 卷名：/home centos:rw`# 只写

数据卷容器：

> `--volumes-from` 容器间挂载
>
> `docker run -it --name mysql02 -p 3311:3306 -e MYSQL_ROOT_PASSWORD=my-secret-pw -d  --volumes-from mysql01 mysql:5.7`
