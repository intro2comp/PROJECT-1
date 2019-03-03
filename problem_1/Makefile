CC = gcc
CFLAGS = -std=c99 -g -Wall

myfiles:
	$(CC) $(CFLAGS) -o myfiles myfiles.c

all: myfiles

test: myfiles
	./myfiles
	./myfiles file_1.txt
	./myfiles file_1.txt file_2.txt
	./myfiles file_1.txt file_2.txt myfile_2.out
	./myfiles file_1.txt file_2.txt file_3.txt file_4.txt myfile_3.out
	./myfiles file_1.txt bad.txt myfile_4.out

clean: clean-test
	rm -vf myfiles

clean-test:
	rm -vf *.out
