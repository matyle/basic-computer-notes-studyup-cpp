## lock与latch
- latch轻量级的锁，用于线程，如mutex，rwlock等
- lock的对象是事务
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-14-15-39-07.png
  width=450px>
  </center>
  
  ```sql
  SHOW ENGINE INNODB MUTEX;
  ```


## INNODB存储引擎中的锁

### 锁的类型
- 共享锁 S Lock，允许事务读一行数据
- 排他锁 X lock 允许事务删除或更新一行数据