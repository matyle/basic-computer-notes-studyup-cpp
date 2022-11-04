![](https://gitee.com/matytan/tupic/raw/master/uPic/ch0-01.png)

Go语言有时候被描述为“C类似语言”，或者是“21世纪的C语言”。Go从C语言继承了相似的表达式语法、控制流结构、基础数据类型、调用参数传值、指针等很多思想，还有C语言一直所看中的编译后机器码的运行效率以及和现有操作系统的无缝适配。

但是在Go语言的家族树中还有其它的祖先。其中一个有影响力的分支来自[Niklaus Wirth](https://en.wikipedia.org/wiki/Niklaus_Wirth)所设计的[Pascal][Pascal]语言。然后[Modula-2][Modula-2]语言激发了包的概念。然后[Oberon][Oberon]语言摒弃了模块接口文件和模块实现文件之间的区别。第二代的[Oberon-2][Oberon-2]语言直接影响了包的导入和声明的语法，还有[Oberon][Oberon]语言的面向对象特性所提供的方法的声明语法等。

go语言的另一支祖先，带来了Go语言区别其他语言的重要特性，灵感来自于贝尔实验室的[Tony Hoare](https://en.wikipedia.org/wiki/Tony_Hoare)于1978年发表的鲜为外界所知的关于并发研究的基础文献 *顺序通信进程* （ *[communicating sequential processes][CSP]* ，缩写为[CSP][CSP]。


在Plan9操作系统中，这些优秀的想法被吸收到了一个叫[Alef][Alef]的编程语言中。Alef试图将Newsqueak语言改造为系统编程语言，但是因为缺少垃圾回收机制而导致并发编程很痛苦。（译注：在Alef之后还有一个叫[Limbo][Limbo]的编程语言，Go语言从其中借鉴了很多特性。 具体请参考Pike的讲稿：http://talks.golang.org/2012/concurrency.slide#9 ）

比如iota语法是从[APL][APL]语言借鉴，词法作用域与嵌套函数来自于[Scheme][Scheme]语言（和其他很多语言）。
比如Go语言的切片为==动态数组提供了有效的随机存取的性能==，这可能会让人联想到链表的底层的共享机制。还有Go语言新发明的defer语句。

{% include "../links.md" %}
