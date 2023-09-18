#include "menu_bar.h"

#include "../../engine/globals.h"
#include "../../engine/manager/music.h"

#define N_ITEMS 2

volatile MenuItem selected_item = NoneItem;

extern volatile int meal_requested;
extern volatile int snack_requested;

extern volatile int game_over;

extern volatile const Music music_snack_selected;
extern volatile const Music music_meal_selected;
extern volatile const Music music_menu_left;
extern volatile const Music music_menu_right;

void move_selection_to_right() {
    if (selected_item >= N_ITEMS) {
        selected_item = (MenuItem)1;
    } else {
        selected_item++;
    }
}
void move_selection_to_left() {
    if (selected_item <= 1) {
        selected_item = (MenuItem)N_ITEMS;
    } else {
        selected_item--;
    }
}

void on_item_pressed() {
    if (selected_item == Meal) {
        meal_requested = 1;
        request_music(&music_meal_selected);
    } else if (selected_item == Snack) {
        snack_requested = 1;
        request_music(&music_snack_selected);
    }
    selected_item = NoneItem;
}

void menu_bar_on_load(Entity* e) {
    selected_item = NoneItem;
}

void menu_bar_on_input(Entity* e, Input* input) {
    if (game_over) {
        return;
    }
    if (meal_requested || snack_requested) {
        return;
    }
    if (input->action == Released) {
        return;
    }

    switch (input->key) {
        case JoystickLeft:
            move_selection_to_left();
            request_music(&music_menu_left);
            break;
        case JoystickRight:
            move_selection_to_right();
            request_music(&music_menu_right);
            break;
        case JoystickButton:
            on_item_pressed();
            break;
    }
}
