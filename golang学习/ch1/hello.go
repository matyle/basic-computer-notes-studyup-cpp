package main

import (
	"fmt"
	"math"
	"strings"
	"reflect"
)

func main(){


	fmt.Println(math.Floor(12.3))
	fmt.Println(strings.Title("auto"))
	fmt.Println(reflect.TypeOf(1.4))
	fmt.Println(reflect.TypeOf("fja"))
	fmt.Println(4>5)

	//变量声明
	var quantity int
	var length,width float64
	var name string

	quantity = 4
	length = 1.4
	width = 1.5
	name = "tom"

	fmt.Println(quantity)
	fmt.Println(length*width)
	fmt.Println(name)


	//同一行赋值
	var friend string = "demon"
	fmt.Println(friend)
}
