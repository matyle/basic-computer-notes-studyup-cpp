void transpose(int A[M][M])
{
	int i, j;
	for (i = 0; i < M; i++)
		for (j = 0; j < i; j++) {
			int t = A[i][j];
			A[i][j] = A[j][i];
			A[j][i] = t;
		}
}