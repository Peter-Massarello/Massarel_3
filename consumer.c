#define _XOPEN_SOURCE 700
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

int sem_id, sleep_count;

void kill_func() {
	exit(0);
}

int main(int argc, char* argv[]) {
	srand(time(NULL));

	signal(SIGINT, kill_func);

	while (1)
	{
		consume();
		sleep_count = (rand() % 11) + 1; // 1-10
		sleep(sleep_count);
	}
	return 0;
}
