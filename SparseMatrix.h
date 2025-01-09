#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

//row index, col index start from 0
typedef struct sparsematrix SparseMatrix;

SparseMatrix* SparseMatrixConstructor(int row, int col, int num);
void putElement(SparseMatrix *m, int row, int col, int val);
void printMatrix(SparseMatrix *a);
SparseMatrix* Transpose(SparseMatrix *a);
SparseMatrix *Multiply(SparseMatrix *a, SparseMatrix *b);

void destroySparseMatrix(SparseMatrix *m);

#endif