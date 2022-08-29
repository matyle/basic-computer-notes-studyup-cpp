## Go 线程同步 

1. 导入 sync包
   - `sync.WaitGroup`
2. 匿名函数前 可以加go(表示go route)

```go
package main

import "sync"

func main() {
	var a string
	var wg sync.WaitGroup
	wg.Add(1)
	go func() {
		a = "hello world"
		wg.Done()
	}() //调用传参数
	wg.Wait()
	println(a)
}

```





-race 参数
