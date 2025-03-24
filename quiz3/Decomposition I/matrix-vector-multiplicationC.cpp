#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono> 

using namespace std;
using namespace chrono;

#define NUM_THREADS 4

int main(int argc, char* argv[]) {

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);

    vector<vector<double>> matrix(m, vector<double>(n, 1.0));
    vector<double> vec(n, 2.0);
    vector<double> result(m, 0.0);

    omp_set_num_threads(NUM_THREADS);

    auto start = high_resolution_clock::now();

    #pragma omp parallel for
    for (int i = 0; i < m; ++i) {
        double sum = 0.0;
        for (int j = 0; j < n; ++j) {
            sum += matrix[i][j] * vec[j];
        }
        result[i] = sum;
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
    
    cout << "Resultado:\n";
    for (double val : result) {
        cout << val << " ";
    }
    
    cout << "\nTiempo de ejecucion: " << elapsed.count()  << "s" << endl;

    return 0;
}
