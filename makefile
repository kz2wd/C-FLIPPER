CC = gcc
CFLAG = --std=c99 -Wall -Wextra
LIBS = -lm 
DISPLAY_TYPE ?= SDL
SRCS = vector.c world.c 

SDL_DISPLAY = sdl_display.c
SDL_LIBS = $(shell pkg-config --cflags --libs sdl2) 

ASCII_DISPLAY = ascii_display.c
ASCII_CFLAG = -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200112L
ASCII_LIBS = -lncurses


OBJS = $(SRCS:.c=.o)

.PHONY: clean build

build: $(OBJS)
ifeq (SDL, $(DISPLAY_TYPE))
	@echo 'Using SDL display'
	@make build-standard LIBS='$(LIBS) $(SDL_LIBS)' SRCS='$(SRCS) $(SDL_DISPLAY)'
else
	@echo 'Using ASCII display'
	@make build-standard LIBS='$(LIBS) $(ASCII_LIBS)' SRCS='$(SRCS) $(ASCII_DISPLAY)' CFLAG='$(CFLAG) $(ASCII_CFLAG)'
endif

build-standard: $(OBJS)
	$(CC) $(CFLAG) $^ main.c -o main $(LIBS)


run: build 
	./main

build-ascii: 
	@make build DISPLAY_TYPE=ASCII

run-ascii: 
	@make run DISPLAY_TYPE=ASCII


clean:
	rm -f *.o main

%.o : %.c
	$(CC) $(CFLAG) -c $^ -o $@ $(LIBS) 



	

