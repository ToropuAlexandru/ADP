#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define numberOfRowsA 200                 
#define numberOfColumnsA 200                
#define numberOfColumnsB 200             

int main(int argc, char *argv[])
{
	int	tid, nthreads = 2, i, j, k, chunk;
	int	a[numberOfRowsA][numberOfColumnsA],
		b[numberOfColumnsA][numberOfColumnsB],
		c[numberOfRowsA][numberOfColumnsB];

	chunk = 100;

#pragma omp parallel shared(a,b,c,nthreads,chunk) private(tid,i,j,k)
	{
		tid = omp_get_thread_num();
		if (tid == 0)
		{
			nthreads = omp_get_num_threads();
		}

#pragma omp for schedule (static, chunk) 
		for (i = 0; i<numberOfRowsA; i++)
		for (j = 0; j<numberOfColumnsA; j++)
			a[i][j] = 1;
#pragma omp for schedule (static, chunk)
		for (i = 0; i<numberOfColumnsA; i++)
		for (j = 0; j<numberOfColumnsB; j++)
			b[i][j] = 1;
#pragma omp for schedule (static, chunk)
		for (i = 0; i<numberOfRowsA; i++)
		for (j = 0; j<numberOfColumnsB; j++)
			c[i][j] = 0;

#pragma omp for schedule (static, chunk)
		for (i = 0; i<numberOfRowsA; i++)
		{
			printf("Thread %d multiplied row %d\n", omp_get_thread_num(), i);

			for (j = 0; j<numberOfColumnsB; j++)
			for (k = 0; k<numberOfColumnsA; k++)
				c[i][j] += a[i][k] * b[k][j];
		}
	}

	system("pause");
}