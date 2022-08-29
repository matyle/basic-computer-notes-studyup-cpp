int var_prod_ele(int n, int A[n][n], int B[n][n], int i, int k)
{
	int j = 0;
	int result = 0;
	// int *Arrow = &A[i][0];
	// int *Bptr = &B[0][k];

	for (j = 0; j < n; j++)
	{
		// 1
		//result += *(Arrow + j) * *(Bptr += 4*n);
		// 2
		// result += Arrow[j] * (*Bptr);
		// Bptr += 4 * n;
		result += A[i][j] * B[j][k];
	}

	return result;
}