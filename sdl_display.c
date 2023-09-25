#include "display.h"

#include "vector.h"
#include "world.h"

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#define UNUSED(x) (void)(x)

// SDL DISPLAY FUNCTIONS



// INTERFACE IMPLEMENTATION

void init_display(world* world){
    UNUSED(world);
    // returns zero on success else non-zero
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}
	SDL_Window* win = SDL_CreateWindow("C-FLIPPER",
									SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED,
									1000, 1000, 0);
    (void)win; // Temporary warning suppressor
}

void loop_start_display(world* world){
    UNUSED(world);
}

bool handle_events(world *world){
    UNUSED(world);
    SDL_Event event;
 
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
    }
    return true;
}

void loop_end_display(world* world){
   UNUSED(world);
}

void loop_wait(int max_fps){
    SDL_Delay(1000 / max_fps);
}

void terminate_display(world* world){
    UNUSED(world);
    SDL_Quit();
}



// SDL DISPLAY IMPLEMENTATION
