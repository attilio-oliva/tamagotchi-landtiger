#ifndef ENTITY_H
#define ENTITY_H

#include "component.h"
#include "../globals.h"

typedef enum
{
	Character = 0, //Can use all the components
	ImageUI,	   //Use AnimatedSheet, position and script
	TextUI,		   //Use Text, position and script
	Logic,		   //Use only script
} Tag;

typedef struct entity
{
	unsigned int id;
	Tag tag;
	Position position;
	AnimatedSprite sprite;
	Text text;
	Kinematic kinematic;
	Script script;
	int is_visible;
} Entity;

extern Position screen_centered_entity_position(Entity *e);
extern void move_entity_to_screen_center_x(Entity *e);
extern void move_entity_to_screen_center_y(Entity *e);
extern void move_entity_to_screen_center(Entity *e);
extern unsigned int get_sprite_width(Entity *e);
extern unsigned int get_sprite_height(Entity *e);

#define NEW_CHARACTER_ENTITY(ID, X, Y)                              \
	{                                                               \
		.id = ID,                                                   \
		.tag = Character,                                           \
		.position = {X, Y},                                         \
		.sprite = DEFAULT_ANIMATED_SPRITE_COMPONENT,                \
		.text = {.offset = {X, Y}, .content = "", .color = 0xFFFF}, \
		.kinematic = DEFAULT_KINEMATIC_COMPONENT,                   \
		.script = DEFAULT_SCRIPT_COMPONENT,                         \
		.is_visible = 1,                                            \
	}

#define NEW_TEXT_UI_ENTITY(ID, X, Y, TEXT, COLOR)                                                         \
	{                                                                                                     \
		.id = ID,                                                                                         \
		.tag = TextUI,                                                                                    \
		.position = {X, Y},                                                                               \
		.sprite = DEFAULT_ANIMATED_SPRITE_COMPONENT,                                                      \
		.text = {.offset = {0, 0}, .content = TEXT, .color = COLOR, .highlight_color = BACKGROUND_COLOR}, \
		.kinematic = DEFAULT_KINEMATIC_COMPONENT,                                                         \
		.script = DEFAULT_SCRIPT_COMPONENT,                                                               \
		.is_visible = 1,                                                                                  \
	}

#define NEW_TEXT_UI_SPRITE_ENTITY(ID, X, Y, TEXT, COLOR, SPRITE)     \
	{                                                                \
		.id = ID,                                                    \
		.tag = TextUI,                                               \
		.position = {0, 0},                                          \
		.sprite = SPRITE,                                            \
		.text = {.offset = {X, Y}, .content = TEXT, .color = COLOR}, \
		.kinematic = DEFAULT_KINEMATIC_COMPONENT,                    \
		.script = DEFAULT_SCRIPT_COMPONENT,                          \
		.is_visible = 1,                                             \
	}

#endif
