#include "age_textui.h"

#include <stdio.h>

#include "../../engine/manager/time.h"
#include "string.h"

static char time_string[9];
extern volatile int game_over;

void age_ui_on_update(Entity* e) {
    if (game_over) {
        return;
    }

    unsigned int seconds = get_clock_seconds() * GAME_TIME_MULTIPLIER;
    
    unsigned int minutes = seconds / 60;
    int hours = seconds / 3600;
    sprintf(time_string, "%02i:%02i:%02i", hours, minutes % 60, seconds % 60);
    strcpy(e->text.content, time_string);
};
