#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int shmid;
int *numbers;
int size;

int main(int argc, char *argv[]){
  struct timeval start,end;
  gettimeofday(&start,NULL);

  shmid = shmget(IPC_PRIVATE, 1000000*sizeof(int), 0666 | IPC_CREAT);
  numbers = shmat(shmid, 0, 0);

  FILE *input = fopen(argv[1], "r");

  size = 0;
  while(fscanf(input, "%d\n", &numbers[size++]) != EOF);

  int count = 0;
  int sum = 0;
  int max = INT_MIN;
  int min = INT_MAX;
  while(count < size){
    int readn = numbers[count];
    if(readn < min) min = readn;
    if(readn > max) max = readn;
    sum += readn;
    count++;
  }

  FILE *out = fopen(argv[2], "w+");
  fprintf(out, "hi, i\'m process %d and my parent is %d\n", getpid(), getppid());
  fprintf(out, "min: %d\n", min);
  fprintf(out, "max: %d\n", max);
  fprintf(out, "sum: %d\n", sum);

  shmdt(numbers);
  shmctl(shmid, IPC_RMID, NULL);

  gettimeofday(&end,NULL);
  printf("%ld microseconds\n", (end.tv_sec*100000 + end.tv_usec) - (start.tv_sec*100000 + start.tv_usec));

  return 0;
}
