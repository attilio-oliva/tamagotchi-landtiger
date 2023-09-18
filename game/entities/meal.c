#include "meal.h"

#include "creature.h"

volatile int meal_requested = 0;
extern volatile CreatureState creature_state;

void meal_on_update(Entity* e) {
    if (meal_requested) {
        e->is_visible = 1;
    } else {
        e->is_visible = 0;
    }
}
