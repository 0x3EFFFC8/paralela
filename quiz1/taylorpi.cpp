#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

int main(int args, char *argv[]){
  int upper_bound = atoi(argv[1]);
  double ans = 0;
  time_t start_wall = time(nullptr);  
  clock_t start_cpu = clock();
  for (int n = 0; n < upper_bound; n++){
    if (n % 2 != 0) ans -= 1.0/(2.0 * n + 1);
    else ans += 1.0/(2.0 * n + 1);
  }
  ans *= 4;

  clock_t end_cpu = clock();
  time_t end_wall = time(nullptr);
  double cpu_time = static_cast<double>(end_cpu - start_cpu) / CLOCKS_PER_SEC;
  double wall_time = difftime(end_wall, start_wall);
  std::cout << "Pi estimado: " << ans << std::endl;
  std::cout << "Error relativo: " << std::abs((ans - M_PI) / M_PI) << std::endl;
  std::cout << "Tiempo de CPU: " << cpu_time << " segundos" << std::endl;
  std::cout << "Tiempo de reloj (wall-clock time): " << wall_time << " segundos" << std::endl;
  return 0;
}
