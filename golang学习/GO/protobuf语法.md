参考谷歌文档，[翻译](https://colobu.com/2015/01/07/Protobuf-language-guide/#定义一个消息类型)

# 定义一个消息类型
```protobuf
message Request{
	required string query = 1;
  optional int32 page_number = 2;
  optional int32 result_per_page = 3;
	string example = 4
}
```

## 指定字段类型
在上面的例子中，所有字段都是标量类型：两个整型（page_number和result_per_page），一个string类型（query）。当然，你也可以为字段指定其他的合成类型，包括枚举（enumerations）或其他消息类型。

## 分配标识号
每一个消息字段都有一个唯一编码，这些数字用于以消息二进制格式标识字段，一旦使用消息类型，就不应更改
1-15 占用1字节编码
16-2047 占用2字节
因此，应该为非常频繁出现的消息字段保留字段编号 1 到 15

您可以指定的最小字段编号为 1，最大字段编号为$2^{29} - 1$ 或 536,870,911。您也不能使用数字 19000 到 19999（FieldDescriptor::kFirstReservedNumber 到 FieldDescriptor::kLastReservedNumber），因为它们是为 Protocol Buffers 实现保留的，如果您在 .proto 中使用这些保留数字之一，protocol buffer 编译器会报错。同样，不能使用任何以前保留的字段编号。


## 指定字段规则
必填（required）：格式良好的消息必须正好包含此字段之一。

可选(optional)：格式良好的消息可以包含此字段中的零或一个（但不得超过一个）。

重复(repeated)：此字段可以在格式良好的消息中重复任意次数（包括零）。将保留重复值的顺序。
由于一些历史原因，基本数值类型的repeated的字段并没有被尽可能地高效编码。在新的代码中，用户应该使用特殊选项[packed=true]来保证更高效的编码。如：
`repeated int32 samples = 4 [packed=true];`
Google的一些工程师得出了一个结论：使用required弊多于利；他们更 愿意使用optional和repeated而不是required。当然，这个观点并不具有普遍性。


## 标量数值类型

## 保留字段
如果您通过完全删除某个字段或将其注释掉来更新消息类型，则未来的用户可以在对类型进行自己的更新时重复使用该字段编号。如果他们以后加载同一 .proto 的旧版本，这可能会导致严重的问题，包括数据损坏、隐私错误等。确保不会发生这种情况的一种方法是指定保留
```protobuf
message Foo {
  reserved 2, 15, 9 to 11;
  reserved "foo", "bar";
}
```

## 枚举

当需要定义一个消息类型的时候，可能想为一个字段指定某“预定义值序列”中的一个值。例如，假设要为每一个SearchRequest消息添加一个 corpus字段，而corpus的值可能是UNIVERSAL，WEB，IMAGES，LOCAL，NEWS，PRODUCTS或VIDEO中的一个。 其实可以很容易地实现这一点：通过向消息定义中添加一个枚举（enum）就可以了。一个enum类型的字段只能用指定的常量集中的一个值作为其值（如果尝 试指定不同的值，解析器就会把它当作一个未知的字段来对待）。在下面的例子中，在消息格式中添加了一个叫做Corpus的枚举类型——它含有所有可能的值 ——以及一个类型为Corpus的字段：

```protobuf
message SearchRequest {
  required string query = 1;
  optional int32 page_number = 2;
  optional int32 result_per_page = 3 [default = 10];
  enum Corpus {
    UNIVERSAL = 0;
    WEB = 1;
    IMAGES = 2;
    LOCAL = 3;
    NEWS = 4;
    PRODUCTS = 5;
    VIDEO = 6;
  }
  optional Corpus corpus = 4 [default = UNIVERSAL];
}
enum EnumAllowingAlias {
  option allow_alias = true;
  UNKNOWN = 0;
  STARTED = 1;
  RUNNING = 1;
}
enum EnumNotAllowingAlias {
  UNKNOWN = 0;
  STARTED = 1;
  // RUNNING = 1;  // Uncommenting this line will cause a compile error inside Google and a warning message outside.
}
```

## 导入定义
在上面的例子中，Result消息类型与SearchResponse是定义在同一文件中的。如果想要使用的消息类型已经在其他.proto文件中已经定义过了呢？
你可以通过导入（importing）其他.proto文件中的定义来使用它们。要导入其他.proto文件的定义，你需要在你的文件中添加一个导入声明，如：
```protobuf
import "myproject/other_protos.proto";
```