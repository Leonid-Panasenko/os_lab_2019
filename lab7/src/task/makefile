paCC=gcc
CFLAGS=-I.

all: task

task: client.c server.c
	$(CC) client.c -o client $(CFLAGS)
	$(CC) server.c -o server $(CFLAGS)

clean :
	rm client server