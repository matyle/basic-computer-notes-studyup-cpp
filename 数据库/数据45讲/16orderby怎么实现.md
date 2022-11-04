



```sql
CREATE TABLE `t` (
  `id` int(11) NOT NULL,
  `city` varchar(16) NOT NULL,
  `name` varchar(16) NOT NULL,
  `age` int(11) NOT NULL,
  `addr` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `city` (`city`)
) ENGINE=InnoDB;
建表语句

```



```sql
城市是“杭州”的所有人名字，并且按照姓名排序返回前 1000 个人的姓名、年龄。
select city,name,age from t where city='杭州' order by name limit 1000  ;
```



- where 在order by之前



### 这个语句执行流程







### 联合索引 

MySQL 之所以需要生成临时表（sortbuffer），并且在临时表上做排序操作，其原因是原来的数据都是无序的。

并不是所有的 order by 语句，都需要排序操作。如果能够保证从 city 这个索引上取出来的行，天然就是按照 name 递增排序的话，是不是就可以不用再排序了呢？



```sql
alter table t add index city_user(city, name);
```

<img src="16orderby怎么实现.assets/f980201372b676893647fb17fac4e2bf.png" alt="img" style="zoom:40%;" />

<img src="16orderby怎么实现.assets/3f590c3a14f9236f2d8e1e2cb9686692.jpg" alt="img" style="zoom:40%;" />



### 覆盖索引

第 5 篇文章《 深入浅出索引（下）》

- 覆盖索引是指，索引上的信息足够满足查询请求，不需要再回到主键索引上去取数据。

```sql
alter table t add index city_user_age(city,name,age);
```





并不是说要为了每个查询能用上覆盖索引，就要把语句中涉及的字段都建上联合索引，毕竟索引还是有维护代价的。这是一个需要权衡的决定。







### 小结

不可避免地会使用到 order by 语句，心里要清楚每个语句的排序逻辑是怎么实现的，还要能够分析出在最坏情况下，每个语句的执行对系统资源的消耗，这样才能做到下笔如有神，不犯低级错误。





问题：

假设你的表里面已经有了 city_name(city, name) 这个联合索引了

```sql

select * from t where city in ('杭州',"苏州") order by name limit 100;
```



会排序吗？怎么设计数据库段不用排序的算法

1. 分成2个语句分别对两个排序，然后在客户端合并
2. 添加name_city(name, city)联合索引，利用索引下推，先找name用看name是否在( 杭州',"苏州")中，如果在就留下，知道找到100个。最坏情况下的确要扫描整个二级索引树。



