// csapp 习题2.59

#include <stdio.h>
typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, int len) {
    int i;
    for (i = 0; i < len; i++)
        printf(" %.2x", start[i]);    //line:data:show_bytes_printf
    printf("\n");
}

int main(void)
{
	int x = 0x89abcdef, y = 0x765432ef;
	int z;
	z = (x & 0xff) | (y & ~0xff);
	byte_pointer s = (byte_pointer) &z;		// 别忘了 z 之前的取址符
	show_bytes(s, sizeof (int));

	return 0;
}


