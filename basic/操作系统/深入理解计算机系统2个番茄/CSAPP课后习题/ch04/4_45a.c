// 家庭作业4.45
// 冒泡排序的数组实现
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LIMIT 100

void bubble_a(int *data, int count) {
	int i, next;
	for (next = 1; next < count; next++) {
		for (i = next-1; i >= 0; i--)
			if (data[i+1] < data[i]) {
				int t = data[i+1];
				data[i+1] = data[i];
				data[i] = t;
			}
	}
}

int main(void)
{
	int array[LIMIT];
	srand(time(0));
	// 使用随机函数初始化数组
	for (int i = 0; i < LIMIT; i++) {
		array[i] = rand() % 100 + 1;
		printf("%4d ", array[i]);
		if ((i+1) % 15 == 0)
			printf("\n");	
	}
	printf("\n-----------------排序后---------------------\n");

	bubble_a(array, LIMIT);		 	//排序算法
	//  显示排序后的数组
	for (int i = 0; i < LIMIT; i++) {
		printf("%4d ", array[i]);
		if ((i+1) % 15 == 0)
			printf("\n");
	}

	return 0;
}