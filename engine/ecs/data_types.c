#include "data_types.h"

unsigned int get_frame_size(SpriteSheet *self) {
    return self->width * self->height * 2 + 1;  //2 bytes per pixel
};

const uint8_t *get_frame(SpriteSheet *self, int frame) {
    return self->pixel_data + frame * MAX_DATA_SIZE;
};

SpriteSheet *get_animation(AnimationController *self, int anim_type) {
    return &self->animations[anim_type];
};

SpriteSheet *get_current_animation(AnimationController *self) {
    return &self->animations[self->current_animation];
};

void set_current_animation_index(AnimationController *self, int animation_index) {
    self->current_frame = 0;
    self->current_animation = animation_index;
}

const uint8_t *get_current_frame(AnimationController *self) {
    SpriteSheet *sheet = get_current_animation(self);
    return get_frame(sheet, self->current_frame);
};

void play_next_frame(AnimationController *self) {
    SpriteSheet *sheet = get_current_animation(self);
    if (self->current_frame == sheet->frames - 1) {
        self->current_frame = 0;
        self->on_playback(self);
    } else {
        self->current_frame++;
    }
};
