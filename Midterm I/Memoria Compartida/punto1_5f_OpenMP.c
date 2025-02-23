#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define ARRAYSIZE 1000

struct ThreadData {
  int array[ARRAYSIZE];
  int size, sum, sub, ansMin, ansMult;
};

void functionA(struct ThreadData *data){
  data->ansMin = 1000000;
  for(int i = 0; i < data->size; i++){
    if (data->ansMin > data->array[i])
      data->ansMin = data->array[i];
  }
}

void functionB(struct ThreadData *data){
  data->sum = 0;
  for(int i = 0; i < data->size; i++)
    data->sum += data->array[i];
}

void functionC(struct ThreadData *data){
  data->sub = 0;
  for(int i = 0; i < data->size; i++)
    data->sub -= data->array[i];
}

void functionD(struct ThreadData *data){
  data->ansMult = data->sum * data->sub;
}

double functionE(int data_f_a, int data_f_b){
  return data_f_a + 2.7172 * pow(data_f_b, 3) / pow(2, data_f_a);
}

int main(){
  char input[ARRAYSIZE];
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
  
  #pragma omp parallel 
  {
    #pragma omp sections 
    {
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
        functionA(threadData);
      }
      #pragma omp section 
      {
        functionD(threadData);
      }
    }
  }

  double result = functionE(threadData->ansMin, threadData->ansMult);
  printf("Result of funcionE: %f\n", result);
  
  return 0;
}
