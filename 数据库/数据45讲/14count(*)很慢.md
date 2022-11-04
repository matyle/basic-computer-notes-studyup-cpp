

- select count(*) from t 记录数越来越多，这条语句执行得也会越来越慢。

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-24-15-52-07.png
width=450px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">图片描述</div>
</center>




- 在保证逻辑正确的前提下，尽量减少扫描的数据量，是数据库系统设计的通用法则之一。
- MyISAM 会记录总行数可以直接返回（没有where条件下）
- show table status 中TABLE_ROWS是统计行数，并不准确



- 3个问题
  - MyISAM 表虽然 count(*) 很快，但是不支持事务；*
  - *`show table status` 命令虽然返回很快，但是不准确；*
  - InnoDB 表直接 count(*) 会遍历全表，虽然结果准确，但会导致性能问题。





自己找一个地方，把操作记录表的行数存起来。



### 用缓存系统Redis保存计数



用一个 Redis 服务来保存这个表的总行数。这个表每被插入一行 Redis 计数就加 1，每被删除一行 Redis 计数就减 1



- 问题
  - 缓存系统可能丢失更新。Redis 的数据不能永久地留在内存里，所以你会找一个地方把这个值定期地持久化存储起来。如果刚刚在数据表中插入了一行，Redis 中保存的值也加了 1，然后 Redis 异常重启了，重启后你要从存储 redis 数据的地方把这个值读回来，而刚刚加 1 的这个计数操作却丢失了。（可以解决，就是重新读一次mysql的总行数，全表扫描）
  - 除了这个问题，还有可能值并不精确。将计数保存在缓存系统中的方式，还不只是丢失更新的问题。即使 Redis 正常工作，这个值还是逻辑上不精确的

- 不精确

  - 一种是，查到的 100 行结果里面有最新插入记录，而 Redis 的计数里还没加 1；

  - 另一种是，查到的 100 行结果里没有最新插入的记录，而 Redis 的计数里已经加了 1。

    <img src="https://static001.geekbang.org/resource/image/39/33/39898af053695dad37227d71ae288e33.png" alt="im" style="zoom:80%;" />

<center>
<div style="color:orange; border-bottom: 1px solid #d9d9d9;
display: inline-block;
color: #999;
padding: 2px;">会话 A、B 执行时序图</div>
</center>



- 调整顺序，先计数。但是如果这时查询，查询到还没插入的行的。

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=14count(*)很慢.assets/5c2f786beae1d8917cdc5033b7bf0bdb.png
width=450px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">调整顺序后的会话 A、B 执行时序图</div>
</center>





### 在数据库保存计数

如果我们把这个计数直接放到数据库里单独的一张计数表 C 中，又会怎么样呢？



- 这解决了崩溃丢失的问题 利用redolog和binlog
- InnoDB 要支持事务，从而导致 InnoDB 表不能把 count(*) 直接存起来，然后查询的时候直接返回形成的。
- 虽然会话 B 的读操作仍然是在 T3 执行的，但是因为这时候更新事务还没有提交，所以计数值加 1 这个操作对会话 B 还不可见。（可重复读）

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=14count(*)很慢.assets/9e4170e2dfca3524eb5e92adb8647de3.png
width=450px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">调整顺序后的会话 A、B 执行时序图</div>
</center>





###  不同的 count 用法

count() 是一个聚合函数，对于返回的结果集，一行行地判断，如果 count 函数的参数不是 NULL，累计值就加 1，否则不加。最后返回累计值。



count(*)、count(主键 id)、count(字段) 和 count(1) 等不同用法的性能

- 效率：count(字段)<count(主键 id)<count(1)≈count(\*)，所以我建议你，尽量使用 count(*)。



分析性能差别的时候，你可以记住这么几个原则：

- server 层要什么就给什么；

- InnoDB 只给必要的值；

- 现在的优化器只优化了 count(*) 的语义为“取行数”，其他“显而易见”的优化并没有做。

**对于 count(主键 id) 来说** ，InnoDB 引擎会遍历整张表，把每一行的 id 值都取出来，返回给 server 层。server 层拿到 id 后，判断是不可能为空的，就按行累加。

**对于 count(1) 来说** ，InnoDB 引擎遍历整张表，但不取值。server 层对于返回的每一行，放一个数字“1”进去，判断是不可能为空的，按行累加。

=count(1) 执行得要比 count(主键 id) 快。因为从引擎返回 id 会涉及到解析数据行，以及拷贝字段值的操作。



对于 count(字段) 来说：

- 如果这个“字段”是定义为 not null 的话，一行行地从记录里面读出这个字段，判断不能为 null，按行累加；
- 如果这个“字段”定义允许为 null，那么执行的时候，判断到有可能是 null，还要把值取出来再判断一下，不是 null 才累加。也就是server 层要什么字段，InnoDB 就返回什么字段。



但是 count(\*) 是例外，并不会把全部字段取出来，而是专门做了优化，不取值。count(*) 肯定不是 null，按行累加。





### 小结

- 利用Redis服务器缓冲存在两个问题：数据可能丢失更新以及更重要的是数据不精确的问题（逻辑不一致），这两个不同的存储构成的系统，不支持分布式事务，无法拿到精确一致的视图。
- InnoDB 引擎支持事务，我们利用好事务的原子性A和隔离性I，就可以简化在业务开发时的逻辑。ACID



