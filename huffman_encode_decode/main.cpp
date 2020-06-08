#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "huffman.h"
#include "markov_chain_gen.h"
#include "matrix_op.h"

const char *P_ij_filename = "p_i_j.csv";
const char *px1_filename = "px1.csv";

void matrix_file_read();
void markov_chain(int *gen_source, float *s, unsigned int space, unsigned int n);
void state_distribution(float **x, unsigned int space, float **px1, float **P, unsigned int n);
void markov_chain_gen();
void stationary_distribution(float *z, float **P, unsigned int space);
float entropy_rate(float *z, float **P, unsigned int space);
void matrix_mul(float **product, float **matrix_0, float **matrix_1, unsigned int row_num, unsigned int *col_num);
void assign_symbol(char *symbol_inout, unsigned int space);
void conditional_huffman_encode(string *codeword, int *msgIn, Huffman_Code *codebookIn, unsigned int n);
void conditional_huffman_decode(int *msgOut, string *codewordIn, Huffman_Code *codebookIn, unsigned int n);
// User-defined parameters
unsigned int M, N;
unsigned int rowN[2];
unsigned int colN[2];
// For Huffman code
Huffman_Code *cx;
char *symbol; 
int *markov_source;
float **px1, **P, **x;
float Hx; // entropy rate
float *z; // stationary distribution

int main(int argc, char **argv)
{
 string *encoded_codeword;
 int *decoded_msg;
 unsigned int err_sum;
 srand(time(NULL)); 

 // Construction of transition matrix and px1
 matrix_file_read();

 // generate a Markov chain x with length n based on p_x1 and P
 markov_chain_gen();

 // Encode x using conditional Huffman coding
 // Codebook generation:
 cx = new Huffman_Code[M];
 assign_symbol(symbol, M);
 for(unsigned int i=0; i < M; i++) {
 	cout << endl << "X_" << i << ":" << endl;
 	cx[i].param_config(symbol, P, M, i);
 	cx[i].tree_construction();
 	cx[i].show_codebook();
	cout << "The code rate R: " << cx[i].code_rate() << " bits/symbol";
 	cout << endl;
 }

 // Evaluation of entropy rate and code rate
 // Calculate stationary distribution
 z = new float[M];
 stationary_distribution(z, P, M);
 cout << "The stationary distribution z:" << endl << "[";
 for(unsigned int i=0; i<M; i++) cout << z[i] << " ";
 cout << "]" << endl;
 
 // Calculate entropy rate
 Hx = entropy_rate(z, P, M);
 cout << "Entropy rate Hx: " << Hx << " bits" << endl;

 // Encode the generated source Markov sequence
 encoded_codeword = new string[N];
 conditional_huffman_encode(encoded_codeword, markov_source, cx, N);

 // Decode the sequence of codewords
 decoded_msg = new int[N];
 conditional_huffman_decode(decoded_msg, encoded_codeword, cx, N);

 // Check that the decoding is correct
 err_sum = 0;
 for(unsigned int i=0; i<N; i++) err_sum += abs(markov_source[i]-decoded_msg[i]);
 cout << "The sum of errors: " << err_sum << endl;
 if(err_sum == 0) cout << "All messages are correctly decoded" << endl;
 else cout << "There are " << err_sum << " errors on deocding process" << endl; 

 return 0;
}

void matrix_file_read()
{
	string str;
	int row_cnt, col_cnt; 
 
 	// Configure the cardinality of the sample space M and the N
	cout << "Please give the value of M:"; cin >> M;
	cout << endl << "Please give the value of N:"; cin >> N; cout << endl; 
	rowN[0] = 1; rowN[1] = M;
	colN[0] = M; colN[1] = M;

	symbol = new char[M];
	px1 = new float* [rowN[0]]; P = new float* [rowN[1]]; x = new float* [rowN[1]];
	for(unsigned int i=0; i < rowN[0]; i++) px1[i] = new float [colN[0]];
	for(unsigned int i=0; i < rowN[1]; i++) P[i] = new float [colN[1]];
	for(unsigned int i=0; i < rowN[0]; i++) x[i] = new float [colN[1]];

	// Reading the file of transition matrix
	ifstream ifs(P_ij_filename);
 	if(!ifs) {
		cout<<"Read File Error";
		exit(1);
	}
	ifstream ifs_px1(px1_filename);
 	if(!ifs_px1) {
		cout<<"Read File Error";
		exit(1);
	}

    //Reading .CSV file column by column for constructing transition matrix P_i,j
    row_cnt = 0;
    while(getline(ifs,str)) {
        string token;
        istringstream stream(str);

	col_cnt = 0;
        while(getline(stream,token,',')) {
		P[row_cnt][col_cnt] = stof(token.c_str());	
	    	col_cnt += 1;
        }
	row_cnt += 1;
    }

    //Reading .CSV file column by column for constructing matrix px1
    row_cnt = 0;
    while(getline(ifs_px1,str)) {
        string token;
        istringstream stream(str);

	col_cnt = 0;
        while(getline(stream,token,',')) {
		px1[row_cnt][col_cnt] = stof(token.c_str());	
	    	col_cnt += 1;
        }
	row_cnt += 1;
    }

    ifs.close(); ifs_px1.close();
}

void state_distribution(float **x, unsigned int space, float **px1, float **P, unsigned int n)
{
 unsigned int row_num = 1, col_num = space;
 float **product = new float* [row_num];
 product[0] = new float[col_num];
 float **temp = new float* [row_num];
 temp[0] = new float[col_num];

 for(unsigned int i=0; i<row_num; i++)
  for(unsigned int j=0; j<col_num; j++)
   temp[i][j] = px1[i][j];

 for (unsigned int i=0; i < n; i++) {
	matrix_mul(product, temp, P, row_num, colN);
	 for(unsigned int i=0; i<row_num; i++)
	  for(unsigned int j=0; j<col_num; j++)
	   temp[i][j] = product[i][j];
 }
 
 for(unsigned int i=0; i<row_num; i++)
  for(unsigned int j=0; j<col_num; j++)
   x[i][j] = product[i][j];

 delete [] product;
 delete [] temp;
}

void markov_chain(int *gen_source, float *s, unsigned int space, unsigned int n)
{
	int *freq = new int[space];
	int *symbol = new int[space];
	for(unsigned int i=0; i<space; i++) {
		symbol[i] = i+1; // label each symbol by a number
		freq[i] = s[i]*100.0; 
		freq[i] = (freq[i] <= 0) ? 1:freq[i];
	}
	for(unsigned int i=0; i<n; i++) gen_source[i] = pdf(symbol, freq, space);
}

void matrix_mul(
	float **product,
	float **matrix_0, 
	float **matrix_1, 
	unsigned int row_num, 
	unsigned int *col_num
)
{
 unsigned int r0 = row_num; 
 unsigned int c0 = col_num[0]; unsigned int c1 = col_num[1];
 // Initialise all element to "0"
 for(unsigned int i=0; i < r0; i++)
  for(unsigned int j=0; j < c1; j++)
   product[i][j] = 0;
 
 // Perform multiplication of two given matrices
 for(unsigned int i=0; i < r0; i++)
  for(unsigned int j=0; j < c1; j++)
   for(unsigned int k=0; k < c0; k++) {
    product[i][j] += matrix_0[i][k]*matrix_1[k][j];
   }
}

void assign_symbol(char *symbol_inout, unsigned int space)
{
 for(unsigned int i=0; i < space; i++)
  sprintf(&symbol_inout[i], "%d", i+1);
}

void markov_chain_gen()
{
 state_distribution(x, M, px1, P, N);
 for(unsigned int i=0; i < rowN[0]; i++) {
  for(unsigned int j=0; j < colN[1]; j++) {
   cout << x[i][j] << " ";
  }
  cout << endl;
 }
 markov_source = new int[N];
 markov_chain(markov_source, x[0], M, N);
 cout << "The generated source Markov sequence: " << endl;
 for(unsigned int i=0; i<N; i++) cout << markov_source[i] << " ";
 cout << endl;
}

void stationary_distribution(float *z, float **P, unsigned int space)
{
	float **adj, **inv, **Q;
	adj = new float* [space];	
	inv = new float* [space];	
	Q = new float* [space];	
	for(unsigned int i=0; i<space; i++) { 
		adj[i] = new float[space];
		inv[i] = new float[space];
		Q[i] = new float[space];
	}
	for(unsigned int i=0; i<space; i++) {
		for(unsigned int j=0; j<space; j++) {
			if(j==0) Q[i][j] = 1.0;
			else if(i==j) Q[i][j] = P[i][j] - 1.0;
			else Q[i][j] = P[i][j];
		}
	}

	adjoint(Q, adj, space); 
	if (inverse(Q, inv, space)) {
		for(unsigned int i=0; i<space; i++)
			z[i] = inv[0][i];
	}
	else {
		cout << endl << "Wrong calculation on inverse matrix" << endl;
		exit(1);
	}	
}

float entropy_rate(float *z, float **P, unsigned int space)
{
	float temp = 0.0;
	for(unsigned int i=0; i<space; i++)
	 for(unsigned int j=0; j<space; j++) 
		temp = temp+((-1)*z[i]*P[i][j]*log2(P[i][j]));
	return temp;
}

void conditional_huffman_encode(string *codeword, int *msgIn, Huffman_Code *codebookIn, unsigned int n)
{
	codeword[0] = codebookIn[0].show_codeword(msgIn[0]-1);
	for(unsigned int i=1; i<n; i++) 
		codeword[i] = codebookIn[msgIn[i-1]-1].show_codeword(msgIn[i]-1);
	
	cout << endl << "The encoded codewords is:" << endl;
	for(unsigned int i=0; i<n; i++) cout << codeword[i] << " ";
	cout << endl;
}

void conditional_huffman_decode(
	int *msgOut, 
	string *codewordIn, 
	Huffman_Code *codebookIn, 
	unsigned int n
)
{
	unsigned int i;
	i = 0;
	while(codewordIn[0].compare(codebookIn[0].codebook[i].codeword) != 0) i+=1;
	msgOut[0] = i+1;

	for(unsigned int id=1; id<n; id++) {
		i = 0;
		while(codewordIn[id].compare(codebookIn[msgOut[id-1]-1].codebook[i].codeword) != 0) i+=1;
		msgOut[id] = i+1;
	}

	cout << endl << "The sequence of decoded messages is:" << endl;
	for(unsigned int id=0; id<n; id++) cout << msgOut[id] << " ";
	cout << endl;
	
}
