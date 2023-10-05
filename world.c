#include "world.h"

#include <math.h>
#include <stdbool.h>

// World internal functions declaration

float get_distance(ball* b, surface* s);

float get_bouncer_distance(ball* b, bouncer* bouncer);

bool check_collision(ball* b, surface* s);

void handle_collision(ball* b, surface* s);

void handle_bouncer_collision(ball* b, bouncer* bouncer);


// World functions implementations

void world_init(world* world){
    // Makes sure that normals of surfaces have norms of size 1.
      for (int i = 0; i < world->surface_amount; ++i){
        world->surfaces[i].normal = normalized_vector(&world->surfaces[i].normal);
    }
}

void world_update(world* world, float delta_time){
    world->b->speed = add_vectors(&world->b->speed, world->gravity);
    world->b->speed = scaled_vector(&world->b->speed, world->friction);
    vector movement = scaled_vector(&world->b->speed, delta_time);
    world->b->position = add_vectors(&world->b->position, &movement);

    for (int i = 0; i < world->surface_amount; ++i){
        handle_collision(world->b, &world->surfaces[i]);
    }
    for (int i = 0; i < world->bouncer_amount; ++i){
        handle_bouncer_collision(world->b, &world->bouncers[i]);
    }
}


// World internal functions implementations

float get_distance(ball* b, surface* s){
    float distance = 
        distance_point_and_line(&b->position, &s->start, &s->end);
    return distance;
}

float get_bouncer_distance(ball* b, bouncer* bouncer){
    return vectors_distance(&b->position, &bouncer->position);
}


bool check_collision(ball* b, surface* s){
    return get_distance(b, s) <= b->radius;
}


void handle_collision(ball* b, surface* s){
    float distance = get_distance(b, s);
    
    // In case of collision
    if (distance <= b->radius){
        // Move the ball out of the surface
        float out_epsilon = 0.1f; // extra displacement to prevent collision detection for the next step
        vector out_of_surface = scaled_vector(&s->normal, b->radius - distance + out_epsilon);
        b->position = add_vectors(&out_of_surface, &b->position);

        // Change the speed of the ball
        vector v = { -s->normal.y, s->normal.x };
        vector v1 = scaled_vector(&s->normal, dot_product(&s->normal, &b->speed));
        vector v2 = scaled_vector(&v, -dot_product(&v, &b->speed));
        vector new_speed = add_vectors(&v1, &v2);
        b->speed = scaled_vector(&new_speed, -1.f);
    }
}

void handle_bouncer_collision(ball* b, bouncer* bouncer){
    float distance = get_bouncer_distance(b, bouncer);
    
    // In case of collision
    if (distance <= b->radius + bouncer->radius){
        // Move the ball out of the surface
        float out_epsilon = 0.1f; // extra displacement to prevent collision detection for the next step
        vector normal = substract_vectors(&b->position, &bouncer->position);
        normal = normalized_vector(&normal);
        vector out_of_surface = scaled_vector(&normal, b->radius + bouncer->radius - distance + out_epsilon);
        b->position = add_vectors(&out_of_surface, &b->position);

        // Change the speed of the ball
        
        b->speed = scaled_vector(&normal, fmaxf(bouncer->bounce_force, vector_norm(&b->speed)));
    }
}

void world_activate_kickers(world* world, enum KICKER_TYPE type){
  // TODO
}
