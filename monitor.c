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
#include <time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include "lib_monitor.h"


extern int errno;

pid_t *pid_list;

int producers = 2; // Default
int consumers = 6; // Deafult
int alarm_time = 100; // Default
int test_value = 100;
int shmid, log_id, sem_id;
int *shmptr;
char *log_ptr;
char *log_name = "logfile"; // Default

void kill_child(){
	for (int i = 0; i < 19; i++)
	{
		if (pid_list[i] != 0)
			kill(pid_list[i], SIGKILL);
	}
}

void kill_mem(){
	shmdt(shmptr);
	shmdt(log_ptr);

	shmctl(shmid, IPC_RMID, NULL);
	shmctl(log_id, IPC_RMID, NULL);
	semctl(sem_id, 0, IPC_RMID, NULL);

	kill_child();
}

void signal_handler(){
	kill_mem();
}

void end_func(){
	signal_handler();
}

void help_menu(){
	printf("HELP MENU\n\n");
	printf("Program wil take in four arguments, filename, consumers, producers and time\n");
	printf("Defaults are logfile.txt, 2 producers, 6 consumers, and 100 seconds\n");
	printf("PROGRAM OPTIONS:\n\n");
	printf("OPTION [-o]:\n");
	printf("           When called, will overwrite the default logfile with given logfile name\n");
	printf("           If given filename is longer than 20, will default to logfile.txt\n");
	printf("           EX: ./montior -o newfilename\n\n");
	printf("OPTION [-c]:\n");
	printf("           When called, will overwrite the default value of consumers with given value\n");
	printf("           EX: ./montior -c 20\n\n");
	printf("OPTION [-p]:\n");
	printf("           When called, will overwrite the default value of producers with given value\n");
	printf("           EX: ./monitor -p 20\n\n");
	printf("OPTION [-t]:\n");
	printf("           When called, will overwtie the default value of time with given value\n");
	printf("           EX: ./monitor -t 20\n\n");
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

void init_sems(){
	semctl(sem_id, MUTEX, SETVAL, 1);
	semctl(sem_id, FREE_SPACE, SETVAL, 4);
	semctl(sem_id, IN_BUF, SETVAL, 0);
	semctl(sem_id, CON_WAIT, SETVAL, consumers);
	semctl(sem_id, FREE_PROC, SETVAL, 19);
}
	
void remove_child(pid_t  pid){
	for (int i = 0; i < 19; i++)
	{
		if (pid_list[i] = pid)
		{
			pid_list[i] = 0;
			break;
		}
	}
}

void child_handler(int sig){
	pid_t pid;
	while ((pid = waitpid((pid_t)(-1), 0, WNOHANG)) > 0)
	{
		remove_child(pid);
	}
}

int find_empty(){
	for (int i = 0; i < 19; i++)
	{
		if (pid_list[i] == 0)
			return i;
	}
}

void init_pids(){
	for (int i = 0; i < 19; i++)
	{
		pid_list[i] = 0;		
	}
}

int main(int argc, char* argv[]){

	int opt;
	char *opt_buf;	


//*****************************************************************************************************
//
//	Signal Creation
//	
//*****************************************************************************************************

	signal(SIGALRM, signal_handler);
	signal(SIGKILL, signal_handler);
	
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = child_handler;
	sigaction(SIGCHLD, &sa, NULL);

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
				if (strlen(opt_buf) > 20)
				{
					printf("File name given is too long, defaulting to logfile.txt\n");
					create_file(log_name);
				}
				else
				{
					log_name = opt_buf;
					create_file(log_name);
				}
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
				alarm_time = atoi(opt_buf);
				printf("time given %d\n", alarm_time);
				alarm(alarm_time);
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
 	printf("Out of getopt\n");

	pid_list = malloc(sizeof(pid_t) * 19);
	init_pids();

	key_t key = ftok("./README.md", 'a');
	shmid = shmget(key,  sizeof(int) * MAX_PROC, IPC_CREAT | 0666);

	if (shmid < 0)
	{
		errno = 5;
		perror("monitor: Error: Could not create shared memory");
		end_func();
		exit(0);
	}

	shmptr = (int *)shmat(shmid, 0, 0);

	if (shmptr == (int *) -1)
	{
		errno = 5;
		perror("monitor: Error: Could not attach shared memory");
		end_func();
		exit(0);
	}

	shmptr[NEXT_IN] = 0;
	shmptr[NEXT_OUT] = 0;

	key_t sem_key = ftok("./Makefile", 'a');
	sem_id = semget(sem_key, NUM_SEMS, IPC_CREAT | 0666);

	init_sems();

	if (sem_id < 0)
	{
		errno = 5;
		perror("monitor: Error: Could not create shared memory");
		end_func();
		exit(0);
	}

	key_t log_key = ftok(".", 'a');
	log_id = shmget(log_key, sizeof(char) * 50, IPC_CREAT | 0666);

	if (log_id < 0)
	{
		errno = 5;
		perror("monitor: Error: Could not create shared memory");
		end_func();
		exit(0);
	}
	
	log_ptr = (char *)shmat(log_id, 0, 0);

	if (log_ptr == (char *) -1)
	{
		errno = 5;
		perror("monitor: Error: Could not attach shared memory");
		end_func();
		exit(0);
	}

	printf("Forking now\n");

	int empty, i;
	
	for (int p = 0; p < producers; p++)
	{
		sem_wait(FREE_PROC);
		empty = find_empty();
		pid_list[empty] = fork();

		if (pid_list[empty] == 0)
			execl("./producer", "./producer", (char *) 0);
	}

	for (int c = 0; c < consumers; c++)
	{
		sem_wait(FREE_PROC);
		empty = find_empty();
		pid_list[empty] = fork();

		if (pid_list[empty] == 0)
			execl("./consumer", "./consumer", (char *) 0);
	}

	while(1)
	{
		if (semctl(sem_id, RUN_CON, GETVAL, NULL) == 0)
			break;
	}

	end_func();
	return 0;
}
