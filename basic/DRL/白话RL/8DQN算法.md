2013年版本DQN

<img src="https://gitee.com/matytan/tupic/raw/master/uPic/image-20211112155427195.png" alt="image-2021111215547195" style="zoom:67%;" />

### 参数

**卷积计算公式**：**参数量=（filter size \* 前一层特征图的通道数 ）\* 当前层filter数量**

16x8x8+4x4x32+9x9x32x256+

**【全连接层的参数量】**

上面已经说过卷积层的参数量计算方法了，那**如何计算全连接层的参数量**？其实**和卷积层参数量的计算方法是一样的**。

VGG-16最后一次卷积得到的feature map为 ![[公式]](https://www.zhihu.com/equation?tex=7\times7\times512) ，全连接层是将feature map展开成一维向量 ![[公式]](https://gitee.com/matytan/tupic/raw/master/uPic/equation-20211112155932945.jpg) 。**实际上，我们就是用4096个![[公式]](https://www.zhihu.com/equation?tex=7%5Ctimes7%5Ctimes512) 的filter去做卷积**（可以理解为是一个卷积层)。

## 输入

网络输入是连续4帧 84x84的图像

为什么要连续图像？

- 动态游戏和象棋是有区别的，象棋的盘面固定

<img src="https://gitee.com/matytan/tupic/raw/master/uPic/image-20211112160806455.png" alt="image-2021111216080455" style="zoom:70%;" />







## 2015年DQN

主网络和目标网络(target network)





<img src="https://gitee.com/matytan/tupic/raw/master/uPic/image-20211112162052350.png" alt="image-2021112162052350" style="zoom:70%;" />



不同之处：

<img src="https://gitee.com/matytan/tupic/raw/master/uPic/image-20211112162351944.png" alt="image-2021111162351944" style="zoom:50%;" />

这时的一个状态s的估值yi，并不是由Action-Value网络给出的，而是由TargetAction-Value网络给出的。





### target网络

Target网络在这里只提供了一个估值yi，供主网络作为标签值去学习。在这个过程中，每隔C步，就把Target网络的参数赋值为主网络的参数——这就是全部内容。

2013版的标签不断变化会带来什么问题？标签不断变化，将直接导致损失函数的“形状”







## Double DQN



## Dueling DQN

<img src="https://gitee.com/matytan/tupic/raw/master/uPic/image-20211112164848004.png" alt="image-2021111216484800" style="zoom:40%;" />

状态估值函数网络：$V^π(s).$

优势函数网络：$A^π(s,a)$

假设$A^π(s,a)=Q^π(s,a)−V^π(s).$

个完整的估值函数Qπ(s,a)（就是前面一直讨论的Q函数），实际上是由两部分组成的，一部分是由状态s下的动作质量a贡献的，另一部分是由状态s本身的价值决定的，两部分直接线性叠加，构成了Qπ(s,a)

 $Q^π(s,a)=A^π(s,a)+V^π(s).$

