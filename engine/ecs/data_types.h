#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include "stdint.h"
#include "../globals.h"

typedef struct position
{
	int x;
	int y;
} Position;

typedef struct velocity
{
	double x;
	double y;
} Velocity, Acceleration;

typedef enum input_key
{
	None = -1,
	JoystickButton,
	JoystickDown,
	JoystickLeft,
	JoystickRight,
	JoystickUp,
	Int0,
	Key1,
	Key2
} InputKey;

typedef enum input_action
{
	Pressed,
	Released
} InputAction;

typedef struct input
{
	InputKey key;
	InputAction action;
} Input;

#define EMPTY_INPUT        \
	{                      \
		.key = None,       \
		.action = Pressed, \
	}

#define MAX_FRAMES 4
#define MAX_DATA_SIZE (24 * 24)

static uint8_t empty_pixel_data[1][1] = {0};
#define DEFAULT_SPRITE_SHEET                  \
	{                                         \
		.width = 0,                           \
		.height = 0,                          \
		.frames = 1,                          \
		.scale = 1,                           \
		.pixel_data = &empty_pixel_data[0][0] \
	}

typedef struct sprite_sheet
{
	unsigned int width;
	unsigned int height;
	unsigned int frames;
	unsigned int scale;
	const volatile uint8_t *pixel_data;
} SpriteSheet;

extern unsigned int get_frame_size(SpriteSheet *self);
extern const uint8_t *get_frame(SpriteSheet *self, int frame);

/*------*/
#define MAX_ANIMATIONS 4

#define DEFAULT_ANIMATION_CONTROLLER          \
	{                                         \
		.current_animation = 0,               \
		.current_frame = 0,                   \
		.animations = {DEFAULT_SPRITE_SHEET}, \
		.on_playback = _dummy_playback        \
	}

typedef struct animation_controller
{
	unsigned int current_animation;
	unsigned int current_frame;
	SpriteSheet animations[MAX_ANIMATIONS];
	void (*on_playback)(struct animation_controller *);
} AnimationController;

static void _dummy_playback(AnimationController *c){};

extern SpriteSheet *get_animation(AnimationController *self, int anim_type);

extern SpriteSheet *get_current_animation(AnimationController *self);
extern void set_current_animation_index(AnimationController *self, int animation_index);

extern const uint8_t *get_current_frame(AnimationController *self);
extern void play_next_frame(AnimationController *self);
/*------*/

typedef enum BlendMode
{
	Normal = 0,
	Multiply
} BlendMode;

typedef struct Blend
{
	BlendMode mode;
	uint16_t color;
	int enabled;
} Blend;

#define DEFAULT_BLEND         \
	{                         \
		.mode = Normal,       \
		.color = BLACK_COLOR, \
		.enabled = 0          \
	}

#endif
