#include <stdio.h>

FILE *f1, *f2, *fout, *fin;
const char *f1Name, *f2Name, *foutName;
char ch;

void WriteInFile (int fd, const char *buff, int len)
{
    fout = fdopen(fd ,"w");
    while ( (ch = fgetc(fin)) != EOF ) {
        fputc(ch, fout);
    }
    fclose (fout);
}

void CopyFile (int fd, const char *file_in)
{
    fin = fopen(file_in ,"r");
    WriteInFile(fd, file_in, 1);
}

int main (int argc, char *argv[])
{

    if (argc < 2) {
        printf("Usage: ./myfiles file_in_1 file_in_2 [file_out (default:myfile.out)]\n");
        return(1);
    }

    else {
        f1Name = argv[1];
        f2Name = argv[2];
    }

    CopyFile(fileno(fout), f1Name);
    CopyFile(fileno(fout), f2Name);

    return 0;
}
