// csapp第2章课后习题2.57

#include <stdio.h>
typedef unsigned char *byte_pointer;

//
void show_bytes(byte_pointer start, int len) {
    int i;
    for (i = 0; i < len; i++)
        printf(" %.2x", start[i]);    //line:data:show_bytes_printf
    printf("\n");
}

// 
void show_short (short x)
{
	show_bytes((byte_pointer) &x, sizeof(short));
}

void show_long (long x)
{
	show_bytes ( (byte_pointer)&x, sizeof(long));
}

void show_double (double x)
{
	show_bytes( (byte_pointer)&x, sizeof(double));
}

int main(void)
{
	short v1 = 0x8fd;
	long v2 = 0x17347cdf;
	double v3 = 12.125;
	double v4 = 0x374837;
 	//byte_pointer valp = (byte_pointer) &v1;
	printf("%d   %x\n", v1, v1);
	show_short (v1);
	printf("%ld   %lx\n", v2, v2);
	show_long (v2);
	printf("%f \n", v3);
	show_double (v3);
	printf("%f \n", v4);
	show_double (v4);

	return 0;
}