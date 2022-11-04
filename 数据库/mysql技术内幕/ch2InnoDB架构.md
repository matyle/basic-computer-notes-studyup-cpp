## INnoDB概述

- INnoDB的特点？
- ACID？原子，一致，隔离，持久分别是什么
- 

## InnoDB 体系架构

多线程的作用？

内存池的作用？

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-03-17-13-51.png
width=450px>
</center>
### 后台线程

有哪些后台线程，这些多线程的作用是什么？



- 后台线程：刷新内存池中的数据，保证缓冲池中的数据为最小的
- `Master Thread`：核心线程，**将缓冲池中的数据异步刷新到磁盘**,保证数据的一致性，包括脏页刷新，合并插入缓冲
- `IO Thread`：AIO，异步IO提高数据库的性能。主要负责这些IO请求的回调（call back）处理
  
  ```sql
  mysql> SHOW VARIABLES LIKE 'innodb_%io_threads'\G;
    *************************** 1. row ***************************
    Variable_name: innodb_read_io_threads
            Value: 4
    *************************** 2. row ***************************
    Variable_name: innodb_write_io_threads
            Value: 4
    2 rows in set (0.01 sec)
  ```

- Purge Thread :事务提交后，undolog可能不再需要，因此需要PurgeThread来回收已经使用并分配的undo页。1.1之前purge操作在Master Thread中完成


- Page Cleaner Thread
  

### 内存

内存池的核心是缓冲。为了降低内存和磁盘之间的速度差异。

- 如何处理内存和磁盘？
- 每次读取多少？按什么存储

#### 缓冲池 重点LRU

- 怎么读取数据？
- 怎么修改修改？
- 缓冲池到底有哪些数据？
- 如何管理这些缓冲区？
  - 使用LRU，但有所优化
  - 为什么这么优化？
  - 如何实现？
- 如何在数据库中查询这些信息？



InnoDB存储引擎是基于磁盘，因此要减小cpu内存与磁盘的速度的差异，通常使用缓冲池技术来提高数据库的整体性能

- 注意从磁盘读取数据是按“页”读取？
- 页多大？

缓冲池是一块内存区域，首先将磁盘中读到的页 放在缓冲池中 “FIX”



**数据读取** ：首先在缓冲池中查找，若找到则命中，直接读取该页。否则读取磁盘上的==页==到缓冲区“fix”在内存



**修改**：首先修改在缓冲池中的页，然后以一定频率刷新到磁盘。通过一种Checkpoint机制刷新回磁盘。 

- 什么是Checkpoint？ 怎么实现？
- 这里实际上是指的redo-log（想一想老板的粉板）



**缓冲池中的页的类型** ：索引页，数据页，redo-log buffer，插入缓冲，自适应哈希索引，InnoDB存储的锁信息（lock info），数据字典信息等

- redo-log？
- bin-log？

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-07-04-16-52-36.png
width=450px>
</center>
####  LRU list 和Free list和 Flush list

如何管理缓冲区？



LRU用来管理已经读取的页：

- 缓冲池是通过LRU最近最少使用算法来管理的。
- 最频繁页在最前端，最少使用在尾端（使用双向链表和map，与之前实现的LRU相反哈哈）。当不能存放新读取的页时，首先释放LRU列表中尾端的页
- **对LRU的优化：** 插入的新页（活跃页）没有直接放在首端，而插入在midpoint的位置（通常在3/8), `midpoint`之后的称为old列表。之前的列表称为new列表。**new列表中都是比较活跃的热点数据**
- `SHOW VARIABLES LIKE 'innodb_old_blocks_pct'\G;`
- `show engine innodb status\g;`

为啥这么做呢？如果直接放到首部，这些SQL操作可能会使缓冲区中的页被刷出，影响缓冲池的效率
- （错杀）查询操作往往都要访问表中的许多页，甚至是全部页（后访问的变活跃）。但是可能只是简单的查询。并不想把不活跃的变活跃。反而错杀了活跃页。
- 引入`innodb_old_blocks_time`.用于读取到mid位置后需要等待多久才会被加入到LRU列表的热端（加入热端操作叫`page_made_young`)
- 全表扫描可能导致LRU污染（也就是热点数据被替换）

**Free list** 是什么？

- 数据库刚启动，LRU列表为空。此时页由free buffer储存。
- 分页时，首先在free buffer查找可用空闲页，若有则删除free buffer中的页，加入LRU列表中。

LRU是管理全部内存页吗？

不属于LRU管理的页有

- 自适应哈希索引
- lock 信息
- INSERT buffer等页



#### 压缩页

- 对于非16kb的页使用unzip_LRU列表进行管理的。
- 伙伴算法分配内存 若申请4kb的页大小，过程如下
  - 检查4kb的unzip_LRU列表，检查是否有可用的空闲页
  - 若有，则直接使用
  - 否则，检查8KB的unzip_LRU列表
  - 若能够得到空闲页，将页分为2个4kb的页，存放到4kb的unzip_LRU列表
  - 若不能得到空闲页，则申请一个16kb的页。将页分为一个8kb的页，2个4kb的页。

什么是伙伴算法？





#### 数据库显示

`show engine innodb status\g;`

```sql
show engine innodb status\g;
 ----------------------
BUFFER POOL AND MEMORY
----------------------
Total large memory allocated 136970240
Dictionary memory allocated 371511
Buffer pool size   8191
Free buffers       7017
Database pages     1170
Old database pages 451
Modified db pages  0
Pending reads      0
Pending writes: LRU 0, flush list 0, single page 0
Pages made young 0, not young 0
0.00 youngs/s, 0.00 non-youngs/s
Pages read 1028, created 142, written 151
0.00 reads/s, 0.00 creates/s, 0.00 writes/s
No buffer pool page gets since the last printout
Pages read ahead 0.00/s, evicted without access 0.00/s, Random read ahead 0.00/s
LRU len: 1170, unzip_LRU len: 0
I/O sum[0]:cur[0], unzip sum[0]:cur[0]


//观察缓冲
select pool_id,pool_size
from information_schema.innodb_buffer_pool_stats\G;
mysql> select pool_id,pool_size,database_pages from information_schema.innodb_buffer_pool_stats;
+---------+-----------+
| pool_id | pool_size |
+---------+-----------+
|       0 |      8191 |
+---------+-----------+
1 row in set (0.00 sec)
```



### flash 列表

脏页是什么？

flash中的页则是脏页列表。

### 重做日志缓冲 redolog

想一想粉笔板的作用



- redo log buffer重做日志缓冲。

什么时候写回磁盘（账本）：

- Master Thread每一秒将重做日志缓冲刷新到重做日志文件
- 每个事务提交时会将重做日志缓冲提交到文件
- 当重做日志缓冲大小小于1/2时，会将重做日志缓冲提交到文件


### 额外的内存池

- InnoDB的内存管理是通过一种称为内存堆来进行管理的。
- 在对一些数据结构本身的内存进行分配时，需要从额外的内存池中进行申请。
- 分配的缓冲池，每个缓冲池需要缓冲帧还有对应的缓冲控制对象，这些对象保存了LRU，锁，等待等信息，而这个对象需要从额外的内存池中申请。

## checkpoint技术



- 缩短数据库的恢复时间
- 缓冲池不够用时，将脏页刷新到磁盘
- 重做日志不可用时，刷新脏页

- 数据库宕机后，只需要恢复checkpoint之后的重做日志进行恢复
- LRU溢出最少使用的页，如果这个页是脏页，需要强制执行Checkpoint

- 对于INnoDB存储引擎，通过LSN（日志序列号）来标记版本
- checkpoint类型
  - Sharp CheckPoint：数据库关闭时，将所有脏页写回到磁盘
  - Fuzzy CheckPoint：刷新一部分脏页，而不是全部
    - Master Thread Checkpoint：每秒每10秒刷新一定比例的脏页到磁盘
    - FLUSH_LRU_LIST Checkpoint：表示LRU中至少要保留100个空闲页可用
    - Async/Sync Flush Checkpoint：当重做日志文件不可用的时候
    - Dirty Page too much：脏页太多，一般是占75%以上


## Master Thread工作方式
- 其中包含很多循环：主循环（loop），后台循环，刷新循环，暂停循环
- 其中主循环：
  - 每秒的操作：
    - 将重做日志缓冲刷新到磁盘，即使事务还没有提交
    - 合并插入缓冲（可能）
    - 至多刷新100个InnoDB的缓冲池中的脏页到磁盘（可能）
    - 如果当前没有用户活动，切换到background loop （可能）
  - 每10秒的操作
  ```c
  void master_thread(){
  loop:
      background loop:
  
    flush loop:
    suspend loop:
    }
  



- InnoDB 1.0.x的改进
  - 改变了之前的hard coding（硬编码） 俗称写死了，最开始的loop中刷新多少页或者合并多少缓冲都是写死的。
  - 改进为一个innnodb_io_capacity的百分比

- InnoDB 1.2.x的改进
  - 把之前刷新1s和10s刷新的方法分开，每10s刷新的方法放到了一个clean_page的线程中，提高了并发性

## InnoDB的关键特性

### 插入缓冲
- 行记录的插入顺序是按照主键递增的顺序进行插入的。
- 聚集索引(primary key)
- 非聚集索引
```sql
CREATE TABLE t
(
a INT AUTO_INCREMENT,
b VARCHAR(30),
PRIMARY(a),#聚集索引
key(b)# 非聚集索引
)
```

- 使用插入缓冲Insert Buffer
  - 索引是辅助索引
  - 索引不是唯一的

### 两次写
- 提高数据页的可靠性
- 什么是double write
  - 应用重做之前，用户需要一个页的副本，当写入失效时，先通过页的副本还原该页。再进行重做。这就是double write
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-07-14-37-48.png
  width=450px>
  </center>
  

### 自适应哈希索引
- AHI （自适应哈希索引）
- 哈希索引只能用来搜索等值的查询，如WHERE col=xxx


### 异步IO
- 异步IO（Asynchronous IO）：用户可以发送一个IO之后，然后立即可以发送另一个IO请求，当全部IO请求发送完毕后，等待所有IO操作的完成，这就是AIO
- 同步IO Sync IO：需要等待此次操作结束才能继续接下来的操作

- Native AIO

### 刷新邻接页


## 启动关闭和恢复
