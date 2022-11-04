

## 例子

- 例子一 显然会用a作为索引
```sql

CREATE TABLE `t` (
  `id` int(11) NOT NULL,
  `a` int(11) DEFAULT NULL,
  `b` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `a` (`a`),
  KEY `b` (`b`)
) ENGINE=InnoDB;


# 存储过程

delimiter ;;
create procedure idata()
begin
  declare i int;
  set i=1;
  while(i<=100000)do
    insert into t values(i, i, i);
    set i=i+1;
  end while;
end;;
delimiter ;
call idata();


explain select * from t where a between 10000 and 20000;
```

- 第二种 新版本已修复
  ```sql
  start transaction with consistent snapshot;

  select * from information_schema.innodb_trx\G;#查询事务


  set long_query_time=0;
  #第一句，是将慢查询日志的阈值设置为 0，
  #表示这个线程接下来的语句都会被记录入慢查询日志中；
  ```



## 优化器的逻辑

优化器选择索引的目的，是找到一个最优的执行方案，并用最小的代价去执行语句。
- 在数据库里面，扫描行数是影响执行代价的因素之一。
- 扫描的行数越少，意味着访问磁盘数据的次数越少，消耗的 CPU 资源越少。

### 如何判断扫描行呢？

- 一个索引上不同的值越多，这个索引的区分度就越好。而一个索引上不同的值的个数，我们称之为“基数”（cardinality）


### MySQL 是怎样得到索引的基数的呢？
- MySQL 采样统计的方法。
- InnoDB 默认会选择 N 个数据页，统计这些页面上的不同值，得到一个平均值，然后乘以这个索引的页面数，就得到了这个索引的基数。
- 数据表是会持续更新的，索引统计信息也不会固定不变。所以，当变更的数据行数超过 1/M 的时候，会自动触发重新做一次索引统计。


### 选错索引的原因
- 索引统计不准确

- 索引统计只是一个输入，对于一个具体的语句来说，优化器还要判断，执行这个语句本身要扫描多少行。

```sql
 explain select * from t where (a between 1 and 1000)  and (b between 50000 and 100000) order by b limit 1;
```
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-22-15-29-53.png
width=450px>
</center>

- 如果使用索引 a 进行查询，那么就是扫描索引 a 的前 1000 个值，然后取到对应的 id，再到主键索引上去查出每一行，然后根据字段 b 来过滤。显然这样需要扫描 1000 行
- 如果使用索引 b 进行查询，那么就是扫描索引 b 的最后 50001 个值，与上面的执行过程相同，也是需要回到主键索引上取值再判断，所以需要扫描 50001 行。

- 如果使用索引 a 的话，执行速度明显会快很多
- 选错索引b
扫描行数的估计值依然不准确；
这个例子里 MySQL 又选错了索引。

## 索引选择异常和处理

- **一种方法是，像我们第一个例子一样，采用 force index 强行选择一个索引。**
- 所以第二种方法就是，我们可以考虑修改语句，引导 MySQL 使用我们期望的索引。比如，在这个例子里，显然把“order by b limit 1” 改成 “order by b,a limit 1” ，语义的逻辑是相同的。
- **第三种方法是，在有些场景下，我们可以新建一个更合适的索引，来提供给优化器做选择，或删掉误用的索引。**
## 修正错误 
可以用来重新统计索引信息。我们来看一下执行效果。
- analyze table t；
- 所以在实践中，如果你发现 explain 的结果预估的 rows 值跟实际情况差距比较大，可以采用这个方法来处理。