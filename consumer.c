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
#include "lib_monitor.h"

void kill_func(){
	exit(0);
}

int main(int argc, char* argv[]){

	signal(SIGKILL, kill_func);

	printf("Here from consumer\n");
	return 0;
}
