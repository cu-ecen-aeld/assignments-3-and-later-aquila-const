CC=$(CROSS_COMPILE)gcc

CFILES = aesdsocket.c
SRCS = ${CFILES}
OBJS = ${CFILES:.c=.o}

CFLAGS += -Wall -Werror


all: aesdsocket

clean:
	rm -rf *.o aesdsocket

aesdsocket: $(OBJS)
		$(CC) $(OBJS) -o $(INCLUDES) aesdsocket
