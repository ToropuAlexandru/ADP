
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

__global__
void RoyFloyd(int* mat, int k, int N) {

	int i = blockIdx.x;
	int j = threadIdx.x;
	mat[i*N + j] = (mat[i*N + k] + mat[k*N + j]) < mat[i*N + j] ?
					(mat[i*N + k] + mat[k*N + j]) : 
					mat[i*N + j];

	
}


int main(int argc, char **argv)
{
	
	int thread_per_block = 512, i, j;
	size_t N = 700;
	int* cuda_mat;
	int size = sizeof(int)* N * N;
	time_t  time_start, time_end;
	

	srand(time(NULL));
	int *mat = (int*)malloc(sizeof(int)*N*N);
	for (int i = 0; i < N*N; i++)
		mat[i] = rand() % 500;
	for (int i = 0; i < N; i++)
		mat[i*N + i] = 0;

	time(&time_start);
	int *result = (int*)malloc(sizeof(int)*N*N);
	memcpy(result, mat, sizeof(int)*N*N);

	cudaMalloc((void**)&cuda_mat, size);
	cudaMemcpy(cuda_mat, mat, size, cudaMemcpyHostToDevice);
	int num_block = ceil((float)(N*N / (thread_per_block)));
	for (int k = 0; k < N; ++k) {
		RoyFloyd << <num_block, (thread_per_block) >> >(cuda_mat, k, N);
	}
	cudaMemcpy(mat, cuda_mat, size, cudaMemcpyDeviceToHost);
	cudaFree(cuda_mat);
	time(&time_end);

	//printf("%lld\n", (long long)(time_end - time_start));
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
			printf("dist[%d][%d] = %d \n", i, j, result[i*N + j]);
	}

}