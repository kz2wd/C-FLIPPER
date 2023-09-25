CC = gcc
CFLAG = `sdl2-config --libs --cflags` -std=c99 -Wall -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200112L
LIBS = -lm -lncurses
SDLFLAGS = $(shell pkg-config --cflags --libs sdl2)
SRCS = vector.c world.c ascii_display.c
OBJS = $(SRCS:.c=.o)


build: $(OBJS)
	$(CC) $(CFLAG) $^ main.c -o main $(LIBS) 

run: build 
	./main

clean:
	rm -f *.o main

%.o : %.c
	$(CC) $(CFLAG) -c $^ -o $@ $(LIBS) $(SDLFLAGS)
