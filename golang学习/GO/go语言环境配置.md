



## 使用Go基本配置



```bash
go env -w GOBIN=/usr/local/go/bin
go env -w GO111MODULE=on
go env -w GOPROXY=https://goproxy.cn,direct
```

```bash
GO111MODULE=off 不使用go mod，go 会从 GOPATH 和 vendor 文件夹寻找包。
GO111MODULE=on 使用go mod，go会忽略GOPATH设置，只根据 go.mod 下载依赖。
GO111MODULE=auto 在 $GOPATH/src 外面且根目录有 go.mod 文件时，开启模块支持。
```



## 为什么使用go mod

我们先来看一下之前GOPATH模式的一些缺点：

1. 没法在GOPATH工作区以外的地方写代码
2. 不能实现依赖的版本化管理



当我们在GOPATH之外创建项目并使用外部依赖时，运行时go会提醒我们找不到导入的包。那我们怎么解决这个问题呢？

我们可以使用一个特殊的文件，使用它指定仓库的规范名称。这个文件可以理解为是GOPATH的一个替代品，在它其中定义了仓库的规范名称，这样go就可以通过这个名称解析源码的导入包的位置。

## 例子

```bash
   71  mkdir example
   74  cd example
   76  go mod init
   77  go mod init example
   79  vim go.mod
   
   #go.mod内容
   module example
	 go 1.13
```

