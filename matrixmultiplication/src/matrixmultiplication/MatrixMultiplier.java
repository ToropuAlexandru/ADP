package matrixmultiplication;

class MatrixMultiplier implements Runnable 
{

	private final Multiply mul;

	public MatrixMultiplier(Multiply mul)
	{
		this.mul=mul;
	}

	@Override
	public void run() 
	{
		mul.multiplyMatrix();
	}
}
