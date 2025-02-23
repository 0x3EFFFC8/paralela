#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NUM_THREADS 3
#define ARRAYSIZE 100

struct ThreadData {
  int array[ARRAYSIZE];
  int size, sum, sub, min, mult, div;
};

void* functionA(void *arg){
  struct ThreadData* data = (struct ThreadData*)arg; 
  for(int i = 0; i < data->size; i++){
    if (data->min > data->array[i]){
      data->min = data->array[i];
    }
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
  data->mult = data->sum * data->sub;
  pthread_exit(NULL);
}
 
void* functionF(void *arg){
  struct ThreadData* data = (struct ThreadData*)arg;
  data->div = data->sum/abs(data->min);
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
  threadData->min = 1000000;
  fgets(input, sizeof(input), stdin); 
  char *token = strtok(input, " ");
  while (token != NULL && threadData->size < ARRAYSIZE){
    sscanf(token, "%d", &threadData->array[threadData->size]);
    threadData->size++;
    token = strtok(NULL," ");
  }
  pthread_create(&threads[0],NULL,functionA,threadData);
  pthread_create(&threads[1],NULL,functionB,threadData);
  pthread_create(&threads[2],NULL,functionC,threadData);
  pthread_join(threads[0],NULL);
  pthread_join(threads[1],NULL); 
  pthread_join(threads[2],NULL);
  pthread_create(&threads[0],NULL,functionF,threadData);
  pthread_create(&threads[1],NULL,functionD,threadData);
  pthread_join(threads[0],NULL);
  pthread_join(threads[1],NULL);

  double result = functionE(threadData->div,threadData->mult);
  printf("Result of funcion E: %f\n", result);
}
