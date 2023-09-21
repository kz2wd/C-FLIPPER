#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include <vector.h>
#include <world.h>
#include <display.h>


int main(void){
    ball b = { { 50.f, 40.f }, { 0.f, 0.f }, 10.f };
    float delta_time = 1.f;  // Doesn't work as intended :(
    vector gravity = { 0.f * delta_time, -0.1f * delta_time };
    float friction = .990f * delta_time;
    
    int surface_amount = 3;
    surface surfaces[] = {
        { { -60.f, 30.f }, { -30.f,  0.f }, {  1.f, 1.f } },
        { { -30.f,  0.f }, {  30.f,  0.f }, {  0.f, 1.f } },
        { {  30.f,  0.f }, {  60.f, 30.f }, { -1.f, 1.f } },
        };

    int bouncer_amount = 3;
    bouncer bouncers[] = {
        { { -60.f, 30.f }, 7.f, 2.8f },
        { {   0.f,  0.f }, 7.f, 2.8f },
        { {  60.f, 30.f }, 7.f, 2.8f }
    };

    initscr();
    curs_set(0);
    for (int i = 0; i < surface_amount; ++i){
        surfaces[i].normal = normalized_vector(&surfaces[i].normal);
        print_surface(&surfaces[i]);
    }
    for (int i = 0; i < bouncer_amount; ++i){
        print_bouncer(&bouncers[i]);
    }

    while(true){
        
        erase_ball(&b);

        b.speed = add_vectors(&b.speed, &gravity);
        b.speed = scaled_vector(&b.speed, friction);
        vector movement = scaled_vector(&b.speed, delta_time);
        b.position = add_vectors(&b.position, &movement);

        for (int i = 0; i < surface_amount; ++i){
            handle_collision(&b, &surfaces[i]);
        }
        for (int i = 0; i < bouncer_amount; ++i){
            handle_bouncer_collision(&b, &bouncers[i]);
        }
        
        print_ball(&b);
        
        refresh();
        usleep(50 * 1000 * delta_time);
    }
    endwin();
    return EXIT_SUCCESS;
}