# CPU工作模式

- CPU 的工作模式有实模式、保护模式、长模式

### 从一段死循环代码说起
- 这段代码的后果？
  - 首先关了cpu中断，停止响应中断信号。说明系统无法中断 会一直运行，地址从0开始写入0，清空了内存。
```cpp
int main(){
    int *addr = (int*)0;
    cli() //关中断
    while(1){
        addr = 0;
        addr++;
    }
    return 0;
}
```
- 很恐怖--实模式可以运行


### 实模式
- 实：真实的指令执行指令真实的功能，真实的地址，任何地址都没有限制的发往内存

- 实模式下的寄存器 16位
  - 其中栈指针：
    ```c
    push %ebp;
    实际操作：取值（icode，ifun，rA：rB，ValP），
    解码：ValA=R[rA],valB=R[%esp];
    执行：valE = valB +(-4); //地址减小
    访存：M[valE] = valA;//存入
    写回：R[%esp] =valE;


    为什么过程调用中需要 movl %esp %ebp;//保存起始esp，因为后面还需要入栈，esp还会变呀～～
    ```

-<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-19-10-48-00.png
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
    src=img/2021-05-19-10-45-21.png
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
    src=img/2021-05-19-10-51-49.png
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
    src=https://static001.geekbang.org/resource/image/f8/f8/f837811192730cc9c152afbcccf4eff8.jpeg
width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">实模式下的寄存器</div>
</center>


- 实模式下访问内存
  - 分段模式
  - 
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=https://static001.geekbang.org/resource/image/69/1c/690cbe5e4371eabeb9a1eaea6f37881c.jpg
  width=400px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;">图片描述</div>
  </center>
  

- 实模式中断
  
