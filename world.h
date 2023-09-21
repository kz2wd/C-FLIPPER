#ifndef WORLD_H
#define WORLD_H

#include <vector.h>

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


float get_distance(ball* b, surface* s);

float get_bouncer_distance(ball* b, bouncer* bouncer);

bool check_collision(ball* b, surface* s);

void handle_collision(ball* b, surface* s);

void handle_bouncer_collision(ball* b, bouncer* bouncer);

#endif
