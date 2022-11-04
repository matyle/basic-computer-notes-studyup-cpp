// CSAPP 练习题4.3

int rSum(int *Start, int Count)
{
	if (Count <= 0)
		return 0;
	return *Start + rSum(Start + 1, Count - 1);
}