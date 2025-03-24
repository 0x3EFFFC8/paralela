#include <iostream>
#include <vector>
#include <omp.h>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace chrono;

void initializeMatrix(vector<vector<int>>& matrix, int rows, int cols) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = rand() % 2; 
        }
    }
}

void printMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

void labelComponents(vector<vector<int>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    bool changed;

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (matrix[i][j] == 1) {
                matrix[i][j] = i * cols + j + 1;
            }
        }
    }

    do {
        changed = false;

        #pragma omp parallel for collapse(2) reduction(||:changed)
        for (int i = 1; i < rows - 1; ++i) {
            for (int j = 1; j < cols - 1; ++j) {
                if (matrix[i][j] != 0) {
                    int minLabel = matrix[i][j];

                    minLabel = min(minLabel, matrix[i - 1][j]);
                    minLabel = min(minLabel, matrix[i + 1][j]);
                    minLabel = min(minLabel, matrix[i][j - 1]);
                    minLabel = min(minLabel, matrix[i][j + 1]);

                    if (minLabel < matrix[i][j]) {
                        matrix[i][j] = minLabel;
                        changed = true;
                    }
                }
            }
        }

    } while (changed);
}

int main(int argc, char* argv[]) {

    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    vector<vector<int>> matrix(rows, vector<int>(cols, 0));
    initializeMatrix(matrix, rows, cols);

    cout << "Matriz original:\n";
    printMatrix(matrix);

    auto start = high_resolution_clock::now();

    labelComponents(matrix);

    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;

    cout << "\nMatriz etiquetada:\n";
    printMatrix(matrix);

    cout << "\nTiempo de ejecución: " << elapsed.count() << "s\n";

    return 0;
}
