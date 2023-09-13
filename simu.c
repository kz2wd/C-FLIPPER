#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ncurses.h>
#include <unistd.h>

#define BALL_CHAR '0'
#define EMPTY_CHAR ' '


typedef struct vector {
    float x;
    float y;
} vector;

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

vector add_vectors(vector* v1, vector* v2){
    vector result = { v1->x + v2->x, v1->y + v2->y };
    return result;
}

vector scaled_vector(vector* v, float scale){
    vector result = { v->x * scale, v->y * scale };
    return result;
}

void print_vector(vector* v){
    printf("x: %f, y: %f\n", v->x, v->y);
}

float vector_norm(vector* v){
    return sqrtf( v->x * v->x + v->y * v->y);
}

float vectors_distance(vector* v1, vector* v2){
    float x_diff = (v2->x - v1->x);
    float y_diff = (v2->y - v1->y);
    return sqrtf( x_diff * x_diff + y_diff * y_diff);
}


float distance_point_and_line(vector* p1, vector* l1, vector* l2){
    // ref : https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_two_points
    return ((float) fabs((double) (l2->x - l1->x)* (l1->y - p1->y) - (l1->x - p1->x)* (l2->y - l1->y))) / vectors_distance(l1, l2);
}

float get_distance(ball* b, surface* s){
    float distance = 
        distance_point_and_line(&b->position, &s->start, &s->end);
    return distance;
}

float dot_product(vector *v1, vector* v2){
    return v1->x * v2->x + v1->y * v2->y;
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

vector normalized_vector(vector* v){
    return scaled_vector(v, 1 / vector_norm(v));
}

int world_x(float x){
    return ((int) x) + COLS / 2;
}

int world_y(float y){
    return (- ((int) y) + LINES - 3) / 2;
}

void print_element(vector* pos, char chr){
    mvaddch(world_y(pos->y), world_x(pos->x), chr);
}

char get_char_at_ball(ball* b){
    return mvinch(world_y(b->position.y), world_x(b->position.x)) & A_CHARTEXT;
}
void print_ball(ball* b){
    if (get_char_at_ball(b) == EMPTY_CHAR){
        print_element(&b->position, BALL_CHAR);
    }
}

void erase_ball(ball *b){
    if (get_char_at_ball(b) == BALL_CHAR){
        print_element(&b->position, EMPTY_CHAR);
    }
}

void print_surface(surface* s){
    vector opposed_start = scaled_vector(&s->start, -1.f);
    vector draw_line = add_vectors(&opposed_start, &s->end);
    vector shift = normalized_vector(&draw_line);
    int length = (int) vector_norm(&draw_line);
    for (int i = 1; i < length; ++i){
        vector scaled_shift = scaled_vector(&shift, (float) i);
        vector draw_pos = add_vectors(&s->start, &scaled_shift);
        print_element(&draw_pos, 219);
    }
}


int main(void){
    ball b = { { 0.f, 5.f }, { 1.f, 1.f }, 1.f };
    vector gravity = { 0.f, -0.1f };


    int surface_amount = 3;
    surface surfaces[] = {
        { { -20.f, 10.f }, { -10.f, 0.f   }, { 1.f, 1.f } },
        { { -10.f, 0.f }, { 10.f, 0.f }, { 0.f, 1.f} },
        { {10.f, 0.f }, { 20.f, 10.f  }, { -1.f, 1.f } },
        };

    initscr();
    curs_set(0);
    for (int i = 0; i < surface_amount; ++i){
        surfaces[i].normal = normalized_vector(&surfaces[i].normal);
        print_surface(&surfaces[i]);
    }

    while(true){
        
        erase_ball(&b);
        b.speed = add_vectors(&b.speed, &gravity);
        b.position = add_vectors(&b.position, &b.speed);

        for (int i = 0; i < surface_amount; ++i){
            handle_collsion(&b, &surfaces[i]);
        }
        
        print_ball(&b);
        
        refresh();
        usleep(50 * 1000);
    }
    endwin();
    return EXIT_SUCCESS;
}

