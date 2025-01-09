#include "SparseMatrix.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define MAXTERMS 1000

//dataStructure
typedef struct element{
    int row, col, val;
}Element;

struct sparsematrix{
    Element *matrix;
    int row, col, dataNum;
};
//--dataStructure end


//--function
SparseMatrix* SparseMatrixConstructor(int row, int col, int num){
    SparseMatrix *matrix = (SparseMatrix *)malloc(sizeof(SparseMatrix));
    matrix->matrix = (Element *)malloc(sizeof(Element) * (MAXTERMS));
    matrix->row = row;
    matrix->col = col;
    matrix->dataNum = num;

    return matrix;
}

void putElement(SparseMatrix *m, int row, int col, int val){
    //find the row and col
    int left = 0, right = m->dataNum - 1;
    int position = 0;
    while(left <= right){
        int middle = (left + right) / 2;

        if(m->matrix[middle].row < row ||
            (m->matrix[middle].row == row && m->matrix[middle].col < col)){
            left = middle + 1;
            position = left;    
        }
        else if(m->matrix[middle].row > row ||
            (m->matrix[middle].row == row && m->matrix[middle].col > col)){
            right = middle - 1;
        }
        else{
            //update value
            m->matrix[middle].val = val;
            return;
        }
    }

    //insert at position + 1
    memmove(m->matrix + position + 1, m->matrix + position, sizeof(Element) * (m->dataNum - position));
    m->matrix[position].row = row, m->matrix[position].col = col, m->matrix[position].val = val;
    m->dataNum++;
    return;
}

void printMatrix(SparseMatrix *a){
    int index = 0;
    for(int r = 0; r < a->row; r++){
        for(int c = 0; c < a->col; c++){
            if(c != a->matrix[index].col || r != a->matrix[index].row)
                printf("0 ");
            else
                printf("%d ", a->matrix[index++].val);
        }
        printf("\n");
    }
    return;
}

SparseMatrix* Transpose(SparseMatrix *a){
    SparseMatrix *b = SparseMatrixConstructor(a->col, a->row, a->dataNum);

    if(a->dataNum <= 0)
        return b;
    else{
        int *colNum = (int *)calloc(a->col, sizeof(int));
        for(int i = 0; i < a->dataNum; i++)
            colNum[a->matrix[i].col]++;
        
        int *start = (int *)malloc((a->col) * sizeof(int));
        start[0] = 0;
        for(int i = 1; i < a->col; i++)
            start[i] = start[i - 1] + colNum[i - 1];

        for(int i = 0; i < a->dataNum; i++){
            int j = start[a->matrix[i].col]++;
            b->matrix[j].row = a->matrix[i].col;
            b->matrix[j].col = a->matrix[i].row;
            b->matrix[j].val = a->matrix[i].val;
        }

        free(colNum);
        free(start);
        return b;
    }
}

//--helper
void storeSum(SparseMatrix *m, int row, int col, int sum){
    if(sum == 0)
        return;
    
    m->matrix[m->dataNum].row = row;
    m->matrix[m->dataNum].col = col;
    m->matrix[m->dataNum++].val = sum;

    return;
}

SparseMatrix *Multiply(SparseMatrix *a, SparseMatrix *b){
    assert(a->col == b->row);

    SparseMatrix *newb = Transpose(b);
    SparseMatrix *result = SparseMatrixConstructor(a->row, b->col, 0);
    int rowBegin = 0, sum = 0, rowNow = a->matrix[0].row;

    //set boundary
    a->matrix[a->dataNum].row = a->row;
    newb->matrix[b->dataNum].row = b->col;
    newb->matrix[b->dataNum].col = 0;

    while(rowBegin <= a->dataNum){
        int aIndex = rowBegin, bIndex = 0;
        int colNow = newb->matrix[0].row;

        while(bIndex <= newb->dataNum){
            if(a->matrix[aIndex].row != rowNow){
                storeSum(result, rowNow, colNow, sum);
                sum = 0;
                aIndex = rowBegin;
                while(newb->matrix[bIndex++].row == colNow)
                    ;
                colNow = newb->matrix[bIndex].row;
            }
            else if(newb->matrix[bIndex].row != colNow){
                storeSum(result, rowNow, colNow, sum);
                sum = 0;
                aIndex = rowBegin;
                colNow = newb->matrix[bIndex].row;
            }
            else{
                if(a->matrix[aIndex].col > newb->matrix[bIndex].col)
                    bIndex++;
                else if(a->matrix[aIndex].col < newb->matrix[bIndex].col)
                    aIndex++;
                else
                    sum += a->matrix[aIndex++].val * newb->matrix[bIndex++].val;
            }
        }

        while(a->matrix[aIndex++].row == rowNow)
            ;
        rowBegin = aIndex;
        rowNow = a->matrix[aIndex].row;
    }

    return result;
}


void destroySparseMatrix(SparseMatrix *m){
    free(m->matrix);
    free(m);

    return;
}