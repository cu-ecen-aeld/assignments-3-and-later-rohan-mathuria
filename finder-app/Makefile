CFLAGS := -Wall
CROSS_COMPILE ?=
CC := $(CROSS_COMPILE)gcc

all: writer

writer: writer.o
	$(CC) $(CFLAGS) -o writer writer.o

writer.o: writer.c
	$(CC) $(CFLAGS) -c writer.c -o writer.o

clean:
	rm writer *.o

