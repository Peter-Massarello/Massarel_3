#define SIZE 4
#define NUM_SEMS 6
#define MUTEX 0
#define FREE_SPACE 1
#define IN_BUF 2
#define	CON_WAIT 3
#define FREE_PROC 4
#define NEXT_IN 4
#define NEXT_OUT 5
#define PROD_WORK 5
#define RUN_CON 6

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <time.h>
#include "lib_monitor.h"


key_t buf_key, sem_key,log_key;
int buf_id, sem_id, log_id;
int *buf_ptr;
char *log_ptr;

struct sembuf op;

void ending_func(){
	shmdt(buf_ptr);
	shmdt(log_ptr);
}

void produce(){
	srand(time(NULL));
	signal(SIGINT, ending_func);

	sem_wait(CON_WAIT);
	sem_signal(PROD_WORK);
	sem_wait(FREE_SPACE);
	sem_wait(MUTEX);

	buf_key = ftok("./README.md", 'a');
	buf_id = shmget(buf_key, sizeof(int) * SIZE, IPC_EXCL);

	if (buf_id < 0)
	{
		errno = 5;
		perror("produce: Error: Shared memory could not be found");
		ending_func();
		exit(0);
	}

	buf_ptr = (int *)shmat(buf_id, 0, 0);

	if (buf_ptr == (int *) -1)
	{
		errno - 5;
		perror("produce: Error: Shared memory could not be attached");
		ending_func();
		exit(0);
	}

	log_key = ftok(".", 'a');
	log_id = shmget(log_key, sizeof(char) * 50, IPC_EXCL);

	if (log_id < 0)
	{
		errno = 5;
		perror("produce: Error: Shared Memory could not be found");
		ending_func();
		exit(0);
	}

	log_ptr = (char *)shmat(log_id, 0, 0);

	if (log_ptr == (char*) -1)
	{
		errno = 5;
		perror("produce: Error: Shared Memory could not be attached");
		ending_func();
		exit(0);
	}
	
	int p;

	p = (rand() % 100);

	printf("Produced %d\n", p);

	sleep(1);

	ending_func();
	
	sem_wait(PROD_WORK);
	sem_signal(MUTEX);
	sem_signal(IN_BUF);
		
}

void consume(){
	srand(time(NULL));
	signal(SIGINT, ending_func);

	sem_wait(IN_BUF);
	sem_wait(MUTEX);

	buf_key = ftok("./README.md", 'a');
	buf_id = shmget(buf_key, sizeof(int) * SIZE, IPC_EXCL);

	if (buf_id < 0)
	{
		errno = 5;
		perror("consumer: Error: Shared memory could not be found");
		ending_func();
		exit(0);
	}

	buf_ptr = (int *)shmat(buf_id, 0, 0);

	if (buf_ptr == (int *) -1)
	{
		errno - 5;
		perror("consumer: Error: Shared memory could not be attached");
		ending_func();
		exit(0);
	}

	log_key = ftok(".", 'a');
	log_id = shmget(log_key, sizeof(char) * 50, IPC_EXCL);

	if (log_id < 0)
	{
		errno = 5;
		perror("consume: Error: Shared Memory could not be found");
		ending_func();
		exit(0);
	}

	log_ptr = (char *)shmat(log_id, 0, 0);

	if (log_ptr == (char*) -1)
	{
		errno = 5;
		perror("comsume: Error: Shared Memory could not be attached");
		ending_func();
		exit(0);
	}

	printf("Consuming\n");

	ending_func();

	sem_signal(MUTEX);
	sem_signal(FREE_SPACE);

}

void sem_wait(int sem) {
	sem_key = ftok("./Makefile", 'a');
	sem_id = semget(sem_key, NUM_SEMS, 0);

	op.sem_num = sem;
	op.sem_op = -1;
	op.sem_flg = 0;

	semop(sem_id, &op, 1);

}

void sem_signal(int sem) {
	sem_key = ftok("./Makefile", 'a');
	sem_id = semget(sem_key, NUM_SEMS, 0);
	
	op.sem_num = sem;
	op.sem_op = 1;
	op.sem_flg = 0;

	semop(sem_id, &op, 1);
}
