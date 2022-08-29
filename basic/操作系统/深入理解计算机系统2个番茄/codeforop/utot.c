# include  <stdio.h>
int main()
{
    /*
  有符号短整型：有效位数为16位 ，2个字节，范围【-32768，32767】
*/
//   short  int  st1=- 32768, //溢出
//           st2=- 32769 ,//溢出
//           st3=- 32768 ,
//           st4=- 32767,
//           st5=- 32766 ;
//     //unsigned short int st4= -32767
//     //分别以有符号整型%d和无符号整型%u输出
//     printf( "st1=%d=%u\n" ,st1,st1);
//     printf( "st2=%d=%u\n" ,st2,st2);
//     printf( "st3=%d=%u\n" ,st3,st3);
//     printf( "st4=%d=%u\n" ,st4,st4);
//     printf( "st5=%d=%u\n\n" ,st5,st5);

//unsigned int a = 0xfffffff7;
unsigned int a = 0xffffff07; // 高位为0 输出
unsigned char i = (unsigned char)a;
char *b = (char *)&a;
printf("%08x,%08x\n", i, *b);// 高位为0 截断为有符号输出00000007
return 0;

}