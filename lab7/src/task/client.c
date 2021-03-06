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

  write(1, "Input message to send: 1111\n", 28);
    if (write(fd, "1111\n", 5) < 0) {
      perror("write");
      exit(1);
    }


struct sockaddr_in new_servaddr;
char ip[9]; 
char c_port[5];

  memset(&new_servaddr, 0, SIZE);
  new_servaddr.sin_family = AF_INET;

  if ((nread = read(fd, buf, BUFSIZE)) > 0) {
      //write(1, &buf, nread);
      printf("Recieved: %s\n", buf);
      //strncpy(ip, buf, 15);
      for(int i = 0; i<15; i++)
      {
          if(i>9)
          {
              c_port[i-10] = buf[i];
          }
          if(i<9)
          {
              ip[i] = buf[i];
          }
      }
      char c_port_form[5];
      strncpy(c_port_form, c_port, 5);
      printf("IP: |%s|\n", ip);
      printf("Port: |%s|\n", c_port_form);
      new_servaddr.sin_port = htons(atoi(c_port_form));
      //write(1, ip, nread);
    }

//   if ((nread = read(fd, buf, BUFSIZE)) > 0) {
//       printf("Port: ");
//       write(1, &buf, nread);
//       servaddr.sin_port = htons(atoi(buf));
//       printf("\n");
//     }


//new socket

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creating");
    exit(1);
  }

  if (inet_pton(AF_INET, ip, &new_servaddr.sin_addr) <= 0) {
    perror("bad address");
    exit(1);
  }

    printf("--Point 0--\n");

  if (connect(fd, (SADDR *)&new_servaddr, SIZE) < 0) {
    perror("connect");
    exit(1);
  }


  write(1, "Input message to send\n", 22);
  while ((nread = read(0, buf, BUFSIZE)) > 0) {

    printf("--Point 1--\n");

    if (write(fd, buf, nread) < 0) {
      perror("write");
      exit(1);
    }

    printf("--Point 2--\n");
  }

  close(fd);
  exit(0);
}
