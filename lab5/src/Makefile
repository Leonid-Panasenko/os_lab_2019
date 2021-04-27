paCC=gcc
CFLAGS=-I.

all: mutex factorial deadlock

mutex : 
	$(CC) -pthread mutex.c -o mutex $(CFLAGS)

factorial : factorial.c
	$(CC) -pthread factorial.c -o factorial $(CFLAGS)
	
deadlock : deadlock.c
	$(CC) -pthread deadlock.c -o deadlock $(CFLAGS)

clean :
	rm mutex mutex_no factorial deadlock