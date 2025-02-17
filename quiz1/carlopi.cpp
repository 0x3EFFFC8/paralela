#include <iostream>
#include <pthread.h>
#include <ctime>
#include <random>
#include <cmath>  

struct ThreadData {
    int tosses;
    int hits;
    std::mt19937 *gen; 
    std::uniform_real_distribution<> *dis;
};

void* monteCarlo(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    data->hits = 0;
    for (int i = 0; i < data->tosses; i++) {
        double x = (*data->dis)(*data->gen); 
        double y = (*data->dis)(*data->gen); 
        if (x * x + y * y <= 1.0)
            data->hits++;
    }
    pthread_exit(nullptr);
}

int main(int argc, char* argv[]) {
    int numThreads = std::atoi(argv[1]);
    int numTosses = std::atoi(argv[2]);
    
    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];
    int tossesPerThread = numTosses / numThreads;
    std::mt19937 gen(921945);
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    time_t start_wall = time(nullptr);
    clock_t start_cpu = clock();

    for (int i = 0; i < numThreads; ++i) {
        threadData[i].tosses = tossesPerThread;
        threadData[i].gen = &gen;
        threadData[i].dis = &dis;
        pthread_create(&threads[i], nullptr, monteCarlo, &threadData[i]);
    }

    int totalHits = 0;
    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], nullptr);
        totalHits += threadData[i].hits;
    }

    double pi = 4.0 * totalHits / numTosses;

    clock_t end_cpu = clock();
    time_t end_wall = time(nullptr);

    double cpu_time = static_cast<double>(end_cpu - start_cpu) / CLOCKS_PER_SEC;
    double wall_time = difftime(end_wall, start_wall);

    std::cout << "Pi estimado: " << pi << std::endl;
    std::cout << "Error relativo: " << std::abs((pi - M_PI) / M_PI) << std::endl;
    std::cout << "Tiempo de CPU: " << cpu_time << " segundos" << std::endl;
    std::cout << "Tiempo de reloj (wall-clock time): " << wall_time << " segundos" << std::endl;

    return 0;
}
