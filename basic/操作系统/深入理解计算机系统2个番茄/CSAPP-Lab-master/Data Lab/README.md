# Data Lab

## 介绍

学生实现简单的逻辑，二进制补码和浮点函数，但使用C语言运算符受限。例如，可能会要求他们仅使用位级运算和直线代码来计算数字的绝对值。本实验可帮助学生理解C数据类型的位表示形式以及对数据进行操作的位级别行为。

[实验地址](http://csapp.cs.cmu.edu/3e/labs.html)

[README文档](http://csapp.cs.cmu.edu/3e/README-datalab)

[实验指导](http://csapp.cs.cmu.edu/3e/datalab.pdf)

## 实验步骤

1. 阅读`bits.c`文件的注释，在函数处填入合适的代码。
2. 命令行使用`./dlc bits.c`命令，检查实验编码规范是否符合。
3. 使用`make`命令进行编译文件。
4. 使用`./btest`程序检查函数的正确性。
5. 使用`./driver.pl`获取函数正确性以及评分。

下图为实验步骤的展示。
![](../Resource/实验步骤.png)

### 实验内容

#### bitXor
```c
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  return ~(~(x & ~y) & ~(~x & y));
}
```

> 仅使用`~`和`&`运算符实现异或。

分析：
我们需要求：相同为1，不同为0。我们位操作数是a, b。

当`a = 1, b = 0` -> `a & ~b = 1 = a ^ b`。

当`a = 0，b = 0` -> `a & ~b = 0 = a ^ b`。

当`a = 1, b = 1` -> `a & ~b = 0 = a ^ b`。

当`a = 0, b = 1` -> `a & ~b = 0 != a ^ b`。

我们使用`a & ~b`，发现在位操作数是1和0的情况下，只需要把为0的操作数取反即可以得到异或的效果。而我们并不知道取反的位操作数是第一个操作数还是第二个，我们可以使用`(a & ~b) | (~a & b)`来达到效果。我们可以使用`&`和`~`来达到`|`的效果。
我们利用`a | b = ~(~a & ~b)`。所以我们得到`x ^ y = ~((~(x & ~y) & ~(~x & y)))`。

#### tmin

```c
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return (0x1 << 31);
}
```

> 返回最小的二进制补码。

分析：
实验假定int为32位表示，tMin便是0x80000000。利用左移动运算符可以轻松得到tMin。

#### isTmax

```c
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  int isNegate = !(~x);
  return !((~(x + 1) ^ x) | isNegate);
}
```

> 如果x是最大的二进制补码数返回1，否则返回0。

分析：
tMax的二进制表示是0x7fffffff，观察特征后，tMax加1后取反仍是tMax。我们可以通过`~(x + 1) ^ x`是否等于0来判断一个数是否是tMax，我们发现还有一个数符合这种条件，0xffffffff也就是-1也满足该条件。我们只需判断是-1的情况下便可以用该条件判断是否是tMax。

#### allOddBits

```c
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  int t1 = (0xAA << 8) + 0xAA; // 0x0000AAAA
  int t2 = (t1 << 16) + t1;    // 0xAAAAAAAA
  return !((x & t2) ^ t2);
}
```
> 检查x二进制表示0-31位中是否奇数位全为1。

分析：
我们只需要将0xAAAAAAAA这个奇数位全为1的掩码构出来与x进行先与后异或的比较即可。

#### negate

```c
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return (~x) + 1;
}
```

> 返回-x。

分析：
在二进制补码中，正数与负数的关系是：正数按位取反 + 1 = 负数。

#### isAsciiDigit

```c
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  int flag1 = !!((0x2F + ~x + 1) >> 31);
  int flag2 = !((0x39 + ~x + 1) >> 31);
  return flag1 & flag2;
}
```

> 检查x是否是'0'字符与'9'字符之间的数。

分析：
题目要求是`0x30 <= x <= 0x39`，分别得到`0x30 - x <= 0`和`0x39 - x >= 0`，我们分别思考这两个式子。

`0x30 - x <= 0`，我们利用`0x30 + (-x) = 0x30 + ~x + 1`即可得到减后的结果，但是我们如何判断是否小于等于0呢？我们可以利用符号位来判断是否小于0却无法因此判断是否等于0。因为x是int类型整数，所以我们可以不等式中的小于等于化为小于，来取消等号判断的烦恼。`(0x30 - 1) - x < 0`,这样当`x = 0x30`时，表达式仍是负数，我们便可以使用符号位来判断是否满足条件了。

而`0x39 - x >= 0`，我们只需检查相减后的符号位是否为0即可。

所以我们便判断，x是否同时满足这两个条件即可，由于负数右移，实验要求的右移是**算术右移**(负数高位补1，非负数高位补0)，所以`(0x2F + ~x + 1) >> 31`后如果满足结果则是`0xffffffff`，我们使用`!!`将结果转为逻辑值`1`，使用逻辑值和按位与来检查x是否同时满足2个条件更加方便。

#### conditional

```c
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  x = !!x;
  x = (~x) + 1;
  return (x & y) | (~x & z);
}
```

> 实现`x ? y : z`条件运算符。

分析：
由于我们不能if语句所以return限制了我们只能写一条。我们可以从条件运算符的要求出发，当x为非0时，返回y；当x为0时，返回z。当x为非0时，我们可以返回`(0x11111111 & y) | (0x0000000 & y)`, 当x为0时，我们可以返回`(0x0000000 & y) | (0x11111111 & y)`。我们只需要根据x的值构造一个全1或者全0的掩码即可。我们将x转为逻辑值，再取反加1构造成功。

#### isLessOrEqual

```c
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int xIsNeg = !!(x >> 31); // x是否为负数
  int yIsNeg = !!(y >> 31); // y是否为负数 
  int flag = xIsNeg ^ yIsNeg; // x和y是否异号
  return (flag & xIsNeg & !yIsNeg) | (!flag & !!((x + ~y) >> 31));
}
```

> 如果x <= y，返回1，否则返回0。

分析：
根据之前的`isAsciiDigit`实现思路，我们很快想到`x - 1 - y < 0 = x - 1 + ~y + 1 < 0 = x + ~y < 0`来判断，但是发现有个bug。`x + ~y`会在x、y异号时可能出现溢出从而导致无法判断。而在x和y异号的时候，只有`x < 0, y >= 0`条件满足，并且无需检查x和y的绝对大小。
所以我们得出以下判断策略：

1. 如果`x < 0 , y >= 0`，则返回1。
2. 在1不满足的基础下，如果`x + ~y < 0`，则返回1，否则返回0。

在x等于0的情况，可以在条件2中检测。

#### logicalNeg

```c
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  int minusX = ~x + 1;
  return ((x | minusX) >> 31) + 1;
}
```

> 实现逻辑非，非0为0，0为1。

分析：
也就是要构造出一个特征区分是否为0。一个数或自己的相反数的结果是-1， 这个数非0。

#### howManyBits

```c
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  int b16, b8, b4, b2, b1, b0, sign = x >> 31;
  x = (sign & ~x) | (~sign & x);
  b16 = !!(x >> 16) << 4;
  x = x >> b16;
  b8 = !!(x >> 8) << 3;
  x = x >> b8;
  b4 = !!(x >> 4) << 2;
  x = x >> b4;
  b2 = !!(x >> 2) << 1;
  x = x >> b2; 
  b1 = !!(x >> 1);
  x = x >> b1;
  b0 = x;
  return b16 + b8 + b4 + b2 + b1 + b0 + 1;
}
```

> 求一个数最少需要多少位二进制表示

分析：
正数的最少表示位数，应该是最高的1位加上符号位0。
负数的最少表示位数，应该是最高的0位加上符号位1。
而正数的最少表示位数与负数的最少表示位数是一样的，例如3 = 011，-3 = 101。我们只需要求正数的最高位的1所处的位置再加上符号位即可。用分治法，每次划分一半区间。
先在高16位找，如果有1，则把x右移动16位；
接着高8位找，如果有1，则把x右移动8位；
以此类推...
即可把表示正数的位数算出来，再加上符号位即可。

#### floatScale2

```c
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  int exp = (uf & 0x7f800000) >> 23;
  int sign = uf & (1 << 31);
  if (exp == 0) { 
    return uf << 1 | sign;
  }
  if (exp == 255) {
    return uf;
  }
  exp++;
  if (exp == 255) {
    return 0x7f800000 | sign;
  }
  return (exp << 23) | (uf & 0x807fffff);
}
```

> 参数和返回结果以无符号int类型表示float浮点数，返回2*f。

分析：
首先我们需要知道float32位的表示方法，符号位1位，阶码字段8位，尾数23位。
根据exp的编码分为3种不同的情况：

1. 规范化：当exp不全为0，且不全为1时，也就是(exp != 0 && exp != 255)，阶码字段被解释为以偏置形式表示的有符号整数，阶码`E = e - Bias`，e为有符号数为e<sub>7</sub>e<sub>6</sub>e<sub>5</sub>e<sub>4</sub>e<sub>3</sub>e<sub>2</sub>e<sub>1</sub>e<sub>0</sub>，Bias为2<sup>7</sup>-1，float单精度的Bias为127。小数字段frac，`0 <= f < 1`，尾数定义为`M = f + 1`，使得`1 <= M < 2`。
2. 非规范化：exp全为0，表示的数非常接近于0
3. 特殊值：exp全为1，小数域全为0，得到无穷大或者无穷小；exp全为1，小数域非0，NaN(Not a Number)。

所以我们需要根据exp的情况来判断返回值：

1. 如果exp为0，表示的数非常接近于0。将uf保留符号位左移一位即可得到结果2*uf。
2. 如果exp为255，根据题目要求直接返回uf。
3. 如果exp不为255且不为0，那么此时的uf为标准化的形式。由于浮点数的表示是V = (-1)<sup>s</sup> * M * 2<sup>E</sup>，将`uf * 2`，即可以把`E -> E + 1`，将`exp++`即可。此时如果exp为255，返回无穷值；否则返回原数加上改变的exp。

#### floatFloat2Int

```c
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  int s = uf >> 31;
  int exp = (uf & 0x7f800000) >> 23;
  int Bias = 127;
  int E = exp - Bias;
  int frac = uf & 0x007fffff;
  int M = frac | 0x00800000; // M = frac + 1;
  if (!(uf & 0x7fffffff)) {
    return 0;
  }
  if (E > 31) {
    return 0x80000000u;
  }
  if (E < 0) {
    return 0;
  }
  if (E > 23) {
    M = M << (E - 23);
  } else {
    M = M >> (23 - E);
  }
  if (!(M >> 31) ^ s) {
    return M;
  } else if (M >> 31) {
    return 0x80000000u;
  } else {
    return ~M + 1;
  }
}
```

> 将float转为int。

分析：
将浮点数中的符号、exp、frac、尾数、E计算出来，尾数`M = 1 + frac`，故如果E阶码大于31会溢出，小于0得到0。如果E大于23，将尾数M左移；如果E小于23，将尾数M右移动。最后检查M的符号来判断是否溢出。


#### floatPower2

```c
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
    int INF = 0xff << 23;
    int exp = x + 127;
    if (exp >= 255) {
      return INF;
    } else if (exp <= 0) {
      return 0;
    } else {
      return exp << 23;
    }
}
```

> 计算2.0的x次方。

分析：
根据V = (-1)<sup>s</sup> * M * 2<sup>E</sup>次方，我们已有参数x即E,根据`E = exp - Bias`可以得到exp，如果exp>=255或exp<=0皆为非规范化情况。