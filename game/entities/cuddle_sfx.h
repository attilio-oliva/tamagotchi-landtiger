#ifndef CUDDLE_ENTITY
#define CUDDLE_ENTITY

#include "../../engine/ecs/entity.h"

extern volatile int cuddle_requested;

extern void cuddle_sfx_on_load(Entity* e);
extern void cuddle_sfx_before_new_frame(Entity* e);

#endif
