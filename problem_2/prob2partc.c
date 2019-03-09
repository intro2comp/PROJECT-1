#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>



FILE *writeFile;


int *integers, sharedmem, size, num;


void process(int begin, info_struct *data, int numRemainder){
  if (begin >= size) return;
  int end = begin+num;
  if (numRemainder > 0) end++;

  int fd[2];
  pipe(fd);
  pid_t childpid;

  fflush(writeFile);
  childpid = fork();

  if (childpid == 0){
    fprintf(writeFile, "Hi I\'m process %d and my parent is %d.\n", getpid(), getppid());
    close(fd[0]);

    while(begin < end){
      int number = integers[begin];
      if (number < data->min) data->min = number;
      if (number > data->max) data->max = number;
      data->sum += number;
      data->count++;
      begin++;
    }

    write(fd[1], data, sizeof(info_struct));
    exit(0);
  }
  else{
    close(fd[1]);

    process(end, data, numRemainder-1);

    info_struct new_info;
    read(fd[0], &new_info, sizeof(info_struct));
    if (new_info.min < data->min) data->min = new_info.min;
    if (new_info.max > data->max) data->max = new_info.max;
    data->sum += new_info.sum;
    data->count += new_info.count;

    waitpid(childpid);
  }
}

int main(int argc, char *argv[]){
  struct timeval start,end;
  gettimeofday(&start,NULL);

  sharedmem = shmget(IPC_PRIVATE, 1000000*sizeof(int), 0666 | IPC_CREAT);
  integers = shmat(sharedmem, 0, 0);

  FILE *readFile = fopen(argv[1], "r");
  writeFile = fopen(argv[2], "w+");
  int numProcesses = atoi(argv[3]);

  size = 0;
  while(fscanf(readFile, "%d\n", &integers[size++]) != EOF);
  num = size/numProcesses;
  int perProcessRemainder = size%numProcesses;

  info_struct info = {.min = INT_MAX, .max = INT_MIN, .sum = 0, .count = 0};
  process(0, &info, perProcessRemainder);

  fprintf(writeFile, "min: %d\n", info.min);
  fprintf(writeFile, "max: %d\n", info.max);
  fprintf(writeFile, "sum: %d\n", info.sum);

  fclose(readFile);
  fclose(writeFile);

  shmdt(integers);
  shmctl(sharedmem, IPC_RMID, NULL);

  gettimeofday(&end,NULL);
  printf("%ld microseconds\n", (end.tv_sec*100000 + end.tv_usec) - (start.tv_sec*100000 + start.tv_usec));
