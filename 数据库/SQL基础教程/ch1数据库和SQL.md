### 什么是DBMS



### DBMS有什么作用？



### DBMS的类型





### 关系型数据库的操作

DDL

DML

DCL

### 表的创建

```sql
create database shop; //创建数据库

create table t
(
  <列1> <数据类型> <该列所需约束>,
  <列2> <数据类型> <该列所需约束>,
  <列3> <数据类型> <该列所需约束1><该列所需约束2>,
  ...
);

#例如
create table Product
(
  product_id char(4) NOT NULL,
  product_name varchar(100) NOT NULL,
  product_type varchar(32) NOT NULL,
  sale_price integer,
  purchase_price integer,
  regist_data date,
  primary key(product_id)
);

```



数据类型之间的区别



- integer：
- char
- varchar
- date





## 表的删除与更新

```sql
drop table <table>;

#更新表
alter table <table name> add column <列的定义>;

alter table <tablename> drop column;
```





## 练习

```sql
create table Addressbook
(
  regist_no integer NOT NULL,
  name varchar(128) not null,
  address varchar(256) not null,
  tel_no char(10),
  mail_address char(20),
  primary key(regist_no)
);

#添加一列
alter table Addressbook add postal_code char(8) not null;

#删除表

drop table Addressbook;

#恢复表=== 无法恢复 只能重建 插入
create table Addressbook
(
  regist_no integer NOT NULL,
  name varchar(128) not null,
  address varchar(256) not null,
  tel_no char(10),
  mail_address char(20),
  postal_code char(8) not null,
  primary key(regist_no)
);
```







