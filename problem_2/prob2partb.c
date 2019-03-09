#include "info.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


FILE *writeFile;
int numIntegers;
int sharedid;
int totalIntegers;
int *num;



void process(int startPosition, data_struct *data, int totalRemainder){
    if (startPosition >= totalIntegers) return;
    fprintf(writeFile, "Hi I'm process %d and my parent is %d\n", getpid(), getppid());
    
    int endPosition = startPosition+numIntegers;
    if (totalRemainder > 0) endPosition++;
    
    
    int fd[2];
    pipe(fd);
    pid_t childpid;
    
    fflush(writeFile);
    childpid = fork();
    
    
    if (childpid == 0){
        close(fd[0]);
        process(endPosition, data, totalRemainder-1);
        write(fd[1], data, sizeof(data_struct));
        exit(0);
    }
    else{
        close(fd[1]);
        
        
        while(startPosition < endPosition){
            int number = num[startPosition];
            if (number < data->min) data->min = number;
            if (number > data->max) data->max = number;
            data->sum += number;
            data->count++;
            startPosition++;
        }
        
        data_struct new_data;
        read(fd[0], &new_data, sizeof(data_struct));
        if (new_data.min < data->min) data->min = new_data.min;
        if (new_data.max > data->max) data->max = new_data.max;
        data->sum += new_data.sum;
        data->count += new_data.count;
        
        waitpid(childpid);
    }
}

int main(int argc, char *argv[]){
    struct timeval startPosition,endPosition;
    gettimeofday(&startPosition,NULL);
    
    
    sharedid = shmget(IPC_PRIVATE, 1000000*sizeof(int), 0666 | IPC_CREAT);
    num = shmat(sharedid, 0, 0);
    
    
    FILE *readFile = fopen(argv[1], "r");
    writeFile = fopen(argv[2], "w+");
    int numProcesses = atoi(argv[3]);
    
    totalIntegers = 0;
    while(fscanf(readFile, "%d\n", &num[totalIntegers++]) != EOF);
    numIntegers = totalIntegers/numProcesses;
    int totalRemainder = totalIntegers%numProcesses;
    
    
    data_struct data = {.min = INT_MAX, .max = INT_MIN, .sum = 0, .count = 0};
    process(0, &data, totalRemainder);
    
    fprintf(writeFile, "min: %d\n", data.min);
    fprintf(writeFile, "max: %d\n", data.max);
    fprintf(writeFile, "sum: %d\n", data.sum);
    
    
    fclose(readFile);
    fclose(writeFile);
    
    
    shmdt(num);
    shmctl(sharedid, IPC_RMID, NULL);
    
    gettimeofday(&endPosition,NULL);
    printf("%ld microseconds\n", (end.tv_sec*100000 + end.tv_usec) - (startPosition.tv_sec*100000 + start.tv_usec));
    
    
    return 0;
}
