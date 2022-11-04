

编译器protoc-gen-go， 如果没有设置$GOBIN，将会被安装到`GOPATH/bin` 下。
```bash
# 在GOPATH/bin目录 下
cp protoc-gen-go /usr/local/bin
```
编译器protoc-gen-go必须在path目录下，编译的时候才能找到。
如果需要下载指定版本的protoc-gen-go，那么：

```bash
GIT_TAG="v1.2.0" # change as needed version
go get -d -u github.com/golang/protobuf/protoc-gen-go
git -C "$(go env GOPATH)"/src/github.com/golang/protobuf checkout $GIT_TAG
go install github.com/golang/protobuf/protoc-gen-go

```