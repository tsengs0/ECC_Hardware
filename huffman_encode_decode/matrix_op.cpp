#include "matrix_op.h"
// Function to get cofactor of A[p][q] in temp[][]. n is current 
// dimension of A[][] 
void getCofactor(float **A, float **temp, int p, int q, int n)
{
	int i = 0, j = 0; 
	// Looping for each element of the matrix 
	for (int row = 0; row < n; row++) 
	{ 
		for (int col = 0; col < n; col++) 
		{ 
			// Copying into temporary matrix only those element 
			// which are not in given row and column 
			if (row != p && col != q) 
			{ 
				temp[i][j++] = A[row][col]; 

				// Row is filled, so increase row index and 
				// reset col index 
				if (j == n - 1) 
				{ 
					j = 0; 
					i++; 
				} 
			} 
		} 
	} 
} 

/* Recursive function for finding determinant of matrix. 
n is current dimension of A[][]. */
float determinant(float **A, int n, unsigned int space) 
{ 
	unsigned int N = space; 
	float **temp; // To store cofactors 
	float sign = 1.0; // To store sign multiplier 
	float D = 0; // Initialize result 

	temp = new float* [N];
	for(unsigned int i=0; i<N; i++) temp[i] = new float[N];

	// Base case : if matrix contains single element 
	if (n == 1) return A[0][0]; 
	// Iterate for each element of first row 
	for (int f = 0; f < n; f++) 
	{ 
		// Getting Cofactor of A[0][f] 
		getCofactor(A, temp, 0, f, n); 
		D += sign * A[0][f] * determinant(temp, n - 1, N); 

		// terms are to be added with alternate sign 
		sign = -sign; 
	} 
	return D; 
} 

// Function to get adjoint of A[N][N] in adj[N][N]. 
void adjoint(float **A, float **adj, unsigned int space)
{ 
	unsigned int N = space; 
	if (N == 1) 
	{ 
		adj[0][0] = 1; 
		return; 
	} 

	// temp is used to store cofactors of A[][] 
	float sign = 1.0;
	float **temp; 
	temp = new float* [N];
	for(unsigned int i=0; i<N; i++) temp[i] = new float[N];

	for (int i=0; i<(int)N; i++) 
	{ 
		for (int j=0; j<(int)N; j++) 
		{ 
			// Get cofactor of A[i][j] 
			getCofactor(A, temp, i, j, N); 

			// sign of adj[j][i] positive if sum of row 
			// and column indexes is even. 
			sign = ((i+j)%2==0)? 1: -1; 

			// Interchanging rows and columns to get the 
			// transpose of the cofactor matrix 
			adj[j][i] = (sign)*(determinant(temp, N-1, N)); 
		} 
	} 
} 

// Function to calculate and store inverse, returns false if 
// matrix is singular 
bool inverse(float **A, float **inverse, unsigned int space) 
{ 
	unsigned int N = space; 
	// Find determinant of A[][] 
	float det = determinant(A, N, N); 
	if (det == 0) 
	{ 
		cout << "Singular matrix, can't find its inverse"; 
		return false; 
	} 

	// Find adjoint 
	float **adj; 
	adj = new float* [N];
	for(unsigned int i=0; i<N; i++) adj[i] = new float[N];
	adjoint(A, adj, N); 

	// Find Inverse using formula "inverse(A) = adj(A)/det(A)" 
	for (int i=0; i<(int)N; i++) 
		for (int j=0; j<(int)N; j++) 
			inverse[i][j] = adj[i][j]/float(det); 

	return true; 
} 

// Generic function to display the matrix. We use it to display 
// both adjoin and inverse. adjoin is integer matrix and inverse 
// is a float. 
template<class T> 
void display(T **A, unsigned int space)
{ 
	unsigned int N = space; 
	for (int i=0; i<(int)N; i++) 
	{ 
		for (int j=0; j<(int)N; j++) 
			cout << A[i][j] << " "; 
		cout << endl; 
	} 
} 
