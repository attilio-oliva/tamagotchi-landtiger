#include "volume_ui.h"

extern volatile float current_volume_level;

#define VOLUME_ICON_MUTED_INDEX 1
#define VOLUME_ICON_ENABLED_INDEX 0

void volume_ui_on_update(Entity* e) {
    if (current_volume_level == 0) {
        set_current_animation_index(&e->sprite.anim_controller, VOLUME_ICON_MUTED_INDEX);
    } else {
        set_current_animation_index(&e->sprite.anim_controller, VOLUME_ICON_ENABLED_INDEX);
    }
}
