## 1.1 mysql逻辑框架
- mysql最重要，最与众不同的特性是它的存储引擎构架
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-28-17-12-24.png
  width=290px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;">mysql逻辑框架图</div>
  </center>

  - 第一层大多数基于网络的客户端/服务器工具类似的架构
  - 第二层构架是mysql的核心服务功能这一层，服务器层
    - 包括查询解析，分析，优化缓存以及所有的内置函数
  - 第三层包含存储引擎


### 连接管理和安全性
- 每个客户端连接都会在服务器进程中拥有一个线程，查询只会在单独的线程中执行
- 服务器会使用线程池 使用少量线程服务大量连接

### 优化与执行
- 解析查询，创建内部结构（解析树），然后对其优化（如重写查询，决定表的读取顺序）
- 例如对于SELECT语句，服务器会先检查查询缓存。如果能找到就不用执行查询解析，优化和执行整个过程
  

## 1.2 并发执行

### 读锁和写锁(读写锁是两个锁)
- 多个用户同时读不会有问题。但是某线程在读，其他线程同时在修改，因此读也需要注意
- 共享锁和排他锁（读锁和写锁）
  - 共享锁是不阻塞的，即共享的
  - 写锁是排他的：写锁会阻塞其他的写锁和读锁
  - 当读写锁被加了写锁时，其他线程对该锁加**读锁或者写锁**都会**阻塞**（不是失败）。当读写锁被加了读锁时，其他线程对该锁加写锁会阻塞，加读锁会成功。
  ```c
  pthread_rwlock_t rwlock;

  //加读锁
  pthread_rwlock_rdlock(&rwlock);
  ...//共享资源的读操作

  pthread_rwlock_unlock(&rwlock); //解锁是同一个函数

  pthread_rwlock_wrlock(&rwlock);
    ...

  pthread_rwlock_unlock(&rwlock);

  pthread_rwlock_destroy(&rwlock);
  ```
- 线程内避免使用信号量，除了维护的代码较多以外，还有一个重要的原因是它容易用错。陈硕在他的著作《Linux多线程服务端编程》P85页中明确指出了，避免使用信号量（semaphore），它的功能与条件变量重合，但容易出错。在《并发编程的 15 条建议(译)》也提及如果Mutex就能解决问题，就不要使用信号量semaphore。

### 锁粒度
- 只对会修改的数据进行锁定，锁定的越少并发程度越高
- 加锁也需要消耗资源

- 表锁
  - 表锁是Mysql中最进本的锁策略。开销最小的策略。锁定整张表。一个用户需要进行写（删除，插入，更新等)操作时，需要先获得写锁。这会阻塞其他用户对该表的读写操作。没有写锁时，其他读取的用户才能获取读锁。
  - READ，LOCAL表锁支持某些类型的并发写操作？
  - 写锁比读锁有更高优先级。写锁请求会被排到读锁前。
  - 服务器层可能会使用

- 行级锁
  - 只在存储引擎层，在mysql服务层没有实现
  
## 1.3 事务transaction
- 事务就是一组原子性的SQL查询，一个独立的工作单元。
- 如果数据库引擎能够对数据库应用该组查询的全部语句，那就执行该组查询。如果其中有任何一条语句因为崩溃或其他原因无法执行，所有语句都不会执行
- ACID：原子性（atomicity),一致性(consistency)，隔离性(isolation),持久性(durability).
  - 原子性：一个事务必须被视为一个不可分割的最小工作单元。整个事务要么全部成功提交，要么全部失败回滚
  - 一致性：数据库总是从一个一致性的状态转换到另一个一致性的状态。事务如果没有最终提交，那么事务所做的修改不会存入数据库
  - 隔离性:一个事务所做的修改在提交前，对其他事务是不可见的
  - 持久性：一旦事务提交，所做的修改就会永久保存在数据库。
- mysql存储引擎可以选择是否需要事务处理

### 隔离级别
- READ UNCOMMTTED（未提交读）：可以读取未提交的数据（脏读）
- READ COMMTTED（提交读）：一个事务从开始到提交之前，所做的任何修改都是对其他事务不可见的。也叫不可重复读
- REPEATABLE READ 可重复读：在同一个事务中多次读取同样的记录结果一致。可重复读隔离级别没有解决幻读问题。可重复读是MySQL的默认事务隔离级别

- SERIALIZABLE：可串行化
  - 可串行化是最高的隔离级别。强制事务串行执行，避免幻读
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-28-20-16-05.png
width=490px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">隔离级别</div>
</center>


### 死锁
- 死锁是指两个或者多个事务在同一资源上相互占用，并请求锁定对方占用的资源，从而导致恶性循环的现象。
```sql
事务1
START TRANSACTION;
UPDATE StockPrice SET close = 45.50 WHERE stock_id=4 and data ='2002-05-01';
UPDATE StockPrice SET close = 19.80 WHERE stock_id=3 and data ='2002-05-02';
COMMIT;

事务2：
START TRANSACTION;
UPDATE StockPrice SET close = 20.12 WHERE stock_id=3 and data ='2002-05-02';
UPDATE StockPrice SET close = 47.20 WHERE stock_id=4 and data ='2002-05-01';
COMMIT;
```
- 如果两个事务都执行了第一条数据，更新了一行同时也锁定了另外一行，接着事务都尝试更新第二条语句，发现被对方锁定，然后就等对方释放锁。
- 死锁检测和死锁超时机制。InnoDB目前处理死锁方式是，将持有最少行级排他锁的的事务进行回滚。


### 事务日志
- 存储引擎在修改表数据时只需要修改其内存拷贝，在将修改行为记录到持久在硬盘的事务日志中。而不用每次都修改的数据本身持久到磁盘
- 事务日志采用的是追加方式。


### Mysql中的事务
- mysql提供两种事务性的存储引擎：InnoDB和NDB Cluster

- 自动提交（AUTOCOMIT)  
  - mysql默认使用自动提交模式,如果不是显式开始一个事务，则每个查询都会被当做一个事务执行提交操作
  - SHOW VARIABLES LIKE 'AUTOCOMMIT';
  ```sql
    +---------------+-------+
    | Variable_name | Value |
    +---------------+-------+
    | autocommit    | ON    |
    +---------------+-------+
    1 row in set (0.06 sec)
  ```
  - 1或者ON表示启用，0或者OFF表示禁用。如果AUTOCOMMIT=0，所有查询都在一个事务中，直到显式的COMMIT或者ROLLBACK。该事务结束后，马上开始另一个新事务
  - `SET TRANSACTION ISOLATION LEVEL READ COMMITTED` 设置事务隔离级别


- 在事务中混合使用存储引擎
  - Mysql在服务器层不管事务，事务是由下层的存储引擎实现的。所以在同一个事务中，使用多种存储引擎是不可靠的。
  - 非事务的表不能回滚，如果混用事务型和非事务型的表，可能导致不一致。
- 隐式锁定和显式锁定
  - InnoDB采用两阶段锁定协议。在事务执行过程中，随时都可以锁定，锁只有执行了COMMIT或者ROLLBACK的时候才会释放
  - InnoDB显式加锁
    - SELECT ... LOCK IN SHARE MODE
    - SELECT ... FOR UPDATE


## 多版本并发控制
- 多版本并发控制(MVCC)。MVCC是行级锁的一个变种
- MVCC更加高效，避免了很多加锁操作，实现了非阻塞的读操作，写操作，只锁定特定行
  - 实现：保存数据在某个时间点的快照实现。
- 乐观并发控制和悲观并发控制
- InnoDB的MVCC：通过每行记录后面保存的两个隐藏列实现
  - 创建时间（系统版本号）：开始一个新事务，系统版本号会递增**事务开始时，系统版本号会作为事务的版本号**，用来和查询到的每行记录的版本号进行比较
  - 过期时间（系统版本号）
  - SELECT：只**查找行版本早于当前事务版本的数据行**（可以保证事务读取的行要么之前存在(小于），要么是事务自身插入或者修改过的（相等））。行的删**除版本**要么未定义要么大于当前事务版本号(确保事务开始之前没有被删除)
  - INSERT：保存当前系统版本号作为行版本号
  - DELETE：保存当前系统版本号作为行删除标识 ？？？
  - UPDATE：保存当前系统版本号作为行版本号和作为行删除标识
  
  |  a  |  b  |   c  |   d   |创建版本|删除版本号|
  |----|----|----|----|----|----|
  |    |    |   |   |   |   |
  |    |    |   |   |   |   |   |


## Mysql的存储引擎

```c
*************************** 1. row ***************************
           Name: user
         Engine: InnoDB
        Version: 10
     Row_format: Dynamic
           Rows: 4
 Avg_row_length: 4096
    Data_length: 16384
Max_data_length: 0
   Index_length: 0
      Data_free: 6291456
 Auto_increment: NULL
    Create_time: 2021-04-15 16:07:45
    Update_time: NULL
     Check_time: NULL
      Collation: utf8_bin
       Checksum: NULL
 Create_options: row_format=DYNAMIC stats_persistent=0
        Comment: Users and global privileges
1 row in set (0.04 sec)
```

- InnoDB
- MyISAM引擎
- BlackHole
- CSV引擎
- Federated引擎
- Memory引擎
- Merge
- NDB

### 第三方引擎
- OLTP类引擎：
  - XtraDB：是一个InnoDB的改进版本


- 面向列的存储引擎


### 选择合适的引擎
- 事务：InnoDB或者XtraDB
- 备份
- 崩溃恢复：MyISAM崩溃后发生损坏的概率比InnoDB，恢复速度也要慢
- 特有的特性



### 转换表的引擎
#### ALTER
- ALTER TABLE mytable ENGINE = InnoDB;
- 上述命令将原表复制到一张新表中。
#### 导出与导入
- mysqlldump工具将数据导出到文件

#### 创建于查询
- CREATE TABLE innodb_table LIKE myisam_table;
- CREATE TABLE innodb_table ENGINE = InnoDB;
- INSERT INTO innodb_table SELECT * FROM myisam_table;

- 如果数据量太大，可以按事务提交
  ```sql
    START TRANSACTION;
    
  ```