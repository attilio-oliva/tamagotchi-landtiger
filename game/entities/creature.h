#ifndef CREATURE_ENTITY
#define CREATURE_ENTITY

#include "../../engine/ecs/entity.h"

typedef enum creature_state {
    Idle,
    FoodSmelled,
    FoodDetected,
    RunningToFood,
    Eating,
    WalkingAfterEating,
    Walk,
    Runaway,
} CreatureState;

extern volatile int happiness;
extern volatile int hungriness;

extern volatile int game_over;

extern void creature_on_load(Entity* e);
extern void creature_on_update(Entity* e);
extern void creature_on_pressed(Entity* e);
extern void creature_before_new_frame(Entity* e);

#endif
