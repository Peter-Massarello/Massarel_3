CC = gcc
CFLAGS = -std=c99 -g

all: monitor consumer producer

%.o: %.c
	$(CC) $(CFLAGS) -I . -c $< -o $@

monitor: monitor.o
	$(CC) $(CFLAGS) $< -o $@ -lm

consumer: consumer.o
	$(CC) $(CFLAGS) $< -o $@ 

producer: producer.o
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o *.txt monitor consumer producer
