#include <sys/stat.h>
#include <fcntl.h>    // close, open

#include <unistd.h>  // read, write

#include <stdio.h>   // printf
#include <stdlib.h>  // exit
#include <string.h>  // strlen


const int BUFFER_SIZE = 256;

const char *HELP_MESSAGE =
    "%s file_1 file_2 [file_n...] [file_out]\n"
    "  file_1, file_2, file_n: file paths to be copied\n";

char *ReadFileContents(int f)
/* Read contents from a file descriptor *f* into a heap allocated
 * string *buffer*
 */
{
    char c;
    int buffer_index = 0;

    // Allocate BUFFER_SIZE bytes on the heap
    char *buffer = malloc(BUFFER_SIZE * sizeof(char));

    // read returns 0 when there is no more to read
    // If the result is zero (false) break the loop
    while (read(f, &c, 1))
        buffer[buffer_index++] = c;

    buffer[buffer_index++] = '\0';

    return buffer;
}


void WriteInFile(int fd, const char *buff, int len)
/* Write data (of length *len*) in *buff* to file descriptor *fd* */
{
    for (int i = 0; i < len; i++)
        write(fd, &buff[i], 1);
}


void CopyFile(int fd, const char *file_in)
/* Write data in file named *file_in* to file descriptor *fd*.
 * If the requested file cannot be opened, then the function
 * simply returns
 */
{
    int fdin;

    fdin = open(file_in, O_RDONLY);

    if (fdin == -1) {
        printf("Could not open file '%s'\n", file_in);
        return;
    }

    char *cont;

    cont = ReadFileContents(fdin);

    close(fdin);

    WriteInFile(fd, cont, strlen(cont));
}


int main(int argc, char *argv[])
{
    // Guard for invalid input
    // Note: argv also includes the command name
    if (argc < 3) {
        printf(HELP_MESSAGE, argv[0]);
        return 0;
    }

    int fdout;
    char *fname;
    int arg_end;

    if (argc == 3) {
        fname = "myfile.out";  // The default file name
        arg_end = argc;        // Read all arguments
    } else {
        fname = argv[argc - 1];  // The last argument
        arg_end = argc - 1;      // Skip the last argument
    }

    //  Open this file as write only, and create it if neccesary
    // with read/write permissions
    fdout = open(fname, O_WRONLY | O_CREAT, 0666);

    if (fdout == -1) {
        printf("Error: Could not open file '%s'\n", fname);
        return 1;
    }

    for (int i = 1; i < arg_end; i++) {
        CopyFile(fdout, argv[i]);
        printf("Copying file '%s' to '%s'\n", argv[i], fname);
    }

    close(fdout);

    return 0;
}
