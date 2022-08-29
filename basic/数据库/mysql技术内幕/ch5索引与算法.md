## 数据结构与算法
### 二分查找



## 二叉查找树与二叉平衡树


## B+树

### 插入操作
- 需要大量的页拆分
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-13-16-54-23.png
  width=450px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>


### 删除操作
- 填充因子控制树的删除变化。



## B+树索引
- **B+树索引不能找到给定键值的具体行数据，只能到数据行所在的页。然后数据库把页读入内存**
### 聚集索引
- **按照每张表的主键构造一棵B+树，叶结点中存放的即是整张表的行记录数据。**
- 数据也是索引的一部分
- 非数据页的索引页中，存放的仅仅是键值以及指向数据页的偏移量，而不是完整的行记录。数据页（叶结点存放了完整的行记录）
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-13-17-50-41.png
width=450px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>
  

### 辅助索引
- 辅助索引中，叶结点不包含行记录的全部数据。叶子节点除了包含键值以外，每个叶子节点的索引号还包含一个书签，书签用来告诉InnoDB存储引擎在哪里查与索引对应的行数据
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-13-17-57-54.png
  width=450px>
  </center>
  
  ```sql
  CREATE TABLE t(
      a INT NOT NULL,
      b VARCHAR(8000),
      c INT NOT NULL,
      PRIMARY KEY (a),
      KEY idx_c(c)
  )ENGINE=INNODB;

  INSERT INTO t SELECT 1,REPEAT('a',7000),-1;
  INSERT INTO t SELECT 2,REPEAT('a',7000),-2;
  INSERT INTO t SELECT 3,REPEAT('a',7000),-3;
  INSERT INTO t SELECT 4,REPEAT('a',7000),-4;


  ```

### B+树索引的管理
1. 索引管理
   ```sql
   #ALTER TABLE创建索引
   ALTER TABLE table_name
   |ADD{INDEX|KEY}[index_name]


   ALTER TABLE tbl_name
   DROP PRIMARY KEY


   #CREATE/DROP INDEX

   CREATE [UNIQUE] INDEX index_name
   [index_type]
   ON tbl_name

   DROP INDEX index_name ON tbl_name


   ALTER TABLE t
   ADD KEY idx_b(b(100)); # 索引前100个字段

   ALTER TABLE t
   ADD KEY idx_ac(a,c);
   ```

- Cardinality值非常关键，优化器会根据这个值来判断是否使用这个索引




2. Fast Index Creation
   - 只限于辅助索引的创建和删除，对于主键索引仍然需要重建一张表


3. Online Schema Change




## B+树的使用
### 不同应用中B+树索引的使用


### 联合索引
```sql
CREATE TABLE buy_log(
    userid INT UNSIGNED NOT NULL,
    buy_date DATE
)ENGINE=INNODB;

INSERT INTO buy_log VALUE (1,'2009-01-01');
INSERT INTO buy_log VALUE (2,'2009-01-01');
INSERT INTO buy_log VALUE (3,'2009-01-01');
INSERT INTO buy_log VALUE (1,'2009-02-01');
INSERT INTO buy_log VALUE (3,'2009-02-01');
INSERT INTO buy_log VALUE (1,'2009-03-01');
INSERT INTO buy_log VALUE (1,'2009-04-01');

ALTER TABLE buy_log ADD KEY (userid);
ALTER TABLE buy_log ADD KEY (userid,buy_date);

SELECT * FROM buy_log WHERE userid=2;

SELECT COUNT(*) FROM buy_log;
```



### 覆盖索引
- 从辅助索引中就可以得到查询的记录，而不需要查询聚集索引中的记录

### 不使用索引
- 范围查找，JOIN操作时




### Multi-Range Read优化


## 哈希算法
- 哈希表是由直接寻址表改进而来。
- 直接寻值表的问题：如果最大值为n，那么这个表的大小也得至少为n，而我实际的的元素并不多可能只有m个
- 哈希表如果两个不同的值映射到了同一个槽上，称之为碰撞，**数据库一般使用链接法解决**
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-14-15-25-29.png
  width=450px>
  </center>
  

  
