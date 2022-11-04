int fun_c(unsigned x) {
	int val = 0;
	int i;
	for (i = 0; i < 32; i++)
	{
		val = (val << 1) | (x &0x1);
		x >>= 1;
	}
	return val;
}