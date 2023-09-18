#include "../../engine/ecs/entity.h"

typedef enum menu_item {
    NoneItem = 0,
    Meal,
    Snack
} MenuItem;

extern volatile MenuItem selected_item;

extern void menu_bar_on_load(Entity* e);
extern void menu_bar_on_input(Entity* e, Input* input);
