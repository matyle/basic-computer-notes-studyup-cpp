## KUbernets 如何实现容器编排

云原生核心技术剖析

### kubernets核心组件介绍



什么是云原生？

![image-20211108201051630](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211108201051630.png)

单体应用：

分层应用

微服务：

![image-20211108201617299](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211108201617299.png)

云原生：面向云环境，自动伸缩容，资源管理，



![image-20211108202608602](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211108202608602.png)



## 实现



- POD
  - 一组（一个或者多个）容器
  - 共享存储，网络以及怎么运行容器的声明
  - Pod是可以创建和管理的，最小单位可部署的计算单元
  - 被某种工作负责或者控制器管理
- 