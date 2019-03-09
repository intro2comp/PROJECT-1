#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int *integers, size, sharedmem;


int main(int argc, char *argv[]){
    struct timeval start,end;
    gettimeofday(&start,NULL);

    sharedmem = shmget(IPC_PRIVATE, 1000000*sizeof(int), 0666 | IPC_CREAT);

    integers = shmat(sharedmem, 0, 0);

    FILE *input = fopen(argv[1], "r");

    size = 0;
    while(fscanf(input, "%d\n", &integers[size++]) != EOF);

    int iteration = 0;
    int sum = 0;
    int min = INT_MAX;
    int max = INT_MIN;
    while(iteration < size){
        int readin = integers[iteration];
        if(readin < min) min = readin;
        if(readin > max) max = readin;
        sum += readin;
        iteration++;
    }


    FILE *out = fopen(argv[2], "w+");
    fprintf(out, "Hi I\'m process %d and my parent is %d.\n", getpid(), getppid());
    fprintf(out, "Min: %d\n", min);
    fprintf(out, "Max: %d\n", max);
    fprintf(out, "Sum: %d\n", sum);

    shmdt(integers);
    shmctl(sharedmem, IPC_RMID, NULL);

    gettimeofday(&end,NULL);
    printf("%ld microseconds\n", (end.tv_sec*100000 + end.tv_usec) - (start.tv_sec*100000 + start.tv_usec));

    return 0;
}
