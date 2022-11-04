title: 深入理解go语言接口
date:30/11/2021 23:05
tags: go
categories: go语言
keywords: go
description: go语言学习笔记记录

cover:
---

>If it looks like a duck, swims like a duck, and quacks like a duck, then it probably is a duck.

翻译过来就是：如果某个东西长得像鸭子，像鸭子一样游泳，像鸭子一样嘎嘎叫，那它就可以被看成是一只鸭子。

Duck Typing，鸭子类型，是动态编程语言的一种对象推断策略，它更关注对象能如何被使用，而不是对象的类型本身。Go 语言作为一门静态语言，它通过通过接口的方式完美支持鸭子类型。

Go 语言作为一门现代静态语言，是有后发优势的。它引入了动态语言的便利，同时又会进行静态语言的类型检查，写起来是非常 Happy 的。Go 采用了折中的做法：不要求类型显示地声明实现了某个接口，只要实现了相关的方法即可，编译器就能检测到。

1. 定义接口
2. 实现接口（利用类型，或者结构）
3. 使用接口

```go
package main

import "fmt"

//定义接口
type IGreet interface {
	sayHello()
}

//定义结构体
type Go struct {
	//成员
}

type Java struct {
	//成员
}

//实现接口
func (g Go) sayHello() {
	fmt.Println("Hello, I am Go")
}

func (j Java) sayHello() {
	fmt.Println("Hello, I am Java")
}

//传入任意类型 使用接口
func Hello(i IGreet) {
	i.sayHello()
}

func main() {
	golang := Go{}
	java := Java{}

	Hello(golang)
	Hello(java)

```

动态语言的特点：
>变量绑定的类型是不确定的，在运行期间才能确定 函数和方法可以接收任何类型的参数，且调用时不检查参数类型 不需要实现接口

总结一下，鸭子类型是一种动态语言的风格，在这种风格中，==一个对象有效的语义，不是由继承自特定的类或实现特定的接口，而是由它"当前方法和属性的集合"决定==。Go 作为一种静态语言，通过接口实现了 鸭子类型，实际上是 Go 的编译器在其中作了隐匿的转换工作。


## 值接收者和指针接收者的区别

方法能给用户自定义的类型添加新的行为。它和函数的区别在于方法有一个接收者，给一个函数添加一个接收者，那么它就变成了方法。接收者可以是值接收者，也可以是指针接收者。

在==调用方法的时候==，值类型既可以调用值接收者的方法，也可以调用指针接收者的方法；指针类型既可以调用指针接收者的方法，也可以调用值接收者的方法。（无接口）


```go

type Person struct {
	age int
}

func (p Person) howOld() int {
	return p.age
}

func (p *Person) grow() {
	p.age += 1
}

func InitPerson() {
	//值类型
	tom := Person{age: 18}
	fmt.Println(tom.howOld())

	//值类型调用指针接收器
	tom.grow()
	fmt.Println(tom.howOld())

	//指针类型
	jack := &Person{age: 28}
	// 指针类型 调用接收者是值类型的方法
	fmt.Println(jack.howOld())

	jack.grow()
	fmt.Println(jack.howOld())

}
```


## 值接收者和指针接收者（接口）

前面说过，不管接收者类型是值类型还是指针类型，都可以通过值类型或指针类型调用，这里面实际上通过语法糖起作用的。

先说结论：实现了接收者是值类型的方法，相当于自动实现了接收者是指针类型的方法；而实现了接收者是指针类型的方法，不会自动生成对应接收者是值类型的方法。


（类似于指针或者引用 才能实现多态一样）比如父类的指针或者引用，使用了接口


>如果实现了接收者是值类型的方法，会隐含地也实现了接收者是指针类型的方法。

```go
//coder接口

type coder interface {
	code()
	debug()
}

type Gopher struct {
	language string
}
type Javaer struct {
	language string
}

func (p Gopher) code() {
	fmt.Println("I am coding %s", p.language)
}

func (p *Gopher) debug() {
	fmt.Println("I am debuging %s", p.language)
}

func (p Javaer) code() {
	fmt.Println("I am coding %s", p.language)
}

func (p *Javaer) debug() {
	fmt.Println("I am debuging %s", p.language)
}

func initCode(c coder) {
	c.code()
	c.debug()
}

func main() {
	initCode(&Gopher{"Go"})
	initCode(&Javaer{"Java"})
}

//输出
I am coding %s Go
I am debuging %s Go
I am coding %s Java
I am debuging %s Java
```

但如果我修改一下main函数为
```go
func main() {
	initCode(Gopher{"Go"})
	initCode(Javaer{"Java"})
}
```

运行一下，会报错
```bash 
./interfacetest.go:112:17: cannot use Gopher literal (type Gopher) as type coder in argument to initCode:
        Gopher does not implement coder (debug method has pointer receiver)
./interfacetest.go:113:17: cannot use Javaer literal (type Javaer) as type coder in argument to initCode:
        Javaer does not implement coder (debug method has pointer receiver)
```

>如果实现了接收者是值类型的方法，会隐含地也实现了接收者是指针类型的方法。

但是实现的是指针类型的方法，那么只能传指针，而不能传值！


## 两者分别在何时使用
使用指针作为方法的接收者的理由：
- 方法能够修改接收者指向的值。
- 避免在每次调用方法时复制该值，在值的类型为大型结构体时，这样做会更加高效。
- 是使用值接收者还是指针接收者，不是由该方法是否修改了调用者（也就是接收者）来决定，而是应该基于该类型的**本质**。
- 如果类型具备“原始的本质”，也就是说它的成员都是由 Go 语言里内置的原始类型，如字符串，整型值等，那就定义值接收者类型的方法。像内置的引用类型，如 slice，map，interface，channel，这些类型比较特殊，声明他们的时候，实际上是创建了一个 header， 对于他们也是直接定义值接收者类型的方法。这样，调用函数时，是直接 copy 了这些类型的 header，而 header 本身就是为复制设计的。
- 如果类型具备非原始的本质，不能被安全地复制，这种类型总是应该被共享，那就定义指针接收者的方法。比如 go 源码里的**文件结构体（struct File）**就不应该被复制，应该只有一份实体。

其他时候都应该使用值接收

## 底层实现

### iface 和 eface 的区别是什么

iface 和 eface 都是 Go 中描述接口的底层结构体，区别在于==iface描述的接口包含方法==，而 ==eface 则是不包含任何方法的空接口：interface{}==。

```go
type iface struct {
    tab  *itab
    data unsafe.Pointer
}
type itab struct {
    inter  *interfacetype
    _type  *_type
    link   *itab
    hash   uint32 // copy of _type.hash. Used for type switches.
    bad    bool   // type does not implement interface
    inhash bool   // has this itab been added to hash?
    unused [2]byte
    fun    [1]uintptr // variable sized
}
```
`iface` 内部维护两个指针，`tab` 指向一个 `itab` 实体， 它表示接口的类型(interfacetype)以及赋给这个接口的实体类型(type)。`data` 则指向接口具体的值，一般而言是一个==指向堆内存的指针==。





 `itab` 结构体：`_type` 字段描述了实体的类型，包括内存对齐方式，大小等；`inter` 字段则描述了接口的类型。`fun` 字段放置和接口方法对应的具体数据类型的方法地址，实现接口调用方法的动态分派，一般在每次给接口赋值发生转换时会更新此表，或者直接拿缓存的 itab。

这里只会列出实体类型和接口相关的方法，实体类型的其他方法并不会出现在这里。类似于==虚表==



另外，你可能会觉得奇怪，为什么 `fun` 数组的大小为 1，要是接口定义了多个方法可怎么办？实际上，这里存储的是第一个方法的函数指针，如果有更多的方法，在它之后的内存空间里继续存储。从汇编角度来看，通过增加地址就能获取到这些函数指针，没什么影响。顺便提一句，这些方法是按照函数名称的字典序进行排列的。





接口的类型（interfacetype）

```go
type interfacetype struct {
    typ     _type
    pkgpath name
    mhdr    []imethod
}
```

可以看到，它包装了 `_type` 类型，`_type` 实际上是描述 Go 语言中各种数据类型的结构体。我们注意到，这里还包含一个 `mhdr` 字段，表示接口所定义的函数列表， `pkgpath` 记录定义了接口的包名。



<img src="https://gitee.com/matytan/tupic/raw/master/uPic/56564826-82527600-65e1-11e9-956d-d98a212bc863-20211216164959277.png" alt="iface 结构体景" style="zoom:30%;" />









`eface` 的源码：

```go
type eface struct {
    _type *_type
    data  unsafe.Pointer
}
```

相比 `iface`，`eface` 就比较简单了。只维护了一个 `_type` 字段，表示空接口所承载的具体的实体类型。`data` 描述了具体的值。



汇编命令：

`go tool compile -S ./src/main.go`







### `_type` 结构体：



```go
type _type struct {
    // 类型大小
    size       uintptr
    ptrdata    uintptr
    // 类型的 hash 值
    hash       uint32
    // 类型的 flag，和反射相关
    tflag      tflag
    // 内存对齐相关
    align      uint8
    fieldalign uint8
    // 类型的编号，有bool, slice, struct 等等等等
    kind       uint8
    alg        *typeAlg
    // gc 相关
    gcdata    *byte
    str       nameOff
    ptrToThis typeOff
}
```





Go 语言各种数据类型都是在 `_type` 字段的基础上，增加一些额外的字段来进行管理的



```go
type arraytype struct {
    typ   _type
    elem  *_type
    slice *_type
    len   uintptr
}

type chantype struct {
    typ  _type
    elem *_type
    dir  uintptr
}

type slicetype struct {
    typ  _type
    elem *_type
}

type structtype struct {
    typ     _type
    pkgPath name
    fields  []structfield
}
```





## 接口的动态类型和动态值

从源码里可以看到：`iface`包含两个字段：`itab` 是接口表指针，指向类型信息；`data` 是数据指针，则指向具体的数据。它们分别被称为`动态类型`和`动态值`。而接口值包括`动态类型`和`动态值`。

【引申1】接口类型和 `nil` 作比较

接口值的零值是指`动态类型`和`动态值`都为 `nil`。当仅且当这两部分的值都为 `nil` 的情况下，这个接口值就才会被认为 `接口值 == nil`。









##  编译器自动检测类型是否实现接口



`var _ io.Writer = (*myWriter)(nil)`

这时候会有点懵，不知道作者想要干什么，实际上这就是此问题的答案。编译器会由此检查 `*myWriter` 类型是否实现了 `io.Writer` 接口。



## 类型转换和断言的区别

我们知道，Go 语言中不允许隐式类型转换，也就是说 `=` 两边，不允许出现类型不相同的变量。

`类型转换`、`类型断言`本质都是把一个类型转换成另外一个类型。不同之处在于，类型断言是对接口变量进行的操作。



### 类型转换

对于`类型转换`而言，转换前后的两个类型要相互兼容才行。类型转换的语法为：

> <结果类型》 ：= <目标类型>（<表达式>）
>
> var f float64
>
> i := (int)f

### 断言

前面说过，因为空接口 `interface{}` 没有定义任何函数，因此 Go 中所有类型都实现了空接口。当一个函数的形参是 `interface{}`，那么在函数中，需要对形参进行断言，从而得到它的真实类型。

断言的语法为：

> // 安全类型断言
>
> <目标类型的值>，<布尔参数> := <表达式>.( 目标类型 )  （使用if判断）
>
> //非安全类型断言
>
> <目标类型的值> := <表达式>.( 目标类型 )
>
> s,ok := i.(Student)
>
> if ok {
>
> }



```go
package main
import "fmt"
type Student struct {
	age  int
	name string
}

func main() {
	var i interface{} = new(Student)
	s, ok := i.(Student)
	if ok {
		fmt.Println(s)
	}
}
```



 `fmt.Println` 函数的参数是 `interface`。对于内置类型，函数内部会用穷举法，得出它的真实类型，然后转换为字符串打印。而对于自定义类型，首先确定该类型是否实现了 `String()` 方法，如果实现了，则直接打印输出 `String()` 方法的结果；否则，会通过反射来遍历对象的成员进行打印。



> 类型 `T` 只有接受者是 `T` 的方法；而类型 `*T` 拥有接受者是 `T` 和 `*T` 的方法。语法上 `T` 能直接调 `*T` 的方法仅仅是 `Go` 的语法糖。



```go
func (s Student) String() string {
    return fmt.Sprintf("[Name: %s], [Age: %d]", s.Name, s.Age)
}
```

所以， `Student` 结构体定义了接受者类型是值类型的 `String()` 方法时，通过

```go
fmt.Println(s)
fmt.Println(&s)
```

都能调用



如果Student结构体定义的接受者是指针类型的String()方法时，只能通过

```go
fmt.Println(&s)
```

才能按照自定义的格式打印。





## 接口转换的原理

`iface`结构体中的`itab`：

- 接口类型：interfacetype
- 实体类型：_type

> <interface 类型， 实体类型> ->itable

当判定一种类型是否满足某个接口时，Go 使用类型的方法集和接口所需要的方法集进行匹配，如果类型的方法集完全包含接口的方法集，则可认为该类型实现了该接口。

例如某类型有 `m` 个方法，某接口有 `n` 个方法，则很容易知道这种判定的时间复杂度为 `O(mn)`，Go 会对方法集的函数按照函数名的字典序进行排序，所以实际的时间复杂度为 `O(m+n)`。



当然，能转换的原因必然是类型兼容。



