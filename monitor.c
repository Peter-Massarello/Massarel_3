#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>


extern int errno;

int producers = 2; // Default
int consumers = 6; // Deafult
int time = 100; // Default

void timer_func(){
	printf("Time ran out, exiting...\n");
	exit(0);
}

int main(int argc, char* argv[]){
	int opt;
	char *opt_buf;	


//*****************************************************************************************************
//
//	Signal Creation
//	
//*****************************************************************************************************

	signal(SIGALRM, timer_func);

//*****************************************************************************************************
//
//	Loop of getopt
//
//*****************************************************************************************************

	if (argc == 1)
	{
		system("clear");
		errno = 1;
		perror("monitor: Error: Program called without agruments, use ./monitor -h for help");
		return 0;
	}

	system("clear");
	while((opt = getopt(argc, argv, "ho:p:c:t:")) != -1)
	{
		switch(opt)
		{
			case 'h':
				printf("help menu here\n");
				break;
			case 'o':
				printf("name of logfile here\n");
				break;
			case 'p':
				opt_buf = optarg;
				producers = atoi(opt_buf);
				printf("number of producers given %d\n", producers);
				break;
			case 'c':
				opt_buf = optarg;
				consumers = atoi(opt_buf);
				printf("number of consumers given %d\n", consumers);
				break;
			case 't':
				opt_buf = optarg;
				time = atoi(opt_buf);
				printf("time given %d\n", time);
				alarm(time);
				break;
			default:
				errno = 1;
				perror("monitor: Error: Wrong option given");
				break;
		}
	}
	sleep(20);
	return 0;
}
