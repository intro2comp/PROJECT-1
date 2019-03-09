#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid, pid1;

    pid = fork();

    if (pid == 0) {
        pid1 = fork();

        if (pid1 == 0) {
            sleep(5);
            printf("D\n");
            return 10;
        }

        else {
            wait(NULL);
            printf("B\n");
            return 4;
        }
    }

    else {
        wait(NULL);
        printf("C\n");
        return 6;
    }
}
