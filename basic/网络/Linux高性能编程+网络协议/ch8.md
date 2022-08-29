### 高性能服务器程序框架

#### 8.1 服务器模型

##### 8.1.1 服务器模型
- c/s模型
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-12-16-45-12.png
width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

- p2p模型
  - 带发现服务器的p2p模型

#### 8.2 服务器编程框架
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-12-16-53-30.png
width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

|  模块  |  单个服务器程序  |  服务器群  |
|----|----|----|
|  i/o处理单元  |  处理客户连接，读写网络数据  |  作为接入服务器，实现负载均衡  |
|  逻辑单元  |   业务进程或线程 |  逻辑服务器  |
|  网络存储单元  |  本地数据库，文件或缓存  |  数据库服务器  |
|  请求队列  |  各个单元之间的通信方式  |  各服务器之间的永久TCP链接  |


#### 8.3 IO模型
##### 同步io：
- 阻塞I/O：阻塞的文件描述符。socket默认为阻塞的
  - 系统调用无法立即完成而被系统挂起
- 非阻塞I/O：非阻塞的文件描述符。socket默认为阻塞，第二个参数传递SOCK——NONBLOCK或者通过fcntl 调用F_SETFL
  - 总是立即返回，不管事件是否发生
- 阻塞io，SIGIO和IO复用 都是同步io
  - IO复用是最常使用的 IO通知机制，即应用程序通过IO复用函数向内核注册事件，内核又通过它将消息通知给应用程序，本身是阻塞的

##### 异步io
- 由内核完成io操作

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-15-14-46-15.png
width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">io模型对比</div>
</center>

#### 8.4 两种模式

##### 8.4.1 reactor模式
- 主线程（io处理单元）只负责监听文件描述符上是否有事件发生，有就立即通知工作进程（逻辑单元）
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-15-14-52-33.png
width=500px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">reactor模式</div>
</center>


##### 8.4.2 proactor模式
- 所有的io操作都交给主线程和内核处理

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-15-15-13-25.png
width=500px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">proactor模式</div>
</center>


##### 8.4.3 模拟proactor模式：主线程监听，同时读写。读写完成后通知工作进程处理逻辑操作

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-15-15-18-14.png
width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">图片模拟proactor模式描述</div>
</center>



#### 8.5 两种高效的并发模式

- 半同步/半异步
  - 同步：完全按照代码的顺序执行
  - 异步：程序的执行需要系统事件来驱动
    <center>
        <img style="border-radius: 1.125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
        src= img/2021-05-15-15-22-25.png
    width=400px>
        <br>
        <div style="color:orange; border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;">a）同步读，b)异步读</div>
    </center>

  - 同步进程用于处理客户逻辑，异步进程处理io事件
    <center>
        <img style="border-radius: 1.125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
        src=img/2021-05-15-15-32-31.png
    width=500px>
        <br>
        <div style="color:orange; border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;"></div>
    </center>



- 领导者/追随者
  - 句柄集（Handle set）：用于表示IO资源
  - 线程集：负责线程的同步，领导的推选，领导先进行io操作。
    - leader
    - processing
    - follower
  - 事件处理器和具体事件处理器

    <center>
        <img style="border-radius: 1.125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
        src=img/2021-05-15-15-44-05.png
    width=600px>
        <br>
        <div style="color:orange; border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;">图片描述</div>
    </center>


#### 8.7 提高服务器性能的其他建议
- 池：池是一组资源的集合，是静态资源，无需动态分配
- 数据复制：避免数据复制
- 上下文切换和锁：不应该使用过多的进程或者线程，避免使用锁
  - 读写锁：不会增加系统额外开销。

