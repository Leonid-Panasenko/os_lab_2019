#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t child_pid = fork();
    if (child_pid >= 0) 
    {
            if (child_pid == 0) 
            {
                printf("Done \n");
                exit(0);
            }
    } 
    else 
    {
            printf("Fork failed!\n");
            return 1;
    }
}