#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#define MATR(i, j) matrix[mSize * (i) + (j)]
#define P_MATR(i, j) procMatrix[mSize * (i) + (j)]
#define MY_RND() rand() % 20
#define TEST 1

FILE *f_matrix, *f_time, *f_res;

int size;
int rank;

int* procMatrix;
int mSize; 

int* pProcInd; 
int* pProcNum;

typedef struct { int parent; int child; } Edge;

int* MST; 
int weight; 


void ProcessInitialization()
{
	int i, j;

 
	pProcInd = (int*)malloc(sizeof(int)* size);
	pProcNum = (int*)malloc(sizeof(int)* size);

	pProcInd[0] = 0;
	pProcNum[0] = mSize / size;
	int remains = size - (mSize % size); 
	for (i = 1; i < remains; ++i)
	{
		pProcNum[i] = pProcNum[0];
		pProcInd[i] = pProcInd[i - 1] + pProcNum[i - 1];
	}
	for (i = remains; i < size; ++i)
	{
		pProcNum[i] = pProcNum[0] + 1;
		pProcInd[i] = pProcInd[i - 1] + pProcNum[i - 1];
	}

	int* matrix;
	matrix = (int*)malloc(mSize*mSize*sizeof(int));
	//matrix = generateMatrix();
	if (!rank)
	{
		for (i = 0; i < mSize; ++i)
		{
			MATR(i, i) = 0;
			for (j = i + 1; j < mSize; ++j)
			{
#ifdef TEST	
				int buf;
				fscanf(f_matrix, "%d\n", &buf);
				MATR(i, j) = MATR(j, i) = buf;
#else
				MATR(i, j) = MATR(j, i) = MY_RND();
#endif
			}
		}
#ifdef TEST
		fclose(f_matrix);
#endif 
	}	

	procMatrix = (int*)malloc(pProcNum[rank] * mSize*sizeof(int));

	MPI_Datatype matrixString;
	MPI_Type_contiguous(mSize, MPI_INT, &matrixString);
	MPI_Type_commit(&matrixString);
	MPI_Scatterv(matrix, pProcNum, pProcInd, matrixString, procMatrix, pProcNum[rank], matrixString, 0, MPI_COMM_WORLD);
	if (!rank)
	{
#ifdef TEST
		for (i = 0; i < mSize; ++i)
		{
			for (int j = 0; j < mSize; ++j)
			{
				printf("%d ", P_MATR(i, j));
			}
			printf("\n");
		}
#endif
		free(matrix);
	}

	MST = (int*)malloc(sizeof(int)*mSize); 
	for (int i = 0; i < mSize; ++i)
	{
		MST[i] = -1;
	}
}


void PrimsAlgorithm()
{
	MST[0] = 0;
	weight = 0;

	int mini;
	int parent = 0;
	int child = 0;

	struct { int miniValue; int rank; } miniRow, row;
	Edge edge;
	for (int k = 0; k < mSize - 1; ++k)
	{
		mini = INT_MAX;
		for (int i = 0; i < pProcNum[rank]; ++i)
		{
			if (MST[i + pProcInd[rank]] != -1)
			{
				for (int j = 0; j < mSize; ++j)
				{
					if (MST[j] == -1)
					{

						if (P_MATR(i, j) < mini && P_MATR(i, j) != 0)
						{
							mini = P_MATR(i, j);
							child = j;
							parent = i;
						}
					}
				}
			}
		}
		row.miniValue = mini;
		row.rank = rank;
		MPI_Allreduce(&row, &miniRow, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);
		edge.parent = parent + pProcInd[rank];
		edge.child = child;
		MPI_Bcast(&edge, 1, MPI_2INT, miniRow.rank, MPI_COMM_WORLD);

		MST[edge.child] = edge.parent;
		weight += miniRow.miniValue;
	}
}

void ProcessTerminiation()
{
	free(procMatrix);
	free(MST);
}



int** generateMatrix() {

	/* initialize random seed: */
	srand(time(NULL));

	int l = 20000;
	int c = 20000;

	int ** m = (int**)malloc(sizeof(int*)* l);
	for (int i = 0; i < l; i++) {
		m[i] = (int*)malloc(sizeof(int)* c);
		for (int j = 0; j < c; j++) {
			if (i == j) {
				m[i][j] = 0;
			}
			else {
				if (rand() % 10 == 0) {
					m[i][j] = rand() % 100;
					m[j][i] = m[i][j];
				}
				else {
					m[i][j] = INT_MAX;
					m[j][i] = INT_MAX;
				}
			}
		}
	}
	return m;
}



int main(int argc, char *argv[])
{
	double start, finish, duration;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	srand(time(NULL) + rank);

	if (!rank)
	{
#ifdef TEST
		f_matrix = fopen("example.txt", "r");
		fscanf(f_matrix, "%d\n", &mSize);
#else
		if (argc < 2)
		{
			printf("No size parameter\n");
			MPI_Finalize();
			return -1;
		}
		else
		{
			mSize = atoi(argv[1]);
		}
#endif
	}

	MPI_Bcast(&mSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

	ProcessInitialization();

	start = MPI_Wtime();

	PrimsAlgorithm();

	finish = MPI_Wtime();
	duration = finish - start;

	if (!rank)
	{
#ifdef TEST
		f_res = fopen("Result.txt", "w");
		fprintf(f_res, "%d\n ", weight);
		for (int i = 0; i< mSize; ++i)
		{
			fprintf(f_res, "%d %d\n", i, MST[i]);
		}
		fclose(f_res);
#endif

		f_time = fopen("Time2.txt", "a+");
		fprintf(f_time, " Number of processors: %d\n Number of vertices: %d\n Time of execution: %f\n\n", size, mSize, duration);
		fclose(f_time);
		printf("\n Number of processors: %d\n Time of execution: %f\n\n", size, duration);
	}

	ProcessTerminiation();
	MPI_Finalize();

	system("pause");
	return 0;
}