2-1 select语句基础

- 列查询
- 查询表所有列
- 列别名
- 常数查询
- 删除重复行 distinct
- where语句选择记录
- 注释书写

2-2算术运算符和比较运算符



2-3逻辑运算符



## select方法



```sql
//插入数据
INSERT INTO Product VALUES 
('0001', 'T恤' ,'衣服', 1000, 500, '2009-09-20'),
('0002', '打孔器', '办公用品', 500, 320, '2009-09-11'),
('0003', '运动T恤', '衣服', 4000, 2800, NULL),
('0004', '菜刀', '厨房用具', 3000, 2800, '2009-09-20'),
('0005', '高压锅', '厨房用具', 6800, 5000, '2009-01-15'),
('0006', '叉子', '厨房用具', 500, NULL, '2009-09-20'),
('0007', '擦菜板', '厨房用具', 880, 790, '2008-04-28'),
('0008', '圆珠笔', '办公用品', 100, NULL, '2009-11-11');

```



- select

```sql
select product_id,product_name,purchase_price from Product;
select product_id,purchase_price,product_name from Product;

select * from Product;//*代表全部的意思 按建表顺序
```



- 设置别名 关键字SA

  ```sql
  select product_id AS id, #可以使用中文，中文用""括起来
  product_name AS "商品名称" from Product;
  +------+--------------+
  | id   | 商品名称     |
  +------+--------------+
  | 0001 | T恤          |
  | 0002 | 打孔器       |
  | 0003 | 运动T恤      |
  | 0004 | 菜刀         |
  | 0005 | 高压锅       |
  | 0006 | 叉子         |
  | 0007 | 擦菜板       |
  | 0008 | 圆珠笔       |
  +------+--------------+
  ```

  

- 常数的查询 (书写常数，还可以书写计算式)

  ```sql
  select '商品'AS string,28 AS number,'2009-02-01' AS date,
  product_id,product_name 
  from Product;
  
  +--------+--------+------------+------------+--------------+
  | string | number | date       | product_id | product_name |
  +--------+--------+------------+------------+--------------+
  | 商品   |     28 | 2009-02-01 | 0001       | T恤          |
  | 商品   |     28 | 2009-02-01 | 0002       | 打孔器       |
  | 商品   |     28 | 2009-02-01 | 0003       | 运动T恤      |
  | 商品   |     28 | 2009-02-01 | 0004       | 菜刀         |
  | 商品   |     28 | 2009-02-01 | 0005       | 高压锅       |
  | 商品   |     28 | 2009-02-01 | 0006       | 叉子         |
  | 商品   |     28 | 2009-02-01 | 0007       | 擦菜板       |
  | 商品   |     28 | 2009-02-01 | 0008       | 圆珠笔       |
  +--------+--------+------------+------------+--------------+
  8 rows in set (0.00 sec)
  ```

  

- 过滤重复数据 distinct关键字

  ```sql
  select distinct product_type
  from product;
  ```

  - NULL会被认为是一列数据被保留

    ```sql
    select distinct purchase_price
    from product;
    +----------------+
    | purchase_price |
    +----------------+
    |            500 |
    |            320 |
    |           2800 |
    |           5000 |
    |           NULL |
    |            790 |
    +----------------+
    ```

  - distinct关键字只能用在第一个列名之前

- 根据where 语句来选择记录

  ```sql
  select <columnname>...
  from ...
  where <条件表达式> ;
  
  --注释
  ```

  - where子句必须紧跟在from之后
  - 

- 注释
  - 使用 -- 注释一行
  - 使用 /\*...\*/ 注释多行







## 算术运算符和比较运算符



### 算术运算符

- select子句中可以使用常数或者表达式

  ```sql
  select purchase_price, purchase_price*2 AS pricex2
  from Product;
  +----------------+---------+
  | purchase_price | pricex2 |
  +----------------+---------+
  |            500 |    1000 |
  |            320 |     640 |
  |           2800 |    5600 |
  |           2800 |    5600 |
  |           5000 |   10000 |
  |           NULL |    NULL |
  |            790 |    1580 |
  |           NULL |    NULL |
  +----------------+---------+
  8 rows in set (0.00 sec)
  ```

  - 所有包含NULL的运算结果都是NULL，不管加减乘除。就算是5+NULL=NULL(显然不符逻辑，第六章解决），0/NULL都是

- from语句并不是不可缺少。直接计算。

  ```sql
  select (100+200) * 3 AS calculation;
  +-------------+
  | calculation |
  +-------------+
  |         900 |
  +-------------+
  ```

  

### 比较运算符

- 用在`where`条件子句中

| 运算符 | 含义     |
| ------ | -------- |
| =      | 相等     |
| <>     | 不等     |
| >=     | 大于等于 |
| >      | 大于     |
| <=     | 小于等于 |
| <      | 小于     |



- 对字符串使用不等号
  - 字符串的数据按照字典顺序进行排序，不能与数字大小顺序混淆（如果要将字符串和数字比较。数据库要先把字符串隐式转换转为数子比较）
  - 如果是字符和字符比较大小，会按字典顺序比较，如果长度不同，只会比较长度相同的字段



-  不能对NULL使用比较运算符，等号，不等号都不行 例如`where price=NULL;`

  ```sql
  select *
  from product
  where purchase_price<>NULL; //逻辑虽然是不等于NULL
  
  Empty set (0.00 sec)
  ```

  







## 逻辑运算符

and

or

not

- 主要注意括号，有时and和or的顺序需要括号





## 练习题

```sql
--修改下列名
alter table Product change column regist_data regist_date date;

--2.1
select product_name,regist_date
from Product
where regist_date>2009-04-28;


--2.2
--1) 无 Empty set (0.00 sec)
select *
from product
where purchase_price=NULL;
--2)
select *
from product
where purchase_price<>NULL;


--3）
select product_name,sale_price,purchase_price
from product
where sale_price-purchase_price>=500;

--4)
select product_name,product_type,sale_price*0.9-purchase_price AS profit
from product
where sale_price*0.9>=100 and (product_type="办公用品" or product_type="厨房用具");
```

