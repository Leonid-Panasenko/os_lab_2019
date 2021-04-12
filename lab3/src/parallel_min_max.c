#include <ctype.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"
     
pid_t p_to_kill = 0;
void Kill_ps()
{
    //if(p_to_kill != 0)
    //{
        kill(p_to_kill, SIGKILL);
        printf("Killed %d\n", p_to_kill);
    //}
}

int main(int argc, char **argv) 
{
    int seed = -1;
    int array_size = -1;
    int pnum = -1;
    bool with_files = true;
    ////
    int timeout = 0;
    ////

    while (true) 
    {
        int current_optind = optind ? optind : 1;

        static struct option options[] = 
        {
            {"seed", required_argument, 0, 0},
            {"array_size", required_argument, 0, 0},
            {"pnum", required_argument, 0, 0},
            ////
            {"timeout", required_argument, 0, 0},
            ////
            {"by_files", no_argument, 0, 'f'},
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
                        pnum = atoi(optarg);
                        printf("pnum = %d \n", pnum);
                        if (pnum <= 0)
                        { 
                            printf("pnum must be a positive number\n");
                            return 1;
                        }
                        break;
                    //////
                    case 3:
                        timeout = atoi(optarg);
                        printf("timeout = %d \n", timeout);
                        break;
                    //////
                    case 4:
                        with_files = true;
                        break;
                    defalut:
                        printf("Index %d is out of options\n", option_index);
                }
                break;
            case 'f':  
                with_files = true;
                break;
            case '?':
                break;
            default:
                printf("getopt returned character code 0%o?\n", c);
        }
    }
  
    if (optind < argc) 
    {
        printf("Has at least one no option argument\n");
        return 1;
    }

    if (seed == -1 || array_size == -1 || pnum == -1) 
    {
        printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n", argv[0]);
        return 1;
    }

    int *array = malloc(sizeof(int) * array_size);
    GenerateArray(array, array_size, seed);
    int active_child_processes = 0;
    
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    
    int fds[2];
    pipe(fds);
    int fdlen = array_size / pnum;
    
    for (int i = 0; i < pnum; i++) 
    {
        pid_t child_pid = fork();
        if (child_pid >= 0) 
        {
            ////////
            if(timeout != 0)
            {
                p_to_kill = child_pid;
                signal(SIGALRM, Kill_ps);
                alarm(timeout);
                printf("Alarm started \n");
            }
            ////////
            active_child_processes += 1;
            if (child_pid == 0) 
            {
                struct MinMax minmax;
                if (i != pnum - 1) minmax = GetMinMax(array, i * fdlen, (i + 1) * fdlen);
                else minmax = GetMinMax(array, i * fdlen, array_size);
                if (with_files) 
                {
                    FILE* file;
                    char* str = (char*)malloc(15*sizeof(char));
                    sprintf(str, "file_%d.txt", i);
                    file = fopen (str, "a");
                    if (file == 0)
                    {
                        printf("Couldn't open file to write \n");
                        return 1;
                    }
                    else
                    {
                        fwrite(&minmax, sizeof(struct MinMax), 1, file);
                    }
                    fclose(file);
                    ////test
                    while(true)
                    {}
                    ////
                }
                else
                {
                    close(fds[0]);
                    write(fds[1], &minmax, sizeof(struct MinMax));
                }
                //p_to_kill = 0;
                return 0;
            }
        } 
        else 
        {
            printf("Fork failed!\n");
            return 1;
        }
    }
    while (active_child_processes > 0) 
    {
        active_child_processes -= 1;
        wait(NULL);
        close(fds[1]);
    }
    struct MinMax min_max;
    min_max.min = INT_MAX;
    min_max.max = INT_MIN;

    for (int i = 0; i < pnum; i++) 
    {
        struct MinMax curr;
        curr.min = INT_MAX;
        curr.max = INT_MIN;
        if (with_files) 
        {
            char* str = (char*)malloc(15*sizeof(char));
            sprintf(str, "file_%d.txt", i);
            FILE * file = fopen(str,"rt");
            if (file == 0)
            {
                printf("Couldn't open file to read \n");
                return 1;
            }
            else
            {
                fread(&curr, sizeof(struct MinMax), 1, file);     
            }
            fclose(file);
            remove(str);
        } 
        else 
        {
            close(fds[1]);
            read(fds[0], &curr, sizeof(struct MinMax));
        }
        if (curr.min < min_max.min) min_max.min = curr.min;
        if (curr.max > min_max.max) min_max.max = curr.max;
    }
    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);
    
    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;
    
    for (int i = 0; i < array_size; i++)
    {
        printf("%d \n", array[i]);
    }
    printf("\n");
    
    free(array);
    
    printf("Min: %d\n", min_max.min);
    printf("Max: %d\n", min_max.max);
    printf("Elapsed time: %fms\n", elapsed_time);
    fflush(NULL);
    return 0;
}