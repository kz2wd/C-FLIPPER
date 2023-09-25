#include <display.h>
#include <ncurses.h>
#include <vector.h>
#include <world.h>

// ASCII DISPLAY FUNCTIONS

#define BALL_CHAR 'Q'
#define EMPTY_CHAR ' '

int world_x(float x);

int world_y(float y);

void print_element(vector* pos, char chr);

void print_ball(ball* b);

void erase_ball(ball *b);

void print_surface(surface* s);

void print_bouncer(bouncer* bouncer);

// INTERFACE IMPLEMENTATION

void init_display(world* world){
    initscr();
    curs_set(0);
    
     for (int i = 0; i < world->surface_amount; ++i){
        print_surface(&world->surfaces[i]);
    }
    for (int i = 0; i < world->bouncer_amount; ++i){
        print_bouncer(&world->bouncers[i]);
    }
}

void loop_start_display(world* world){
    erase_ball(world->b);
}

void loop_end_display(world* world){
    print_ball(world->b);
    refresh();
}

void terminate_display(world* world){
    endwin();
}



// ASCII DISPLAY IMPLEMENTATION

int world_x(float x){
    return ((int) x) + COLS / 2;
}

int world_y(float y){
    return (- ((int) y)) / 2 + LINES - 3;
}

void print_element(vector* pos, char chr){
    mvaddch(world_y(pos->y), world_x(pos->x), chr);
}

void print_element_with_offset(vector* pos, int x_offset, int y_offset, char chr){
    mvaddch(world_y(pos->y + y_offset), world_x(pos->x + x_offset), chr);
}

char get_char_at_ball(ball* b, int x_offset, int y_offset){
    return mvinch(world_y(b->position.y + y_offset), world_x(b->position.x + x_offset)) & A_CHARTEXT;
}

void draw_circle(vector* origin, float radius, char chr){
    int r = (int) radius;
    float r_squared = radius * radius;
    for (int i = -r; i <= r; ++i){
        for (int j = -r; j <= r; ++j){
            if (i * i + j * j < r_squared){
                print_element_with_offset(origin, i, j, chr);
            }
        }
    }
}

void iterate_over_ball_draw(ball *b, char print, char replace){
    int radius = (int) b->radius;
    float square_radius = b->radius * b->radius;
    for (int i = -radius; i <= radius; ++i){
        for (int j = -radius; j <= radius; ++j){
            if ((float) (i * i) + (j * j) < square_radius && get_char_at_ball(b, i, j) == replace){
                print_element_with_offset(&b->position, i, j, print);
            }
        }
    }
}

void print_ball(ball* b){
    iterate_over_ball_draw(b, BALL_CHAR, EMPTY_CHAR);
}

void erase_ball(ball *b){
    iterate_over_ball_draw(b, EMPTY_CHAR, BALL_CHAR);
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

void print_bouncer(bouncer* bouncer){
    draw_circle(&bouncer->position, (int) bouncer->radius, 219);
}

