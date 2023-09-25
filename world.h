#ifndef WORLD_H
#define WORLD_H

#include "vector.h"

#include <stdbool.h>

typedef struct ball {
    vector position;
    vector speed;
    float radius;
} ball;

typedef struct surface {
    vector start;
    vector end;
    vector normal;
} surface;

typedef struct bouncer {
    vector position;
    float radius;
    // Minimal norm of the velocity given to the ball after collision
    float bounce_force;
} bouncer;

typedef struct world {
    ball* b;
    int surface_amount;
    surface* surfaces;
    int bouncer_amount;
    bouncer* bouncers;
    vector* gravity;
    float friction;
} world; 


void world_init(world* world);

void world_update(world* world, float delta_time);

#endif
