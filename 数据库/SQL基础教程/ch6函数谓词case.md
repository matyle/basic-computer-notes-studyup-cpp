## 函数



### 函数种类

- 算术函数

  建表

  numeric(**M,D**) 类型 浮点数，decimal

  ```sql
  create table sampleMath
  (
    m numeric (10,3),
    n integer,
    p integer
  ```

  

  - 绝对值函数abs
  - 求余mod(被除数，除数)
  - round四舍五入

- 字符串函数

  - || 拼接函数 `字符串1||字符串2` 。注意mysql中使用`concat`函数
  - `length`字符串长度,mysql中还有`char_length`函数
  - `lower/upper`小写/大写转换
  - `replace` 替换字符串`replace(对象字符串，替换前字符串，替换后字符串)`
  - `substring` 字符串截取

  ```sql
  select str1,str2,concat(str1,str2) as str_concat
  from samplestr;
  
  select str1,str2,concat(str2,str1) as str_concat
  from samplestr;
  
  //计算多字节
  select str1,length(str1) as len
  from samplestr;
  
  ## 中文显示1字符
  select str1,char_length(str1) as len
  from samplestr; 
  
  
  ```

  

- 日期函数

  - `current_date`当前日期

  - `Current_time`当前时间

  - `current_timestamp `当前日期和时间

  - `extract`截取日期元素 年月日，时分秒

    ```sql
    
    select str1,char_length(str1) as len,current_date as day
    from samplestr; 
    +-----------+------+------------+
    | str1      | len  | day        |
    +-----------+------+------------+
    | opx       |    3 | 2021-08-02 |
    | abcdefabc |    9 | 2021-08-02 |
    | micmic    |    6 | 2021-08-02 |
    +-----------+------+------------+
    
    mysql> select current_timestamp;
    +---------------------+
    | current_timestamp   |
    +---------------------+
    | 2021-08-02 10:49:36 |
    +---------------------+
    1 row in set (0.00 sec)
    
    select current_timestamp,
    extract(year from current_timestamp) as year,
    extract(month from current_timestamp) as month;
    ...
    +---------------------+------+-------+
    | current_timestamp   | year | month |
    +---------------------+------+-------+
    | 2021-08-02 10:51:50 | 2021 |     8 |
    +---------------------+------+-------+
    
    ```

  - cast 类型转换，如 `cast('101' as integer) as int_col`

  - coalesce 将NULL转换为其他值 `coalesce(str2,'NULL');

### 条件函数

```sql
select 
if(age>=25,"25岁及以上","25岁以下") as age_cnt,
count(*) as Number
from user_profile
GROUP BY age_cnt
```

运算符case……when 条件 then ……else ……end as……

##谓词

什么是谓词？` 返回值为真值的函数`，如true/false/unknown

有哪些谓词？

like

between

is null,is not null 

in

exist





### like

模糊查询

```sql
select strcol
from samplelike
where strcol like '%ddd';

#中间为ddd
select strcol
from samplelike
where strcol like '%ddd%';

select strcol
from samplelike
where strcol like 'ddd%';

select strcol
from samplelike
where strcol like 'abc__';
```



### between

注意：包含临界区



### IN

IN 类似于or，是or的简便用法

in(100，200，300) //or集合 == 100 or 200 or 300

not in()//排除

- in和子查询



### exists

通常指定关联子查询为`exists`的参数

```sql
select product_name,sale_price
from product as p
where exists(select *
      	from shopproduct as sp
      where sp.shop_id = '000c' and p.product_id = sp.product_id);
      
exists只关心是否存在，子查询中可以使用常量
select product_name,sale_price
from product as p
where exists(select 1
      	from shopproduct as sp
      where sp.shop_id = '000c' and p.product_id = sp.product_id);
      
      
 primary key 的记录可以重复吗？
 
create table  test(
  id integer primary key not null,
  name varchar(20) not null,
  product varchar(100) not null
);
错误 主键1 重复
insert into test values (1,"tom","pen");
insert into test values (1,"tom","pad");


多个主键的情况 
create table  test2(
  id integer  not null,
  name varchar(20) not null,
  shop_id integer not null,
  product varchar(100) not null,
  primary key(id,shop_id)
);
//primary key是两个字段，只要后面那个不同就行
insert into test2 values (1,"tom",002,"pen");
insert into test2 values (1,"tom",002,"mpen");//错误，两个字段都一样 
ERROR 1062 (23000): Duplicate entry '1-2' for key 'test2.PRIMARY'

insert into test2 values (1,"tom",003,"pad"); //正确
‘
insert into test2 values (2,"tom",003,"pad"); //正确

```





`not exists` 不存在



## case表达式

条件表达式



### 简单case表达式



```sql
case <求值表达式> 
when <表达式> then <表达式>
	when <表达式> then <表达式>
	when <表达式> then <表达式>
	...
	else <表达式>
end
```

简单中，如果对于不同列的条件就不行了

只能用搜索case表达式

### 搜索case表达式

```sql
case when <求值表达式> then <表达式>
	when <求值表达式> then <表达式>
	when <求值表达式> then <表达式>
	...
	else <表达式>
end
```

求值表达式类似`列=值`，条件语句使用`=,< >`，like，in ，not in等



```sql
select product_name,
	case when product_type='衣服'
	then concat('A:',product_type)
	when product_type='办公用品'
	then concat('B:',product_type)
	when product_type='厨房用具'
	then concat('C:',product_type)
	else null
	end as ab_product_type
	from product;
```



`else` 可以省略 但是最好还是写上

`End`不可省略







## 练习



```sql

select product_name,purchase_price
from product
where purchase_price not in (500,2800,5000); //返回空集

select product_name,purchase_price
from product
where purchase_price not in (500,2800,5000,null); //返回空集
```





2.

```sql
select 
from product;

when bewteen() then count(*) as mid_price
when sale_price<=1000 then count(*) as high_price
else null


select
(
 select count(*)
from product
where sale_price<=1000
) as low_price;
(
 select count(*)
from product
where sale_price between 1001 and 3000
) as mid_price,
(
 select count(*)
from product
where sale_price>=3001
) as high_price;

方法2：
select
count(case 
      when sale_price<=1000 
      then sale_price 
      else null 
      end)as low_price,
count(case
     when sale_price between 1001 and 3000
     then sale_price
     else null
     end)as mid_price
from product;



select
count(case 
      when sale_price<=1000 
      then true
      else null 
      end)as low_price
from product;
```



