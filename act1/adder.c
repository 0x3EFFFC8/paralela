#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define NUM_THREADS sysconf(_SC_NPROCESSORS_ONLN);
#define ARRAY_SIZE 10000000

int main() {
    pid_t pid;
    int tmpsum, start, end, pipefd[2];
    int array[ARRAY_SIZE];
    int n = 0;
    intptr_t arraySum = 0;
    char input[1000000];

    fgets(input, sizeof(input), stdin);

    char *token = strtok(input, " ");
    while (token != NULL && n < ARRAY_SIZE) {
        sscanf(token, "%d", &array[n]);
        n++;
        token = strtok(NULL, " ");
    }

    NUM_THREADS = round(n / 3);

    if (pipe(pipefd) == -1) exit(-1);
    
    for (int i = 0; i < NUM_THREADS; i++){
      if ((pid=fork()) < 0) exit(-1);
      if (pid == 0){
        close(pipefd[0]);
        tmpsum = 0;
        start = i * (n/NUM_THREADS); 
        end =  (i+1)*(n/NUM_THREADS);
        if (i == NUM_THREADS-1) end = n;
        for (int j= start; j < end; j++)
          tmpsum += array[j];
        write(pipefd[1],&tmpsum,sizeof(tmpsum));
        close(pipefd[1]);
        exit(0);
      }
    }  
    
    close(pipefd[1]);
    for (int i = 0; i < NUM_THREADS; i++){
      read(pipefd[0],&tmpsum, sizeof(tmpsum));
      arraySum += tmpsum;
    }
    close(pipefd[0]);
    
    for (int i = 0; i < NUM_THREADS; i++)
      wait(NULL);

    printf("Array sum: %ld\n", (long)arraySum);

    return 0;
}
