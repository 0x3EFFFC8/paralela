#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define ARRAYSIZE 100

struct ThreadData {
  int array[ARRAYSIZE];
  int size, sum, sub, min, mult, div;
};

void* functionA(struct ThreadData *data){
  for(int i = 0; i < data->size; i++){
    if (data->min > data->array[i]){
      data->min = data->array[i];
    }
  }
}

void* functionB(struct ThreadData *data){
  data->sum = 0;
  for(int i = 0; i < data->size; i++)
    data->sum += data->array[i];
}

void* functionC(struct ThreadData *data){
  data->sub = 0;
  for(int i = 0; i < data->size; i++)
    data->sub -= data->array[i];
}

void* functionD(struct ThreadData *data){
  data->mult = data->sum * data->sub;
}
 
void* functionF(struct ThreadData *data){
  data->div = data->sum/abs(data->min);
}

double functionE(int data_f_a, int data_f_b){
  return data_f_a + 2.7172 * pow(data_f_b, 3) / pow(2, data_f_a);
}

int main(){
  char input[ARRAYSIZE];
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

  #pragma omp parallel
  {
    #pragma omp sections
    { 
      #pragma omp section
      {
        functionA(threadData);
      }
      #pragma omp section 
      {
        functionB(threadData);
      }
      #pragma omp section 
      {
        functionC(threadData);
      }
    }

    #pragma omp barrier

    #pragma omp sections
    {
      #pragma omp section
      {
        functionF(threadData);
      }
      #pragma omp section 
      {
        functionD(threadData);
      }
    }
  }

  double result = functionE(threadData->div,threadData->mult);
  printf("Result of funcion E: %f\n", result);
}
