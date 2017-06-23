#include <iostream>
#include <iomanip>
#include <random>
#include <string>


const int COL_NUM = 2;


extern "C" double ddot_(int*, double*, int*, double*, int*);
void DDOT();
extern "C" void dgemm_(char*, char*, int*, int*, int*, double*, 
			double*, int*, double*, int*, double*, double*, int*);
void DGEMM();
void printSqMatrix(double[][COL_NUM], std::string, int);
void printVector(double*, std::string, int);


int main (int argc, char *argv[])
{	
	DDOT();
	DGEMM();

	return 0;
}


void DDOT()
{
	// Random non-deterministic seed
	std::random_device ndGen;
	std::uniform_real_distribution<double> ndDist(0, 200);

	// Variables
	int vectorLen = 100;
	int n = vectorLen;
	int incx = 1;
	int incy = 1;
	double C = 4.00; //test
	double A[vectorLen], B[vectorLen];
	
	// Populate
	for (int i = 0; i < vectorLen; i++)
	{
		A[i] = ndDist(ndGen);
		B[i] = ndDist(ndGen);
	}

	// Before
	printVector(A, "Vector A", vectorLen);
	printVector(B, "Vector B", vectorLen);
	 
	C = ddot_(&n, (double*)A, &incx, (double*)B, &incy);

	// After	
	std::cout << std::setprecision(4) << "<A|B> = " << C << std::endl;
}


void DGEMM()
{
	// Random non-deterministic seed
       	std::random_device ndGen;
       	std::uniform_real_distribution<double> ndDist(0, 200);
	
	// Variables
	int row_num = 2;
	int m = row_num;
	int n = row_num;
	int k = row_num;
	double a = 1, b = 0;
	double A[row_num][COL_NUM];
	double B[row_num][COL_NUM];
	double C[row_num][COL_NUM];
	int lda = m, ldb = n, ldc = m;
	char transa = 't', transb = 't'; //Switch to row, not column (fortran default)

	// Populate
	for (int i = 0; i < row_num; i++)
	{
		for (int j = 0; j < COL_NUM; j++)
		{
			A[i][j] = ndDist(ndGen);
			B[i][j] = ndDist(ndGen);
		}
	}

	// Before
	printSqMatrix(A, "Matrix A", row_num);
	printSqMatrix(B, "Matrix B", row_num);

	dgemm_(&transa, &transb, &m, &n, &k, &a, (double*)A, &lda, (double*)B, &ldb, &b, (double*)C, &ldc);

	// After
	printSqMatrix(C, "A x B = C", row_num);

}


void printSqMatrix(double M[][COL_NUM], std::string mName, int dim)
{
	std::cout << mName << ":" << std::endl;
	for (int i = 0; i < dim; i++)
	{
		std::cout << "| ";
		for (int j = 0; j < COL_NUM; j++)
		{
			std::cout << std::setprecision(4) << M[i][j] << " ";
		}
		std::cout << "|" << std::endl;
	}
}


void printVector(double* V, std::string vName, int length)
{
	std::cout << vName << ": < ";
	for (int i = 0; i < length; i++)
	{
		std::cout << V[i] << " ";
	}
	std::cout << " >" << std::endl;
}
