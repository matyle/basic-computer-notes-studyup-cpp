### go服务器简单示例
```go

```

### 服务器处理流程
	1. 创建listen socket，绑定特定端口（linux网络编程）
	2. accept请求
	3. 处理客户端的请求，首先从 Client Socket 读取 HTTP 请求的协议头，如果是 POST 方法，还可能要读取客户端提交的数据，然后交给相应的 handler 处理请求，handler 处理完毕准备好客户端需要的数据，通过 Client Socket 写给客户端。
	
	即：
	- 监听端口
	- 接受请求
	- 分配handler

```go

func (srv *Server) Serve(l net.Listener) error {
    defer l.Close()
    var tempDelay time.Duration // how long to sleep on accept failure
    for {
        rw, e := l.Accept() //类似于返回一个conn描述符
        if e != nil {
            if ne, ok := e.(net.Error); ok && ne.Temporary() {
                if tempDelay == 0 {
                    tempDelay = 5 * time.Millisecond
                } else {
                    tempDelay *= 2
                }
                if max := 1 * time.Second; tempDelay > max {
                    tempDelay = max
                }
                log.Printf("http: Accept error: %v; retrying in %v", e, tempDelay)
                time.Sleep(tempDelay)
                continue
            }
            return e
        }
        tempDelay = 0
		    //生成conn描述符对象？
        c, err := srv.newConn(rw)
        if err != nil {
            continue
        }
			// goroutine
        go c.serve()
    }
}
```

监控之后如何接收客户端的请求呢？上面代码执行监控端口之后，调用了 srv.Serve(net.Listener) 函数，这个函数就是处理接收客户端的请求信息。这个函数里面起了一个 for{}，首先通过 Listener 接收请求，其次创建一个 Conn，最后单独开了一个 goroutine，把这个请求的数据当做参数扔给这个 conn 去服务：go c.serve()。这个就是高并发体现了，用户的每一次请求都是在一个新的 goroutine 去服务，相互不影响。

那么如何具体分配到相应的函数来处理请求呢？conn 首先会解析 `request:c.readRequest()`, 然后获取相应的 `handler:handler := c.server.Handler`，也就是我们刚才在调用函数` ListenAndServe` 时候的第二个参数，我们前面例子传递的是 nil，也就是为空，那么默认获取 handler = DefaultServeMux, 那么这个变量用来做什么的呢？对，这个变量就是一个路由器，它用来匹配 url 跳转到其相应的 handle 函数，那么这个我们有设置过吗？有，我们调用的代码里面第一句不是调用了 http.HandleFunc("/", sayhelloName) 嘛。这个作用就是注册了请求 / 的路由规则，当请求 uri 为 "/"，路由就会转到函数 sayhelloName，DefaultServeMux 会调用 ServeHTTP 方法，这个方法内部其实就是调用 sayhelloName 本身，最后通过写入 response 的信息反馈到客户端。
![3.3.illustrator](https://gitee.com/matytan/tupic/raw/master/uPic/3.3.illustrator.png)