paCC=gcc
CFLAGS=-I.

all: tcp udp

tcp: tcpclient.c
	$(CC) tcpclient.c -o tcpclient $(CFLAGS)
	$(CC) tcpserver.c -o tcpserver $(CFLAGS)

udp: udpclient.c
	$(CC) udpclient.c -o udpclient $(CFLAGS)
	$(CC) udpserver.c -o udpserver $(CFLAGS)

clean :
	rm tcpclient tcpserver udpclient udpserver