#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

//#define SERV_PORT 10050
//#define BUFSIZE 100
#define SADDR struct sockaddr


//Usage: ./server Port Bufsize
int main(int argc, char *argv[]) {
    int SERV_PORT = atoi(argv[1]);
    int BUFSIZE = atoi(argv[2]);

    //
    //SERV_PORT = 20001;
    BUFSIZE = 100;
    //

  const size_t kSize = sizeof(struct sockaddr_in);

  int lfd, cfd;
  int nread;
  char buf[BUFSIZE];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&servaddr, 0, kSize);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  if (bind(lfd, (SADDR *)&servaddr, kSize) < 0) {
    perror("bind");
    exit(1);
  }

  if (listen(lfd, 5) < 0) {
    perror("listen");
    exit(1);
  }

  while (1) {
    unsigned int clilen = kSize;
    int sent = 0;

    if ((cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
      perror("accept");
      exit(1);
    }
    printf("connection established\n");

    while ((nread = read(cfd, buf, BUFSIZE)) > 0) {
      write(1, &buf, nread);
      printf("Sendind a new address...\n");
      write(cfd, "127.0.0.1/20002", 15);
      
      //write(cfd, "20002", 5);
      sent = 1;
      printf("Sent\n");
      break;
    }

    if(sent == 1)
    {
        break;
    }

    if (nread == -1) {
      perror("read");
      exit(1);
    }
    close(cfd);
  }

    printf("--Point 1--\n");

  while (1) {
    unsigned int clilen = kSize;

    if ((cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
      perror("accept");
      exit(1);
    }
    printf("connection established\n");

    while ((nread = read(cfd, buf, BUFSIZE)) > 0) {
      write(1, &buf, nread);
      printf("--Point 2--\n");
    }

      printf("--Point 3--\n");

    if (nread == -1) {
      perror("read");
      exit(1);
    }
    close(cfd);
  }
}
