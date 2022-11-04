---
title: linux实用程序Screen
date: 2021-11-16 15:48:17
tags: docker
categories: 容器
keywords: 
description: 
cover: 
---



方法很多，这里我说一下使用 docker-connector解决这个问题

这是一个github开源项目docker-connector 

1. Mac 通过 brew 安装 docker-connector

` brew install wenjunxiao/brew/docker-connector`


2. 执行以下命令把 docker 的所有 bridge 网络都添加到路由中(拷贝已下指令)

` docker network ls --filter driver=bridge --format "{{.ID}}" | xargs docker network inspect --format "route {{range .IPAM.Config}}{{.Subnet}}{{end}}" >> /usr/local/etc/docker-connector.conf`
或者修改` /usr/local/etc/docker-connector.conf `文件中的路由格式是
` route 172.17.0.0/16`
路由的子网决定了你能访问那些容器，docker-connector.conf这个文件之前是注释了的需要手动修改

3. 配置完成，直接启动服务（需要 sudo，路由配置启动之后仍然可以修改，并且无需重启服务立即生效）
` sudo brew services start docker-connector`
4.使用以下命令在 docker 端运行 wenjunxiao/mac-docker-connector，需要使用 host 网络，并且允许 NET_ADMIN，pull connector容器的作用是作为桥接
` docker run -it -d --restart always --net host --cap-add NET_ADMIN --name connector wenjunxiao/mac-docker-connector`


成功访问