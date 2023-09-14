#include <display.h>
#include <ncurses.h>


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