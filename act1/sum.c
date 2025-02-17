#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  int pipefd[2];
  int pid;
  int start = atoi(argv[1]);
  int end = atoi(argv[2]);
  int sum = 0;
  
  if ((pipe(pipefd)) < 0) exit(-1);
  if ((pid=fork()) < 0) exit(-1);
    
  if (pid==0){
    for (int i = start; i <= end; i++)
        sum += i;
    close(pipefd[0]);
    write(pipefd[1],&sum, sizeof(sum));
    close(pipefd[1]);
  } 
  else {
    wait(NULL);
    close(pipefd[1]);
    read(pipefd[0],&sum,sizeof(sum));
    close(pipefd[0]);
    printf("sum: %d\n",sum);

  }
  return 0;
}



