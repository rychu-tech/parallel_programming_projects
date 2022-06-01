#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define NUM_THREADS 1
#define MATRIX_SIZE 3
double EPSILON = 0.01;


void generateMagicSquare(int magicMatrix[MATRIX_SIZE][MATRIX_SIZE])
{
	int i, row, column;
	for( i = 1; i <= MATRIX_SIZE*MATRIX_SIZE; i++)
    {
        row = (MATRIX_SIZE - i % MATRIX_SIZE + 1 + 2 * ((i - 1) / MATRIX_SIZE)) % MATRIX_SIZE;
        column =((MATRIX_SIZE - 1) / 2 + i - 1 - (i - 1) / MATRIX_SIZE) % MATRIX_SIZE;   
        magicMatrix[row][column] = i;
    }

}

void printIntMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) 
{
	int h, w;
    for (h = 0; h < MATRIX_SIZE; h++)
    {
        for (w = 0; w < MATRIX_SIZE; w++)
        {
            printf("%d ", matrix[h][w]);
        }
        printf("\n");
    }
    printf("\n");
}

void printdoubleMatrix(double matrix[MATRIX_SIZE][MATRIX_SIZE]) 
{
	int h, w;
    for (h = 0; h < MATRIX_SIZE; h++)
    {
        for (w = 0; w < MATRIX_SIZE; w++)
        {
            printf("%.15lf ", matrix[h][w]);
        }
        printf("\n");
    }
    printf("\n");
}

void transformMatrix(double multipliedMatrix[MATRIX_SIZE][MATRIX_SIZE], int magicMatrix[MATRIX_SIZE][MATRIX_SIZE], int magicNumber)
{
	int row, column;
    for (row = 0; row < MATRIX_SIZE; row++)
    {
        for (column = 0; column < MATRIX_SIZE; column++)
        {
			multipliedMatrix[row][column] = (double)magicMatrix[row][column] / magicNumber;
        }
        
    }
	
	
}

void generateMultipliedMatrix(double multipliedMatrix[MATRIX_SIZE][MATRIX_SIZE])
{
	int row, column;
    for (row = 0; row < MATRIX_SIZE; row++)
    {
        for (column = 0; column < MATRIX_SIZE; column++)
        {
			multipliedMatrix[row][column] = 0;
        }
        
    }
	
	
}

void multiplyMatrices(double matrix1[MATRIX_SIZE][MATRIX_SIZE], double matrix2[MATRIX_SIZE][MATRIX_SIZE], double retMatrix[MATRIX_SIZE][MATRIX_SIZE])
{
	int row, column, iterator;
    omp_set_num_threads(NUM_THREADS);
	
    for (row = 0; row < MATRIX_SIZE; row++)
    {
        for (column = 0; column < MATRIX_SIZE; column++)
        {
			retMatrix[row][column] = 0;
        }
        
    }
    #pragma omp parallel
	{
		int r, c, i;
		#pragma omp for
		for (r = 0; r < MATRIX_SIZE; r++)
	    {
	        for (c = 0; c < MATRIX_SIZE; c++)
	        {
				for (i = 0; i < MATRIX_SIZE; i++)
				{
					retMatrix[r][c] += matrix1[r][i] * matrix2[i][c];
					
				}
					
	        }
	    }
	}
	    
}

int main() 
{
	int magicMatrix[MATRIX_SIZE][MATRIX_SIZE];
	generateMagicSquare(magicMatrix);
	
	printIntMatrix(magicMatrix);
	
	double multipliedMatrix[MATRIX_SIZE][MATRIX_SIZE];
	generateMultipliedMatrix(multipliedMatrix);
	
	
	int numOfElements = MATRIX_SIZE * MATRIX_SIZE;
	
	int magicNumber = MATRIX_SIZE * (MATRIX_SIZE * MATRIX_SIZE + 1) / 2;
	printf("Magic sum : %d\n", magicNumber);
	 
	transformMatrix(multipliedMatrix, magicMatrix, magicNumber);
	
	clock_t start_t, end_t;
   	double total_t;
	
	int checker = 0, iter = 1;
	
	start_t = clock();
	
	double retMatrix[MATRIX_SIZE][MATRIX_SIZE];
	while (checker == 0)
	{
		int counter = 0;
		multiplyMatrices(multipliedMatrix, multipliedMatrix, retMatrix);
		
		
		int row, column;
    
    
	    for (row = 0; row < MATRIX_SIZE; row++)
	    {
	        for (column = 0; column < MATRIX_SIZE; column++)
	        {
				if (fabs(multipliedMatrix[row][column] - retMatrix[row][column]) < EPSILON)
				{
					counter++;
					
				}
				multipliedMatrix[row][column] = retMatrix[row][column];
	        }
    	}
		
		if (counter == numOfElements)
		{
			checker = 1;
			end_t = clock();
		}
		//printf("%d\n", counter);
		iter++;
		printdoubleMatrix(multipliedMatrix);
	}
	
	total_t = (double)(fabs(end_t - start_t)) / CLOCKS_PER_SEC;
	
	printf("ITER %d : Total time taken by CPU: %.30lf\n", iter, total_t  );
    return 0;
}
