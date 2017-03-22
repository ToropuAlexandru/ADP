package matrixmultiplication;

class Multiply extends MatrixMultiplication 
{

	private int numberOfRowsA;
	private int numberOfColumnsB;
	private int numberOfColumnsA;

	public Multiply(int numberOfRowsA, int numberOfColumnsB, int numberOfColumnsA)
	{
		this.numberOfRowsA = numberOfRowsA;
		this.numberOfColumnsB = numberOfColumnsB;
		this.numberOfColumnsA = numberOfColumnsA;
	}

	public synchronized void multiplyMatrix()
	{

		for (int i = 0; i<numberOfRowsA; i++)
		{
			for (int j = 0; j<numberOfColumnsB; j++)
			{
				for (int k = 0; k<numberOfColumnsA; k++)
				{
					c[i][j] += a[i][k] * b[k][j];
				}
			}
		}
	}
}