#ifndef _HUFFMAN_H
#define _HUFFMAN_H
#include <bits/stdc++.h> 
using namespace std; 

typedef struct CodeBook {
	float px;
	char symbol;
	string codeword;
} Codebook;

// One node of Huffman binary tree
typedef struct Node_t { 
	char symbol; 
	float px1; // from given probability mass function
	struct Node_t *left, *right; 

	Node_t(char symbol, float px1) { 
		this -> symbol = symbol; 
		this -> px1 = px1; 
		left = right = NULL; 
	} 
} Node; 

struct compare { 
	bool operator() (Node *left, Node *right) { 
		return (left -> px1 > right -> px1); 
	} 
};
 
class Huffman_Code {
	private:
		char *symbol;
		float *px1;
		unsigned int size;
		float R; // code rate
	public:
	Codebook *codebook;
	void param_config(char *symbol, float **P, unsigned int size, int col_id);
	void traverse_HuffmanCodes(Node *root, string str);
	void tree_construction();
	void show_codebook();
	float code_rate();
	string show_codeword(int id);
};
#endif // _HUFFMAN_H
