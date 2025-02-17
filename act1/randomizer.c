#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS sysconf(_SC_NPROCESSORS_ONLN)
#define ARRY_SIZE 10000000

int array[ARRY_SIZE];

void *generateNumbers(void *arg) {
  int id = (intptr_t)arg;
  int start = id * (ARRY_SIZE/NUM_THREADS);
  int end = (id + 1)*(ARRY_SIZE/NUM_THREADS);
  if (id == NUM_THREADS-1) end = ARRY_SIZE;
  unsigned int seed = time(NULL)^(intptr_t)pthread_self();
  for (int i = start; i < end; i++){
    array[i] = rand_r(&seed) % 100;
  }
}

int main(int argc, char *argv[]){
  int rc, offset;
  int n = atoi(argv[1]);
  char *buffer;
  pthread_t threads[NUM_THREADS];

  for (int t = 0; t < NUM_THREADS; t++){
    rc = pthread_create(&threads[t], NULL, generateNumbers, (void *)(intptr_t)t);
    if (rc) exit(-1);
  }

  for (int t = 0; t < NUM_THREADS; t++)
    pthread_join(threads[t],NULL);
  
  buffer = malloc(n*10);
  if (buffer == NULL) exit(-1);
  offset = 0;
  for (int i = 0; i < n; i++) {
    offset += snprintf(buffer + offset, n*10 - offset, "%d ", array[i]);
    if (offset >= n*10){ 
      free(buffer);
      exit(-1);
    }
  }
  printf("%s\n", buffer);
  free(buffer);
  return 0;
}
