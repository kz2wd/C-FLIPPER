#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include <vector.h>
#include <world.h>
#include <display.h>


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