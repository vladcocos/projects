CC = gcc
CFLAGS = -Wall
PIC = -fPIC
LDFLAGS = -L.

build: tema1

tema1: main.o priority_queue.o libcompare.so
	$(CC) $(CFLAGS) -o tema1 main.o priority_queue.o -lcompare $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) $(PIC) -c main.c

priority_queue.o: priority_queue.c
	$(CC) $(CFLAGS) $(PIC) -c priority_queue.c

clean:
	rm -f *.o tema1
