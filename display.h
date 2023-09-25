#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include "world.h"

void init_display(world* world);

void loop_start_display(world* world);

// Returns a boolean indicating if the process is over. True = not over / false = over.
bool handle_events(world* world);

void loop_end_display(world* world);

void loop_wait(int max_fps);

void terminate_display(world* world);

#endif

