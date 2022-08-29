// CSAPP 2.65
// 2016/4/24
#include <stdio.h>
void printBinary(int num);
int even_ones (unsigned x);

int main(void)
{
	unsigned x = 0;

	printf("Enter a number in 16;'q' to quit.\n");
	while (scanf("%x", &x) == 1)
	{
		printBinary(x);
		if (even_ones (x))
			printf ("%x contains an even number of 1s.\n", x);
		else
			printf ("%x doesn't contain an even number of 1s.\n", x);

		printf("Enter a number in 16\n");
	}

	return 0;
}

/* Return 1 when x contains an even number of 1s; 0 otherwise */
int even_ones (unsigned x)
{
	unsigned x1 = x ^ (x >> 16);
	unsigned x2 = x1 ^ (x1 >> 8);
	unsigned x3 = x2 ^ (x2 >> 4);
	unsigned x4 = x3 ^ (x3 >> 2);
	unsigned x5 = x4 ^ (x4 >> 1);
	unsigned x6 = x5 & 0x1;

	return !x6;
}

// 粘贴自 http://www.cnblogs.com/cxbblog/p/3704825.html
// 右移31位，从最高为开始和1做&运算，得到每一位的二进制数值
void printBinary(int num)
{
    int count = (sizeof(num)<<3)-1;//值为31
    while (count>=0) {
        int bitnum = num>>count; //除去符号位，从最高位开始得到每一位
        int byte = bitnum & 1; //和1进行与运算得到每一位的二进制数
        printf("%d",byte);
        
        if (count%4 ==0) {//每隔四位打印空格
            printf(" ");
        }
        
        count--;
    }
    printf("\n");
}
