## 链表

LinkedList

列表的底层数据结构

在redis中有哪些应用？

注意c语言的数据和操作是分割开的

## 链表和链表结点实现

结点：

```c
typedef struct listNode
{
  struct listNode *prev;
  struct listNode *next;
  void *value; //节点值
}listNode
```



链表：（从c语言看c++的发展，妙啊）

```c
typedef struct Linkedlist
{
  listNode *head; //prev指向null
  listNode *tail;//next指向null
  unsigned long len;// 8字节
  void *(*dup)(void *ptr); //节点值复制函数 函数指针 返回值void*
  void (*free)(void *ptr);//节点值释放
  int (*match)(void *ptr,void *key);//函数指针，返回int ，查看结点值是否匹配
}
```

为什么需要那几个函数指针？

- c语言中结构体内不能直接有函数（只能从外部set函数，有点callback那个味道）

为什么需要这几个函数？

- 节点值为了能存各种类型的数据，不得不设置为void* （奈何没有模板啊）
- 那么怎么比较两者的值呢？不同的数据肯定有不同的方法呀（因此我们需要自己设定然后作为函数指针传进来，c++的继承重写虚函数就是这个味道，回调也是作用呀），有点类似重写==
- 既然是指针，可能伴随着空间的分配，因此需要free回调。
- 数据可能需要拷贝复制（理解为重写拷贝构造函数），直接一个等号可能是不行的，涉及到深拷贝，等号只能是地址之间赋值，这是浅拷贝（加深了我对浅拷贝的理解）
- 是不是有多态那味了？（模板也是一种多态呀）

我们可以发现链表的特点：

- 双端
- 无环
- 有首尾端节点
- 记录长度
- 多态



## 操作API

- 设置回调（函数指针）
- 获取回调函数指针

## 总结

- 对多态有了更深的理解

- 链表中的函数指针，联想到c++的产生

- 对于无模板方法多态的实现

  