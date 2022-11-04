## 什么是反射

>在计算机科学中，反射是指计算机程序在运行时（Run time）可以访问、检测和修改它本身状态或行为的一种能力。用比喻来说，反射就是程序在运行的时候能够“观察”并且修改自己的行为。

==程序运行期对程序本身进行访问和修改的能力==，程序在编译时变量被转换为内存地址，变量名不会被编译器写入到可执行部分，在运行程序时程序无法获取自身的信息。
支持反射的语言可以在程序编译期将变量的反射信息，如字段名称、类型信息、结构体信息等整合到可执行文件中，并给程序提供接口访问反射信息，这样就可以在程序运行期获取类型的反射信息，并且有能力修改它们。

## 为什么使用反射
1.有时你需要编写一个函数，但是并不知道传给你的参数类型是什么，可能是没约定好；也可能是传入的类型很多，这些类型并不能统一表示。这时反射就会用的上了。
2. 有时候需要根据某些条件决定调用哪个函数，比如根据用户的输入来决定。这时就需要对函数和函数的参数进行反射，在运行期间动态地执行函数。
3. 按条件合并一些结构，而这些条件由结构的成员，类型等决定

### 为什么不使用反射

1. 与反射相关的代码，经常是难以阅读的。在软件工程中，代码可读性也是一个非常重要的指标。
2. Go 语言作为一门静态语言，编码过程中，编译器能提前发现一些类型错误，但是对于反射代码是无能为力的。所以包含反射相关的代码，很可能会运行很久，才会出错，这时候经常是直接 panic，可能会造成严重的后果。

3. 反射对性能影响还是比较大的，比正常代码运行速度慢一到两个数量级。所以，对于一个项目中处于运行效率关键位置的代码，尽量避免使用反射特性。

## 反射相关
`reflect`包，Go语言中的反射是由 reflect 包提供支持的，它定义了两个重要的类型 Type 和 Value。 ==任意接口值==在反射中都可以理解为由 ==reflect.Type== 和 ==reflect.Value== 两部分组成，并且 reflect 包提供了 reflect.TypeOf 和 reflect.ValueOf 两个函数来获取任意对象的 Value 和 Type。

实际上，它的本质是程序在运行期探知对象的类型信息和内存结构，不用反射能行吗？可以的！使用汇编语言，直接和内层打交道，什么信息不能获取？但是，当编程迁移到高级语言上来之后，就不行了！就只能通过反射来达到此项技能。


```go
func mergeDualExchangeCfgFields(to, from *stu Student) error {
	tov := reflect.ValueOf(to)
	fromv := reflect.ValueOf(from)
	return mergeDualExchWholePtrValue(tov, fromv)
}
```


## types 和 interface
Go 语言中，每个变量都有一个静态类型，在编译阶段就确定了的，比如 int, float64, \[]int 等等。注意，这个类型是声明时候的类型，不是底层数据类型。

Go 官方博客里就举了一个例子：
```go
type MyInt int

var i int
var j MyInt
```

尽管 i，j 的底层类型都是 int，但我们知道，他们是不同的静态类型，除非进行类型转换，否则，==i 和 j 不能同时出现在等号两侧==。j 的静态类型就是 MyInt。

反射主要与 `interface{}` 类型相关,`interface{}`底层实现
```go
type iface struct {
    tab  *itab
    data unsafe.Pointer
}

type itab struct {
    inter  *interfacetype
    _type  *_type
    link   *itab
    hash   uint32
    bad    bool
    inhash bool
    unused [2]byte
    fun    [1]uintptr
}
```

## reflect

```go
func TypeOf(i interface{}) Type 
func ValueOf(i interface{}) Value
```

注意，返回值 `Type` 实际上是一个接口，定义了很多方法，用来获取类型相关的各种信息，而 `*rtype` 实现了 `Type` 接口。





`Elem() Type`返回内部子元素类型，只能由类型 Array, Chan, Map, Ptr, or Slice 调用

```go
type Type interface {
    // 所有的类型都可以调用下面这些函数

    // 此类型的变量对齐后所占用的字节数
    Align() int
    
    // 如果是 struct 的字段，对齐后占用的字节数
    FieldAlign() int

    // 返回类型方法集里的第 `i` (传入的参数)个方法
    Method(int) Method

    // 通过名称获取方法
    MethodByName(string) (Method, bool)

    // 获取类型方法集里导出的方法个数
    NumMethod() int

    // 类型名称
    Name() string

    // 返回类型所在的路径，如：encoding/base64
    PkgPath() string

    // 返回类型的大小，和 unsafe.Sizeof 功能类似
    Size() uintptr

    // 返回类型的字符串表示形式
    String() string

    // 返回类型的类型值
    Kind() Kind

    // 类型是否实现了接口 u
    Implements(u Type) bool

    // 是否可以赋值给 u
    AssignableTo(u Type) bool

    // 是否可以类型转换成 u
    ConvertibleTo(u Type) bool

    // 类型是否可以比较
    Comparable() bool

    // 下面这些函数只有特定类型可以调用
    // 如：Key, Elem 两个方法就只能是 Map 类型才能调用
    
    // 类型所占据的位数
    Bits() int

    // 返回通道的方向，只能是 chan 类型调用
    ChanDir() ChanDir

    // 返回类型是否是可变参数，只能是 func 类型调用
    // 比如 t 是类型 func(x int, y ... float64)
    // 那么 t.IsVariadic() == true
    IsVariadic() bool

    // 返回内部子元素类型，只能由类型 Array, Chan, Map, Ptr, or Slice 调用
    Elem() Type

    // 返回结构体类型的第 i 个字段，只能是结构体类型调用
    // 如果 i 超过了总字段数，就会 panic
    Field(i int) StructField

    // 返回嵌套的结构体的字段
    FieldByIndex(index []int) StructField

    // 通过字段名称获取字段
    FieldByName(name string) (StructField, bool)

    // FieldByNameFunc returns the struct field with a name
    // 返回名称符合 func 函数的字段
    FieldByNameFunc(match func(string) bool) (StructField, bool)

    // 获取函数类型的第 i 个参数的类型
    In(i int) Type

    // 返回 map 的 key 类型，只能由类型 map 调用
    Key() Type

    // 返回 Array 的长度，只能由类型 Array 调用
    Len() int

    // 返回类型字段的数量，只能由类型 Struct 调用
    NumField() int

    // 返回函数类型的输入参数个数
    NumIn() int

    // 返回函数类型的返回值个数
    NumOut() int

    // 返回函数类型的第 i 个值的类型
    Out(i int) Type

    // 返回类型结构体的相同部分
    common() *rtype
    // 返回类型结构体的不同部分
    uncommon() *uncommonType
}
```







<img src="https://gitee.com/matytan/tupic/raw/master/uPic/57130652-bb060280-6dcc-11e9-9c63-6e2bc4e33509.png" alt="三者" style="zoom:33%;" />







`reflect.Value` 是一个==结构体==。表示 `interface{}` 里存储的实际变量，它能提供实际变量的各种信息。相关的方法常常是需要结合类型信息和值信息。例如，如果要提取一个结构体的字段信息，那就需要用到 _type (具体到这里是指 structType) 类型持有的关于结构体的字段信息、偏移信息，以及 `*data` 所指向的内容 —— 结构体的实际值。

Type 是一个接口，定义了很多相关方法，用于获取类型信息。Value 则持有类型的具体值。Type、Value、Interface 三者间通过函数 TypeOf，ValueOf，Interface 进行相互转换。





## 反射三大定律

1. 反射将接口变量转换成反射对象 Type 和 Value；
2. 反射可以通过反射对象 Value 还原成原先的接口变量；
3. 反射可以用来修改一个变量的值，前提是这个值可以被修改。

