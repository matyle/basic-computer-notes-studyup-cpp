## 数据目录

查看mysql的数据目录 show variables like 'datadir'

```sh
mysql> show variables like 'datadir';
+---------------+------------------------+
| Variable_name | Value                  |
+---------------+------------------------+
| datadir       | /usr/local/mysql/data/ |
+---------------+------------------------+
1 row in set (0.32 sec)
```



### 表在文件系统中的表示

1. 表结构的定义

   表名.frm表示表的结构，表的名称，表里面有多少列，列的类型，约束条件和索引

2. 表中的数据

   1. 系统表空间

      

   2. 独立表空间（索引+数据）

      1. 表名.ibd

## 系统数据库

information_schema:维护所有其他数据库的信息，有哪些表，视图，触发器，哪些列，索引，

Preformance_schema

sys

