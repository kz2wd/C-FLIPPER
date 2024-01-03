#include <stdlib.h>

#include "vector.h"
#include "world.h"
#include "display.h"


int main(void){

    ball b = { { 50.f, 40.f }, { 0.f, 0.f }, 8.f };
    float delta_time = 2.f;  // Doesn't work as intended :(
    vector gravity = { 0.f * delta_time, -0.1f * delta_time };
    float friction = .990f;
    
    int surface_amount = 5;
    int kicker_amount = 4;
    surface surfaces[] = {
        { { -70.f, 100.f }, { -70.f,  35.f }, { 0.f, 0.f }, true  },
        { { -70.f,  35.f }, { -35.f,   0.f }, { 0.f, 0.f }, false },
        { { -35.f,   0.f }, {  35.f,   0.f }, { 0.f, 0.f }, true  },
        { {  35.f,   0.f }, {  70.f,  35.f }, { 0.f, 0.f }, false },
        { {  70.f,  35.f }, {  70.f, 100.f }, { 0.f, 0.f }, true  },
        { { -50.f,  50.f }, { -15.f,  35.f }, { 0.f, 0.f }, false },  // surface of left kicker
        { {  15.f,  35.f }, {  50.f,  50.f }, { 0.f, 0.f }, false },  // surface of right kicker
        { { -70.f, 100.f }, { -50.f,  85.f }, { 0.f, 0.f }, false  },
        { {  50.f,  85.f }, {  70.f, 100.f }, { 0.f, 0.f }, false  },
    };

    int bouncer_amount = 1;
    bouncer bouncers[] = {
        { {   0.f,  0.f }, 10.f, 3.2f },
        { { -50.f,  50.f }, 5.f, 1.8f },
        { {  50.f,  50.f }, 5.f, 1.8f },
    };

    kicker kickers[] = {
        {
            .surface=&surfaces[surface_amount],
            .rotation_speed=.3f,
            .max_angle=.9f,
            .current_angle=.0f,
            .activated=false,
            .side=LEFT_SIDE,    
        },
        {
            .surface=&surfaces[surface_amount + 1],
            .rotation_speed=.3f,
            .max_angle=.9f,
            .current_angle=.0f,
            .activated=false,
            .side=RIGHT_SIDE,    
        },
        {
            .surface=&surfaces[surface_amount + 2],
            .rotation_speed=.3f,
            .max_angle=.9f,
            .current_angle=.0f,
            .activated=false,
            .side=LEFT_SIDE,    
        },
        {
            .surface=&surfaces[surface_amount + 3],
            .rotation_speed=.3f,
            .max_angle=.9f,
            .current_angle=.0f,
            .activated=false,
            .side=RIGHT_SIDE,    
        },
    };

    world world = { 
        .b=&b,
        .surface_amount=surface_amount + kicker_amount,
        .surfaces=surfaces,
        .bouncer_amount=bouncer_amount,
        .bouncers=bouncers,
        .gravity=&gravity,
        .friction=friction,
        .kicker_amount=kicker_amount,
        .kickers=kickers,
    };

    world_init(&world);

    init_display(&world);

    bool run = true;
    while(run){
        
        loop_start_display(&world);
        run = handle_events(&world);

        world_update(&world, delta_time);
        
        loop_end_display(&world);
        loop_wait(50);
    }
    
    terminate_display(&world);
    return EXIT_SUCCESS;
}