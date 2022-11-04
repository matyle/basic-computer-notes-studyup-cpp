



## defer配合recover

recover只能配合defer才能生效！

注意，recover()函数只在defer的上下文中才有效（且只有通过在defer中用==匿名函数调用才有效==），==直接调用的话，只会返回`nil`==





```go
func main() {
	defer fmt.Println("defer main")
	//var user = os.Args[1] //参数
	var user = os.Getenv("USER_")
	done := make(chan int, 1)
	go func() {
		defer func() {
			fmt.Println("defer caller")
			if err := recover(); err != nil {
				fmt.Println("recover success. err:", err)
			}
		}()

		func() {
			defer func() {
				fmt.Println("defer here")
			}()
			if user == "" {
				panic("should set env user")
			}
			fmt.Println("after panic")
			fmt.Println(user)
			done <- 1
		}()
	}()

	<-done
}

```

