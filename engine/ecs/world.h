#ifndef WORLD_H
#define WORLD_H

#include "entity.h"

#define MAX_ENTITIES 16
#define MAX_PHYSICS_ENTITIES 2

volatile extern Entity entities[];
volatile extern int entities_size;
volatile extern Entity *physic_entities[];
volatile extern int physic_size;

extern void world_setup(void);
extern void world_reset(void);
extern void world_update(void);
extern void world_update_physics(void);
extern void world_before_new_frame(void);
extern void world_on_input(Input *input);
extern void world_on_pressed(Position *pos);

extern void add_entity_to_buffer(Entity *new_entity);
extern void add_entity(Entity *new_entity);
extern Entity *get_entity_with_id(int id);
extern Entity *get_entities_with_tag(Tag tag);

extern Entity *get_entities(void);
extern int get_num_entities(void);

#endif
