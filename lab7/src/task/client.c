#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

//#define BUFSIZE 100
#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)

//Usage: ./client IP Port Bufsize
int main(int argc, char *argv[]) {
    int BUFSIZE = atoi(argv[3]);
    printf("%d\n", BUFSIZE);
    
    //
    BUFSIZE = 100;
    //

  int fd;
  int nread;
  char buf[BUFSIZE];
  struct sockaddr_in servaddr;

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creating");
    exit(1);
  }

  memset(&servaddr, 0, SIZE);
  servaddr.sin_family = AF_INET;

  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
    perror("bad address");
    exit(1);
  }

  servaddr.sin_port = htons(atoi(argv[2]));

  if (connect(fd, (SADDR *)&servaddr, SIZE) < 0) {
    perror("connect");
    exit(1);
  }

  write(1, "Input message to send\n", 22);
    if (write(fd, "1111\n", 5) < 0) {
      perror("write");
      exit(1);
    }

struct sockaddr_in new_servaddr;
char* ip; 

  memset(&servaddr, 0, SIZE);
  servaddr.sin_family = AF_INET;

  if ((nread = read(fd, buf, BUFSIZE)) > 0) {
      write(1, &buf, nread);
      ip = buf;
      printf("\n");
    }

  if ((nread = read(fd, buf, BUFSIZE)) > 0) {
      write(1, &buf, nread);
      servaddr.sin_port = htons(atoi(buf));
      printf("\n");
    }


//new socket
  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creating");
    exit(1);
  }

  if (inet_pton(AF_INET, ip, &new_servaddr.sin_addr) <= 0) {
    perror("bad address");
    exit(1);
  }

  if (connect(fd, (SADDR *)&new_servaddr, SIZE) < 0) {
    perror("connect");
    exit(1);
  }

  write(1, "Input message to send\n", 22);
  while ((nread = read(0, buf, BUFSIZE)) > 0) {
    if (write(fd, buf, nread) < 0) {
      perror("write");
      exit(1);
    }
  }

  close(fd);
  exit(0);
}
