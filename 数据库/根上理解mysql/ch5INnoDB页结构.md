## 页的总体结构

页的结构（结构体）

16kb如何划分呢？各个模块的作用是什么？



<img src="INnoDB页结构.assets/image-20210811091045665.png" alt="image-20210811091045665" style="zoom:40%;" />

```c
struct page
{
file header //文件头 文件通用信息
page header; //页的专用信息
infimum+supermum  //伪记录
user records
free space 大小不确定 分配空间
page directory //页目录 快速找到对应行记录
file tailer //文件尾部 校验页的完整性
}
```



为啥有file header还有page header？

行记录是什么时候有的？一开始就有吗？



## 行记录在页中的存储

行如何存储？
user records一开始就有吗？

记录头信息（record header）至关重要（记录额外信息中）

`变长长度列表--null值列表---记录头信息---列1---列2---row_id(可选)---trx_id---roll_pointer`

### 头记录

`delete mask`

`min_red_mask`:后面b+树会用到，每层非叶结点的最小记录

`n_owned`

`heap_no`

`record_type`

`next_record`

## 页目录

想一想通过（next_record)连接起来的记录，我们在查找某个值的时候怎么找？遍历整个链表吗？（注意这里是记录，B+树结点对应的是每个页），现在是在页中

为啥来个页目录？

快速查找行记录在页中的位置（二分）

如何划分每个槽(页目录项)对应的记录数？



比较难理解的就是不是在本槽（slot）查找其实，链表中的二分（实际很像跳表）

复习了二分法 一致性解决这个问题，即找到对应的mid之后，不一定直接是那个key

但是从mid开始遍历一定最近（怎么遍历 通过next_record呗）



## page haeder 页头部

页头部几个重点字段

``PAGE_N_DIR_SLOTS``

`PAGE_N_HEAP`

`PAGE_LAST_INSERT`

`PAGE_N_RECS`



## 文件头部



- 校验和

- 页号(FIL_PAGE_OFFSET)

- 页类型

  | 类型名称                | 16进制 | 描述                       |
  | ----------------------- | ------ | -------------------------- |
  | FIL_PAGE_TYPE_ALLOCATED |        |                            |
  | FIL_PAGE_UNDO_LOG       |        |                            |
  |                         |        |                            |
  |                         |        |                            |
  | FIL_PAGE_INODE          |        |                            |
  | FIL_PAGE_IBUF_BITMAP    |        |                            |
  | FIL_PAGE_TYPE_TRX_SYS   |        |                            |
  | FIL_PAGE_TYPE_XDES      |        |                            |
  | FIL_PAGE_INDEX          |        | 索引页（我们常说的数据页） |

  

- FIL_PAGE_PREV和FIL_PAGE_NEXT,即页的next和prev字段：双向链表连接所有页
  - INnoDB以页为单位加载（16kb），如果按记录或者字节加载需要读盘太多了，那就太慢了
