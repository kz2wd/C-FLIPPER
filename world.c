#include <world.h>

#include <stdbool.h>

float get_distance(ball* b, surface* s){
    float distance = 
        distance_point_and_line(&b->position, &s->start, &s->end);
    return distance;
}


bool check_collision(ball* b, surface* s){
    return get_distance(b, s) <= b->radius;
}


void handle_collsion(ball* b, surface* s){
    float distance = get_distance(b, s);
    
    // In case of collision
    if (distance <= b->radius){
        // Move the ball out of the surface
        float out_epsilon = 0.01f; // extra displacement to prevent collision detection for the next step
        vector out_of_surface = scaled_vector(&s->normal, distance + out_epsilon);
        b->position = add_vectors(&out_of_surface, &b->position);

        // Change the speed of the ball
        vector v = { -s->normal.y, s->normal.x };
        vector v1 = scaled_vector(&s->normal, dot_product(&s->normal, &b->speed));
        vector v2 = scaled_vector(&v, -dot_product(&v, &b->speed));
        vector new_speed = add_vectors(&v1, &v2);
        b->speed = scaled_vector(&new_speed, -1.f);
    }
}
