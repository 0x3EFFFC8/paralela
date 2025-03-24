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

int main(int argc, char* argv[]) {

    srand(time(0));
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    vector<vector<int>> matrixA(rows, vector<int>(cols));
    vector<vector<int>> matrixB(cols, vector<int>(rows));
    
    fillMatrix(matrixA);
    fillMatrix(matrixB);

    auto start = high_resolution_clock::now();

    vector<vector<int>> sum;
    if(rows == cols) {
        sum = vector<vector<int>>(rows, vector<int>(cols));
        for(int i = 0; i < rows; ++i)
            for(int j = 0; j < cols; ++j)
                sum[i][j] = matrixA[i][j] + matrixB[j][i];
    }

    vector<vector<int>> product(rows, vector<int>(rows, 0));
    for(int i = 0; i < rows; ++i)
        for(int j = 0; j < rows; ++j)
            for(int k = 0; k < cols; ++k)
                product[i][j] += matrixA[i][k] * matrixB[k][j];

    vector<vector<int>> transpose(cols, vector<int>(rows));
    for(int i = 0; i < rows; ++i)
        for(int j = 0; j < cols; ++j)
            transpose[j][i] = matrixA[i][j];

    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;

    if(rows == cols) {
        cout << "Suma:\n"; printMatrix(sum);
    }
    cout << "\nProducto:\n"; printMatrix(product);
    cout << "\nTranspuesta:\n"; printMatrix(transpose);
    cout << "\nTiempo: " << elapsed.count() << "s" << endl;

    return 0;
}
