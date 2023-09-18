#ifndef RESET_UI
#define RESET_UI

#include "../../engine/ecs/entity.h"
#include "string.h"

extern volatile int game_over;
extern void (*on_reset)(void);

static void reset_ui_on_load(Entity* e) {
    strcpy(e->text.content, "RESET");
    e->text.highlight_color = WHITE_COLOR;
    e->text.color = BACKGROUND_COLOR;
    e->is_visible = 0;
}

static void reset_ui_on_update(Entity* e) {
    if (game_over) {
        e->is_visible = 1;
    }
}

static void reset_ui_on_input(Entity* e, Input* input) {
    if (!game_over) {
        return;
    }

    if (input->action == Released) {
        return;
    }

    switch (input->key) {
        case JoystickButton:
            game_over = 0;
            on_reset();
            break;
    }
}
#endif
