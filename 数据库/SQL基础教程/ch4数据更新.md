## 数据插入

创建表

```sql
create table Productcopy3
(
  product_id char(4) NOT NULL,
  product_name varchar(100) NOT NULL,
  product_type varchar(32) NOT NULL,
  sale_price integer default 0,
  purchase_price integer,
  regist_date date,
  primary key(product_id)
);

insert into <tablename> (column 1,column2...) values (val1,val2,...)

insert into productln (product_id,product_name,product_type,sale_price,regist_date) values
('0001','T恤衫','衣服',30,'2021-07-29');



insert into productcopy3 values
('0002','T恤衫','衣服',30,10,'2021-07-29');

insert into productln values
('0003','T恤衫','衣服',default,10,'2021-07-29');


insert into productcopy3
(product_id,product_name,product_type,sale_price,regist_date) values
('0005','T恤衫','衣服','2021-07-29');


alter table productln change regist_data regist_date date;
```

复制表

```sql
create table ProductCopy3
(
  product_id char(4) NOT NULL,
  product_name varchar(100) NOT NULL,
  product_type varchar(32) NOT NULL,
  sale_price integer default 0,
  purchase_price integer,
  regist_date date,
  primary key(product_id)
);

alter table productcopy change regist_data regist_date date;

insert into ProductCopy1 (product_id,product_name,product_type,sale_price,purchase_price,regist_date)
select product_id,product_name,product_type,sale_price,purchase_price,regist_date
from productln;


拷贝全表

insert into productcopy3
select * from productln;
```



各种select语句,insert中的select语句可以用group by 或者 where等任何语法

其实单独使用select相当于创建的是临时表，当然可以插入自建表中

```sql
create table ProductType(
product_type varchar(32) not null,
sum_sale_price integer,
sum_purchase_price integer,
primary key(product_type)
);

//从product表中选择数据插入

insert into producttype (product_type,sum_sale_price,sum_purchase_price)
select product_type,sum(sale_price),sum(purchase_price)
from product
group by product_type;
```





## 数据删除

[13 | 为什么表数据删掉一半，表文件大小不变？ (geekbang.org)](https://time.geekbang.org/column/article/72388)

delete删除的对象是行

```sql
清空表
delete from productcopy3; 

不能写成
delete * from productcopy3; //错误 delete对象是行，不是列，后面跟任何列都是不对的。
```





## 数据更新

```sql
update tablename
set columnname = <表达式>;


update tablename
set columnname = <表达式>
where <条件>;
```



## 事务

```sql
mysql> begin;
Query OK, 0 rows affected (0.00 sec)

mysql> select * from productcopy3;
Empty set (0.01 sec)



```

 mysql中begin之后 需要跟一条语句才会真正开启事务一致性视图





- 显式启动事务语句， begin 或 start transaction。配套的提交语句是 commit，回滚语句是 rollback。
- set autocommit=0，这个命令会将这个线程的自动提交关掉。意味着如果你只执行一个 select 语句，这个事务就启动了，而且并不会自动提交。这个事务持续存在直到你主动执行 commit 或 rollback 语句，或者断开连接。建议总是使用 set autocommit=1, 通过显式语句的方式来启动事务。





事务隔离级别

读未提交是指，一个事务还没提交时，它做的变更就能被别的事务看到。

读提交是指，一个事务提交之后，它做的变更才会被其他事务看到。

可重复读是指，一个事务执行过程中看到的数据，总是跟这个事务在启动时看到的数据是一致的。当然在可重复读隔离级别下，未提交变更对其他事务也是不可见的。（B事务能看见A事务提交的情况是，B事务更新A事务提交之后的内容）

串行化，顾名思义是对于同一行记录，“写”会加“写锁”，“读”会加“读锁”。当出现读写锁冲突的时候，后访问的事务必须等前一个事务执行完成，才能继续执行。



### 练习



4.3 

```sql
create table productionMargin
(
  product_id char(4) not null,
  product_name varchar(100) not null,
 	sale_price integer,
  purchase_price integer,
  margin integer,
  primary key(product_id)
);


insert into productionMargin
select product_id,product_name,sale_price,purchase_price,(sale_price-purchase_price)
from product;


update productionMargin
set sale_price=sale_price-1000,margin=margin-1000
where product_name='运动T恤';


```





