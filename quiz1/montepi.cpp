#include <iostream>
#include <random>
#include <ctime>
#include <cmath>

double monteCarloPi(int numTosses){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(-1.0,1.0);
  int hits = 0;
  for(int i = 0; i < numTosses; i++){
    double x = dis(gen);
    double y = dis(gen);
    if ((x*x + y*y) <= 1.0) hits++;
  }
  return 4.0 * hits/numTosses;
} 

int main(int args, char* argv[]){
  int numTosses = atoi(argv[1]);
  time_t start_wall = time(nullptr);
  clock_t start_cpu = clock();
  double pi = monteCarloPi(numTosses);
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
