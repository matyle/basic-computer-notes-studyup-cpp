表是关于特定实体的数据集合，关系数据库模型的核心


## 索引组织表
- 什么是索引组织表？
  - 根据**主键顺序组织**存放的。每张表都有一个主键
  - 那如果没有定义主键呢？
    - 自动创建主键
    - 首先判断表中是否有非空（NOT NULL）的**唯一索引**，如果有就设置该列为主键
    - 如果不符号，INnoDB自动创建6字节大小的指针，指针指向作为主键？

- 主键的选择是根据索引顺序，而不是建表的顺序
  ```sql
  CREATE TABLE z(
      a INT NOT NULL,
      b INT NULL,
      c INT NOT NULL,
      d INT NOT NULL,
      UNIQUE KEY(b),
      UNIQUE KEY(d),
      UNIQUE KEY(c)
  );
  INSERT INTO z SELECT 1,2,3,4;
  INSERT INTO z SELECT 5,6,7,8;
  INSERT INTO z SELECT 9,10,11,12;

  SELECT a,b,c,d,_rowid FROM z;
  mysql> SELECT a,b,c,d,_rowid FROM z;
    +---+------+----+----+--------+
    | a | b    | c  | d  | _rowid |
    +---+------+----+----+--------+
    | 1 |    2 |  3 |  4 |      4 |
    | 5 |    6 |  7 |  8 |      8 |
    | 9 |   10 | 11 | 12 |     12 |
    +---+------+----+----+--------+
  ```
  - bcd都有唯一索引,但是b不是非空的，因此不会选他，由因为cd中，d先建立索引，所以d是主键
  - _rowid可以显示主键，但是只能显示单列主键的情况
  ```sql
    CREATE TABLE a(
      a INT,
      b INT,
      PRIMARY KEY(a,b)
  )ENGINE=InnoDB;
    mysql> SELECT a,b,_rowid FROM a;
    ERROR 1054 (42S22): Unknown column '_rowid' in 'field list'
  ```


## InnoDB逻辑存储结构
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-07-16-16-45.png
width=450px>
</center>

- 表空间由段（segment），区，页（page）/块（block）组成
  

### 表空间
- InnoDB在默认情况下有一个共享表空间
  - 如果开启innodb_file_per_table参数，则每张表的数据可以单独放到一个表空间
  - 但是单独放的表空间内只放数据，索引和插入缓冲bitmap页。其他类型如回滚，插入缓冲索引页，事务信息等还是放共享表空间ibdata1
  - ROLLBCKE


### 段
- 表空间由段组成
- 常见的段有索引段，数据段，回滚段


### 区
- 区组成了段
- 区是由连续的页组成的空间，每个区大小为1MB，页大小默认为16KB，因此需要64个连续页。当然页是可以压缩的。
- 同样开始了innodb_file_per_table之后，默认表为96KB。区中64个页为啥不是至少1MB呢？
  ```sql
  CREATE TABLE t1(
      col1 INT NOT NULL AUTO_INCREMENT,
      col2 VARCHAR(7000),
      PRIMARY KEY(col1))ENGINE=InnoDB;
  )

  -rw-r-----  1 _mysql  _mysql   112K  7  8 14:53 /usr/local/mysql/data/crashcourse/t1.ibd

  INSERT t1 SELECT NULL,REPEAT('a',7000);
  INSERT into t1 SELECT NULL,REPEAT('a',7000);//
    插入两条还是112k

    继续操作，为了方便，建立一个导入的存储过程（类似函数）
    CREATE PROCEDURE load_t1(count INT UNSIGNED)
    BEGIN
    DECLARE s INT UNSIGNED DEFAULT 1;  #定义
    DECLARE c VARCHAR(7000) DEFAULT REPEAT('a',7000); 
    WHILE s <= count DO
    INSERT t1 SELECT NULL,c;
    SET s = s+1;
    END WHILE;
    END;

  ```





### 页
- 页也可以被称为块。也是INnoDB磁盘管理的最小单位
- 默认页大小为16KB，可以通过参数innodb_page_szie
- 常见页
  - 数据页(B-Tree Node)
  - undo页
  - 系统页
  - 事务数据页
  - ...


### 行
- 面向行的，按行存储。
- Compact格式常用

### Compact行记录格式
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-08-15-29-35.png
width=450px>
</center>

```sql
CREATE TABLE mytest2(
    t1 VARCHAR(10),
    t2 VARCHAR(10),
    t3 CHAR(10),
    t4 VARCHAR(10)
)ENGINE=INNODB CHARSET=LATIN1 ROW_FORMAT=COMPACT;

INSERT INTO mytest2
VALUE('a','b','bb','ccc');

INSERT INTO mytest2
VALUE('d','e','ee','fff');

INSERT INTO mytest2
VALUE('d',NULL,NULL,'fff');

mysql> SELECT* FROM mytest2\G;
*************************** 1. row ***************************
t1: a
t2: b
t3: bb
t4: ccc
*************************** 2. row ***************************
t1: d
t2: e
t3: ee
t4: fff
*************************** 3. row ***************************
t1: d
t2: NULL
t3: NULL
t4: fff
3 rows in set (0.00 sec)

0000c070  73 75 70 72 65 6d 75 6d  01 81 00 00 10 ff f1 00  |supremum........|
0000c080  00 00 02 00 00 00 00 00  00 00 1c 00 00 00 00 45  |...............E|
0000c090  82 82 00 00 00 a4 02 58  00 00 01 a8 00 00 01 01  |.......X........|
0000c0a0  78 9c 6d 90 cd 4e c3 30  10 84 df c5 67 d4 e6 07  |x.m..N.0....g...|
0000c0b0  a1 12 2b 17 c4 85 0b 27  6e 08 59 1b 7b 5b 5c c5  |..+....'n.Y.{[\.|
0000c0c0  76 f0 6e 2a 42 94 77 c7  49 20 02 a9 c7 9d 19 af  |v.n*B.w.I ......|
0000c0d0  bf d9 51 b8 81 3e 5a a3  2e 18 c9 06 af ac 11 d5  |..Q..>Z.........|
0000c0e0  21 cb 8a f2 46 98 4d dd  24 32 f6 9f 96 df 2f b1  |!...F.M.$2..../.|
```
- 注意：上面命令都是需要查看磁盘文件，很有可能还在缓冲区，所以看不到文件


### Compressed和Dynamic行记录格式


### CHAR的行存储结构

- VARCHAR是存储变长长度的字符类型，CHAR是存储固定长度的字符类型
  ```sql
  CREATE TABLE j(
      a CHAR(2)
  )CHARSET=GBK,ENGINE=InnoDB;

  INSERT INTO j SELECT 'ab';
  INSERT INTO j SELECT '我们';
  INSERT INTO j SELECT 'a';
  SELECT a,CHAR_LENGTH(a),LENGTH(a)
  FROM j\G;

    | a      | CHAR_LENGTH(a) | LENGTH(a) |
    +--------+----------------+-----------+
    | ab     |              2 |         2 |
    | 我们   |              2 |         4 |
    | a      |              1 |         1 |
    +--------+----------------+-----------+


  ```



## innodb数据页结构
- InnoDB数据页的内部存储结构
- InnoDB数据页由以下7个部分组成：
  - File Header 文件头 38字节
  - Page Header 页头 56字节
  - Infimun 和Supermum Records 8字节
  - User Records用户记录，即行记录
  - Free Space空闲空间
  - Page Directory页目录
  - File Trailer文件结尾信息
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-09-14-19-09.png
width=350px>
</center>


### File Header
- 记录页的一些头信息 8个部分38字节
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-09-14-25-04.png
  width=450px>
  </center>
    <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-09-14-25-22.png
  width=450px>
  </center>

  - FIL_PAGE_TYPE中，FIL_PAGE_INDEX的16进制0x45BF代表数据页。


### Page Header
- 这部分用来记录数据页的状态信息 14个部分组成 56字节
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-09-14-32-06.png
  width=450px>
  </center>
  

### Infimum和Supermum Record
- 虚拟行记录限定记录的边界
- Infimum是比该页任何主键值都要小的值，Supermum指比任何可能大的值还要大的值。
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-09-14-34-33.png
  width=450px>
  </center>
  
### User Record和Free Record
- InnoDB存储引擎表总是B+树索引阻止的。
- User Record实际存储行记录的内容
- Free Record空闲空间链表，是一个链表结构，在一条记录被删除后，该空间会被加入到空闲链表中

### Page Directory
- 页目录中存放了记录的相对位置（不是偏移），记录指针称为槽（slot），并不是每个记录对应一个槽（有点像多级页表的感觉）
- 比如一个槽中4个记录，(i,d,c,b,e,g,l,h,f,j,k,a),solts中的记录可能是(a,e,i);然后根据next指针查找
- B+树索引本身不能找到具体的记录，能找到该记录对应的页。然后数据库把页读入内存，然后通过Page Directory来查找记录，利用二叉查找


### File Trailer
- 检测页是否已经完整写入了磁盘
- File Trailer只有FIL_PAGE_END_LSN部分
- 每次从磁盘中读取页都会利用File Trailer检测

### InnoDB数据页结构示例分析 （不是很懂）
```sql
DROP TABLE IF EXISTS t;
CREATE TABLE t(
    a INT UNSIGNED NOT NULL AUTO_INCREMENT,
    b CHAR(10),
    PRIMARY KEY(a)
)ENGINE=InnoDB CHARSET=UTF8;

DELIMITER $$
//小写字母
//创建存储过程
CREATE PROCEDURE load_t (count INT UNSIGNED)
BEGIN
SET @c = 0;
WHILE @c < count DO
INSERT INTO t
SELECT NULL,REPEAT(CHAR(97+RAND()*26),10); 
SET @c=@c+1;
END WHILE;
END;
$$

00010000  b3 a2 57 4a 00 00 00 04  ff ff ff ff ff ff ff ff  |..WJ............|
00010010  00 00 00 00 01 2b 56 2d  45 bf 00 00 00 00 00 00  |.....+V-E.......|
00010020  00 00 00 00 00 19 00 1a  0d c0 80 66 00 00 00 00  |...........f....|
00010030  0d a5 00 02 00 63 00 64  00 00 00 00 00 00 00 00  |.....c.d........|
00010040  00 00 00 00 00 00 00 00  00 b4 00 00 00 19 00 00  |................|




00013fc0  00 00 00 00 00 70 0d 1d  0c 95 0c 0d 0b 85 0a fd  |.....p..........|
00013fd0  0a 75 09 ed 09 65 08 dd  08 55 07 cd 07 45 06 bd  |.u...e...U...E..|
00013fe0  06 35 05 ad 05 25 04 9d  04 15 03 8d 03 05 02 7d  |.5...%.........}|
00013ff0  01 f5 01 6d 00 e5 00 63  b3 a2 57 4a 01 2b 56 2d  |...m...c..WJ.+V-|
```


## Named File Formats机制
```sql

```


## 约束
- 关系型数据库和文件系统的不同点之一是：关系型数据库本身可以保证存储数据的完整性
- **约束机制保证数据的完整性**
- **实体完整性**保证表中有一个主键，通过定义Primary key或Unique key约束来保证实体的完整性。
- 域完整性：保证数据每列的值满足特定的条件。
  - 选择合适的数据类型确保一个数据值满足特定条件
  - 外键约束
  - 编写触发器
  - 利用DEFAULT
- 参照完整性：保证两张表之间的关系
  - 定义外键
  - 编写触发器
- InnoDB提供几种约束
  - Primary key
  - Unique key
  - Foreign Key
  - Default
  - NOT NULL

### 约束的创建和查找
- 表建立时进行约束定义
- 利用ALTER TABLE 命令来进行约束创建
- 对于唯一索引的约束：CREATE UNIQUE INDEX
- 主键约束，默认约束名PRIMARY
```sql
CREATE TABLE u(
    id INT,
    name VARCHAR(20),
    id_card CHAR(18),
    PRIMARY KEY(id),
    UNIQUE KEY(name)
);

SELECT constraint_name,constraint_type
FROM information_schema.TABLE_CONSTRAINTS
WHERE table_schema='crashcourse ' AND table_name='u';


ALTER TABLE u
ADD UNIQUE KEY uk_id_card(id_card);

mysql> SELECT constraint_name,constraint_type FROM information_schema.TABLE_CONSTRAINTS WHERE table_schema='crashcourse ' AND table_name='u'\G;
*************************** 1. row ***************************
CONSTRAINT_NAME: name
CONSTRAINT_TYPE: UNIQUE
*************************** 2. row ***************************
CONSTRAINT_NAME: PRIMARY
CONSTRAINT_TYPE: PRIMARY KEY
*************************** 3. row ***************************
CONSTRAINT_NAME: uk_id_card
CONSTRAINT_TYPE: UNIQUE
3 rows in set (0.00 sec)

# 创建外键
CREATE TABLE p(
    id INT,
    u_id INT,
    PRIMARY KEY(id),
    FOREIGN KEY(u_id) REFERENCES p(id)
);
```



### 约束和索引的区别
- 当用户创建了唯一索引就创建了一个唯一的约束
- 约束是一个逻辑概念，保证数据的完整性。索引是一种数据结构，有逻辑上的概念也代表着物理存储的方式


### 对错误数据的约束
- 允许非法或者错误的数据的插入与更新。
  ```sql
  CREATE TABLE c(
      id INT NOT NULL,
      date DATE NOT NULL
  );
  INSERT INTO c2
  SELECT NULL,'2009-02-30';
  ```

### ENUM和SET约束
```sql
CREATE TABLE d(
      id INT,
      sex ENUM('male','female')
  );
INSERT INTO d
SELECT 1,'female';

Query OK, 1 row affected (0.01 sec)
Records: 1  Duplicates: 0  Warnings: 0

mysql> INSERT INTO d SELECT 1,'f';
ERROR 1265 (01000): Data truncated for column 'sex' at row 1
```
- 即只能是ENUM里面的值


### 触发器与约束
- 触发器作用是在执行INSERT,DELETE,UPDATE命令之前或之后自动调用SQL命令或者存储过程
```sql
CREATE
[DEFINER = {user | CURRENT_USER}]
TRIGGER tirgger_name BFFORE|AFTER INSERT|UPDATE|DELETE
ON tbl_name FOR EACH ROW trigger_stmt



CREATE TABLE usercash(
    userid INT NOT NULL,
    cash INT UNSIGNED NOT NULL);

INSERT INTO usercash
SELECT 1,1000;

UPDATE usercash
SET cash=cash-(-20) WHERE userid=1; #语意是错误的，不可能减去一个负值。但是mysql不会报错

//新建一个表
CREATE TABLE usercash_err_log(
    userid INT NOT NULL,
    old_cash INT UNSIGNED NOT NULL,
    new_cash INT UNSIGNED NOT NULL,
    user VARCHAR(30),
    time DATETIME
);

DELIMITER $$ #啥意思？
CREATE TRIGGER tgr_usercash_update BEFORE UPDATE ON usercash
FOR EACH ROW
BEGIN
IF new.cash - old.cash > 0 THEN
INSERT INTO usercash_err_log
SELECT old.userid,old.cash,new.cash,USER(),NOW(); #数据库语句
SET new.cash = old.cash;
END IF;
ELSE
SET new.cash = new.cash;
END ELSE
END;
$$


DELIMITER ; //以;为分割
```
- 脚本中有多行，我们希望不按照分号来一句一句执行，希望一段代码作为一个整体执行。
在脚本最前面加上 delimiter $$



### 外键约束
- 被引用的表称为父表，引用的表称为子表。外键定义时的ON DELETE和ON UPDATE表示对父表进行DELETE和UPDATE时，对子表进行操作
  - CASEADE：子表与父表进行相同的操作
  - SET NULL：父表执行上述操作时，子表置空
  - NO ACTION ：抛出错误，不允许此类操作发生
  - RESTRICT：抛出错误，不允许此类操作发生
  ```sql
  CREATE TABLE parent(
      id INT NOT NULL,
      PRIMARY KEY(id)
  )ENGINE=INNODB;

  CREATE TABLE child(
      id INT, 
      parent_id INT,
      FOREIGN KEY(parent_id) REFERENCES parent(id)
  )ENGINE=INNODB;
  ```


## 视图
- 一个命名的虚表，可以当表使用。视图的数据没有实际的物理存储

### 视图的作用
```sql
CREATE TABLE t(id INT);

CREATE VIEW v_t
AS
SELECT* FROM t WHERE id<10;

ALTER VIEW v_t
AS
SELECT* FROM t WHERE id<10
WITH CHECK OPTION;

SELECT* 
FROM information_schema.VIEWS
WHERE table_schema=database()\G;
```


### 物化视图
- 不是基于基表的虚表，而是根据基表实际存在的实表。也称为索引视图
- mysql实际并不支持物化视图，但是我们可以创建另外一张表统计信息
  ```sql
  CREATE TABLE orders(
      order_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
      product_name VARCHAR(30) NOT NULL,
      price DECIMAL(8,2) NOT NULL,
      amount SMALLINT NOT NULL,
      PRIMARY KEY(order_id)
  )ENGINE=INNODB;

  INSERT INTO orders VALUE
  (NULL,'cpu',135.5,1),
  (NULL,'memory',48.2,3),
  (NULL,'cpu',125.6,3),
  (NULL,'cpu',105.3,4);

  //创建物理视图
  CREATE TABLE orders_MV(
      product_name VARCHAR(30) NOT NULL,
      price_sum DECIMAL(8,2) NOT NULL,
      amount_sum INT NOT NULL,
      price_avg FLOAT NOT NULL,
      order_cnt INT NOT NULL,
      UNIQUE INDEX(product_name)
  );

  INSERT INTO orders_MV
  SELECT product_name,
  SUM(price),SUM(amount),AVG(price),COUNT(*)
  FROM orders
  GROUP BY product_name;
  ```

  - 利用触发器实现ON COMMIT方式的物化视图，就是在基表上面插入新的记录，物化视图会自动更新



## 分区
- 分区的过程是将表或索引分为多个更小，更可管理的部分。启用分区后一个表不再由一个ibd组成
- 启用分区不一定会使数据库运行的更快
- mysql是水平分区（即不同行存在不同分区）且局部分区（既存有数据也存有索引）

```sql
CREATE TABLE t(
    id INT
)ENGINE=INNODB;

PARTITION BY RANGE(id)(
    PARTITION p0 VALUES LESS THAN(10),
    PARTITION p1 VALUES LESS THAN(20),
)
```
- 当id小于10时，存入到p0分区，当id大于等于10小于20时，数据插入到p1分区。
- 查询时可以优化，在某个范围就查某个分区，而不用全部扫描