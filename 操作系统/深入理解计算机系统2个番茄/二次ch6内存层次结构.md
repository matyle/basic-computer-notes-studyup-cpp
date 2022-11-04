# 存储器的层次结构

## 6.1 存储技术

- 存储器系统
  - 靠近cpu的小的，快速的高速缓存器作为主存储器的缓冲区域。

- 随机访问存储器
  - 静态SRAM比动态DRAM更快
  - SRAM一般作为高速缓存
- 存储器模块
- 增强DRAM
- 磁盘

- 局部性
  - 时间局部性：重复引用同一个变量的程序具有良好的时间局部性
  - 空间局部性：步长为1的引用模式的程序有很好的空间局部性，以大步长跳来跳去的空间局部性差

## 存储器的层次结构

- 金子塔
<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-23-10-12-04.png
width=600px>
  <br>
  <div style="color:orange; border-bottom: 1px solid #d9d9d9;
  display: inline-block;
  color: #999;
  padding: 2px;"></div>
</center>

- 缓存cache
<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-23-10-13-40.png
width=600px>
  <br>
  <div style="color:orange; border-bottom: 1px solid #d9d9d9;
  display: inline-block;
  color: #999;
  padding: 2px;"></div>
</center>


## 高速缓存存储器
  - 编写缓存又好的程序
  - 一般缓存一行为32位或者64位