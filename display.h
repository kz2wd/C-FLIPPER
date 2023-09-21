#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector.h>
#include <world.h>

#define BALL_CHAR 'Q'
#define EMPTY_CHAR ' '

int world_x(float x);

int world_y(float y);

void print_element(vector* pos, char chr);

void print_ball(ball* b);

void erase_ball(ball *b);

void print_surface(surface* s);

void print_bouncer(bouncer* bouncer);

#endif