CC=gcc
CFLAGS=-I.

all: zombi process_memory parallel_sum

zombi : 
	$(CC) -o zombi zombi.c $(CFLAGS)

process_memory :
	$(CC) -o process_memory process_memory.c $(CFLAGS)

parallel_sum : utils.o parallel_sum.c utils.c
	$(CC) -pthread -o parallel_sum utils.o parallel_sum.c $(CFLAGS)

utils.o : utils.h utils.c
	$(CC) -o utils.o -c utils.c $(CFLAGS)

clean :
	rm zombi process_memory parallel_sum utils.o