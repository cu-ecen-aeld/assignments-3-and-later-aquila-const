CC=$(CROSS_COMPILE)gcc

# ifeq ($(CROSS_COMPILE),)
# 	CFLAGS =-g -Wall
# else
# 	CFLAGS = aarch64-none-linux-gnu- -Wall -Wextra -pedantic
# endif

all:
	$(CC) -o writer writer.c

clean:
	rm -f *.o writer