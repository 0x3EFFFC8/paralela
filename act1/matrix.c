#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 40

void GMatrix(int matrix[SIZE][SIZE]){
  for (int i=0; i < SIZE; i++){
    for (int j = 0; j < SIZE; j++){
      matrix[i][j] = rand() % 2;
    }
  }
}

void addMatrices(int A[SIZE][SIZE],int B[SIZE][SIZE], int C[SIZE][SIZE], int n){
  for (int i = 0; i < n; i++){
    for (int j = 0; j < n; j++){
      C[i][j] = A[i][j] + B[i][j];
    }
  }
}

void subMatrices(int A[SIZE][SIZE], int B[SIZE][SIZE], int C[SIZE][SIZE], int n){
  for (int i = 0; i < n; i++){
    for (int j = 0; j < n; j++){
      C[i][j] = A[i][j] + B[i][j];
    }
  }
}

void multMatrices(int A[SIZE][SIZE], int B[SIZE][SIZE], int C[SIZE][SIZE], int n){
  if (n == 1){
    C[0][0] = A[0][0]*B[0][0];
  }else{
    int mid = n / 2;
    int A11[mid][mid], A12[mid][mid], A21[mid][mid], A22[mid][mid];
    int B11[mid][mid], B12[mid][mid], B21[mid][mid], B22[mid][mid];
    int C11[mid][mid], C12[mid][mid], C21[mid][mid], C22[mid][mid];
    int P1[mid][mid], P2[mid][mid], P3[mid][mid], P4[mid][mid], P5[mid][mid], P6[mid][mid], P7[mid][mid];
    int tmp1[mid][mid], tmp2[mid][mid];

    for (int i = 0; i < mid; i++){
      for (int j = 0; j < mid; j++){
        A11[i][j] = A[i][j];
        A12[i][j] = A[i][j+mid];
        A21[i][j] = A[i+mid][j];
        A22[i][j] = A[i+mid][j+mid];
        B11[i][j] = B[i][j];
        B12[i][j] = B[i][j+mid];
        B21[i][j] = B[i+mid][j];
        B22[i][j] = B[i+mid][j+mid];
      }
    }
    addMatrices(A11,A22,tmp1,mid);
    addMatrices(B11,B22,tmp2,mid);
    multMatrices(tmp1,tmp2,P5,mid);
    addMatrices(A21,A22,tmp1,mid);
    multMatrices(tmp1,B11,P3,mid);
    subMatrices(B12,B22,tmp2,mid);
    multMatrices(A11,tmp2,P1,mid);
    subMatrices(B21,B11,tmp2,mid);
    multMatrices(A22,tmp2,P4,mid);
    addMatrices(A11,A12,tmp1,mid);
    multMatrices(tmp1,B22,P2,mid);
    subMatrices(A12,A22,tmp1,mid);
    addMatrices(B21,B22,tmp2,mid);
    multMatrices(tmp1,tmp2,P6,mid);
    subMatrices(A11,A21,tmp1,mid);
    addMatrices(B11,B12,tmp2,mid);
    multMatrices(tmp1,tmp2,P7,mid);

    addMatrices(P5,P4,tmp1,mid);
    subMatrices(tmp1,P2,tmp2,mid);
    addMatrices(tmp2,P6,C11,mid);
    addMatrices(P1,P2,C12,mid);
    addMatrices(P3,P4,C21,mid);
    addMatrices(P5,P1,tmp1,mid);
    subMatrices(tmp1,P3,tmp2,mid);
    subMatrices(tmp2,P7,C22,mid);

    for (int i = 0; i < mid; i++){
      for (int j = 0; j < mid; j++){
        C[i][j] = C11[i][j];
        C[i][j+mid] = C12[i][j];
        C[i+mid][j] = C21[i][j];
        C[i+mid][j+mid] = C22[i][j];
      }
    }
  }
}

void printMatrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(){
  int matrixA[SIZE][SIZE];
  int matrixB[SIZE][SIZE];
  int matrixC[SIZE][SIZE];
  int matrixD[SIZE][SIZE];
  int matrixE[SIZE][SIZE];
  srand(time(NULL));
  GMatrix(matrixA);
  GMatrix(matrixB);
  addMatrices(matrixA,matrixB,matrixC,SIZE);
  subMatrices(matrixA,matrixB,matrixD,SIZE);
  multMatrices(matrixC,matrixD,matrixE,SIZE);
  printMatrix(matrixE);
  return 0;
}
