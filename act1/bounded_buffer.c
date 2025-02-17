#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE]; 
int count = 0;          

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond_producer = PTHREAD_COND_INITIALIZER; 
pthread_cond_t cond_consumer = PTHREAD_COND_INITIALIZER;

void* producer(void* arg) {
  while (true) {
    pthread_mutex_lock(&mutex);
    while (count == BUFFER_SIZE) pthread_cond_wait(&cond_producer, &mutex);
    int i = 0;
    bool band = true;
    while ((i < BUFFER_SIZE) && (band)){
      if (buffer[i] == 0){
        buffer[i] = rand() % 100 + 1;
        printf("Productor %ld: puso %d en la posición %d\n", (long)arg, buffer[i], i);
        band = false;
        count++;
        i++;
      }
    }
    pthread_cond_signal(&cond_consumer);
    pthread_mutex_unlock(&mutex);
    sleep(1); 
  }
}

void* consumer(void* arg) {
  while (true) {  
    pthread_mutex_lock(&mutex);
    while (count == 0) pthread_cond_wait(&cond_consumer, &mutex);
      int i = 0;
      bool band = true;
      while ((i < BUFFER_SIZE) && (band)){
        if (buffer[i] != 0){
          printf("Consumidor %ld: consumió %d de la posición %d\n", (long)arg, buffer[i], i);
          buffer[i] = 0;
          band = false;
          count--;
          i++;
        }
      } 
      pthread_cond_signal(&cond_producer); 
      pthread_mutex_unlock(&mutex);
    if (i-1 > 0) sleep(buffer[i-1]);
    else sleep(1);
  }
}

int main(int argc, char* argv[]) {
  srand(time(NULL));
  int p = atoi(argv[1]);
  int c = atoi(argv[2]);
  pthread_t producers[p];
  pthread_t consumers[c];
  for (int i = 0; i < BUFFER_SIZE; i++) buffer[i] = 0;
  for (long t = 0; t < p; t++) pthread_create(&producers[t],NULL,producer,(void*)t);
  for (long t = 0; t < c; t++) pthread_create(&consumers[t],NULL,consumer,(void*)t);
  for (int t = 0; t < p; t++) pthread_join(producers[t],NULL);
  for (int t = 0; t < c; t++) pthread_join(consumers[t],NULL);
  return 0;
}
