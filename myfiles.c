#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

FILE *f1, *f2, *fout, *fin;
char *f1Name, *f2Name, *foutName;
char ch;

int main (int argc, char *argv[])
{

    if (argc < 2) {
        printf("Usage: ./myfiles file_in_1 file_in_2 [file_out (default:myfile.out)]\n");
        return(1);
    }

    else {
        f1 = argv[1];
        f2 = argv[2];
        fout = argv[3];
    }

    CopyFile(fileno(fout), f1);

    return 0;
}

void WriteInFile (int fd, const char *buff, int len)
{
    fout = fopen(fout ,"w");

    while ( (ch = fgetc(fd)) != EOF ) {
        fputc(ch, fout);
    }

    fclose (fout);
}

void CopyFile (int fd, const char *file_in)
{
    fout = fdopen(fd ,"w");
    fin = fopen(file_in ,"r");

    WriteInFile(fd, *file_in, 1);
}
