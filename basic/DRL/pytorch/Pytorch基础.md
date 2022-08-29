## 张量

张量本质是一个多维数组

用数学知识来类比，单个数字就像点， 是零维的，向量就像线段，是一维的，

矩阵则是二维的对象。

三维数字的集合可以用平行六面体表示，但它们不像矩阵一样有特定的名 称。我们可以用“张量”来表示高维集合，如图3.1所示。

<img src="https://gitee.com/matytan/tupic/raw/master/uPic/image-20211120152446708.png" alt="image-2021120152446708" style="zoom:40%;" />

### 创建张量

- 使用Numpy创建多维数组（张量）

```#python
n = np.zeros(shape=(3,2))# 3行2列

b = torch.tensor(n);
print(b)

# 指定类型
n = np.zeros(shape(3,2),dtype=np.float32)

b = torch.tensor(n)

# 或者
n = np.zeros(shape=(3,2))# 3行2列
b = torch.tensor(n,dtype=torch.float32);

```

### 零维张量

现在，已经支持零维张量并由合适的函数返回，并且可以通 过torch.tensor()函数创建。为了访问这种张量的实际Python值，可以使用特殊的item()方法:





### GPU张量

`to(device)`方法，可以创建张量的副本到指定设备（如cuda）

## 梯度

grad:张量的梯度，与原张量形状相同。



**梯度和张量**

`is_leaf`:如果该张量是由用户构造的，则为True;如果是函数转换的结果，则为False。

`requires_grad`:如果此张量需要计算梯度，则为True。此属性是从叶张量继承而来，叶张量从张量构建过程(torch.zeros()或 torch.tensor()等)中获得此值。默认情况下，构造函数的 requires_grad = False，如果要计算张量梯度，则需明确声明



## NN构建块

- `import torch.nn as nn` 神经网络模块在`torch.nn`中

> 注意输入和输出的概念，例如`nn.Linear(3,5)`表示：
>
> 这一层网络的输入是向量是3x1，输出是5x1

```python
s = nn.Sequential(
    nn.Linear(3,5),
    nn.ReLU(),
    nn.Linear(5,20),
    nn.ReLU(),
    nn.Linear(20,5),
    nn.Dropout(p=0.3),
    nn.Softmax(dim=1)
    
)
```

> 输出
>
> ```
> Sequential(
>   (0): Linear(in_features=3, out_features=5, bias=True)
>   (1): ReLU()
>   (2): Linear(in_features=5, out_features=20, bias=True)
>   (3): ReLU()
>   (4): Linear(in_features=20, out_features=5, bias=True)
>   (5): Dropout(p=0.3, inplace=False)
>   (6): Softmax(dim=1)
> )
> ```



**一次前向传递**

```python
#前向传递
s(torch.FloatTensor([[1,2,4],[1,3,5]]))

# 输出
tensor([[0.1874, 0.1975, 0.1874, 0.1874, 0.2404],
        [0.1996, 0.2045, 0.1522, 0.1345, 0.3092]], grad_fn=<SoftmaxBackward0>)
```



### 激活函数

- ReLU()
- Softmax(dim=1)
- Tanh
- Sigmoid



## 自定义层

`nn.Module`可以自定义层

作为核心，nn.Module为其子类提供了相当丰富的功能:
它记录当前模块的所有子模块。例如，构建块可以具有两个前馈 层，可以以某种方式使用它们来执行代码块的转换。 

提供处理已注册子模块的所有参数的函数。

可以获取模块参数的 完整列表(parameters()方法)将其梯度置零(zero_grads()方 法)，将其移至CPU或GPU(to(device)方法)，序列化和反序列 化模块(state_dict()和load_state_dict())，

甚至可以用自己 的callable执行通用的转换逻辑(apply()方法)。 建立了Module针对数据的约定。

每个模块都需要覆盖forward()方 法来执行数据的转换。 

还有更多的函数，例如注册钩子函数以调整模块转换逻辑或梯度流，它们更加适合高级的使用场景。

```python
class OurModule(nn.Module):
    def __init__(self,input_num,class_num,dropout_prob=0.3):
        super(OurModule,self).__init__()
        self.pipe = nn.Sequential(
                    nn.Linear(input_num,5),
                    nn.ReLU(),
                    nn.Linear(5,20),
                    nn.ReLU(),
                    nn.Linear(20,class_num),
                    nn.Dropout(p=dropout_prob),
                    nn.Tanh()
                    )
    
    def forward(self,x):
        return self.pipe(x)
      

# mian函数
net = OurModule(input_num=2,class_num=3) #定义网络
v = torch.FloatTensor([[2,3]]);
out = net(v) #传递数据 forward的参数
print(net)
print(out)
```



## 损失函数和优化器

将输入数据转换为输出的网络并不是训练唯一需要的东西。我们还需要定义学习目标，==即要有一个接受两个参数(网络输出和预期输出)的函数==。

它的责任是返回一个表示网络预测结果与预期结果之间的差距的数字。此函数称为==损失函数，==其输出为损失值。

使用损失值，可以计算网络参数的梯度，并对其进行调整以减小损失值，以便优化模型的结果

### 损失函数

损失函数在nn包中，并实现为nn.Module的子类。

nn.MSELoss:参数之间的均方误差，是回归问题的标准损失。 

nn.BCELoss和nn.BCEWithLogits:二分类交叉熵损失。前者期望 输入是一个概率值(通常是Sigmoid层的输出)，而后者则假定原 始分数为输入并应用Sigmoid本身。第二种方法通常在数值上更稳 定、更有效。这些损失(顾名思义)经常用于分类问题。 

nn.CrossEntropyLoss和nn.NLLLoss:著名的“最大似然”标准， 用于多类分类问题。前者期望的输入是每个类的原始分数，并在 内部应用LogSoftmax，而后者期望将对数概率作为输入



### 优化器

基本优化器的职责是获取模型参数的梯度，并更改这些参数来降 低损失值。通过降低损失值，使模型向期望的输出靠拢，使得模型性能越来越好。

更改参数听起来很简单，但是有很多细节要处理，优化器仍是一个热门的研究主题。在torch.optim包中，PyTorch提供了许 多流行的优化器实现，其中最广为人知的是:

SGD:具有可选动量的普通随机梯度下降算法。 

RMSprop:Geoffrey Hinton提出的优化器。 

Adagrad:自适应梯度优化器。 

Adam:一种非常成功且流行的优化器，是RMSprop和Adagrad的组合。






**将数据样本传递给网络**(第4行)，==并将其输出值和目标标签提供给损失函数(第5行)，==

损失函数的结果显示了网络结果和目标标签的差距。网络的输入和网络的权重都是张量，所以网络的所有转换只不过是中间张量实例的操作图。

损失函数也是如此，它的结果也是一个只有一个损失值的张量

