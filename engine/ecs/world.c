#include "world.h"

#include "../globals.h"
#include "../manager/time.h"

#define GAME_TIME get_clock_time() * GAME_TIME_MULTIPLIER;

volatile Entity entities[MAX_ENTITIES];
volatile int entities_size = 0;
volatile Entity *physic_entities[MAX_PHYSICS_ENTITIES];
volatile int physic_size = 0;

void world_setup()
{
	entities_size = 0;
	physic_size = 0;
}

void world_reset()
{
	world_setup();
}

void world_update()
{
	for (int i = 0; i < entities_size; i++)
	{
		entities[i].script.on_update(&entities[i]);
	}
}

void world_update_physics()
{
	for (int i = 0; i < physic_size; i++)
	{
		Velocity v = physic_entities[i]->kinematic.velocity;
		physic_entities[i]->position.x += v.x;
		physic_entities[i]->position.y += v.y;
	}
}

void world_before_new_frame(void)
{
	for (int i = 0; i < entities_size; i++)
	{
		entities[i].script.before_new_frame(&entities[i]);
	}
}

void world_on_input(Input *input)
{
	for (int i = 0; i < entities_size; i++)
	{
		entities[i].script.on_input(&entities[i], input);
	}
}
int is_position_inside_entity_sprite(Entity *e, Position *pos)
{
	Position sprite_top_left = {e->position.x, e->position.y};
	Position sprite_bottom_right = {e->position.x + get_sprite_width(e), e->position.y + get_sprite_height(e)};

	if (pos->x > sprite_top_left.x && pos->x < sprite_bottom_right.x)
		if (pos->y < sprite_bottom_right.y && pos->y > sprite_top_left.y)
			return 1;

	return 0;
}

void world_on_pressed(Position *pos)
{
	for (int i = 0; i < entities_size; i++)
	{
		if (is_position_inside_entity_sprite(&entities[i], pos))
		{
			entities[i].script.on_pressed(&entities[i]);
		}
	}
}

void push_entity_ptr(Entity **list, Entity *e)
{
	list[physic_size++] = e;
}

void insert_entity(Entity *e, int index)
{
	entities[index] = *e;
	entities_size++;
}

void add_entity_to_buffer(Entity *new_entity)
{
	switch (new_entity->tag)
	{
	case Character:
		push_entity_ptr(physic_entities, new_entity);
		break;
	default:
		break;
	}
}

void add_entity(Entity *new_entity)
{
	insert_entity(new_entity, new_entity->id);
	add_entity_to_buffer(&entities[new_entity->id]);

	new_entity->script.on_load(get_entity_with_id(new_entity->id));
}

Entity *get_entity_with_id(int id)
{
	return &entities[id];
}
/*
Entity* get_entities_with_tag(Tag tag) {
	switch(tag) {
		case Character:
				return physic_entities;
			break;
		default:
			break;
	}
	return physic_entities;
}
*/
Entity *get_entities()
{
	return entities;
}

int get_num_entities()
{
	return entities_size;
}

void push_entity(Entity e)
{
	entities[entities_size++] = e;
}
