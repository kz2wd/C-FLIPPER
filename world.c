#include "world.h"

#include <math.h>
#include <stdbool.h>
#include <assert.h>

#define HALF_PI 1.57079632679 
// World internal functions declaration

float get_distance(ball* b, surface* s);

float get_bouncer_distance(ball* b, bouncer* bouncer);

bool check_collision(ball* b, surface* s);

void handle_collision(ball* b, surface* s);

void handle_bouncer_collision(ball* b, bouncer* bouncer);


void surface_update_normal(surface* surface){
    vector surface_vector = substract_vectors(&surface->end, &surface->start);
    vector rotated_surface_vector = rotated_vector(&surface_vector, HALF_PI);
    vector new_normal = normalized_vector(&rotated_surface_vector);
    copy_vector(&surface->normal, &new_normal);
}

enum ROTATION_POINT {
  SURFACE_START,
  SURFACE_END,
};

float kicker_side_to_rotation_direction(enum KICKER_SIDE side){
    switch (side)
    {
        case LEFT_SIDE:
            return 1.f;    
        default:
            return -1.f;
    }
}

enum ROTATION_POINT kicker_side_to_rotation_point(enum KICKER_SIDE side){
    switch (side)
    {
        case LEFT_SIDE:
            return SURFACE_START;    
        default:
            return SURFACE_END;
    }
}

void surface_rotate(surface* surface, float theta, enum ROTATION_POINT rotation_point){
    vector *rotation_center = (rotation_point == SURFACE_END) ? &surface->end : &surface->start;
    vector *target = (rotation_point == SURFACE_END) ? &surface->start : &surface->end;

    vector shifted_target = substract_vectors(target, rotation_center);
    vector rotated_shifted_target = rotated_vector(&shifted_target, theta);
    vector rotated_target = add_vectors(&rotated_shifted_target, rotation_center);
    copy_vector(target, &rotated_target);

    surface_update_normal(surface);
}   



// World functions implementations

void world_init(world* world){
    // Makes sure that normals of surfaces have correct norms
      for (int i = 0; i < world->surface_amount; ++i){
        surface_update_normal(&world->surfaces[i]);
    }
}

void world_update(world* world, float delta_time){

    // Move things
    world->b->speed = add_vectors(&world->b->speed, world->gravity);
    world->b->speed = scaled_vector(&world->b->speed, world->friction);
    vector movement = scaled_vector(&world->b->speed, delta_time);
    world->b->position = add_vectors(&world->b->position, &movement);

    // Handle collision
    for (int i = 0; i < world->surface_amount; ++i){
        handle_collision(world->b, &world->surfaces[i]);
    }
    for (int i = 0; i < world->bouncer_amount; ++i){
        handle_bouncer_collision(world->b, &world->bouncers[i]);
    }

    // Update kickers
    for (int i = 0; i < world->kicker_amount; ++i){
        kicker *k = &world->kickers[i];
        enum ROTATION_POINT rotation_point = kicker_side_to_rotation_point(k->side);
        float rotation_direction = kicker_side_to_rotation_direction(k->side);
        if (k->activated){
            if (k->current_angle < k->max_angle){
                surface_rotate(k->surface, k->rotation_speed * rotation_direction, rotation_point);
                k->current_angle += k->rotation_speed;
            }
        } else {
            if (k->current_angle >= k->rotation_speed){
                surface_rotate(k->surface, -k->rotation_speed * rotation_direction, rotation_point);
                k->current_angle -= k->rotation_speed;
            } 
            else if (k->current_angle != 0){
                // Reset angle
                surface_rotate(k->surface, -k->current_angle * rotation_direction, rotation_point);
                k->current_angle = 0;
            }
        }
        
    }
}


// World internal functions implementations

float get_distance(ball* b, surface* s){
    float distance = 
        distance_point_and_line(&b->position, &s->start, &s->end, s->is_infinite);
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

void world_activate_kickers(world* world, enum KICKER_SIDE side){
    for (int i = 0; i < world->kicker_amount; ++i){
        if (side != world->kickers[i].side){
            continue;
        }
        world->kickers[i].activated = true;
    }
}

void world_deactivate_kickers(world* world, enum KICKER_SIDE side){
    for (int i = 0; i < world->kicker_amount; ++i){
        if (side != world->kickers[i].side){
            continue;
        }
        world->kickers[i].activated = false;
    }
}
