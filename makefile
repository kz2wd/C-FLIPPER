CC = gcc
CFLAG = -std=c99 -Wall -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200112L -I .
LIBS = -lm -lncurses
SRCS = vector.c world.c ascii_display.c
OBJS = $(SRCS:.c=.o)


build: $(OBJS)
	$(CC) $(CFLAG) $^ main.c -o main $(LIBS)

run: build 
	./main

clean:
	rm -f *.o main

%.o : %.c
	$(CC) $(CFLAG) -c $^ -o $@ $(LIBS)
