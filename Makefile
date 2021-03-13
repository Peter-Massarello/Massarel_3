CC = gcc
CFLAGS = -std=c99

all: monitor consumer producer

monitor: monitor.o lib_monitor.a
	$(CC) -L. -o $@ monitor.o -l_monitor $(CFLAGS)

consumer: consumer.o lib_monitor.a
	$(CC) -L. -o $@ consumer.o -l_monitor $(CFLAGS)

producer: producer.o lib_monitor.a
	$(CC) -L. -o $@ producer.o -l_monitor $(CFLAGS)

lib_monitor.o: lib_monitor.c
	$(CC) -c -O $^

consumer.o: consumer.c
	$(CC) -c -O $^

producer.o: producer.c
	$(CC) -c -O $^

monitor.o: monitor.c
	$(CC) -c -O $^ $(CFLAGS)

lib_monitor.a: lib_monitor.o
	ar rcs $@ $^

clean:
	rm -f *.o *.a *.txt monitor consumer producer
