#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <ctime>

using namespace std;
using namespace chrono;

int main(int argc, char* argv[]) {
    
    const int SIZE = atoi(argv[1]);       
    const double THRESHOLD = 0.1;          
    const double HOT_LIMIT = 0.75;          
    const double COLD_LIMIT = 0.45;         

    vector<vector<double>> plateA(SIZE, vector<double>(SIZE, 0.0));
    vector<vector<double>> plateB(SIZE, vector<double>(SIZE, 0.0));
    
    int iterations = 0;
    int hotCells = 0;
    int totalHot = 0;
    int totalCold = 0;

    auto startWall = high_resolution_clock::now();
    clock_t startCpu = clock();

    do {
        hotCells = 0;
        
        for (int i = 1; i < SIZE-1; ++i) {
            for (int j = 1; j < SIZE-1; ++j) {
                double neighborsAvg = (plateA[i+1][j] + plateA[i-1][j] + plateA[i][j+1] + plateA[i][j-1]) / 4.0;
                plateB[i][j] = (neighbors_avg * 4 + plateA[i][j] * 4) / 8.0;
                if (fabs(plateA[i][j] - neighbors_avg) > THRESHOLD) {
                    hotCells++;
                }
            }
        }
        
        swap(plateA, plateB);
        iterations++;
        
    } while (hotCells > 0 && iterations < 10000);

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (plateA[i][j] >= HOT_LIMIT) totalHot++;
            else if (plateA[i][j] <= COLD_LIMIT) totalCold++;
        }
    }

    auto endWall = high_resolution_clock::now();
    double wallTime = duration_cast<milliseconds>(endWall - startWall).count() / 1000.0;
    double cpuTime = double(clock() - startCpu) / CLOCKS_PER_SEC;

    cout << "Tiempo de ejecucion: " << wallTime
         << "\nTiempo CPU: " << cpuTime
         << "\nCeldas calientes (>=" << HOT_LIMIT << "): " << totalHot
         << "\nCeldas frías (<=" << COLD_LIMIT << "): " << totalCold
         << "\nCeldas en transición: " << SIZE*SIZE - totalHot - totalCold 
         << endl
    return 0;
}
