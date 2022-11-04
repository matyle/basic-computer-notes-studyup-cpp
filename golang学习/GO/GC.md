





## 检测工具

### 方法一 `GODEBUG=gctrace=1`

```bash
go build -o main
GODEBUG=gctrace=1 ./main
```



### 方式2：`go tool trace` [#](https://golang.design/go-questions/memgc/principal/#方式2go-tool-trace)

创建trace.out文件：

```go
f, _ := os.Create("trace.out")
defer f.Close()
trace.Start(f)
defer trace.Stop()
```

运行：

```bash
go tool trace trace.out # trace.out文件
```

![image-20211222173553444](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211222173553444.png)

![image-20211222173520263](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211222173520263.png)

### 方式3：`debug.ReadGCStats`

```go
func printGCStats() {
	t := time.NewTicker(time.Second)
	s := debug.GCStats{}
	for {
		select {
		case <-t.C:
			debug.ReadGCStats(&s)
			fmt.Printf("gc %d last@%v, PauseTotal %v\n", s.NumGC, s.LastGC, s.PauseTotal)
		}
	}
}
func main() {
	go printGCStats()
	(...)
}
```



## 内存泄露



在一个具有 GC 的语言中，我们常说的内存泄漏，用严谨的话来说应该是：预期的能很快被释放的内存由于附着在了长期存活的内存上、或生命期意外地被延长，导致预计能够立即回收的内存而长时间得不到回收。（类似于陈硕网路库中的说法）





### 形式1：预期能被快速释放的内存因被根对象引用而没有得到迅速释放

当有一个全局对象时，可能不经意间将某个变量附着在其上，且忽略的将其进行释放，则该内存永远不会得到释放。例如：

