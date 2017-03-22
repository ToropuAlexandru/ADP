package matrixmultiplication;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.TimeUnit;

public class MatrixMultiplication 
{
	private static final int numberOfRowsA = 1000;
	private static final int numberOfColumnsA = 1000;	
	private static final int numberOfColumnsB = 1000;
	private static final int numberOfThreads = 8;

	static short[][] a = new short[numberOfRowsA][numberOfColumnsA];
	static short[][] b = new short[numberOfColumnsA][numberOfColumnsB];
	static short[][] c = new short[numberOfRowsA][numberOfColumnsB];

	public static void main(String [] args)
	{
		Random rand = new Random();

		for (int i = 0; i < a.length; i++) 
		{
			for (int j = 0; j < a[i].length; j++) 
			{
				a[i][j]=(short) rand.nextInt(10);
			}
		}

		for (int i = 0; i < b.length; i++) 
		{
			for (int j = 0; j < b[i].length; j++) 
			{
				b[i][j]=(short)rand.nextInt(10);
			} 
		}
		
		try
		{
			double startTime = System.currentTimeMillis();
			Multiply multiply = new Multiply(numberOfRowsA,numberOfColumnsA,numberOfColumnsB);
			List<Thread> threads = new ArrayList<>(numberOfThreads);

			for (int th = 0; th < numberOfThreads; th++) 
			{
				Thread t = new Thread(new MatrixMultiplier(multiply));
				t.start();
				threads.add(t);
			}

			for (Thread t : threads) 
			{
				t.join();
			}
			System.out.println(((System.currentTimeMillis() - startTime) / 1000) + " seconds for matrix");

		}
		catch (Exception e) 
		{
			e.printStackTrace();
		}

		/*System.out.println("\n\nResult of matrix:");
		for (int i = 0; i < c.length; i++) 
		{
			for (int j = 0; j < c[i].length; j++) 
			{
				System.out.print(c[i][j]+" ");
			}
			System.out.println();
		}*/
	}
}