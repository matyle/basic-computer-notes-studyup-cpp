# IO库

## 8.1 IO类
- 所谓流：就是FILE*类型
- iostrem
- fstream
- sstream
- 以上流的使用都是相同的，通过继承和模板实现，例如getline,<<和>>操作符都相同
- 例如cin>>str 将一个标准输入读取到str中 ifstream >> str 从ifstream读取到str中
- cout<<str 从str中写到标准输出文件描述符中，ofstream<< str 写到文件中


### IO对象无拷贝和赋值

### 条件状态

- IO操作一个问题是发生错误，发生错误无法避免，一些可恢复一些不能恢复
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-26-10-43-13.png
width=490px>
</center>

```cpp
int ival;
cin>> ival; //输入整数，如果输入Boo报错

while(cin>>ival){
    //ok操作成功
}
//循环检查>>返回的流的状态
```

- 查询流的状态
  - badbit表示系统级错误，一旦置位无法恢复
  - failbit 期望数值，却出现字符，可以被修复
  - eofbit: 文件到达结束位置，failbit也会被置位
  - 上面任意一个置位，检查流条件就会失败

- 管理条件状态
  ```cpp
  auto old_state = cin.rdstate();//记住cin当前状态
  cin.clear()//使cin有效
  process_input(cin);//使用
  cin.setstate(old_state);//恢复原有状态

  复位failbit和eofbit
  cin.clear(setstate()&~cin.failbit&~cin.eofbit);

  istream&  process_input(istream &is){
        auto old_state = is.rdstate();//记住cin当前状态
        is.clear()//使cin有效
        string str;
        while(is >> str){
            cout<<str;
        }
        is.setstate(old_state);
        return is;

  }
  ```

### 管理输出缓冲
缓冲区可以将多个输出操作组合成单一的系统级写操作，提升性能
- 刷新输出缓冲区
  ```cpp
  cout<<"hi"<<endl;//输出换行，并刷新缓冲区
  cout<<"hi"<<flush;//刷新缓冲区
  cout<<"hi"<<ends;//刷新缓冲区,加一个空字符

  //unitbuf操纵符
  //可以在每次输出后都刷新缓冲区
  cout<<unitbuf; //所有输出操作后立即刷新
  cout<<nounitbuf; //恢复正常缓冲方式、

- 关联输出与输出流
```cpp
cin.tie(&cout);//关联
```  



## 文件输入输出


- ifstream : 从一个给定文件读取数据
- ofstream : 向一个文件写入数据
- fstream ： 读写给定文件，
- 可以使用<<和>>以及getline 从一个ifstream读取数据
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-27-09-57-30.png
width=490px>
</center>


### 使用文件流对象

```cpp
ifstream in(file); //构造一个ifstream并打开给定文件
ofstream out;// 输出文件流未绑定到任何文件
```
- file可以是sting或者字符数组

- 用fstream代替iostream
- - 使用open和close打开关闭文件
  - 使用读取文件和写入文件方式代替标准输入输出
  ```cpp
  ifstream input(argv[1]);
  ofstream output(argv[2]);

  //如果定义空文件流对象
  ifstream in(ifile);
  ofstream out;
  out.open(ifile + ".copy");
  ```

  ### 文件模式
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-27-10-32-23.png
  width=490px>
  </center>
  
  ```cpp
  in : 读方式 std::in 只能对ifstream设置
  ```

  - 默认情况下，打开一个ofstream时，文件内容会被丢弃，覆盖写入（因为文件指针）。，阻止清空的方法，使用app模式，追加内容
  ```cpp
  ofstream out("file1");//默认覆盖
  ofstream out("fie1",ofstream::out);//隐式截断文件 默认阶段
  ofstream out("file1",ofstream::app);//隐含输出，且追加

  ofstream out1;
  out1.open("file1",ofstream::app);//追加 每次打开文件如果不想被覆盖都需要指定
  ```


## string流

- istringstream
- ostringstream
- 头文件sstream,stringstream
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-27-10-51-58.png
width=490px>
</center>
