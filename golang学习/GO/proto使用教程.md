## 了解protobuf

### 优点
- 跟Json相比protobuf性能更高，更加规范
- 编解码速度快，数据体积小
- 使用统一的规范，不用再担心大小写不同导致解析失败等问题
### 缺点
- 改动协议字段，需要重新生成文件。
- 数据没有可读性
## 安装
1. 使用brew 安装protoc
`brew install protobuf`
3. 安装go插件
```bash
go get github.com/golang/protobuf/protoc-gen-go
#或者
go get github.com/gogo/protobuf/protoc-gen-gogofaster
```
### 报错一
```bash
➜  protobuf.test protoc --go_out=. *.proto
protoc-gen-go: program not found or is not executable
Please specify a program using absolute path or make sure the program is available in your PATH system variable
--go_out: protoc-gen-go: Plugin failed with status code 1.
```

由于protoc-gen-go是通过go get安装时得到的，安装在$GOPATH/go/bin目录下
我们直接去该目录下找到protoc-gen-go，执行命令

`cp protoc-gen-go /usr/local/bin/`

### 报错二


## 生成go文件
```bash
protoc --gogofaster_out=. *.proto
```

去除omitempty关键字
```bash
 /d/work/src/新建文件夹/新建文件夹/falcon/src/srv/order (dev-bbtc/v0.0.3)
 ls *.pb.go | xargs -n1 -IX bash -c 'sed s/,omitempty// X > X.tmp && mv X{.tmp,}'
```

```bash
protoc --gogofaster_out=paths=source_relative:. pb/indexconfinfopb/indexconfinfo.proto
```