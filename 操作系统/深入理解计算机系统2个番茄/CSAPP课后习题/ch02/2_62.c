// csapp 第二章习题2.62
#include <stdio.h>
int int_shifts_are_logical(void);

int main(void)
{
	if (int_shifts_are_logical())
		printf("int为逻辑右移\n");
	else 
		printf("int为算术右移\n");
	
	return 0;
}

// 检测机器是逻辑右移还是算术右移
int int_shifts_are_logical(void)
{
	int w = sizeof(int) << 3;		// 检测当前机器 int 占多少位
	printf("int 占%d位\n", w);
	int x = 0;

	if (~(~x >> (w - 1)) == 0)		// 如果算术右移，则 ~x 移位后全为 1
	{
		//printf("%.8x\n", ~x >> (w - 1));
		return 0;	
	}
		
	else 
		return 1;
}