![image-20211108120522828](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211108120522828.png)

- 零拷贝：sendfile

### 生产端



生产端：消息压缩，缓存批量发送，异步解耦

多线程并发：防止某一个业务阻塞等待

<img src="https://gitee.com/matytan/tupic/raw/master/uPic/image-20211108120726929.png" alt="image-2021110820726929" style="zoom:200%;" />



接收消息缓存

![image-20211108121338496](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211108121338496.png)



BufferPool设计：不释放

![image-20211108121922071](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211108121922071.png)





### 服务端

Reactor模型，顺序写，页缓存，零拷贝

- Reactor:

  - 连接线程（main线程）
  - 线程处理线程
  - 放置在请求队列中
  - 线程池中的线程去出去请求队列中的Request对象 进行消费（处理）
  - 响应也不是直接发送给客户端（而是放在Response队列）：防止高并发时，线程工作过于饱和，而导致延迟

- 顺序写：追加写，头部读（日志）

  - 后写
  - 预读

  

- 页缓存

  - 缓存当磁盘用，避免频繁读写磁盘   

  

- 零拷贝：cpu不参加拷贝数据的工作，节省大量cpu周期，减少两次cpu在用户态和内核态的切换 

  - 无零拷贝：四次上下文切换，四次拷贝（2次cpu，2次dma）

  ![image-20211108122751136](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211108122751136.png)

  - 零拷贝

![image-20211108122956594](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211108122956594.png)





### 消费端

![image-20211108123234775](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211108123234775.png)

 

网络IO任务和拉取消息任务的解耦，防止拉取消息时的IO阻塞，提高网络IO任务和拉取消息效率



![image-20211108123218366](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211108123218366.png)