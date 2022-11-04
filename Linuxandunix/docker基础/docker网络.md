---
title: docker网络相关
date: 2021-12-4 20:10:17
tags: docker
categories: 容器
keywords: docker
description: 
cover: https://gitee.com/matytan/tupic/raw/master/uPic/69358fdd0b7653e1268a26dd83168483.jpeg
---
### docker 登录

> `docker login -u matytan` 
>
> `docker tag +镜像id +版本`
>
> `docker push 镜像id`
>
> ###### 发布到阿里云
>
> 容器镜像服务
>
> 创建命名空间（防止冲突）
>
> 创建容器镜像
>
> 参考官方文档

![image-20211014110447370](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211014110447370.png)

## Docker 网络

![image-20211014111241428](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211014111241428.png)



> veth-pair技术
>
> 每一个容器都会生成一对网卡
>
> 

![image-20211014144426113](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211014144426113.png)



> `--link` #将容器host文件增加一个其他容器的映射



### 自定义网络



```sh
[root@tanqi ~]# docker network ls
NETWORK ID     NAME      DRIVER    SCOPE
b0f6fc63f4fd   bridge    bridge    local
9d4ddd9d6910   host      host      local
21739635bcf7   none      null      local
```



> `docker network create --driver bridge --subnet 192.168.0.0/16  --gateway 192.168.0.1 mynet` 
>
> `docker run -it --name tomcat01 -P --net mynet tomcat`

![image-20211014170814869](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211014170814869.png)

>`docker network connect mynet centos1`