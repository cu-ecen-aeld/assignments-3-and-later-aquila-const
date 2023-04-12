CC=$(CROSS_COMPILE)gcc

ifeq ($(CROSS_COMPILE),)
	CFLAGS =-g -Wall
else
	CFLAGS = aarch64-none-linux-gnu- -Wall -Wextra -pedantic
endif

default: writer

writer.o: writer.c
	$(CC) $(CFLAGS) -c writer.c -o writer.o

writer: writer.o
	$(CC) $(CFLAGS) writer.o -o writer

clean:
	rm -f *.o writer