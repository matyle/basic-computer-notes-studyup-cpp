// CSAPP 练习4.45
// 使用随机算法生成一个乱序的数组

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LIMIT 100
void bubble_pp(int *data, int count);

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

	bubble_pp(array, LIMIT);		 	//排序算法
	//  显示排序后的数组
	for (int i = 0; i < LIMIT; i++) {
		printf("%4d ", array[i]);
		if ((i+1) % 15 == 0)
			printf("\n");
	}

	return 0;
}

void bubble_pp(int *data, int count) {
	int i, next;
	for (next = 1; next < count; next++) {
		for (i = next-1; i >= 0; i--)
			if (*(data+i+1) < *(data+i)) {
				int t = *(data+i+1);
				*(data+i+1) = *(data+i);
				*(data+i) = t;
			}
	}
}

// 随机函数使用示例
// /**********************************************************************
// /* pe12-7.c  */
// #include <stdio.h>
// #include <stdlib.h>  /* for srand() */
// #include <time.h>    /* for time()  */
// int rollem(int);

// int main(void)
// {
//     int dice, count, roll;
//     int sides;
//     int set, sets;
    
//     srand((unsigned int) time(0));  /* randomize rand() */
    
//     printf("Enter the number of sets; enter q to stop.\n");
//     while ( scanf("%d", &sets) == 1)
//     {
//           printf("How many sides and how many dice?\n");
//         scanf("%d %d", &sides, &dice);
//         printf("Here are %d sets of %d %d-sided throws.\n", sets, dice,
//                 sides);
//         for (set = 0; set < sets; set++)
//         {
//             for (roll = 0, count = 0; count < dice; count++)
//                 roll += rollem(sides);
//                 /* running total of dice pips */
//             printf("%4d ", roll);
//             if (set % 15 == 14)
//                 putchar('\n');
//         }
//         if (set % 15 != 0)
//             putchar('\n');
//         printf("How many sets? Enter q to stop.\n");
//     }
//     printf("GOOD FORTUNE TO YOU!\n");
//     return 0;
// }

// int rollem(int sides)
// {
//     int roll;

//     roll = rand() % sides + 1;
//     return roll;
// }
// ******************************************************************************/
