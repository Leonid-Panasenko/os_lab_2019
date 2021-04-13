#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>

#include <pthread.h>

#include "utils.h"

struct SumArgs {
  int *array;
  int begin;
  int end;
};

int Sum(const struct SumArgs *args) {
  int sum = 0;
  // TODO: your code here 
  for(int i = args->begin; i<args->end; i++)
  {
      sum += args->array[i];
      //printf("%d\n", args->array[i]);
  }

  return sum;
}

void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {
  /*
   *  TODO:
   *  threads_num by command line arguments
   *  array_size by command line arguments
   *	seed by command line arguments
   */
   printf("Usage: ./Parallel_sum seed array_size threads_num \n");

  uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0;
  pthread_t threads[threads_num];

  //
while (true) 
    {
        int current_optind = optind ? optind : 1;

        static struct option options[] = 
        {
            {"seed", required_argument, 0, 0},
            {"array_size", required_argument, 0, 0},
            {"threads_num", required_argument, 0, 0},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        int c = getopt_long(argc, argv, "f", options, &option_index);
        if (c == -1) break;

        switch (c) 
        {
            case 0:
                switch (option_index) 
                {
                    case 0:
                        seed = atoi(optarg);
                        if (seed <= 0) 
                        {
                            printf("seed must be a positive number\n");
                            return 1;
                        }
                        break;
	                case 1:
                        array_size = atoi(optarg);
                        if (array_size <= 0) 
                        {
                            printf("array_size must be a positive number\n");
                            return 1;
                        }
			            break;
                    case 2:
                        threads_num = atoi(optarg);
                        if (threads_num <= 0)
                        { 
                            printf("pnum must be a positive number\n");
                            return 1;
                        }
                        break;
                    defalut:
                        printf("Index %d is out of options\n", option_index);
                }
                break;
            case '?':
                break;
            default:
                printf("getopt returned character code 0%o?\n", c);
        }
    }
  //

  printf("Seed: %d, array_size: %d, threads_num: %d \n", seed, array_size, threads_num);
  /*
   * TODO:
   * your code here
   * Generate array here
   */

  int *array = malloc(sizeof(int) * array_size);
//
  GenerateArray(array, array_size, seed);
  for(int i = 0; i<array_size; i++)
  {
      printf("%d\n", array[i]);
  }
//
  struct SumArgs args[threads_num];
  for (uint32_t i = 0; i < threads_num; i++) {
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args)) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }

  free(array);
  printf("Total: %d\n", total_sum);
  return 0;
}
