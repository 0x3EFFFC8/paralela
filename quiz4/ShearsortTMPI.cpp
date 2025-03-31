#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <omp.h>

using namespace std; 

void printMatrix(const vector<vector<int>>& matrix){
  for (const auto& row : matrix){
    for (int val : row)
      cout << val << " ";
    cout << endl;
  }
}

void transposeMatrix(vector<vector<int>>& matrix) {
  #pragma omp parallel for collapse(2)
  for (size_t i = 0; i < matrix.size(); ++i) {
    for (size_t j = i + 1; j < matrix.size(); ++j) {
      swap(matrix[i][j], matrix[j][i]);
    }
  }
}

void sortRow(vector<vector<int>>& matrix, int row, bool ascending) {
  if (ascending) {
    sort(matrix[row].begin(), matrix[row].end());
  } else {
    sort(matrix[row].rbegin(), matrix[row].rend());
  }
}

void shearSort(vector<vector<int>>& matrix){
  int iterations = static_cast<int>(log2(matrix.size())) + 1; 
  for (int i = 0; i < iterations; i++){
    #pragma omp parallel for
    for (int row = 0; row < matrix.size(); row++)
      sortRow(matrix, row, row % 2 == 0);
    transposeMatrix(matrix);
    #pragma omp parallel for
    for (int row = 0; row < matrix.size(); row++)
      sortRow(matrix, row, true);
    transposeMatrix(matrix);
  }  
}

int main(){
  int size = 100;

  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dist(1, 1000);
  
  vector<vector<int>> matrix(size, vector<int>(size));
  
  for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
          matrix[i][j] = dist(gen);
      }
  }

  //printMatrix(matrix);

  auto start = chrono::high_resolution_clock::now();
  shearSort(matrix);
  auto end = chrono::high_resolution_clock::now();

  //printMatrix(matrix);
  
  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
  cout << "\nExecution time: " << duration.count() << " microseconds\n";
  

  return 0;
}

