#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NUM_THREADS 2
#define ARRAYSIZE 1000

struct ThreadData {
  int array[ARRAYSIZE];
  int size, sum, sub, ansMin, ansMult;
};

void* functionA(void *arg){
  struct ThreadData* data = (struct ThreadData*)arg; 
  data->ansMin = 1000000;
  for(int i = 0; i < data->size; i++){
    if (data->ansMin > data->array[i])
      data->ansMin = data->array[i];
  }
  pthread_exit(NULL);
}

void* functionB(void *arg){
  struct ThreadData* data = (struct ThreadData*)arg;
  data->sum = 0;
  for(int i = 0; i < data->size; i++)
    data->sum += data->array[i];
  pthread_exit(NULL);
}

void* functionC(void *arg){
  struct ThreadData* data = (struct ThreadData*)arg;
  data->sub = 0;
  for(int i = 0; i < data->size; i++)
    data->sub -= data->array[i];
  pthread_exit(NULL);
}

void* functionD(void *arg){
  struct ThreadData* data = (struct ThreadData*)arg;
  data->ansMult = data->sum * data->sub;
  pthread_exit(NULL);
}

double functionE(int data_f_a, int data_f_b){
  return data_f_a + 2.7172 * pow(data_f_b, 3) / pow(2, data_f_a);
}

int main(){
  char input[ARRAYSIZE];
  pthread_t threads[NUM_THREADS];
  struct ThreadData data;
  struct ThreadData *threadData = &data;
  threadData->size = 0;
  fgets(input, sizeof(input), stdin); 
  char *token = strtok(input, " ");
  while (token != NULL && threadData->size < ARRAYSIZE){
    sscanf(token, "%d", &threadData->array[threadData->size]);
    threadData->size++;
    token = strtok(NULL," ");
  }
  
  pthread_create(&threads[0], NULL, functionB, threadData);
  pthread_create(&threads[1], NULL, functionC, threadData);
  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL); 
  pthread_create(&threads[0], NULL, functionA, threadData);
  pthread_create(&threads[1], NULL, functionD, threadData);
  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);

  double result = functionE(threadData->ansMin, threadData->ansMult);
  printf("Result of funcionE: %f\n", result);
  
  return 0;
}
