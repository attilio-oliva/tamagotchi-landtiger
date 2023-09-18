#include "cuddle_sfx.h"

#include "../../engine/manager/time.h"

extern volatile int game_over;

extern volatile int cuddle_requested;
extern volatile Position* creature_position;

volatile unsigned int time_started_cuddling;

#define ANIMATION_DURATION_IN_MILLISECONDS 2000

void cuddle_sfx_on_load(Entity* e) {
    e->is_visible = 0;
}
void cuddle_sfx_before_new_frame(Entity* e) {
    if (game_over) {
        e->is_visible = 0;
        e->kinematic.velocity.y = 0;
        return;
    }

    uint64_t clock_time = get_clock_time_in_milliseconds() * GAME_TIME_MULTIPLIER;
    if (e->kinematic.velocity.y != 0) {
        uint64_t elapsed_ms = clock_time - time_started_cuddling;
        cuddle_requested = 0;
        if (elapsed_ms > ANIMATION_DURATION_IN_MILLISECONDS) {
            e->is_visible = 0;
            e->kinematic.velocity.x = 0;
            e->kinematic.velocity.y = 0;
            e->is_visible = 0;
        }
    }

    if (cuddle_requested) {
        e->is_visible = 1;

        e->position.y = creature_position->y + 36;
        e->position.x = creature_position->x + 32;
        e->kinematic.velocity.y = -14;
        time_started_cuddling = clock_time;
    }
}
