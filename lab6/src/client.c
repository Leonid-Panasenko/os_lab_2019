#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
uint64_t global_res = 1;
uint64_t k = -1;
uint64_t mod = -1;

typedef struct{
  char ip[255];
  int port;
  uint64_t begin;
  uint64_t end;
} Server;

bool ConvertStringToUI64(const char *str, uint64_t *val) {
  char *end = NULL;
  unsigned long long i = strtoull(str, &end, 10);
  if (errno == ERANGE) {
    fprintf(stderr, "Out of uint64_t range: %s\n", str);
    return false;
  }

  if (errno != 0)
    return false;

  *val = i;
  return true;
}

void ListenerHost(void* To) 
{
    pthread_mutex_lock(&mut);
    Server* arg = (Server*)To;
    struct hostent *hostname = gethostbyname( arg->ip);
    if (hostname == NULL) {
      fprintf(stderr, "gethostbyname failed with %s\n",  arg->ip);
      exit(1);
    }


    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons( arg->port);
    server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

    int sck = socket(AF_INET, SOCK_STREAM, 0);
    if (sck < 0) {
      fprintf(stderr, "Socket creation failed!\n");
      exit(1);
    }

    if (connect(sck, (struct sockaddr *)&server, sizeof(server)) < 0) {
      fprintf(stderr, "Connection failed\n");
      exit(1);
    }
    
    printf("%lu\n", arg->begin);
    printf("%lu\n", arg->end);
    printf("%lu\n", mod);
    char task[sizeof(uint64_t) * 3];
    memcpy(task, &(arg->begin), sizeof(uint64_t));
    memcpy(task + sizeof(uint64_t), &(arg->end), sizeof(uint64_t));
    memcpy(task + 2 * sizeof(uint64_t), &mod, sizeof(uint64_t));

    printf("%s\n", task);
    if (send(sck, task, sizeof(task), 0) < 0) {
      fprintf(stderr, "Send failed\n");
      exit(1);
    }
    char response[sizeof(uint64_t)];
    if (recv(sck, response, sizeof(response), 0) < 0) {
      fprintf(stderr, "Recieve failed\n");
      exit(1);
    }
    
    uint64_t answer = 0;
    memcpy(&answer, response, sizeof(uint64_t));
	close(sck);
	global_res = (global_res*answer)%mod;
	pthread_mutex_unlock(&mut);
}

int main(int argc, char **argv) {
  char servers[255] = {'\0'}; // TODO: explain why 255
  FILE* file;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"servers", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        ConvertStringToUI64(optarg, &k);
        // TODO: your code here
        if (k <= 0) 
        {
            printf("k is a positive number\n");
            return 1;
        }
        break;
      case 1:
        ConvertStringToUI64(optarg, &mod);
        // TODO: your code here
        if (mod <= 0) 
        {
            printf("mod is a positive number\n");
            return 1;
        }
        break;
      case 2:
        // TODO: your code here
        memcpy(servers, optarg, strlen(optarg));
        if ((file = fopen(servers, "r")) == NULL) 
        {
			printf("Error with file :%s \n", servers);
			exit(1);
		}
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (k == -1 || mod == -1 || !strlen(servers)) {
    fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n",
            argv[0]);
    return 1;
  }

  // TODO: for one server here, rewrite with servers from file
  file = fopen(servers, "r");
  char str[255];
  unsigned int servers_num = 0;
  while(fscanf(file, "%s", str) != EOF)
  {
      servers_num++;
  }
  fclose(file);
  printf("%d\n", servers_num);
  Server* to = malloc(sizeof(Server) * servers_num);
  int part = k/servers_num;
  // TODO: delete this and parallel work between servers
//   to[0].port = 20001;
//   memcpy(to[0].ip, "127.0.0.1", sizeof("127.0.0.1"));
file = fopen(servers, "r");
  for(int i = 0; i < servers_num; i++)
  {
      fscanf(file, "%s", str);
      char* ip = strtok(str, ":");
      memcpy(to[i].ip, ip, sizeof(ip)+1);
      to[i].port = atoi(strtok(NULL, ":"));

      printf("%s %d\n", to[i].ip, to[i].port);

      to[i].begin = i * part + 1;
      to[i].end = (i + 1)*part + 1;
	  if (i == servers_num - 1)
      {
			to[i].end = k+1;
	  }
  }
  fclose(file);

    pthread_t* threads = malloc(sizeof(pthread_t) * servers_num);
    
    for (int i = 0; i < servers_num; i++)
    {
        if (pthread_create(&threads[i], NULL, (void *)ListenerHost, (void *) &to[i]) != 0)
		{
		    perror("pthread_create");
		    exit(1);
		}
    }
    
    for (size_t i = 0; i < servers_num; i++)
	{
		if (pthread_join(threads[i], NULL) != 0)
        {
			perror("pthread_join");
			exit(1);
		}
	}
    
    free(threads);
    free(to);
	
    printf("Result: %lu\n", global_res);

    return 0;
}
