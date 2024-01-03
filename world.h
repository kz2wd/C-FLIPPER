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
    bool is_infinite;
} surface;

typedef struct bouncer {
    vector position;
    float radius;
    // Minimal norm of the velocity given to the ball after collision
    float bounce_force;
} bouncer;

enum KICKER_SIDE {
  LEFT_SIDE,
  RIGHT_SIDE,
};

typedef struct kicker {
  surface *surface;
  float rotation_speed;
  float max_angle;
  float current_angle;
  bool activated;
  enum KICKER_SIDE side;
} kicker;

typedef struct world {
    ball* b;
    int surface_amount;
    surface* surfaces;
    int bouncer_amount;
    bouncer* bouncers;
    vector* gravity;
    float friction;
    int kicker_amount;
    kicker *kickers;
} world; 


void world_init(world* world);

void world_update(world* world, float delta_time);

void world_activate_kickers(world* world, enum KICKER_SIDE side);
void world_deactivate_kickers(world* world, enum KICKER_SIDE side);

#endif

