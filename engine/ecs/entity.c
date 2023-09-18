#include "string.h"
#include "entity.h"

#include "../globals.h"

Position screen_centered_entity_position(Entity *e)
{
	Position result = {0, 0};
	SpriteSheet *animation = get_current_animation(&e->sprite.anim_controller);
	switch (e->tag)
	{
	case Character:
	case ImageUI:
		result.x = SCREEN_CENTER_X - animation->width * animation->scale * 0.5;
		result.y = SCREEN_CENTER_Y - animation->height * animation->scale * 0.5;
		break;
	case TextUI:
		result.x = SCREEN_CENTER_X - TEXT_WIDTH(e->text) * 0.5;
		result.y = SCREEN_CENTER_Y - TEXT_HEIGHT(e->text) * 0.5;
		break;
	default:
		result.x = SCREEN_CENTER_X;
		result.y = SCREEN_CENTER_Y;
		break;
	}
	return result;
}

void move_entity_to_screen_center_x(Entity *e)
{
	e->position.x = screen_centered_entity_position(e).x;
}
void move_entity_to_screen_center_y(Entity *e)
{
	e->position.y = screen_centered_entity_position(e).y;
}
void move_entity_to_screen_center(Entity *e)
{
	move_entity_to_screen_center_x(e);
	move_entity_to_screen_center_y(e);
}

unsigned int get_sprite_width(Entity *e)
{
	SpriteSheet *animations = e->sprite.anim_controller.animations;
	return animations->width * animations->scale;
}
unsigned int get_sprite_height(Entity *e)
{
	SpriteSheet *animations = e->sprite.anim_controller.animations;
	return animations->height * animations->scale;
}
