## 什么是context

上下文：goroutine的上下文，包含 goroutine 的运行状态、环境、现场等信息。

context 主要用来在 goroutine 之间传递上下文信息，包括：取消信号、超时时间、截止时间、k-v 等。



context.Context 类型的值可以协调多个 groutine 中的代码执行“取消”操作，并且可以存储键值对。最重要的是它是并发安全的。



与它协作的 API 都可以由外部控制执行“取消”操作，例如：取消一个 HTTP 请求的执行





## context作用



```go
  1 package main
  2 
  3 import (
  4     "fmt"
  5     "time"
  6 
  7     "golang.org/x/net/context"
  8 )
  9 
 10 func main() {
 11     ctx, cancel := context.WithTimeout(context.Background(), time.Millisecond*1000)
 12     defer cancel()
 13     go handle(ctx, 500*time.Millisecond)
 14     select {
 15     case <-ctx.Done():
 16         fmt.Println("main", ctx.Err())  
 17     }
 18 }                                                                                                                                   
 19 func handle(ctx context.Context, duration time.Duration) {
 20     select {
 21     case <-ctx.Done():
 22         fmt.Println("handle", ctx.Err())
 23     case <-time.After(duration):    
 24         fmt.Println("process request with", duration)
 25     }
 26 }    
```

Go server ==一个协程对应一个请求==

![request](https://gitee.com/matytan/tupic/raw/master/uPic/0.png)

这些 goroutine 需要共享这个请求的基本数据，例如登陆的 token，处理请求的最大超时时间（如果超过此值再返回数据，请求方因为超时接收不到）等等。当请求被取消或是处理时间太长，这有可能是使用者关闭了浏览器或是已经超过了请求方规定的超时时间，请求方直接放弃了这次请求结果。这时，所有正在为这个请求工作的 goroutine 需要快速退出，因为它们的“工作成果”不再被需要了。在相关联的 goroutine 都退出后，系统就可以回收相关的资源。





Go 语言中的 server 实际上是一个“协程模型”，也就是说一个协程处理一个请求。例如在业务的高峰期，某个下游服务的响应变慢，而当前系统的请求又没有超时控制，或者超时时间设置地过大，那么等待下游服务返回数据的协程就会越来越多。而我们知道，协程是要消耗系统资源的，后果就是协程数激增，内存占用飙涨，甚至导致服务不可用。更严重的会导致雪崩效应，整个服务对外表现为不可用，这肯定是 P0 级别的事故。这时，肯定有人要背锅了。



Go 里，我们不能直接杀死协程，协程的关闭一般会用 `channel+select`方式来控制。但是在某些场景下，例如处理一个请求衍生了很多协程，这些协程之间是相互关联的：需要共享一些全局变量、有共同的 deadline 等，而且可以同时被关闭。再用 `channel+select`就会比较麻烦，这时就可以通过 context 来实现。



## 默认上下文

[`context`](https://github.com/golang/go/tree/master/src/context) 包中最常用的方法还是 [`context.Background`](https://draveness.me/golang/tree/context.Background)、[`context.TODO`](https://draveness.me/golang/tree/context.TODO)，这两个方法都会返回预先初始化好的私有变量 `background` 和 `todo`，它们会在同一个 Go 程序中被复用：









## 如何使用

### 作为取消



context 使用起来非常方便。源码里对外提供了一个==创建根节点 context== 的函数：

```go
func Background() Context
```

background 是一个空的 context， 它不能被取消，没有值，也没有超时时间。

有了根节点 context，又提供了四个函数创建子节点 context：

```go
func WithCancel(parent Context) (ctx Context, cancel CancelFunc)
func WithDeadline(parent Context, deadline time.Time) (Context, CancelFunc)
func WithTimeout(parent Context, timeout time.Duration) (Context, CancelFunc)
func WithValue(parent Context, key, val interface{}) Context
```

context 会在函数传递间传递。只需要在适当的时间调用 cancel 函数向 goroutines 发出取消信号或者调用 Value 函数取出 context 中的值。

在官方博客里，对于使用 context 提出了几点建议：



>1. Do not store Contexts inside a struct type; instead, pass a Context explicitly to each function that needs it. The Context should be the first parameter, typically named ctx.
>
>2. Do not pass a nil Context, even if a function permits it. Pass context.TODO if you are unsure about which Context to use.
>
>3. Use context Values only for request-scoped data that transits processes and APIs, not for passing optional parameters to functions.
>
>4. The same Context may be passed to functions running in different goroutines; Contexts are safe for simultaneous use by multiple goroutines.
>
>   
>
>1. 不要将 Context 塞到结构体里。直接将 Context 类型作为函数的第一参数，而且一般都命名为 ctx。
>2. 不要向函数传入一个 nil 的 context，如果你实在不知道传什么，标准库给你准备好了一个 context：todo。
>3. 不要把本应该作为函数参数的类型塞到 context 中，context 存储的应该是一些共同的数据。例如：登陆的 session、cookie 等。
>4. 同一个 context 可能会被传递到多个 goroutine，别担心，context 是并发安全的。





### 传递共享的数据

```go
func context.WithValue(parent context.Context, key interface{}, val interface{}) context.Context
```



```go
package main

import (
	"context"
	"fmt"
	"time"
)

func main() {
	ctx := context.Background()
	go process(ctx)
	ctx = context.WithValue(ctx, "id", "2019-02")

	go process(ctx)

	time.Sleep(time.Second * 5)

}

func process(ctx context.Context) {
	traceId, ok := ctx.Value("id").(string)

	if ok {
		fmt.Printf("process over. trace_id=%s\n", traceId)
	} else {
		fmt.Printf("process over. no trace_id\n")
	}
}
```





### 取消goroutine



我们先来设想一个场景：打开外卖的订单页，地图上显示外卖小哥的位置，而且是每秒更新 1 次。app 端向后台发起 websocket 连接（现实中可能是轮询）请求后，后台启动一个协程，每隔 1 秒计算 1 次小哥的位置，并发送给端。如果用户退出此页面，则后台需要“取消”此过程，退出 goroutine，系统回收资源。

后端可能的实现如下：

```go
func Perform() {
    for {
        calculatePos()
        sendResult()
        time.Sleep(time.Second)
    }
}
```





如果需要实现“取消”功能，并且在不了解 context 功能的前提下，可能会这样做：给函数增加一个指针型的 bool 变量，在 for 语句的开始处判断 bool 变量是发由 true 变为 false，如果改变，则退出循环。





上面给出的简单做法，可以实现想要的效果，没有问题，但是并不优雅，并且一旦协程数量多了之后，并且各种嵌套，就会很麻烦。



优雅的做法，自然就要用到 context:。

```go
func Perform(ctx context.Context) {
    for {
        calculatePos()
        sendResult()
        select {
        case <-ctx.Done():
            // 被取消，直接返回
            return
        case <-time.After(time.Second):
            // block 1 秒钟 
        }
    }
}

func mian(){
  ctx, cancel := context.WithTimeout(context.Background(), time.Hour)
  go Perform(ctx)

  // ……
  // app 端返回页面，调用cancel 函数
  cancel()
}
```

注意一个细节，WithTimeOut 函数返回的==context 和 cancelFun== 是分开的。context 本身并没有取消函数，这样做的原因是取消函数只能由外层函数调用，防止子节点 context 调用取消函数，从而严格控制信息的流向：由父节点 context 流向子节点 context。





### 防止goroutine泄漏



前面那个例子里，goroutine还是会自己执行完，最后返回，只不过会多浪费一些系统资源。这里改编一个“如果不用 context 取消，goroutine 就会泄漏的例子”，来自参考资料：`【避免协程泄漏】`。

```go
func gen() <-chan int {
    ch := make(chan int)
    go func() {
        var n int
        for {
            ch <- n
            n++
            time.Sleep(time.Second)
        }
    }()
    return ch
}
```



泄露例子如下：

```go

func main() {
	f, _ := os.Create("trace.out")
	defer f.Close()
	trace.Start(f)
	defer trace.Stop()
	for n := range gen() {
		fmt.Println(n)
		if n == 5 {
			break
		}
	}
}

func gen() <-chan int {
	ch := make(chan int)
	go func() {
		var n int
		for {
			ch <- n
			n++
			time.Sleep(time.Second)
		}
	}()
	return ch
}
```

![image-20211222173512576](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211222173512576.png)



使用context改进

```go

func main() {

	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()
	f, _ := os.Create("trace.out")
	defer f.Close()
	trace.Start(f)
	defer trace.Stop()
	for n := range gen(ctx) {
		fmt.Println(n)
		if n == 5 {
			break
		}
	}
}

func gen(ctx context.Context) <-chan int {
	ch := make(chan int)
	go func() {
		var n int
		for {
			select {
			case <-ctx.Done():
				return
			case ch <- n:
				n++
				time.Sleep(time.Second)
			}
		}
	}()
	return ch
}
```

![image-20211222172719674](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211222172719674.png)







### 如何查找

<img src="https://gitee.com/matytan/tupic/raw/master/uPic/4.png" alt="classs" style="zoom:30%;" />