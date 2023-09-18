#ifndef COMPONENT_H
#define COMPONENT_H

#include "data_types.h"
#include "../globals.h"

struct entity;

#define DEFAULT_ANIMATED_SPRITE_COMPONENT                \
	{                                                    \
		.offset = {0, 0},                                \
		.anim_controller = DEFAULT_ANIMATION_CONTROLLER, \
		.is_currently_playing = 0,                       \
		.is_flip_x = 0,                                  \
		.blend = DEFAULT_BLEND                           \
	}

typedef struct animated_sprite
{
	Position offset;
	AnimationController anim_controller;
	int is_currently_playing;
	int is_flip_x;
	Blend blend;
} AnimatedSprite;

#define MAX_TEXT_CHARS 15

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

#define MAX_TEXT_DIM_X (CHAR_WIDTH * MAX_TEXT_CHARS)
#define MAX_TEXT_DIM_Y CHAR_HEIGHT

#define TEXT_WIDTH(TEXT_COMP) CHAR_WIDTH *strlen(&TEXT_COMP.content[0])
#define TEXT_HEIGHT(TEXT_COMP) CHAR_HEIGHT

#define DEFAULT_TEXT_COMPONENT              \
	{                                       \
		.offset = {0, 0},                   \
		.content = "",                      \
		.color = WHITE_COLOR,               \
		.highlight_color = BACKGROUND_COLOR \
	}

typedef struct text
{
	Position offset;
	char content[MAX_TEXT_CHARS];
	uint16_t color;
	uint16_t highlight_color;
} Text;

#define DEFAULT_KINEMATIC_COMPONENT \
	{                               \
		.velocity = {0, 0},         \
		.acceleration = {0, 0},     \
	}

typedef struct kinematic
{
	Velocity velocity;
	Acceleration acceleration;
} Kinematic;

static void kinematic_move_to_in_time(Kinematic *self, Position *current_position, Position *desired_position, unsigned int seconds)
{

	Velocity calculated_velocity = {0, 0};

	// calculate non-normalized directions
	calculated_velocity.x = desired_position->x - (double)current_position->x;
	calculated_velocity.y = desired_position->y - (double)current_position->y;

	// apply scale to velocity norm
	calculated_velocity.x /= (double)(seconds) / ((double)(FRAME_TIME_IN_MILLISECONDS)*0.001);
	calculated_velocity.y /= (double)(seconds) / ((double)(FRAME_TIME_IN_MILLISECONDS)*0.001);

	self->velocity = calculated_velocity;
}

static void _dummy_fn(struct entity *e){};
static void _dummy_input_fn(struct entity *e, Input *input){};
#define DEFAULT_SCRIPT_COMPONENT       \
	{                                  \
		.on_load = _dummy_fn,          \
		.on_update = _dummy_fn,        \
		.before_new_frame = _dummy_fn, \
		.on_input = _dummy_input_fn,   \
		.on_pressed = _dummy_fn,       \
		.metadata = 0,                 \
	}

typedef struct script
{
	void (*on_load)(struct entity *);
	void (*on_update)(struct entity *);
	void (*before_new_frame)(struct entity *);
	void (*on_input)(struct entity *, Input *input);
	void (*on_pressed)(struct entity *);
	int metadata;
} Script;

#endif
