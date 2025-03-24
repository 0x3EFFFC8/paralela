#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;

void fillMatrix(vector<vector<int>>& mat) {
    for(auto& row : mat)
        for(auto& val : row)
            val = rand() % 10;
}

void printMatrix(const vector<vector<int>>& mat) {
    for(const auto& row : mat) {
        for(int val : row) cout << val << " ";
        cout << endl;
    }
}

int main() {
    srand(time(0));
    const int size = 3;
    
    vector<vector<int>> matrixA(size, vector<int>(size));
    vector<vector<int>> matrixB(size, vector<int>(size));
    
    fillMatrix(matrixA);
    fillMatrix(matrixB);

    auto start = high_resolution_clock::now();

    vector<vector<int>> sum(size, vector<int>(size));
    for(int i = 0; i < size; ++i)
        for(int j = 0; j < size; ++j)
            sum[i][j] = matrixA[i][j] + matrixB[i][j];

    vector<vector<int>> product(size, vector<int>(size, 0));
    for(int i = 0; i < size; ++i)
        for(int j = 0; j < size; ++j)
            for(int k = 0; k < size; ++k)
                product[i][j] += matrixA[i][k] * matrixB[k][j];

    vector<vector<int>> transpose(size, vector<int>(size));
    for(int i = 0; i < size; ++i)
        for(int j = 0; j < size; ++j)
            transpose[j][i] = matrixA[i][j];

    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;

    cout << "Suma:\n"; printMatrix(sum);
    cout << "\nProducto:\n"; printMatrix(product);
    cout << "\nTranspuesta:\n"; printMatrix(transpose);
    cout << "\nTiempo: " << elapsed.count() << "s" << endl;

    return 0;
}
