---
- [x] 已上传
title: go语言方法method
date:30/11/2021 23:05
tags: go
categories: go语言
keywords: go
description: go语言学习笔记
cover:
---

## method方法

> Go does not have classes. However, you can define methods on types.

A method is a function with a special *receiver* argument.

The receiver appears in its own argument list between the `func` keyword and the method name.

In this example, the `Abs` method has a receiver of type `Vertex` named `v`.

Go没有类。但是，您可以定义类型的方法。方法是一个具有特殊接收器参数的函数。接收器出现在==func关键字和方法名称==之间的自己的参数列表中。在本例中，Abs方法有一个名为Vertex receiver。

为什么这么做？类似于一个类中的方法！每一个类型可以调用这样的一个方法

```go
package main

import (
	"fmt"
	"math"
)

type Vertex struct {
	X, Y float64
}

func (v Vertex) Abs() float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}

func main() {
	v := Vertex{3, 4}
	fmt.Println(v.Abs())
}
```



## Methods are functions

Remember: ==a method is just a function with a receiver argument==.
方法只是一个函数（特殊的接收器）
Here's `Abs` written as a regular function with no change in functionality.

等价于：

```go
package main

import (
	"fmt"
	"math"
)

type Vertex struct {
	X, Y float64
}

func Abs(v Vertex) float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}

func main() {
	v := Vertex{3, 4}
	fmt.Println(Abs(v))
}


```



## Methods continued方法（二）

You can declare a method on ==non-struct types,== too. 可以定义在非结构体类型 

In this example we see a numeric type `MyFloat` with an `Abs` method.

You can only declare a method with a receiver whose type is defined in the same package as the method. You cannot declare a method with a receiver whose type is defined in another package (which includes the built-in types such as `int`). 需要同一个包,所以内置类型不能直接使用需要
`type Interger int`

当前包内任意任意类型定义方法(包括内置的(但是不在同一包）,但是首先得用type给一个别名类似于一个别名)

```go
type Integer int

func MethodTest(val int) {
	var x Interger = Interger(val)
	fmt.Println("MethodTest", x.Abs())
}
```

### Pointer receivers

可以使用**指针接收器**声明方法。

这意味着接收器类 型具有某些类型T的字面语法*T。（此外，T本身不能像*int这样的指针。）

带有指针接收器的方法可以修改接收器指向的值（如此处的缩放）。由于方法通常需要修改接收器，**指针接收器比值接收器更常见**。

使用值接收器，Scale方法对原始`Vertex`的副本进行操作。（这与任何其他函数参数的行为相同。）Scale方法必须有一个指针接收器来更改主函数中声明的顶点值。

```go
type Vertex struct {
	X, Y float64
}

func (v *Vertex) Abs() float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}
}
```