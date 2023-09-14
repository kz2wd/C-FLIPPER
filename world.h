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


float get_distance(ball* b, surface* s);

bool check_collision(ball* b, surface* s);

void handle_collsion(ball* b, surface* s);

#endif
