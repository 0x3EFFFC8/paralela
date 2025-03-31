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


void sortRow(vector<vector<int>>& matrix, int row, bool ascending) {
  if (ascending) {
    sort(matrix[row].begin(), matrix[row].end());
  } else {
    sort(matrix[row].rbegin(), matrix[row].rend());
  }
}


void sortColumn(vector<vector<int>>& matrix, int col) {
    vector<int> column;
    for (size_t row = 0; row < matrix.size(); ++row) {
        column.push_back(matrix[row][col]);
    }
    
    sort(column.begin(), column.end());
    
    for (size_t row = 0; row < matrix.size(); ++row) {
        matrix[row][col] = column[row];
    }
}

void shearSort(vector<vector<int>>& matrix){
  int iterations = static_cast<int>(log2(matrix.size())) + 1; 
  for (int i = 0; i < iterations; i++){
    #pragma omp parallel for
    for (int row = 0; row < matrix.size(); row++)
      sortRow(matrix, row, row % 2 == 0);
    #pragma omp parallel for
    for (int col = 0; col < matrix.size(); col++)
      sortColumn(matrix, col);
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

