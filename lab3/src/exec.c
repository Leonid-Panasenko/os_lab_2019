#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main( int argc, char **argv ) {
	int pid = fork();

	if ( pid == 0 ) {
		execvp("./sequential", argv);
	}

	/* Put the parent to sleep for 2 seconds--let the child finished executing */
    int time = 2;
	wait(&time);

	printf( "Finished\n" );

	return 0;
}