# SEMAPHORES AND MESSAGE PASSING
-------------------------------------------------------------------------------

# GOAL OF PROJECT
-------------------------------------------------------------------------------

The goal of this project is to become familiar with semaphoes in Linux, and 
implimenting them with the concept of a monitor in C.

-------------------------------------------------------------------------------

# BUGS AND NOTES
-------------------------------------------------------------------------------

- The main bug I found was that the program never fully quits by itself. Even
after all the consumers have consumed and died, the program just halts. I will
try and fix this but as of right now, once the consumers are all dead, you have
to use the ctrl C function to quit.

- I used a source to help with the semaphore creation, code from there is pretty
much replicated in my source code.
Link to website - www.cs.kent.edu/~ruttan/sysprog/lectures/shmem/sem-producer-consumer.c 

-------------------------------------------------------------------------------

# FUNCTIONS USED
-------------------------------------------------------------------------------

IN MONITOR.C

kill_child():

	- Goes through the entire pid_list and uses the SIGKILL command to
	kill each of the currently alive children.

kill_mem():

	- Acts as the function which detatches and removes all shared memory.

end_func():

	- Acts as the end program function. Calls kill_child(), kill_mem(),
	and frees the pid_list.

signal_handler():

	- Called when a signal is caught and proceeds to call end_func().
	
help_menu():

	- Displays the help menu then exits the program.

create_file():

	- Creates the specified log file with the given name from the user.
	It as well creates the shared memory for the file name so that
	other programs can access and write to that created file.

init_sems():

	- Intitalizes all semaphores with semctl.

remove_child():

	- Given a pid, then cycles through the list until it find that 
	specfic pid and sets it to 0.

child_handler():

	- Waits till a pid requests to die and calls
	remove_child() to deleted that specified child from the pid_list.

find_empty():

	- Finds an empty spot in the pid list, and returns its
	index.

init_pids():

	- Initializes all pids in pid_list to 0.

-------------------------------------------------------------------------------

IN PRODUCER.C AND CONSUMER.C

kill_func():

	- Exits out of program on ctrl C command.

-------------------------------------------------------------------------------

IN LIB_MONITOR.C

ending_func():

	- Detaches all shared memory.

produce():

	- Produces a random number and, while in critical section: places that
	number into a buffer, logs time on to a file, and sleeps.

consume():

	- Consumes a number from the buffer, while in critical section: removes
	number from buffer, increments bounded buffer, logs time on to a file,
	and sleeps.

sem_wait():

	- Created functioning semaphore wait function using sys/sem.h operations.

sem_signal():

	- Created functioning semaphor signal function using sys/sem.h operations.

-------------------------------------------------------------------------------






