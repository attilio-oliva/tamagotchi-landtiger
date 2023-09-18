#include "menu_item_textui.h"

#include "../../engine/globals.h"
#include "menu_bar.h"

#define N_ITEMS 2

extern volatile MenuItem selected_item;
extern volatile int game_over;

static uint16_t text_color = 0xFFFF;
static uint16_t highlight_color = BACKGROUND_COLOR;

void hide_item(Entity* e) {
    e->text.color = BACKGROUND_COLOR;
    e->text.highlight_color = BACKGROUND_COLOR;
}

void select_item(Entity* e) {
    e->text.color = highlight_color;
    e->text.highlight_color = text_color;
}

void deselect_item(Entity* e) {
    e->text.color = text_color;
    e->text.highlight_color = highlight_color;
}

void menu_item_ui_on_update(Entity* e) {
    if (game_over) {
        hide_item(e);
        return;
    }

    if (selected_item == e->script.metadata) {
        select_item(e);
    } else {
        deselect_item(e);
    }
}
