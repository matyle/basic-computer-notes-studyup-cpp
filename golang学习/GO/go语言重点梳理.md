### 变量

- var定义变量，编译器支持类型推断，基础数据类型划分清晰明确，有助于编写跨平台应用。编译器确保变量总是被初始化为零值，避免出现意外状况。
- 函数内部,简短模式，可以省略var，使用`:=`

#### 变量赋值

```go
x,y := 1,2

//
x,y := x+3,y+3
```

计算顺序为：先计算右边`x+3,y+3`,再赋值给左边



### 变量命名建议

> 以字母或下划线开始，由多个字母、数字和下划线组合而成。
> 区分大小写。
> 使用驼峰（camel case）拼写格式。
> 局部变量优先使用短名。（i代替index）
> 不要使用保留关键字。
> 不建议使用与预定义常量、类型、内置函数相同的名字。
> 专有名词通常会全部大写，例如escapeHTML。



### 常量

`const`

`iota`自增 

```go
func ConstTest() {
	const (
		x = iota //0
		y        // 1
		z					//2
	)
	const (
		_  = iota //0
		KB = 1 << (10 * iota) // 1024 (1<<10*1) 1向左移动10位 //1024
		MB //  (1<<10*2)
    GB //(1<<10*3)
	)

	fmt.Println(x, B, KB, MB, GB)
}
```



### 流程控制

#### for循环

- 循环嵌套不要超过2层，必要时剥离 重构为函数

#### goto

- goto，continue，break

  - 性能优先可以使用·

  - 定义标签

  - 不能跳转其他函数

### 引用类型

特指：slice，map，channel



### 指针类型

内存地址与指针有区别（在go中）。

内存地址是内存中每个字节单元的唯一编号

而指针则是一个实体。指针会分配内存空间，相当于一个专门用来保存地址的整型变量。

- 不支持++ 或 – 运算

指针类型支持相等运算符，但不能做加减法运算和类型转换。如果两个指针指向同 一地址，或都为nil，那么它们相等。

- 可通过unsafe.Pointer将指针转换为uintptr后进行加减法运算，但可能会造成非 法访问。



“零长度（zero-size）对象的地址是否相等和具体的实现版本有关，不过肯定不等于nil。”

```go

	var a, b struct{}
	println(&a, &b)
//输出
0xc000082e9f 0xc000082e9f
```



### 结构体

- 匿名嵌入

  ```go
  type user struct{
    name string
    age byte
  }
  
  type manager struct{
    user // 匿名
    title string
  }
  ```

- 访问匿名字段

  ```go
  var m manager
  
  m.name = "tom"
  m.age = 29
  m.title = "CTO"
  
  //output
  //{{tom 29}CTO}
  ```

  

- 调用匿名字段方法：

  ```go
  var m Manager = Manager{u, "cto"} //调用匿名字段的方法，类似于继承
  
  m.Print()
  ```

### 初始化

数组，切片，map，结构体初始化的限制：

- 初始化表达式必须含类型标签（即类型名）。

- 左花括号必须在类型尾部，不能另起一行。（否则加逗号）

- 多个成员初始值以逗号分隔。

- 允许多行，但每行须以逗号或右花括号结束

### 函数

特点：

● 无须前置声明。

● 不支持命名嵌套定义(nested)。

● 不支持同名函数重载(overload)。

● 不支持默认参数。

● 支持不定长变参。

● 支持多返回值。

● 支持命名返回值。

● 支持匿名函数和闭包。



函数属于第一类对象，具备相同签名(参数及返回值列表)的视作同一类型。

同时支持命名类型（type）





在避免冲突的情况下，函数命名要本着精简短小、望文知意的原则。

 ● 通常是动词和介词加上名词，例如scanWords。

● 避免不必要的缩写，printError要比printErr更好一些。

● 避免使用类型关键字，比如buildUserStruct看上去会很别扭。 ● 避免歧义，不能有多种用途的解释造成误解。

● 避免只能通过大小写区分的同名函数。● 避免与内置函数同名，这会导致误用。

● 避免使用数字，除非是特定专有名词，例如UTF8。

● 避免添加作用域提示前缀。

● 统一使用camel/pascal case拼写风格。

● 使用相同术语，保持一致性。

● 使用习惯用语，比如init表示初始化，is/has返回布尔值结果。

● 使用反义词组命名行为相反的函数，比如get/set、min/max等



#### 函数闭包

```go
func ClosureTest(x int) func() {
	return func() {
		fmt.Println("x:", x)
	}
}
func main() {
	var x int = 2
	f := ClosureTest(x)
	f()
}

```

闭包如何实现引用环境上下文x？闭包是如何实现的？匿名函数被返回后，为何还能读取环境变量值？修改一下代码如下


```go
func ClosureTest(x int) func() {
	fmt.Println("&x:", &x)
	return func() {
		fmt.Println("&x:", &x)
	}
}
//输出
&x: 0xc0000ae000
&x: 0xc0000ae000
```

1. `go build -gcflags "-N -L"`关闭优化 `go build -gcflags="-N -l"`



### defer 延迟调用

- FILO（先入后出）
- 延迟调用可修改当前函数命名返回值，但其自身返回值被抛弃

- defer 函数时，会先拷贝值，延迟调用可修改当前函数命名返回值，但其自身返回值被抛弃

```go
//defer
func DeferTest() {
	x, y := 1, 2
	defer func(a int) {
		println("defer  x,y= ", a, y) //1,102因为x此时就会拷贝参数
	}(x)

	x += 100
	y += 200
	println(x, y) //先输出 printlt(x,y) 101,102
	//1,102
}
```



### 数据

#### 字符串

字符串是不可变字节(byte)序列，其本身是一个复合结构。

```go
type stringstruct struct{
  str unsafe.Pointer
  len int
}
```

头部指针指向字节数组，但没有NULL结尾。默认以UTF-8编码存储Unicode字符，字 面量里允许使用十六进制、八进制和UTF编码格式。

内置函数len返回字节数组长度，cap不接受字符串类型参数。





允许以索引号访问字节数组(非字符)，但不能获取元素地址。



以切片语法(起始和结束索引号)返回子串时，其内部依旧指向原字节数组

使用for遍历字符串时，分byte和rune两种方式



```go
	//使用for遍历字符串时，分byte和rune两种方式。
	for i := 0; i < len(str); i++ {
		fmt.Printf("%c ", str[i])
	}
	fmt.Println()
	for i, c := range str {
		fmt.Printf("%d:[%c]", i, c)
	}
```





- 修改字符串

  要修改字符串，须将其转换为可变类型([]rune或[]byte)，待完成后再转换回 来。但不管如何转换，都须重新分配内存，并复制数据。

  字符串和[]byte互转

  ```go
  	b := []byte(str) //unit8
  	r := []rune(str) // int32 string to []byte
  
  	println("[]byte to string")
  	str = string(b)
  ```

  

```go
	b := []byte(str) //unit8
	r := []rune(str) // int32

	for i := 0; i < len(str); i++ {
		if i%2 == 0 {
			b[i] = 'd'
		}
	}

	for _, c := range r {
		fmt.Println(c)
	}
	fmt.Println(b, str)
	//
	println("byte to string")
	str = string(b)
	fmt.Println(b, str)

package main 
func main() { 
  m:=map[string]int{ "abc":123} 
  key:=[]byte("abc") 
  x,ok:=m[string(key)] 
  println(x,ok) 
}

```

#### 字符串优化

- 考虑到字符串只读特征，转换时复制数据到新分配内存是可以理解的。当然，性能

  同样重要，编译器会为某些场合进行专门优化，避免额外分配和复制操作：

  - 将[]byte转换为string key，去map[string]查询的时候。
  - 将string转换为[]byte，进行for range迭代时，直接取字节赋值给局部变量

- 除类型转换外，动态构建字符串也容易造成性能问题。用加法操作符拼接字符串时，每次都须重新分配内存。如此，在构建“超大”字符

  串时，性能就显得极差。

  - 预分配内存，把需要的直接分配 然后拷贝



#### unicode

类型rune专门用来存储Unicode码点（code point），它是int32的别名，相当于UCS-4/UTF-32编码格式。使用单引号的字面量，其默认类型就是==rune==



#### 切片

```go
type slice struct{
  array unsaft.Pointer
  len int
  cap int
}
```

左闭右开，右半开区间

<img src="https://gitee.com/matytan/tupic/raw/master/uPic/image-20211201170920841.png" alt="image-2021120117090841" style="zoom=20%;" />



#### json,bson and go

JSON 是一种轻量级的数据交换格式，常用作前后端数据交换，Go 在 [encoding/json](https://golang.org/pkg/encoding/json/) 包中提供了对 JSON 的支持。

把 Go struct 序列化成 JSON 对象，Go 提供了 [Marshal](https://golang.org/pkg/encoding/json/#Marshal) 方法，正如其含义所示表示编排序列化，函数签名如下：

```go
func Marshal(v interface{}) ([]byte, error)
```

注意：Go struct成员变量需要大写开头，才能被Marshal

```go
type Class struct {
	Name  string
	Grade int
}
```

```go
	stu := Student{
		Name: "tom",
		ID:   1023,
		Age:  14,
	} //不一定要在这里初始化全部
	cla := new(Class)
	cla.Name = "1班"
	cla.Grade = 4
	stu.Calss = cla

	jsonStu, err := json.Marshal(stu)
	if err != nil {
		println("failed to generate json", err)
	}
	fmt.Println(string(jsonStu))
```



`UnMarshal`:将json转换为结构,json字符串解析时，需要一个“接收体”接受解析后的数据，且Unmarshal时`接收体必须传递指针`。否则解析虽不报错，但数据无法赋值到接受体中。如这里用的是StuRead{}接收。

```go
func Marshal(data []byte,v interface{}) error 
```

```go
	//UnMarshal
	var Unstu Student
	err = json.Unmarshal(jsonStu, &Unstu)
	if err != nil {
		fmt.Println(err)
	}
	fmt.Println(Unstu) //有一个指针
	fmt.Println(Unstu.Calss)
```



JSON 对象一般都是小写表示，Marshal 之后 JSON 对象的首字母依然是大写，如果序列化之后名称想要改变如何实现，答案就是 **struct tags**。



##### Struct Tag

Struct tag 可以决定 Marshal 和 Unmarshal 函数如何序列化和反序列化数据。JSON object 中的 name 一般都是小写，我们可以通过 struct tag 来实现：

```go
type Student struct {
	Name  string `json:"name,omitempty"`
	ID    int    `json:"id,omitempty"`
	Age   int    `json:"age,omitempty"`
	Calss *Class `json:"class,omitempty"`
}
```





##### 任意类型接收

==使用interface unmarshal==

```go
	ba := []byte(`{"Name":"Wednesday","Age":6,"Parents":["Gomez","Morticia"]}`)
	var f interface{}
	err = json.Unmarshal(ba, &f)
	if err != nil {
		println(err)
	}
	fmt.Println(f)
	ma := f.(map[string]interface{}) //map[Age:6 Name:Wednesday Parents:[Gomez Morticia]]
	for k, v := range ma {
		switch vv := v.(type) {
		case string:
			fmt.Println(k, "is string", vv)
		case float64:
			fmt.Println(k, "is an float64")
		case []interface{}:
			fmt.Println(k, "is an array:")
			for i, u := range vv {
				fmt.Println(i, u)
			}
		default:
			fmt.Println(k, "is of a type I don't know how to handle")
		}
	}
```



```go
type StuRead struct {
	Name  interface{} `json:"name,omitempty"`
	ID    interface{} `json:"id,omitempty"`
	Age   interface{} `json:"age,omitempty"`
	Class interface{} `json:"class,omitempty"`
}
```



### 方法

当前包内任意任意类型定义方法(包括内置的(但是不在同一包）,但是首先得用type给一个别名类似于一个别名)

```go
type Integer int

func MethodTest(val int) {
	var x Interger = Interger(val)
	fmt.Println("MethodTest", x.Abs())
}
```





### 接口

接口的底层interface

Under the hood, interface values can be thought of as a tuple of a value and a concrete type:

在hook幕后下，接口值可以被视为值和具体类型的元组：

`(value, type)`

接口值保存特定底层具体类型的值。调用接口值上的方法会在其底层类型上执行同名方法。

无需在实现类型上添加显式声明,只要包含接口所需要的全部方法，即表示实现了该接口

```go
type F interface {
  f()
}

type S1 struct{}

func (s S1) f() {}

type S2 struct{}

func (s *S2) f() {}

// f1.f()无法修改底层数据
// f2.f() 可以修改底层数据,给接口变量f2赋值时使用的是对象指针
var f1 F = S1{}
var f2 F = &S2{}
```





`duck typing`方式,***Duck Typing 的原话是，走起来像鸭子、游泳起来像鸭子、叫起来也像鸭子，那么它就是一只鸭子。\***

这就是所谓的 Duck Typing，它==只关心事物的外部行为而非内部结构==。它并不关心你这只鸭子是长肉的还是充气的。



```go
type Printer interface {
	Print()
}

func (u *User) Print() {
	fmt.Printf("this is user name:%v,user age :%v\n", u.name, u.age)
}

// func (m *Manager) Print() {
// 	fmt.Printf("this is user name:%v,user age :%v,title:%v\n", m.name, m.age, m.title)
// }

func IntefaceTest() {
	var u User = User{"jack", 12}
	u.Print()
	var m Manager = Manager{u, "cto"} //调用匿名字段的方法，类似于继承
	m.Print()
}

```





#### Python 中的 Duck Typing

```python
def download(fetcher):     
	return fetcher.get("http://xxx");
```

有一个 download 函数，传过来一个 fetcher 参数，fetcher 是可以获取一个 url 链接的资源的。
这个 fetcher 就是一个 Duck Typing 的对象，使用者约定好这个 fetcher 会有一个 get 函数就可以了。
显然这个 download 函数会有以下问题：

运行时才知道传入的 fetcher 有没有 get 函数。那么站在 download 函数的使用者的角度上看，我怎么知道需要给 fetcher 实现 get 方法呢？我不可能去阅读 download 函数的代码，实际情况中，可能 download 函数的代码很长，可能 fetcher 不只要实现 get 方法，还有其它方法需要实现。通常这种情况需要通过加注释来说明。



#### C++ 中的 Duck Typing

使用模版来实现，编译时可以知道有没有实现`get` method

C++ 不是动态语言，但是它也能支持 Duck Typing，它是通过模板来支持的。

```c++
template <class F>
string download(const F& fetcher){
    return fetcher.get("http://xxxx")
}
```

这段代码与 Python 的实现方法类似，这个 fetcher 随便什么类型都可以，只要实现一个 get 方法，就能通过编译。

#### java 中的类似代码

Java 没有 Duck Typing，它只有类似的代码。Java 的 duck typing :

```java
<F extends FetcherInterface>
String download(F fetcher){
    return fetcher.get("http://xxxx")
}
```

它同样也用了模板类型。模板 F 必须 extends FetcherInterface ，有了这个限定，就能逼着 download 函数的使用者对 fetcher 实现 get 方法，它解决了需要注释来说明的缺点。

传入的参数必须实现 FetcherInterface 接口，就没有运行时发现错误，编译时发现错误的问题。
但是，它严格上来说不是 Duck Typing 。

如果 download 函数只依赖 fetcher 的 get 方法，而 FetcherInterface 接口必须要实现除 get 方法以外，还有其它方法，那么也要一一实现，非常不灵活。

#### Go中的Duck Typing

在 Java 的 Duck Typing 类似代码中，如果 ==fetcher 参数需要同时实现两个或以上的接口方法==时，Java 是没有办法做到的。但 Go 语言可以做到。

```go
type Fetcher interface {
    Get(url string) string
}

type Saver interface {
    Save(content string)
}

type FetcherAndSaver interface {
    Fetcher
    Saver
}

func download(f Fetcher) string {
    return f.Get("http://xxxx")
}

func save(f saver) {
    f.Save("some thing")
}

func downloadAndSave(f FetcherAndSaver) {
    content := f.Get("http://xxxx")
    f.Save(content)
}

# 实现者
type MyFetcherAndSaver struct {

}

func (f MyFetcherAndSaver) Get(url string) string {
    ...
}

func (f MyFetcherAndSaver) Save(content string) {
    ...
}

func main() {
  
    f := MyFetcherAndSaver{} //调用匿名接口的方法 类似于继承
    download(f)
    save(f)
    downloadAndSave(f)
}
```

这里定义了三个接口，只要有 Get 方法的就是 Fetcher，只要有 Save 方法的就是 Saver，同时有 Get 方法和 Save 方法就是 FetcherAndSaver 。

```go
//TODO:接口如何调用？
func DuckTest() {
	f := FetcherAndSaver{} //接口
	Download(f)
	Save(f)
}
```



- 空接口类型`interface{}`,用途类似OOP里的system.Object，可接收任意类型对象。



#### 转换



```go
//### 接口类型转换
type Person struct {
	Name string
	Age  int
}

func (p Person) String() string {
	return fmt.Sprintf("%v (%v years)", p.Name, p.Age)
}

type Stringer interface { //接口是一个值
	String() string
}

func SwichTest() string {
	var i Stringer = Person{"tom", 12}
	return interfaceType(i)

}

func interfaceType(value interface{}) string {
	switch vstr := value.(type) {
	case string:
		fmt.Println("string")
		return vstr

	case Stringer:
		fmt.Println("Stringer")
		return vstr.String()
	}
	return ""
}

//输出
Stringer
tom (12 years)
```



### 并发

Go routine 比普通协程和线程更加轻便

并发程序设计是一个比较大的主题，这里我们只讨论一些Go语言特有的亮点。

由于需要考虑很多繁琐的细节以保证对共享变量访问的正确型，使得并发编程在很多情况下都会变得异常复杂。Go语言鼓励开发者采用一种不同的方法，即将共享 变量通过Channel相互传递 —— 事实上并没有真正在不同的执行线程间共享数据 —— 的方式解决上述问题。在任意时刻，仅有一个Goroutine可以访问某个变量。数据竞争问题在设计上就被规避了。为了鼓励采用这种思维模式，我们将其总 结为一句口号：

> 勿以共享方式通信，以通信实现共享。



```go
go list.Sort()  // run list.Sort concurrently; don't wait for it.
```

#### Channels

与map结构类似，channel也是通过`make`进行分配的，其返回值实际上是一个指向底层相关数据结构的引用。如果在创建channel时提供一个可选的整型参数，会设置该channel的缓冲区大小。该值缺省为0，用来构建默认的“无缓冲channel”，也称为“同步channel”。



on't communicate by sharing memory, share memory by communicating. 

CSP: Communicating Sequential Process.

```go
c := make(chan int) //同步channel
c1 := make(chan int 0)
C2 := make(chan *os.File 1000)
```

通道默认是双向的，并不区分发送和接收端。但某些时候，我们可限制收发操作的方向来获得更严谨的操作逻辑。

尽管可用make创建单向通道，但那没有任何意义。通常使用类型转换来获取单向通道，并分别赋予操作双方。

```go
func main() {
	var wg sync.WaitGroup
	wg.Add(2)
	c := make(chan int)
	var send chan<- int = c
	var recv <-chan int = c
	go func() {
		defer wg.Done()
		for x := range recv {
			println(x)
		}
	}()
	go func() {
		defer wg.Done()
		defer close(c)
		for i := 0; i < 3; i++ {
			send <- i
		}
	}()
	wg.Wait()
}

```



##### 异步channel



#### for循环中的bug

```go
func Serve(queue chan *Request){
  for req := range queue{
    <-sem
    go func(){
      process(req)
    }()
  }
}
```

BUG源自Go中`for`循环的实现，循环的迭代变量会在循环中被重用，因此`req`变量会在所有Goroutine间共享。这不是我们所乐见的，我们需要保证`req`变量是每个Goroutine私有的。这里提供一个方法，将`req`的值以参数形式提供给goroutine对应的闭包：



可用==for...range==完成数据迭代，支持字符串、数组、数组指针、切片、字典、通道类型，返回索引、键值数据。

#### condtion条件变量

Golang的sync包中的Cond实现了一种条件变量，可以使用在多个Reader等待共享资源ready的场景（如果只有一读一写，一个锁或者channel就搞定了）。

Cond的汇合点：多个goroutines等待、1个goroutine通知事件发生。

每个Cond都会关联一个Lock（*sync.Mutex or *sync.RWMutex），当修改条件或者调用Wait方法时，必须加锁，**保护condition**。

```
type Cond struct {
        // L is held while observing or changing the condition
        L Locker
        // contains filtered or unexported fields
}


```

**NewCond**

```
func NewCond(l Locker) *Cond
```

新建一个Cond条件变量。



**Broadcast()**

```
func (c *Cond) Broadcast()
```

Broadcast会唤醒**所有**等待c的goroutine。

调用Broadcast的时候，可以加锁，也可以不加锁。



**Signal()**

```
func (c *Cond) Signal()
```

Signal只唤醒**1个**等待c的goroutine。

调用Signal的时候，可以加锁，也可以不加锁。



**wait函数**

```
func (c *Cond) Wait()
```

1. `Wait()`会自动释放`c.L`，并挂起调用者的goroutine。

2. 之后恢复执行

3. `Wait()`会在返回时对`c.L`加锁

   

除非被Signal或者Broadcast唤醒，否则`Wait()`不会返回。

由于`Wait()`第一次恢复时，`C.L`并没有加锁，所以当Wait返回时，调用者通常并不能假设条件为真。

取而代之的是, 调用者应该在循环中调用Wait。（简单来说，只要想使用condition，就必须加锁。）

```go
c.L.Lock()
for !condition() {
    c.Wait()
}
... make use of condition ...
c.L.Unlock()
```



#### select

多路复用：

如要同时处理多个通道，可选用select语句。它会==随机选择一个可用通道做收发操作==。

```go
select {
  case one:
  case two:
  default :
}
```

类似于switch语句，“switch 并不会自动下溯，但 case 可通过逗号分隔来列举相同的处理条件。”





```go
func SelectTest() {
	var wgsel sync.WaitGroup
	wgsel.Add(2)
	a, b := make(chan int), make(chan int)

	go func() { //接收端
		var (
			name string
			ok   bool
			x    int
		)
		defer wgsel.Done()
		for {
			select {
			case x, ok = <-a:
				name = "a"
			case x, ok = <-b:
				name = "b"
			}
			if !ok {
				return
			}
			println(name, x)
		}

	}()

	go func() { //发送端
		defer wgsel.Done()
		defer close(a)
		defer close(b)
		for i := 0; i < 10; i++ {
			select { //随机选择发送channel
			case a <- i:
			case b <- i * 10:
			}
		}
	}()
	wgsel.Wait()
}
```





如要等全部通道消息处理结束(closed)，可将已完成通道设置为nil。这样它就 会被阻塞，不再被select选中。

```go
func SelectTest2() {
	var wgsel sync.WaitGroup
	wgsel.Add(3) //3个done
	a, b := make(chan int), make(chan int)

	go func() { //接收端
		var (
			ok bool
			x  int
		)

		defer wgsel.Done()
		for {
			select {
			case x, ok = <-a:
				if !ok {
					a = nil
					break
				}
				println("a", x)
			case x, ok = <-b:
				if !ok {
					b = nil
					break
				}
				println("b", x)
			}
			if a == nil && b == nil {
				return
			}
		}

	}()

	go func() { //发送端
		defer wgsel.Done()
		defer close(a)
		for i := 0; i < 3; i++ {
			a <- i
		}
	}()

	go func() { //发送端
		defer wgsel.Done()
		defer close(b)
		for i := 0; i < 3; i++ {
			b <- i
		}
	}()
	wgsel.Wait()
}
```



同一个通道也会随机选择





### 异常处理



#### recover捕获异常



### 文件

1. 打开文件

   ```go
   func Open(name string) (*File, error)
   f:= os.Open("input.txt")
   
   func OpenFile(name string, flag int, perm FileMode) (*File, error) 
   //
   OpenFile("input.txt", O_RDWR|O_CREATE|O_TRUNC, 0666)
   ```

   

2. 文件操作以及`ioutil包`和`bufio`

   性能：**bufio** > **ioutil.ReadAll** > File自带Read

```go
func ReadAll(r io.Reader) ([]byte, error)  //Reader接口
ioutil.ReadAll(f)
//或者
func (f *File) Read(b []byte) (n int, err error) 
	b := make([]byte, 4) //最多读4个
	n, err := f.Read(b)
func (f *File) ReadAt(b []byte, off int64) (n int, err error) 

//bufio 读操作
func NewReader(rd io.Reader) *Reader 
func NewReaderSize(rd io.Reader, size int) *Reader //大于4kb
//写操作
func (f *File) Write(b []byte) (n int, err error)
func (f *File) WriteAt(b []byte, off int64) (n int, err error) 
```

- **当每次读取块的大小小于4KB，建议使用bufio.NewReader(f), 大于4KB用bufio.NewReaderSize(f,缓存大小)**
- **要读Reader, 图方便用ioutil.ReadAll()**
- **一次性读取文件，使用ioutil.ReadFile()**
- 不同业务场景，选用不同的读取方式



```go
func main() {
	// var x int = 2
	// f := ClosureTest(x)
	// f()
	f, err := os.Open("1.txt")
	if err != nil {
		log.Fatalln(err)
	}
	fmt.Println("open sucess")

	b := make([]byte, 4) //最多读4个
	//b1, err := ioutil.ReadAll(f)
	r := bufio.NewReader(f)

	n, err := f.Read(b)
	if err != nil {
		log.Fatalln(err, n)
	}
	fmt.Println(string(b))
	b1 := make([]byte, 4) //最多读4个 接着读 因为offset？
	n1, err := r.Read(b1) //按b容量？
	if err != nil {
		println(err)
	}
	fmt.Println(string(b1), n1)

	/* b1接着读
	matytan@matytan的MacBook-Pro test % go run closure.go
	open sucess
	dfdf
	jdjf 4
	*/
}
```



### 反射

反射(reflect)让我们能在运行期探知对象的类型信息和内存结构，这从一定程度上弥补了静态语言在动态行为上的不足。同时，反射还是实现元编程的重要手段。



对象与C数据结构一样，Go对象头部并没有类型指针，通过其自身是无法在运行期获知任何类型相关信息的。反射操作所需的全部信息都源自接口变量。接口变量除存储自 身类型外，还会保存实际对象的类型数据。

`func TypeOf(i interface{})Type `

`func ValueOf(i interface{})Value`





反射在带来“方便”的同时，也造成了很大的困扰。很多人对反射避之不及，因为它会造成很大的性能损失。但损失到底有多大?我们简单测试一下



### 调试 /测试

gdb调试：

o `go build-gcflags=all="-N -l" -ldflags=-compressdwarf=false`

| file <文件名> | 加载被调试的可执行程序文件                       |
| ------------- | ------------------------------------------------ |
| run           | 重新开始运行文件，简写r                          |
| start         | 单步执行，运行程序，停在第一执行语句             |
| list          | 查看源代码，简写l                                |
| set           | 设置变量的值                                     |
| next          | 单步调试（逐过程，函数直接执行）,简写n           |
| step          | 单步调试（逐语句：跳入自定义函数内部执行）,简写s |
| backtrace     | 查看函数的调用的栈帧和层级关系,简写bt            |
| frame         | 切换函数的栈帧,简写f                             |
| info          | 查看函数内部局部变量的数值,简写i                 |
| finish        | 结束当前函数，返回到函数调用点                   |
| continue      | 继续运行,简写c                                   |
| print         | 打印值及地址,简写p                               |
| quit          | 退出gdb,简写q                                    |





testing：

工具链和标准库自带单元测试框架，这让测试工作变得相对容易。

● 测试代码须放在当前包以“_test.go”结尾的文件中。

● 测试函数以Test为名称==前缀==。

● 测试命令(go test)忽略以“_”或“.”开头的测试文件。

● 正常编译操作(go build/install)会忽略测试文件。

