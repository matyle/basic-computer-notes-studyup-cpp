在使用间接的方法求出Q(s,a)的值，形成完整的列表之后，我们得到的并不是一个完整的策略，而是一个列表。机器人输入一个状态s，相当于将这个s作为索引，遍历Q(s,a)，通过比较得到一个估值比较高的a。



使一个输入的s直接对应于一个动作而不用再查表呢？其实是有的，就是策略梯度法（Policy Gradient），简称“PG方法”。

## 9.1策略梯度



### DPG

<img src="https://gitee.com/matytan/tupic/raw/master/uPic/image-20211113153229756.png" alt="image-2021111313229756" style="zoom:50%;" />





### Actor-Critic



### DDPG

网络结构：

<img src="https://gitee.com/matytan/tupic/raw/master/uPic/image-20211113153349879.png" alt="image-2021111315349879" style="zoom:50%;" />



算法：

包括四个网络，两个主网络，两个目标网络！

更新y即标签使用稳定的目标网络，Q目标网络评估u目标网络的动作，得到一个Q值，使用时间差分计算y



<img src="https://gitee.com/matytan/tupic/raw/master/uPic/image-20211113152135784.png" alt="image-2021111315135784" style="zoom:80%;" />



<img src="https://gitee.com/matytan/tupic/raw/master/uPic/image-20211113153004223.png" alt="image-2021111315304223" style="zoom:50%;" />





## A3C

> 异步优势演员-评论家算法

<img src="https://gitee.com/matytan/tupic/raw/master/uPic/image-20211113153646880.png" alt="image-2021113153646880" style="zoom:67%;" />





### A3C AC

算法流程：

<img src="https://gitee.com/matytan/tupic/raw/master/uPic/image-20211113154229953.png" alt="image-2021111315429953" style="zoom:80%;" />

