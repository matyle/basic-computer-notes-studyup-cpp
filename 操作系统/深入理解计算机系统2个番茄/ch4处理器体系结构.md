### 4 处理器体系结构
#### 4.1 Y86体系结构
- 程序员可见状态
  - 程序寄存器：%esp被入栈，出栈，调用，返回指令作为栈指针，其他不固定
  - 条件码：
  - - ZF 零标志
    - SF 符号标志 得到一个负数
    - OF 溢出标志，二进制补码溢出 最近的操作导致一个补码溢出
  - PC 程序计数器，正在执行的指令的地址
  - 存储器（内存）
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-15-09-49-14.png
width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">4.1 Y86程序员可见状态</div>
</center>

- Y86指令集：总共6个字节：00为1个字节
  1. nop指令：无实际功能
  2. halt指令：停止指令的执行。与之类似的hlt会使整个系统停止
  3. movel指令：指令第一个字母表明了源的类型 i为立即数，r寄存器，m为存储器m
   存储器引用方式为简单的基地址加位移形式，不允许从一个立即数直接存储到存储器；不允许从存储器地址直接传送到另一个存储器地址
     1. irmovl:源是立即数，目的是寄存器
     2. rrmovl：
     3. mrmovl：
     4. rmmovl：
  4. OPl：四个整数操作指令：addl，subl，andl，xorl 只对寄存器进行操作
  5. jxx：7个跳转指令：jmp，jle（有符号小于等于），jl（小于），je（等于），jne（不等），jge（大于等于），jg（大于） 
     1. 如cmp y x; x>y跳转
  6. call：将返回地址入栈，跳到目的地址
  7. ret：从过程调用中返回
  8. pushl，popl
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-15-09-59-18.png
width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">4.2 Y86指令集</div>
</center>

- 指令字节级编码：高四位是代码部分，低四位是功能部分
  - 代码值：0-B
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-15-10-47-01.png
width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">4.3指令集功能码</div>
</center>


- 寄存器标志符 0-7 ，程序寄存器被存在cpu的寄存器文件中，即一个以寄存器标志符id为地址的寻址的表
- 8表明此处没有寄存器
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-15-10-51-42.png
width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">4.4 y86程序寄存器标识符</div>
</center>


#### 4.2 逻辑设计和硬件控制语言hcl
- 逻辑1是1v左右的高电压，0是0.0v左右的低电压
- 逻辑门：and or not
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-16-15-02-38.png
  width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
  </center>
- 异或的实现：(!a||!b)&&(a||b);
- 时钟寄存器：寄存器：存储单个位或者字
  - 两个读端口，一个写端口
- 随机访问存储器（内存）：

#### 4.3 Y86顺序实现：
- 取指：将程序计数器pc作为地址，指令存储器读取一个指令的字节（8位）。pc增加计算valp
  - 取出icode，ifun；rA，rB（一个或者两个），或取值valC（一般作为偏移量）
- 解码：寄存器文件有两个读端口A和B，从这两个端口同时读valA，valB。R[rA]和R[rB]
    <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-16-16-35-49.png
    width=300px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
    </center>
- 执行：根据指令的类型即（ifun） ALU（算术逻辑单元计算）。计算存储器引用的有效地址，或者增加或减少栈指针，得到valE；另外也可能设置条件码
- 访存：将数据写入存储器或者读出数据 读出该值叫valM
- 写回：最多写两个结果到寄存器文件
- 更新pc：将pc设置为下一条指令的地址
<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-16-16-07-32.png
width=600px>
  <br>
  <div style="color:orange; border-bottom: 1px solid #d9d9d9;
  display: inline-block;
  color: #999;
  padding: 2px;"></div>
</center>

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-17-10-08-34.png
width=500px>
</center>


<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-16-16-06-45.png
width=600px>
  <br>
  <div style="color:orange; border-bottom: 1px solid #d9d9d9;
  display: inline-block;
  color: #999;
  padding: 2px;">顺序实现</div>
</center>

- push与pop
<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src= img/2021-05-16-16-08-12.png
width=600px>
  <br>
  <div style="color:orange; border-bottom: 1px solid #d9d9d9;
  display: inline-block;
  color: #999;
  padding: 2px;"></div>
</center>


<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-16-16-21-55.png
width=600px>

</center>

#### 4.3.2 SEQ硬件结构
<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-16-16-54-07.png
width=400px>
  <br>
  <div style="color:orange; border-bottom: 1px solid #d9d9d9;
  display: inline-block;
  color: #999;
  padding: 2px;"></div>
</center>

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-16-16-55-15.png
width=400px>
  <br>
  <div style="color:orange; border-bottom: 1px solid #d9d9d9;
  display: inline-block;
  color: #999;
  padding: 2px;"></div>
</center>

#### 4.3.3 SEQ的时序 p251

- 程序计数器，条件码寄存器，数据存储器，寄存器文件四个硬件需要定序
- 这些单元通过一个时钟信号来控制，它触发将新值装载到寄存器以及将值写到随机访问存储器
- 重要原则：处理器从来不需要为了完成一条指令的执行而去读由该指令更新的状态

-<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-17-09-36-54.png
width=500px>
  <br>
  <div style="color:orange; border-bottom: 1px solid #d9d9d9;
  display: inline-block;
  color: #999;
  padding: 2px;"></div>
</center>

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-17-09-21-46.png
width=400px>
  <br>
  <div style="color:orange; border-bottom: 1px solid #d9d9d9;
  display: inline-block;
  color: #999;
  padding: 2px;">4.23</div>
</center>


#### 4.3.4 SEQ的阶段实现 
- seq所需要的控制逻辑块的hcl描述。
<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-17-09-38-05.png
width=400px>
  <br>
  <div style="color:orange; border-bottom: 1px solid #d9d9d9;
  display: inline-block;
  color: #999;
  padding: 2px;"></div>
</center>

- 取指阶段
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-17-09-40-31.png
  width=400px>
  </center>
  

- 解码和写回
  - 读的两个端口AB，两个写回的端口ME
  - srcA srcB 读端口的地址 ，dstA，dstB写端口的地址
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-17-09-51-15.png
  width=450px>
  </center>
  

- 执行阶段

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-17-10-05-25.png
width=400px>
</center>

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-17-10-06-12.png
width=400px>
</center>

- 访存阶段
<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-17-10-10-55.png
width=400px>
</center>

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-17-10-11-50.png
width=600px>
</center>

- 更新PC阶段

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-17-10-17-07.png
width=500px>
</center>

#### 4.4流水线的通用原理
<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-18-11-08-22.png
width=400px>
  <br>
  <div style="color:orange; border-bottom: 1px solid #d9d9d9;
  display: inline-block;
  color: #999;
  padding: 2px;"></div>
</center>

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-18-11-09-57.png
width=400px>
  <br>
  <div style="color:orange; border-bottom: 1px solid #d9d9d9;
  display: inline-block;
  color: #999;
  padding: 2px;"></div>
</center>

- 对信号进行重新排序和标号
- 冒险
  - 数据冒险 ：在某个周期内，上面对指令还没有更新
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-18-11-29-25.png
  width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
  </center>
  
  - 用暂停来避免数据冒险：
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-18-11-33-41.png
  width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">图片描述</div>
  </center>
  
  - 转发
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-19-09-18-31.png
  width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
  </center>
  
  - 暂停+转发（加载/使用）
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-19-09-20-34.png
  width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">图片描述</div>
  </center>
  