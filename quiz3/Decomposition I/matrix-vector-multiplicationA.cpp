#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono> 

using namespace std;
using namespace chrono; 

int main() {
    const int m = 4;
    const int n = 6;

    srand(time(0));

    vector<vector<double>> matrix(m, vector<double>(n));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = rand() % 10; 
        }
    }

    vector<double> vec(n);
    for (int j = 0; j < n; ++j) {
        vec[j] = rand() % 10;
    }

    vector<double> result(m, 0.0);

    auto start = high_resolution_clock::now();

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i] += matrix[i][j] * vec[j];
        }
    }

    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;

    cout << "Matrix:\n";
    for (const auto& row : matrix) {
        for (double val : row) cout << val << " ";
        cout << endl;
    }

    cout << "\nVector:\n";
    for (double val : vec) cout << val << " ";
    cout << endl;

    cout << "\nResultado:\n";
    for (double val : result) cout << val << " ";
    cout << endl;

      cout << "\nTiempo de ejecucion: " << elapsed.count() << "s" << endl;

    return 0;
}
