#include "display.h"

#include "vector.h"
#include "world.h"

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#define UNUSED(x) (void)(x)

#define PI 3.141593
#define SCALE 2
#define MAX_CIRCLE_PRECISION 32
#define STD_PRECISION 10

static SDL_Window* window;
static SDL_Renderer* renderer;

static int window_size_x = 0;
static int window_size_y = 0;

// SDL DISPLAY FUNCTIONS

void update_window_size();

float world_scale(float s);

int world_x(float x);

int world_y(float y);

void draw_circle(int x, int y, int radius, int precision);

void handle_key_down(world* world, SDL_Event* event);

void handle_key_up(world* world, SDL_Event* event);


// INTERFACE IMPLEMENTATION

void init_display(world* world){
    UNUSED(world);
    // returns zero on success else non-zero
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}
	window = SDL_CreateWindow("C-FLIPPER",
									SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED,
									320, 640, 0);
    if (window == NULL){
        fprintf(stderr, "Could not establish window");
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL){
        fprintf(stderr, "Could not establish renderer");
        return;
    }
}

void loop_start_display(world* world){
    update_window_size();
    UNUSED(world);
}

bool handle_events(world *world){
    UNUSED(world);
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        switch(event.type){
        case SDL_QUIT:
	return false;
        case SDL_KEYDOWN:
	        handle_key_down(world, &event);
            break;
        case SDL_KEYUP:
            handle_key_up(world, &event);
	        break;
         default:
	break;
      }
    }
     
    return true;
}

void draw_surface(surface *surface){
    SDL_RenderDrawLine(renderer, 
            world_x(surface->start.x), world_y(surface->start.y), 
            world_x(surface->end.x), world_y(surface->end.y));
}

void loop_end_display(world* world){
   
   SDL_RenderClear(renderer);
   SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Draw surfaces

    for (int i = 0; i < world->surface_amount; ++i){
       draw_surface(&world->surfaces[i]);
    }

    // Draw bouncers

    for (int i = 0; i < world->bouncer_amount; ++i){
        draw_circle(world_x(world->bouncers[i].position.x), world_y(world->bouncers[i].position.y), world->bouncers[i].radius, STD_PRECISION);
    }

    // Draw ball

    draw_circle(world_x(world->b->position.x), world_y(world->b->position.y), world->b->radius, STD_PRECISION);

    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
}

void loop_wait(int max_fps){
    SDL_Delay(1000 / max_fps);
}

void terminate_display(world* world){
    UNUSED(world);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


// SDL DISPLAY IMPLEMENTATION

void update_window_size(){
    SDL_GetWindowSize(window, &window_size_x, &window_size_y);
}

float world_scale(float s){
    return s * SCALE;
}

int world_x(float x){
    return (((int) x) * SCALE + window_size_x / 2);
}

int world_y(float y){
    return (- ((int) y)) * SCALE + window_size_y - 30;
}

void draw_circle(int x, int y, int radius, int precision){
    if (precision <= 2){
        return;
    }
    if (precision > MAX_CIRCLE_PRECISION - 1){
        precision = MAX_CIRCLE_PRECISION - 1;
    }
    radius = world_scale(radius);
    SDL_Point points_buffer[MAX_CIRCLE_PRECISION];
    int i;
    for (i = 0; i < precision; ++i){
        points_buffer[i].x = (cos(2 * PI / precision * i) * radius) + x;
        points_buffer[i].y = (sin(2 * PI / precision * i) * radius) + y;
    }
    points_buffer[i].x = radius + x;
    points_buffer[i].y = y;

    SDL_RenderDrawLines(renderer, points_buffer, precision + 1);
}


 void handle_key_down(world* world, SDL_Event* event){
    switch(event->key.keysym.sym){
    case SDLK_LEFT:
        world_activate_kickers(world, LEFT_SIDE);
        break;
    case SDLK_RIGHT:
        world_activate_kickers(world, RIGHT_SIDE);
        break;
   }
 }

 void handle_key_up(world* world, SDL_Event* event){
    switch(event->key.keysym.sym){
    case SDLK_LEFT:
        world_deactivate_kickers(world, LEFT_SIDE);
        break;
    case SDLK_RIGHT:
        world_deactivate_kickers(world, RIGHT_SIDE);
        break;
   }
 }
