#include "happiness_imageui.h"

extern volatile int happiness;

void happiness_ui_on_update(Entity* e) {
    if (happiness < e->script.metadata) {
        e->sprite.blend.enabled = 1;
    } else {
        e->sprite.blend.enabled = 0;
    }
}
