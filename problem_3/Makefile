CC = gcc
CFLAGS = -std=c99 -g -Wall

myfiles:
	$(CC) $(CFLAGS) -o pT processTree.c

all: pT

test: pT
	./pT

clean: clean-test
	rm -vf pT

clean-test:
	rm -vf *.out