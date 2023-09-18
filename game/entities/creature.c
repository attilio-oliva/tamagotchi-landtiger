#include "creature.h"

#include "../../engine/ecs/entity.h"
#include "../../engine/globals.h"
#include "../../engine/manager/time.h"
#include "../game_music.h"
#include "reset_textui.h"
#include "string.h"

#define SECONDS_TO_DECREASE_STATS 5
#define SECONDS_TO_EAT_FOOD 1
#define SECONDS_TO_REACH_FOOD 2

#define MAX_STATS_VALUE 3

volatile int happiness = MAX_STATS_VALUE;
volatile int hungriness = MAX_STATS_VALUE;
volatile CreatureState state = Idle;
volatile Position* creature_position;
volatile int cuddle_requested = 0;

volatile uint64_t last_time_ate_meal = 0;
volatile uint64_t last_time_ate_snack = 0;
volatile uint64_t last_time_food_requested = 0;
volatile uint64_t last_time_started_eating = 0;

extern volatile int game_over;

static Position food_position = {220, 124};
static Position base_position = {84, 124};
volatile const Position text_status_offset = {20 * 3 * 0.5, -8};

extern int meal_requested;
extern int snack_requested;

typedef enum creature_animation {
    IdleAnimation = 0,
    RunawayAnimation,
    RunAnimation,
    WalkAnimation
} CreatureAnimation;

void emit_game_over(AnimationController* c) {
    game_over = 1;
    set_current_animation_index(c, IdleAnimation);
    c->on_playback = _dummy_playback;
}
void show_text_status(Entity* e, char* str) {
    strcpy(e->text.content, str);
    e->text.color = WHITE_COLOR;
    e->text.offset = text_status_offset;
}

void hide_text_status(Entity* e) {
    show_text_status(e, "");
}

void increase_stat(volatile int* stat) {
    if (*stat == MAX_STATS_VALUE)
        return;

    *stat += 1;
}

void creature_on_load(Entity* e) {
    happiness = MAX_STATS_VALUE;
    hungriness = MAX_STATS_VALUE;

    state = Idle;
    cuddle_requested = 0;
    creature_position = &e->position;

    last_time_ate_meal = 0;
    last_time_ate_snack = 0;
    last_time_started_eating = 0;
    last_time_food_requested = 0;
}

void creature_on_update(Entity* e) {
    uint64_t clock_time = get_clock_time_in_milliseconds() * GAME_TIME_MULTIPLIER;

    volatile uint64_t elapsed_ms_last_meal = clock_time - last_time_ate_meal;
    volatile uint64_t elapsed_ms_last_snack = clock_time - last_time_ate_snack;
    volatile uint64_t elapsed_ms_started_eating = clock_time - last_time_started_eating;

    // react to time events based on its state
    switch (state) {
        case Idle:
            // should decrease stats every X seconds
            if (elapsed_ms_last_meal >= SECONDS_TO_DECREASE_STATS * 1000) {
                hungriness--;
                last_time_ate_meal = clock_time;
            }
            if (elapsed_ms_last_snack >= SECONDS_TO_DECREASE_STATS * 1000) {
                happiness--;
                last_time_ate_snack = clock_time;
            }
            break;
        case Eating:
            if (elapsed_ms_started_eating >= SECONDS_TO_EAT_FOOD) {
                state = WalkingAfterEating;

                if (snack_requested) {
                    increase_stat(&happiness);
                    last_time_ate_snack = clock_time;
                    snack_requested = 0;
                }
                if (meal_requested) {
                    increase_stat(&hungriness);
                    last_time_ate_meal = clock_time;
                    meal_requested = 0;
                }

                kinematic_move_to_in_time(&e->kinematic, &e->position, &base_position, SECONDS_TO_REACH_FOOD);
                e->sprite.is_flip_x = 1;
                e->sprite.is_currently_playing = 1;
                set_current_animation_index(&e->sprite.anim_controller, WalkAnimation);
            }
    }

    // react to dropped food based on its state
    // FoodDetected state is triggered on before_new_frame callback to show text animation
    if (meal_requested || snack_requested) {
        switch (state) {
            case FoodDetected:
                state = RunningToFood;
                last_time_food_requested = clock_time;
                kinematic_move_to_in_time(&e->kinematic, &e->position, &food_position, SECONDS_TO_REACH_FOOD);
                hide_text_status(e);
                e->sprite.is_currently_playing = 1;
                set_current_animation_index(&e->sprite.anim_controller, RunAnimation);
                break;
        }
    }

    // react on reaching positions based on its state
    switch (state) {
        case RunningToFood:
            if (e->position.x >= food_position.x - get_sprite_width(e)) {
                state = Eating;
                last_time_started_eating = clock_time;
                e->kinematic.velocity.x = 0;
                e->sprite.is_currently_playing = 0;
            }
            break;
        case WalkingAfterEating:
            if (e->position.x <= base_position.x) {
                state = Idle;

                if (last_time_ate_meal > last_time_ate_snack) {
                    last_time_ate_meal = clock_time;
                    last_time_ate_snack += clock_time - last_time_food_requested;
                } else {
                    last_time_ate_snack = clock_time;
                    last_time_ate_meal += clock_time - last_time_food_requested;
                }

                e->sprite.is_flip_x = 0;
                e->kinematic.velocity.x = 0;
                set_current_animation_index(&e->sprite.anim_controller, Idle);
            }
            break;
    }

    if (game_over) {
        e->is_visible = 0;
    }
}

void creature_before_new_frame(Entity* e) {
    // text animations to react to dropped food based on its state
    if (meal_requested || snack_requested) {
        switch (state) {
            case Idle:
                state = FoodSmelled;
                show_text_status(e, "?");
                e->sprite.is_currently_playing = 0;
                break;
            case FoodSmelled:
                state = FoodDetected;
                show_text_status(e, "!");
                break;
        }
    }

    // react to stats dropped to 0 based on its state
    if (happiness == 0 || hungriness == 0) {
        switch (state) {
            case Runaway:
                if (e->sprite.anim_controller.current_animation == RunawayAnimation) {
                    if (e->sprite.anim_controller.current_frame == 1) {
                        show_text_status(e, "!");
                    } else {
                        hide_text_status(e);
                    }
                }
                break;
            default:
                state = Runaway;
                if (e->sprite.anim_controller.current_animation != RunawayAnimation) {
                    request_music(&music_runaway);
                    set_current_animation_index(&e->sprite.anim_controller, RunawayAnimation);
                    e->sprite.anim_controller.on_playback = emit_game_over;
                }
        }
    }
}

void creature_on_pressed(Entity* e) {
    if (game_over) {
        return;
    }
    cuddle_requested = 1;
    increase_stat(&happiness);
    request_music(&music_cuddles);
}
