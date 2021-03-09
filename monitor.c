#define _XOPEN_SOURCE 700
#define MAX_PROC 4
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
int test_value = 100;
int shmid;
int *shmptr;
char *log_name = "logfile"; // Default

void timer_func(){
	printf("Time ran out, exiting...\n");
	exit(0);
}

void kill_func(){
	shmdt(shmptr);
	shmctl(shmid, IPC_RMID, NULL);
}

void help_menu(){
	printf("Help menu goes here\n");
	exit(0);
}

void create_file(char *name){
	FILE *fp;
	char buf[50];
	strcpy(buf, name);
	strcat(buf, ".txt");
	fp = fopen(buf, "w");
	fclose(fp);
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
				help_menu();
				break;
			case 'o':
				opt_buf = optarg;
				log_name = opt_buf;
				create_file(log_name);
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
	
//*****************************************************************************************************
//
//	Generating Shared Memory
//
//*****************************************************************************************************


	key_t key = ftok("./README.md", 'a');
	shmid = shmget(key,  sizeof(int) * MAX_PROC, IPC_CREAT | 0666);

	if (shmid < 0)
	{
		errno = 5;
		perror("monitor: Error: Could not create shared memory");
		exit(0);
	}

	shmptr = (int *)shmat(shmid, 0, 0);

	if (shmptr == (int *) -1)
	{
		errno = 5;
		perror("monitor: Error: Could not attach shared memory");
		exit(0);
	}

	shmptr[0] = 1;
	
	printf("Forking now\n");
	int pid = fork();
	char *args[] = {"./consumer", NULL};
	if (pid == 0)
	{
		printf("In monitor child\n");
		int consume_pid = fork();
		if (consume_pid == 0)
			execvp(args[0], args);
	}
	printf("Out of for\n");
	kill_func();
	return 0;
}
