#include <mpi.h>
#include <stdlib.h>
#include <random>
#include <cstdio>
#include <unistd.h>
#include <iostream>

#define PING_PONG_LIMIT 100

int main(int argc, char** argv) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1,1000);
  int random_number = dist(rng)-1;

  MPI_Init(&argc, &argv);
  
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 2){
    std::cerr << "Este programa necesita exactamente 2 procesos" << std::endl;
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
    
  double start_time, end_time;
  char hostname[255];

  gethostname(hostname, sizeof(hostname));
  
  for (int ping_pong_count = 0; ping_pong_count < PING_PONG_LIMIT; ping_pong_count++){
    if (rank == 0) {
      random_number++;
      MPI_Send(&random_number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      start_time = MPI_Wtime();
      MPI_Recv(&random_number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      end_time = MPI_Wtime();
      double rtt = end_time - start_time;
      std::cout << rank << " - Iteración " << ping_pong_count+1 << " numero: " << random_number << "): RTT = " << rtt << " segundos" << " en " << hostname <<std::endl;
    } else {
        start_time = MPI_Wtime();
        MPI_Recv(&random_number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        random_number++;
        MPI_Send(&random_number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        end_time = MPI_Wtime();
        double rtt = end_time - start_time;
        std::cout << rank << " - Iteración " << ping_pong_count+1 << " numero: " << random_number << "): RTT = " << rtt << " segundos" << " en " << hostname << std::endl;
    }
  }
  MPI_Finalize();
  return 0;
}
