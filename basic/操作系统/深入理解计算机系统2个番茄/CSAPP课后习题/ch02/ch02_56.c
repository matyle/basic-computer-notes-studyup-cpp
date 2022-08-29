// csapp第2章课后习题2.56

#include <stdio.h>
typedef unsigned char *byte_pointer;

// 函数声明
void show_bytes (byte_pointer start, int len);

int main(void)
{
	int v1 = 0x87ef331;
	byte_pointer valp = (byte_pointer) &v1;
	printf("%.2x\n", v1);
	show_bytes(valp, sizeof(int));

	return 0;
}

void show_bytes(byte_pointer start, int len) {
    int i;
    for (i = 0; i < len; i++)
        printf(" %.2x", start[i]);    //line:data:show_bytes_printf
    printf("\n");
}
