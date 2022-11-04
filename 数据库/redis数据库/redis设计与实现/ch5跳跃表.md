跳表是什么？

- 有序数据结构
- 类似于一种索引
- 分层



## 跳跃表的实现

![image-20210814090229217](ch5跳跃表.assets/image-20210814090229217.png)

### 跳跃表结点

`zskiplistNode`

```c
typedef struct zskiplistNode
{
  struct zskiplistLevel//层
  {
    struct zskiplistNode* forward;//即next指针
    unsigned int span;
  }level[];
  //后退指针
  struct zskiplistNode *backward;//即pre指针
  double score;//分值
  robj *obj;
}zskiplistNode;
```



1. 层 level数组
2. 前进指针（next指针）
3. 跨度
4. 后退指针
5. 分值和成员





### 跳跃表

```c
typedef struct zskiplist
{
  struct zskiplistNode *header,*next;
  unsigned long length;
  int level;
}zskiplist;
```



header:指向跳表头结点

tail：指向尾结点

level:记录目前跳跃表内，层数最大的那个结点的层数

Length:记录跳表长度

![image-20210814084450949](file:///Users/matytan/Desktop/code/%E5%9F%BA%E7%A1%80/%E6%95%B0%E6%8D%AE%E5%BA%93/redis%E6%95%B0%E6%8D%AE%E5%BA%93/redis%E8%AE%BE%E8%AE%A1%E4%B8%8E%E5%AE%9E%E7%8E%B0/ch5%E8%B7%B3%E8%B7%83%E8%A1%A8.assets/image-20210814084450949.png?lastModify=1628902151)



如何快速查找？二分法找层数？还是从最高层遍历？

加上字典更加快速