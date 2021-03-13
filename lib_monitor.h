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

void produce();
void consume();
void sem_wait(int sem);
void sem_signal(int sem);
