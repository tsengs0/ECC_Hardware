#ifndef _MATRIX_OP_H
#define _MATRIX_OP_H
#include<bits/stdc++.h> 
using namespace std; 

void getCofactor(float **A, float **temp, int p, int q, int n);
float determinant(float **A, int n, unsigned int space); 
void adjoint(float **A, float **adj, unsigned int space); 
bool inverse(float **A, float **inverse, unsigned int space); 
template<class T> 
void display(T **A, unsigned int space); 
#endif // _MATRIX_OP_H
