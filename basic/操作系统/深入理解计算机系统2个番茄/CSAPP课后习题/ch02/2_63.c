// csapp 第二章 2.63
// 2016/04/07
// 2016/04/10

#include <stdio.h>

// 用逻辑右移（由值 xsrl 给出）来完成算术右移
int sra(int x, int k)
{
	int xsrl = (unsigned)x >> k;		//
	int n = 8 * sizeof(int);		// 检测 int 的位数

	// segmentfault 网友的代码
	int mask = 1 << (n - 1 - k);
	return (xsrl ^ mask) - mask;
	// 下面是我的代码:
	// operator '<<' has lower precedence than '-'
	//return (((1 << (n - 1)) - (xsrl &  1 << (n - k - 1))) << 1) | xsrl;
}

// 用算术右移（由值 xsra 给出）来完成逻辑右移
unsigned srl (unsigned x, int k)
{
	unsigned xsra = (int)x >> k;
	unsigned n = 8 * sizeof(int);		// 检测 int 的位数
	return xsra & ~(~0x0 << (n - k));	// 将所有移过的高位直接和 0 取与
}


int main(void)
{
	int x = 0xffff000f;
	int z = 0x0fff683e;
	int k = 8;
	unsigned y = 0xffff000f;
	unsigned y1 = srl (y, k);
	int x1 = sra(x, k);
	int z1 = sra(z, k);
	 
	printf("%.8x ---- %.8x\n", x, x1);
	printf("%.8x ---- %.8x\n", y, y1);
	printf("%.8x ---- %.8x\n", z, z1);


	return 0;
}

