#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include <vector.h>
#include <world.h>
#include <display.h>


int main(void){
    
    ball b = { { 50.f, 40.f }, { 0.f, 0.f }, 10.f };
    float delta_time = 2.f;  // Doesn't work as intended :(
    vector gravity = { 0.f * delta_time, -0.1f * delta_time };
    float friction = .990f;
    
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

    world world = { 
        .b=&b,
        .surface_amount=surface_amount,
        .surfaces=surfaces,
        .bouncer_amount=bouncer_amount,
        .bouncers=bouncers,
        .gravity=&gravity,
        .friction=friction
    };

    world_init(&world);

    init_display(&world);

    while(true){
        
        loop_start_display(&world);

        world_update(&world, delta_time);
        
        loop_end_display(&world);
        usleep(16 * 1000 * delta_time);  // wait a bit :)
    }
    
    terminate_display(&world);
    return EXIT_SUCCESS;
}