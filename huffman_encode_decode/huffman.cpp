#include "huffman.h"

void Huffman_Code::param_config(char *symbol, float **P, unsigned int size, int col_id)
{
	this -> symbol = symbol;
	this -> size = size;
	px1 = new float[size];
	for(unsigned int i=0; i<size;i++) px1[i] = P[i][col_id];
	codebook = new Codebook[size];
}

// To traverse and present the constructed Huffman tree in a recursive manner
void Huffman_Code::traverse_HuffmanCodes(Node *root, string str) 
{ 
	if (!root) 
		return; 
	if (root->symbol != '$') { 
		//cout << root -> symbol << ": " << str << "\n"; 
		// To arrange the order by following the given sequency of symbols
		for(unsigned int i=0; i < size; i++) {
			if(root -> symbol == symbol[i]) {
				codebook[i].symbol = symbol[i];
				codebook[i].px = px1[i];
				codebook[i].codeword = str;
			}
		}
	}

	traverse_HuffmanCodes(root -> left, str + "0"); 
	traverse_HuffmanCodes(root -> right, str + "1"); 
} 

void Huffman_Code::tree_construction()
{ 
	Node *left, *right, *parent; 
	priority_queue<Node*, vector<Node*>, compare> sorting_container; 

	for (unsigned int i = 0; i < size; i++) 
		sorting_container.push(new Node(symbol[i], px1[i])); 

	while (sorting_container.size() != 1) { 
		// Sorting the permutation of given sequence of symbols in an ascending order
		// with respect to the minimum probability
		left = sorting_container.top(); // record 
		sorting_container.pop(); // remove 

		right = sorting_container.top(); 
		sorting_container.pop(); 

		// Combine the left node and right node with one additional node as their parents
		parent = new Node('$', left -> px1 + right -> px1); 
		parent -> left = left; 
		parent -> right = right; 

		sorting_container.push(parent); 
	} 

	traverse_HuffmanCodes(sorting_container.top(), ""); 
} 

void Huffman_Code::show_codebook()
{
	cout << "x\tPr(x)\t|\tC(x)" << endl << "----------------------------------" << endl;
	for(unsigned int i=0; i < size; i++) 
		cout << codebook[i].symbol << "\t"
		     << codebook[i].px << "\t|\t"
		     << codebook[i].codeword << endl;
}

string Huffman_Code::show_codeword(int id)
{
	return codebook[id].codeword;
}

float Huffman_Code::code_rate()
{
 R = 0.0;
 for(unsigned int i=0; i < size; i++)
  R=R+codebook[i].px*codebook[i].codeword.length();
 return R;
}
