#include "hungriness_imageui.h"

extern int hungriness;

void hungriness_ui_on_update(Entity* e) {
    if (hungriness < e->script.metadata) {
        e->sprite.blend.enabled = 1;
    } else {
        e->sprite.blend.enabled = 0;
    }
}
