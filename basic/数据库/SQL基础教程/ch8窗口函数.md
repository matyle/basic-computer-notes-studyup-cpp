### 范围

- 窗口函数

  OLAP函数（online analytical processing实时分析处理函数）

  - 何为窗口函数

    即实时处理数据的一些函数，例如之前的聚合函数

    1. Sum,avg,count,max,min等聚合函数
    2. Rank,dense_rank,row_number专用窗口函数

  - 怎么用？

    - 关键是理解`partition by`和`order by`
    - `partition by`是对表横向的分类（类似于group by，都是划分表分组），但是`partition by`不具备汇总能力
    - `partition by`分组后的记录集合称为窗口（代表范围）
    - `order by`是纵向上对表排序

    ```sql
    <窗口函数> over (partition by<column清单>
                		order by<排序用column清单)
      
      
    //根据商品种类 rank排级
     select product_name,product_type,sale_price,
     rank() over(partition by product_type 
                order by sale_price asc)as ranking
     from product;
     +--------------+--------------+------------+---------+
    | product_name | product_type | sale_price | ranking |
    +--------------+--------------+------------+---------+
    | 圆珠笔       | 办公用品     |        100 |       1 |
    | 打孔器       | 办公用品     |        500 |       2 |
    | 叉子         | 厨房用具     |        500 |       1 |
    | 擦菜板       | 厨房用具     |        880 |       2 |
    | 菜刀         | 厨房用具     |       3000 |       3 |
    | 高压锅       | 厨房用具     |       6800 |       4 |
    | T恤          | 衣服         |       1000 |       1 |
    | 运动T恤      | 衣服         |       4000 |       2 |
    +--------------+--------------+------------+---------+
    8 rows in set (0.00 sec)
    
     select product_name,product_type,sale_price,
     rank() over(partition by product_type
                order by  sale_price desc)as ranking
     from product;
     
     #不使用 partition by
      select product_name,product_type,sale_price,
     rank() over(order by sale_price)as ranking
     from product;
    +--------------+--------------+------------+---------+
    | product_name | product_type | sale_price | ranking |
    +--------------+--------------+------------+---------+
    | 圆珠笔       | 办公用品     |        100 |       1 |
    | 打孔器       | 办公用品     |        500 |       2 |
    | 叉子         | 厨房用具     |        500 |       2 |
    | 擦菜板       | 厨房用具     |        880 |       4 |
    | T恤          | 衣服         |       1000 |       5 |
    | 菜刀         | 厨房用具     |       3000 |       6 |
    | 运动T恤      | 衣服         |       4000 |       7 |
    | 高压锅       | 厨房用具     |       6800 |       8 |
    +--------------+--------------+------------+---------+
    ```

    - 是否使用`partition by`是可选的

  - 注意
    - 专用函数参数为空

### 专用窗口函数

- rank函数：若存在相同位次记录，会跳过之后的位次
- dense_rank函数：若存在相同位次记录，不会跳过之后的位次
- Row_number函数：赋予连续记录位次

```sql
  select product_name,product_type,sale_price,
 rank() over(order by sale_price)as ranking
 from product;
 +--------------+--------------+------------+---------+
| product_name | product_type | sale_price | ranking |
+--------------+--------------+------------+---------+
| 圆珠笔       | 办公用品     |        100 |       1 |
| 打孔器       | 办公用品     |        500 |       2 |
| 叉子         | 厨房用具     |        500 |       2 |
| 擦菜板       | 厨房用具     |        880 |       4 |
...

#使用dense_rank
   select product_name,product_type,sale_price,
 dense_rank() over(order by sale_price)as ranking
 from product;
 +--------------+--------------+------------+---------+
| product_name | product_type | sale_price | ranking |
+--------------+--------------+------------+---------+
| 圆珠笔       | 办公用品     |        100 |       1 |
| 打孔器       | 办公用品     |        500 |       2 |
| 叉子         | 厨房用具     |        500 |       2 |
| 擦菜板       | 厨房用具     |        880 |       3 |
...

#使用row_number
   select product_name,product_type,sale_price,
 row_number() over(order by sale_price)as ranking
 from product;
 +--------------+--------------+------------+---------+
| product_name | product_type | sale_price | ranking |
+--------------+--------------+------------+---------+
| 圆珠笔       | 办公用品     |        100 |       1 |
| 打孔器       | 办公用品     |        500 |       2 |
| 叉子         | 厨房用具     |        500 |       3 |
...
```



### 窗口函数适用范围

- where子句中不能使用聚合函数
- where/group by子句中不能使用窗口函数

因为在select之外使用窗口函数没有意义。窗口函数本身是对where/group by处理的结果进行的操作



### 聚合函数作为窗口函数

- 以自身记录（当前记录）作为基准统计，就是将聚合函数当做窗口函数最大特征
- sum作为窗口函数：当前和的累加，就像和在移动一样，所以叫窗口？（类似前缀和其实）。即累计的统计方法
- avg 
- 计算移动平均（重点）

```sql
   select product_name,product_type,sale_price,
 sum(sale_price) over(order by product_id)as current_sum
 from product;
 +--------------+--------------+------------+-------------+
| product_name | product_type | sale_price | current_sum |
+--------------+--------------+------------+-------------+
| T恤          | 衣服         |       1000 |        1000 |
| 打孔器       | 办公用品     |        500 |        1500 |
| 运动T恤      | 衣服         |       4000 |        5500 |
| 菜刀         | 厨房用具     |       3000 |        8500 |
| 高压锅       | 厨房用具     |       6800 |       15300 |
| 叉子         | 厨房用具     |        500 |       15800 |

# current_avg
 select product_name,product_type,sale_price,
 avg(sale_price) over(order by product_id)as current_avg
 from product;
 +--------------+--------------+------------+-------------+
| product_name | product_type | sale_price | current_sum |
+--------------+--------------+------------+-------------+
| T恤          | 衣服         |       1000 |   1000.0000 |
| 打孔器       | 办公用品     |        500 |    750.0000 |
| 运动T恤      | 衣服         |       4000 |   1833.3333 |
| 菜刀         | 厨房用具     |       3000 |   2125.0000 |
| 高压锅       | 厨房用具     |       6800 |   3060.0000 |

# 移动平均
 select product_name,product_type,sale_price,
 avg(sale_price) over(order by product_id
                     rows 2 preceding)as moveavg
 from product;
 
 +--------------+--------------+------------+-------------+
| product_name | product_type | sale_price | current_sum |
+--------------+--------------+------------+-------------+
| T恤          | 衣服         |     1000 |   1000.0000 | 1000/1
| 打孔器       | 办公用品     |      500 |    750.0000 |  (1000+500)/2
| 运动T恤      | 衣服         |     4000 |   1833.3333 | (1000+500+4000)/3
| 菜刀         | 厨房用具     |     3000 |   2500.0000 |(500+4000+3000)/3
| 高压锅       | 厨房用具     |     6800 |   4600.0000 |(4000+3000+4600)/3
...


 select product_name,product_type,sale_price,
 avg(sale_price) over(order by product_id
                     rows 5 following)as current_sum
 from product;
```



- 计算移动平均
  - 框架：汇总范围即` rows 2 preceding` rows（行），preceding（之前），指**截止到之前~行 **（这里是自身+之前2行）。即最靠近的3行
    - 自身（当前记录)
    - 之前1行记录
    - 之前2行记录
    - 可以将`preceding`改为`following`就是指**截止到之后~行**(mysql报错)







## grouping 运算符(分类时才使用)

- 使用group by函数和聚合函数无法同时得到小计和合计



同时得到合计行

- 方法一：使用union all算两次然后，两个表相加
- 方法二：使用grouping运算符
  - rollup
  - cube
  - grouping sets



### ROLLUP

一次计算出不同聚合键组合的结果

```sql
select product_type,sum(sale_price) as sum_price
from product
group by product_type with rollup;
+--------------+-----------+
| product_type | sum_price |
+--------------+-----------+
| 办公用品     |       600 |
| 厨房用具     |     11180 |
| 衣服         |      5000 |
| NULL         |     16780 |
+--------------+-----------+
4 rows in set (0.00 sec)
```

### grouping函数

该函数在上一个函数基础上能够将NULL值改变。如果是NULL就返回1

```sql
select grouping(product_type) as product_type
grouping(regist_date) as regist_date,sum(sale_price) as sum_price
from product
group by product_type,regist_date with rollup;
```

