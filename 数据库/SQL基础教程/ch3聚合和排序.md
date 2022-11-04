注意后面来整理笔记 先跟着练代码

3-1 聚合函数 count，sum，avg

## 聚合函数



### count函数

- count(*)函数：包含NULL

```sql
select count(*)
from product;


select product_type,count(*) AS prodCount
from product
where product_type="办公用品";

//怎么统计商品类型为厨房用具和办公用品的数量？
select product_type,count(*) AS prodCount
from product
where product_type="办公用品" or product_type="厨房用具"
group by product_type;


//为什么这么写可以？

```

- count(字段)：不包含NULL，但是效率最低





- sum，avg，min，max四个聚合函数

  - 唯一不同，是sum/avg只能对数值类型使用。而MAX/MIN原则上适用于任何类型
  - 注意avg的时候，null不参与计算，也不算在分母

  ```sql
  select sum(purchase_price) AS SumPurPrice
  from product;
  
  --其他几个函数同理
  select avg(purchase_price) AS SumPurPrice
  from product;
  
  select min(purchase_price) AS SumPurPrice
  from product;
  
  select max(purchase_price) AS SumPurPrice
  from product;
  ```









## 对表分组

- group by 指定的列称为==聚合键或者分组列==

先将表分割，再汇总。

- 使用聚合函数和group by时需要注意4点：
  - 只能写在select子句中。
  - group by中不能写select中的别名
  - where子句中不能使用group by （且where子句在group by之前）
  - group by 聚合结果是无需的（需要配合order by）





```sql
select <column1>,<column2>...
from <tablename>
group by <column1>,<column2>...



select product_name,product_type
from product
group by product_name,product_type;

select  distinct product_type,product_name
from product;
```

count函数作为聚合函数，统计总行数（总记录数量）,where 给一个条件可以查询某一字段特定条件的行数

怎么统计某个字段多个条件呢？比如` where product_type="办公用品" or product_type="厨房用具"`

- 思考：group by是什么流程，为什么可以做到分组统计？ 应该是先分组 再统计才行~
  - group by会先对表进行切分（按by后面的字段 如`group by product_type;`）按类型分组





### 聚合键中含有NULL

```sql
select purchase_price,count(*)
from product
group by purchase_price;

+----------------+----------+
| purchase_price | count(*) |
+----------------+----------+
|            500 |        1 |
|            320 |        1 |
|           2800 |        2 |
|           5000 |        1 |
|           NULL |        2 |
|            790 |        1 |
+----------------+----------+
6 rows in set (0.00 sec)
```





### where子句中有group by的执行结果

```sql
select product_type,count(*) AS prodCount
from product
where product_type="办公用品" or product_type="厨房用具"
group by product_type;


//与distinct关键字一样，去重
select product_type
from product
group by product_type;


select 
from 
where
group by
```

- sql书写顺序和执行顺序是不同的~





- 使用group by子句中常见错误

  ```sql
  错误写法1:select中写聚合键之外的字段（列）
  select product_name,product_type,count(*) AS prodCount
  from product
  where product_type="办公用品" or product_type="厨房用具"
  group by product_type;
  
  错误写法2: group by语句中使用别名 原因：sql书写顺序和执行顺序是不同的
  
  
  错误3：where子句中使用聚合函数（使用having代替）只有having和order by子句可以使用聚合函数
  ```

  

## 为聚合结果指定条件

学习重点

- 使用count函数等对表聚合时，应该使用having，而不是where
- where执行在group by之前，也就是分组之前
- having书写和执行都在group by之后，因为是指定分组的条件



### having子句



```sql
select product_type,count(*) AS prodCount
from product
group by product_type
having count(*)=2; #只能使用having 聚合函数

select product_type,count(*) AS prodCount
from product
group by product_type
having product_type="办公用品" or product_type="厨房用具"; #使用having

对比
select product_type,count(*) AS prodCount
from product
where product_type="办公用品" or product_type="厨房用具" #使用where
group by product_type;

```





- 使用having例子

  ```sql
  //求所有类型的平均价格
  select product_type,avg(sale_price) as sale_avg_price
  from product
  group by product_type; //按类型分组 可以想象按类型组建了不同的新表
  
  select product_type,avg(sale_price) as sale_avg_price
  from product
  group by product_type
  having avg(sale_price)>1000;
  ```

  

  



- 什么时候适合用having，什么时候适合用where
  - 对聚合键（分组列）的条件用where，用where选出合适的行 然后拿这些行分组

- having和where的执行速度





## 对查询结果排序



order_by出场

Order_by写在select子句的最后，因为排序操作是在结果即将返回时执行。是select之后再排序返回

```sql
select <column1>,<column2>...
from <table name>
order by <排序基准1>,<排序基准2>...
```



- DESC 和ASC









## 练习题

1. 指出语句错误

```sql
select product_id,sum(product_name)
from product
group by product_type
where regist_date>'2009-09-01';

1. sum中只能计算数值类型
2. group by子句的聚合键只能是select后面的列
3. where 应该在group by之前
4. date类型没有单引号

select product_type,sum(sale_price)
from product
where regist_date>2009-09-01
group by product_type;

```

2. 

```sql
select product_type,sum(sale_price) as sale_sum,sum(purchase_price) as purprice_sum
from product
group by product_type
having sum(sale_price)>=sum(purchase_price)*1.5;

加一个排序把
select product_type,sum(sale_price) as sale_sum,sum(purchase_price) as purprice_sum
from product
group by product_type
having sum(sale_price)>=sum(purchase_price)*1.5
order by sale_sum asc;

- 加一个where吧
select product_type,sum(sale_price) as sale_sum,sum(purchase_price) as purprice_sum
from product
where not product_id=0003
group by product_type
having sum(sale_price)>=sum(purchase_price)*1.5
order by sale_sum asc;
```

3. 排序

   ```sql
   select * 
   from product
   order by regist_date DESC,sale_price ASC;
   ```

   

