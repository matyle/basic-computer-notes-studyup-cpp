## 视图

视图简单来说就是一个select子句，没有存在任何地方



- 创建视图

```mysql
create view viewname(<viewcolumn1>,<view column2>)
AS 
<select语句>

create view Productsum(product_type,cnt_product)
AS
select product_type,count(*)
from product
group by product_type;
```

- 使用视图

  ```sql
  select * from Productsum;
  ```



- 多重视图 ，应该避免会降低性能

  ```sql
  create view productSumJim(product_type,cnt_product)
  AS
  select product_type,cnt_product
  from productsum
  where product_type='办公用品';
  
  select * from productSumJim;
  ```

- 视图的限制1
-  视图的限制2 更新数据 更新数据时表和视图需要同时更新
  - 有些情况表是不能更新的，会导致数据的不一致，例如使用了group by

```sql
不可更新的语句
insert into productsum (product_type,cnt_product) values
("电器",5);
因为原表并没有其他列
The target table productsum of the INSERT is not insertable-into

可更新的
create view ProductsumUpdate(product_id,)
AS
select *
from product;

//然后可以向ProductsumUpdate插入行
```





- 删除视图

  ```sql
  drop view productsum;
  ```





## 子查询



什么是子查询：一次性视图



- 子查询

```sql
sudo /usr/local/mysql/support-files/mysql.server start

 select * from emp where sal = (select max(sal) from emp); //也叫标量子查询
```

==非相关子查询是独立于外部查询的子查询==，子查询执行完毕后将值传递给外部查询

​    特点
        1. 先执行内层查询

2. 再执行外层查询

   

- 标量子查询
  - 优势可以使用在where或者having之后

    ```sql
    select product_id,product_name,sale_price,
    (select avg(sale_price) 
     from product) as avg_price 
    from product;
    
    
    select product_type,AVG(sale_price)
    from product
    group by product_type
    having AVG(sale_price)>(select avg(sale_price) from product);
    ```

    

### 关联子查询

先取到主查询中的相关数据然后进行子查询最后做主查询where筛选的.

where col = "定值"，那这个定值就计算一次就OK了，而关联子查询是每次都得计算，也就是每次都得计算出一个"定值"再根据这个定值去筛选.

按照商品分类计算商品的平均价格

```sql
select product_type,avg(sale_price)
from product
group by product_type;

不能直接用上面的子查询作为where的条件 因为有多行 如
select product_id,product_name,sale_price
from product
where sale_price>(select avg(sale_price)
from product
group by product_type);

ERROR 1242 (21000): Subquery returns more than 1 row


关联子查询
select product_type,product_name,sale_price
from product as p1
where sale_price>(select avg(sale_price)
from product as p2
where p1.product_type=p2.product_type
group by product_type
);

```

- 我们首先来看看正常情况下SELECT的书写顺序和执行顺序：

  **书写顺序：**

  ***SELECT\***》FROM 》WHERE》GROUP BY》HAVE》ORDER BY

  **执行顺序：**

  FROM 》WHERE》GROUP BY》HAVE》***SELECT\***》ORDER BY

- 关联子查询 : 

在关联子查询中，对于外部查询返回的每一行数据，内部查询都要执行一次。

另外，在关联子查询中是信息流是双向的。外部查询的每行数据传递一个值给子查询，然后子查询为每一行数据执行一次并返回它的记录。然后，外部查询根据返回的记录做出决策。



    特点
        1. 先执行外层查询
        2. 再执行内层查询

**记住，关联子查询和正常的SELECT语句完全不同。**







## 练习

1. 注意：where中date需要引号！！！
2. 数据插入视图

```sql
create view view1(product_name,sale_price,regist_date)
as
select product_name,sale_price,regist_date
from product
where sale_price>=1000 and regist_date='2009-09-20';

//如果插入
insert into view1 values('刀子',300,'2009-11-02');
insert into product values()
ERROR 1423 (HY000): Field of view 'shop.view1' underlying table doesn't have a default value
```



3 

```sql
//错误写法
select *, avg(sale_price) as sale_price_all
from product;

标量子查询 

select *,(select avg(sale_price)
          from product ) as sale_price_all  #别名是主查询的因此在外层
from product;
```



4 关联子查询的方法

```sql
select *,(select avg(sale_price)
          from product p2
          where p1.product_type=p2.product_type
          group by product_type ) as sale_price_all  #别名是主查询的因此在外层
from product p1;

//怎么看出来是关联子查询的？ 要求的是各个种类的平均价格
如果不使用关联子查询 子查询中直接用group by 返回多行（按分类都返回了）
这时我们需要它只返回外层查询当前需要的那一行的种类，因此使用了where
```

