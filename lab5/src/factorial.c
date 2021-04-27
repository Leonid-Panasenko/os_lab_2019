#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>

#include <pthread.h>

struct args
{
    int begin;
    int end;
};

int factorial(struct args *f_args)
{
    int result = 1;
    for(int i=f_args->begin; i<f_args->end; i++)
    {
        result *= i;
    }
    return result;
}

int main(int argc, char **argv)
{
    int k, pnum, mod;

    while (true) 
    {
        int current_optind = optind ? optind : 1;

        static struct option options[] = 
        {
            {"k", required_argument, 0, 0},
            {"pnum", required_argument, 0, 0},
            {"mod", required_argument, 0, 0},
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
                        k = atoi(optarg);
                        if (k <= 0) 
                        {
                            printf("seed must be a positive number\n");
                            return 1;
                        }
                        break;
	                case 1:
                        pnum = atoi(optarg);
                        if (pnum <= 0) 
                        {
                            printf("array_size must be a positive number\n");
                            return 1;
                        }
			            break;
                    case 2:
                        mod = atoi(optarg);
                        if (mod <= 0)
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

    printf("k: %d, pnum: %d, mod: %d \n", k, pnum, mod);

    int result = 1;
    pthread_t threads[pnum];
    struct args f_args[pnum];
    int part_size = k/pnum;

    for(int i=0; i<pnum; i++)
    {
        f_args[i].begin = i * part_size + 1;
        f_args[i].end = (i + 1) * part_size + 1;
        if(i == pnum - 1)
        {
            //printf("Last: %d\n", i);
            f_args[i].end = k + 1;
        }
        if (pthread_create(&threads[i], NULL, (void *)factorial,
                     (void *)&f_args[i]) != 0) {
        perror("pthread_create");
        exit(1);
        }
    }

    for (uint32_t i = 0; i < pnum; i++) 
    {
        void* res = 0;
        pthread_join(threads[i], &res);
        result *= (int)res;
        printf("Res: %d\n", (int)res);
    }

    result %= mod;

    printf("Result: %d\n", result);

    return 0;
}