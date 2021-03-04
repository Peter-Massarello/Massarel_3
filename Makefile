CC = gcc
CFLAGS = -std=c99 -g

all: monitor

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

monitor: monitor.o
	$(CC) $(CFLAGS) $< -o $@ -lm

clean:
	rm -f *.o monitor
