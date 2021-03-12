#define SIZE 4
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


key_t buf_key;
int buf_id;
int *buf_ptr;

void produce(){
	srand(time(NULL));

	buf_key = ftok("./README.md", 'a');
	buf_id = shmget(buf_key, sizeof(int) * SIZE, IPC_EXCL);

	if (buf_id < 0)
	{
		errno = 5;
		perror("consumer: Error: Shared memory could not be found");
		exit(0);
	}

	buf_ptr = (int *)shmat(buf_id, 0, 0);

	if (buf_ptr == (int *) -1)
	{
		errno - 5;
		perror("consumer: Error: Shared memory could not be attached");
		exit(0);
	}
}

void consume(){
	srand(time(NULL));

	buf_key = ftok("./README.md", 'a');
	buf_id = shmget(buf_key, sizeof(int) * SIZE, IPC_EXCL);

	if (buf_id < 0)
	{
		errno = 5;
		perror("consumer: Error: Shared memory could not be found");
		exit(0);
	}

	buf_ptr = (int *)shmat(buf_id, 0, 0);

	if (buf_ptr == (int *) -1)
	{
		errno - 5;
		perror("consumer: Error: Shared memory could not be attached");
		exit(0);
	}

}

void sem_wait(int sem) {


}

void sem_signal(int sem) {


}
