paCC=gcc
CFLAGS=-I.

all: server client 

multyModulo.o: MultiModulo.h MultiModulo.c
	$(CC) MultiModulo.c -o multyModulo.o $(CFLAGS)

server : server.c multyModulo.o
	$(CC) -pthread server.c multyModulo.o -o server $(CFLAGS)

client : client.c multyModulo.o
	$(CC) -pthread client.c multyModulo.o -o client $(CFLAGS)

clean :
	rm server client