#include "game.h"

#include "../RIT/RIT.h"
#include "../engine/ecs/world.h"
#include "../engine/manager/time.h"
#include "../engine/renderer.h"
#include "../timer/timer.h"
#include "LPC17xx.h"
#include "entities/age_textui.h"
#include "entities/creature.h"
#include "entities/cuddle_sfx.h"
#include "entities/happiness_imageui.h"
#include "entities/hungriness_imageui.h"
#include "entities/meal.h"
#include "entities/menu_bar.h"
#include "entities/menu_item_textui.h"
#include "entities/reset_textui.h"
#include "entities/snack.h"
#include "entities/volume_ui.h"
#include "game_assets.h"

void (*on_reset)(void);

void create_meal_entity() {
  SpriteSheet meal_sprite_sheet = {.width = 19,
                                   .height = 19,
                                   .frames = 1,
                                   .scale = 1,
                                   &meal_sprite_pixel_index_data[0][0]};

  AnimationController anim_controller = {.current_animation = 0,
                                         .current_frame = 0,
                                         .animations = meal_sprite_sheet,
                                         .on_playback = _dummy_playback};
  AnimatedSprite sprite = {.offset = {0, 0},
                           .anim_controller = anim_controller,
                           .is_currently_playing = 1,
                           .is_flip_x = 0,
                           .blend = DEFAULT_BLEND};

  Entity meal = {.id = 0,
                 .tag = ImageUI,
                 .position = {215, 0},
                 .sprite = sprite,
                 .text = DEFAULT_TEXT_COMPONENT,
                 .kinematic = DEFAULT_KINEMATIC_COMPONENT,
                 .script = DEFAULT_SCRIPT_COMPONENT,
                 .is_visible = 0};
  meal.script.on_update = meal_on_update;
  meal.position.y =
      SCREEN_CENTER_Y + 11 * 3 -
      meal_sprite_sheet.height;  // Used to align with creature height, to
                                 // look like it is on the ground
  add_entity(&meal);
}

void create_snack_entity() {
  SpriteSheet snack_sprite_sheet = {.width = 17,
                                    .height = 20,
                                    .frames = 1,
                                    .scale = 1,
                                    &snack_sprite_pixel_index_data[0][0]};

  AnimationController anim_controller = {.current_animation = 0,
                                         .current_frame = 0,
                                         .animations = snack_sprite_sheet,
                                         .on_playback = _dummy_playback};
  AnimatedSprite sprite = {.offset = {0, 0},
                           .anim_controller = anim_controller,
                           .is_currently_playing = 1,
                           .is_flip_x = 0,
                           .blend = DEFAULT_BLEND};

  Entity snack = {.id = 1,
                  .tag = ImageUI,
                  .position = {215, 0},
                  .sprite = sprite,
                  .text = DEFAULT_TEXT_COMPONENT,
                  .kinematic = DEFAULT_KINEMATIC_COMPONENT,
                  .script = DEFAULT_SCRIPT_COMPONENT,
                  .is_visible = 0};
  snack.script.on_update = snack_on_update;
  snack.position.y =
      SCREEN_CENTER_Y + 11 * 3 -
      snack_sprite_sheet.height;  // Used to align with creature height, to
                                  // look like it is on the ground
  add_entity(&snack);
}

void create_creature_entity() {
  SpriteSheet sprite_sheet_idle = {24, 24, 4, 3,
                                   &creature_idle_sheet_pixel_index_data[0][0]};
  SpriteSheet sprite_sheet_runaway = {
      24, 24, 3, 3, &creature_runaway_sheet_pixel_index_data[0][0]};
  SpriteSheet sprite_sheet_run = {24, 24, 7, 3,
                                  &creature_run_sheet_pixel_index_data[0][0]};
  // SpriteSheet sprite_sheet_walk = {24, 24, 6, 3,
  // &creature_walk_sheet_pixel_index_data[0][0]};
  SpriteSheet sprite_sheet_walk = {24, 24, 7, 3,
                                   &creature_run_sheet_pixel_index_data[0][0]};

  AnimationController anim_controller = {
      .current_animation = 0,
      .current_frame = 0,
      .animations = {sprite_sheet_idle, sprite_sheet_runaway, sprite_sheet_run,
                     sprite_sheet_walk},
      .on_playback = _dummy_playback};
  AnimatedSprite sprite = {
      {0, 0}, anim_controller, 1, 0, .blend = DEFAULT_BLEND};

  Entity creature = {.id = 2,
                     .tag = Character,
                     .position = {0, 0},
                     .sprite = sprite,
                     .text = {.offset = {20 * 3 * 0.5, -8},
                              .content = "",
                              .color = WHITE_COLOR,
                              .highlight_color = BACKGROUND_COLOR},
                     .kinematic = DEFAULT_KINEMATIC_COMPONENT,
                     .script = DEFAULT_SCRIPT_COMPONENT,
                     .is_visible = 1};

  creature.script.on_load = creature_on_load;
  creature.script.on_update = creature_on_update;
  creature.script.before_new_frame = creature_before_new_frame;
  creature.script.on_pressed = creature_on_pressed;
  move_entity_to_screen_center(&creature);
  add_entity(&creature);
}
void create_age_textui_entity() {
  Entity age = NEW_TEXT_UI_ENTITY(3, 0, 0, "00:00:00", WHITE_COLOR);
  age.script.on_update = age_ui_on_update;
  move_entity_to_screen_center_x(&age);
  add_entity(&age);
}
void create_happiness_imageui_entities() {
  SpriteSheet sprite_sheet_happiness = {.width = 13,
                                        .height = 12,
                                        .frames = 1,
                                        .scale = 1,
                                        &happiness_icon_pixel_index_data[0][0]};

  AnimationController anim_controller_happiness = {
      .current_animation = 0,
      .current_frame = 0,
      .animations = sprite_sheet_happiness,
      .on_playback = _dummy_playback};

  AnimatedSprite sprite_happiness = {
      .offset = {0, 0},
      .anim_controller = anim_controller_happiness,
      .is_currently_playing = 1,
      .is_flip_x = 0,
      .blend = DEFAULT_BLEND};
  Script happiness_ui_script = DEFAULT_SCRIPT_COMPONENT;
  happiness_ui_script.on_update = happiness_ui_on_update;
			
  Entity happiness_1 = {.id = 4,
                        .tag = ImageUI,
                        .position = {20, 40},
                        .sprite = sprite_happiness,
                        .text = DEFAULT_TEXT_COMPONENT,
                        .kinematic = DEFAULT_KINEMATIC_COMPONENT,
                        .script = happiness_ui_script,
                        .is_visible = 1};
  happiness_1.script.metadata = 1;
  add_entity(&happiness_1);
												
  Entity happiness_2 = {.id = 5,
                        .tag = ImageUI,
                        .position = {40, 40},
                        .sprite = sprite_happiness,
                        .text = DEFAULT_TEXT_COMPONENT,
                        .kinematic = DEFAULT_KINEMATIC_COMPONENT,
                        .script = happiness_ui_script,
                        .is_visible = 1};
  happiness_2.script.metadata = 2;
  add_entity(&happiness_2);
												
  Entity happiness_3 = {.id = 6,
                        .tag = ImageUI,
                        .position = {60, 40},
                        .sprite = sprite_happiness,
                        .text = DEFAULT_TEXT_COMPONENT,
                        .kinematic = DEFAULT_KINEMATIC_COMPONENT,
                        .script = happiness_ui_script,
                        .is_visible = 1};
  happiness_3.script.metadata = 3;
  add_entity(&happiness_3);
}
void create_hungriness_imageui_entities() {
  SpriteSheet sprite_sheet_hungriness = {
      .width = 16,
      .height = 12,
      .frames = 1,
      .scale = 1,
      &hungriness_icon_pixel_index_data[0][0]};

  AnimationController anim_controller_hungriness = {
      .current_animation = 0,
      .current_frame = 0,
      .animations = sprite_sheet_hungriness,
      .on_playback = _dummy_playback};
	
  AnimatedSprite sprite_hungriness = {
      .offset = {0, 0},
      .anim_controller = anim_controller_hungriness,
      .is_currently_playing = 1,
      .is_flip_x = 0,
      .blend = DEFAULT_BLEND};
	
  Script hungriness_ui_script = DEFAULT_SCRIPT_COMPONENT;
  hungriness_ui_script.on_update = hungriness_ui_on_update;
			
  Entity hungriness_1 = {.id = 7,
                         .tag = ImageUI,
                         .position = {164, 40},
                         .sprite = sprite_hungriness,
                         .text = DEFAULT_TEXT_COMPONENT,
                         .kinematic = DEFAULT_KINEMATIC_COMPONENT,
                         .script = hungriness_ui_script,
                         .is_visible = 1};
  hungriness_1.script.metadata = 1;
  add_entity(&hungriness_1);
												 
  Entity hungriness_2 = {
      .id = 8,
      .tag = ImageUI,
      .position = {184, 40},
      .sprite = sprite_hungriness,
      .text = DEFAULT_TEXT_COMPONENT,
      .kinematic = DEFAULT_KINEMATIC_COMPONENT,
      .script = hungriness_ui_script,
      .is_visible = 1,
  };
	
  hungriness_2.script.metadata = 2;
  add_entity(&hungriness_2);
	
  Entity hungriness_3 = {.id = 9,
                         .tag = ImageUI,
                         .position = {204, 40},
                         .sprite = sprite_hungriness,
                         .text = DEFAULT_TEXT_COMPONENT,
                         .kinematic = DEFAULT_KINEMATIC_COMPONENT,
                         .script = hungriness_ui_script,
                         .is_visible = 1};
  hungriness_3.script.metadata = 3;
  add_entity(&hungriness_3);
}

void create_meal_textui_entity() {
  Entity meal_textui =
      NEW_TEXT_UI_ENTITY(10, 0, MAX_SCREEN_Y - 30, "Meal", WHITE_COLOR);
  meal_textui.script.metadata = 1;
  meal_textui.script.on_update = menu_item_ui_on_update;
	
  Position center = screen_centered_entity_position(&meal_textui);
  meal_textui.position.x = center.x - MAX_SCREEN_X / 4;
  add_entity(&meal_textui);
}
void create_snack_textui_entity() {
  Entity snack_textui =
      NEW_TEXT_UI_ENTITY(11, 0, MAX_SCREEN_Y - 30, "Snack", WHITE_COLOR);
  snack_textui.script.metadata = 2;
  snack_textui.script.on_update = menu_item_ui_on_update;
	
  Position center = screen_centered_entity_position(&snack_textui);
  snack_textui.position.x = center.x + MAX_SCREEN_X / 4;
  add_entity(&snack_textui);
}
void create_menu_bar_logic_entity() {
  Entity menu_bar = NEW_TEXT_UI_ENTITY(12, 0, 0, "", WHITE_COLOR);
  menu_bar.script.on_load = menu_bar_on_load;
  menu_bar.script.on_input = menu_bar_on_input;
  add_entity(&menu_bar);
}
void create_reset_textui_entity() {
  Entity reset_textui =
      NEW_TEXT_UI_ENTITY(13, 0, MAX_SCREEN_Y - 90, "RESET", BACKGROUND_COLOR);
  reset_textui.script.on_load = reset_ui_on_load;
  reset_textui.script.on_update = reset_ui_on_update;
  reset_textui.script.on_input = reset_ui_on_input;
  Position center = screen_centered_entity_position(&reset_textui);
  reset_textui.position.x = center.x;
  add_entity(&reset_textui);
}

void create_cuddle_sfx_entity() {
  SpriteSheet sprite_sheet_happiness = {.width = 13,
                                        .height = 12,
                                        .frames = 1,
                                        .scale = 1,
                                        &happiness_icon_pixel_index_data[0][0]};

  AnimationController anim_controller = {.current_animation = 0,
                                         .current_frame = 0,
                                         .animations = {sprite_sheet_happiness},
                                         .on_playback = _dummy_playback};
  AnimatedSprite sprite = {
      {0, 0}, anim_controller, 1, 0, .blend = DEFAULT_BLEND};

  Entity cuddle_sfx = {.id = 14,
                       .tag = Character,
                       .position = {0, 0},
                       .sprite = sprite,
                       .text = DEFAULT_TEXT_COMPONENT,
                       .kinematic = DEFAULT_KINEMATIC_COMPONENT,
                       .script = DEFAULT_SCRIPT_COMPONENT,
                       .is_visible = 1};

  cuddle_sfx.script.on_load = cuddle_sfx_on_load;
  cuddle_sfx.script.before_new_frame = cuddle_sfx_before_new_frame;
  add_entity(&cuddle_sfx);
}
void create_volume_ui_entity() {
  SpriteSheet sprite_sheet_volume_on = {.width = 13,
                                        .height = 10,
                                        .frames = 1,
                                        .scale = 1,
                                        &volume_on_icon_pixel_index_data[0][0]};

  SpriteSheet sprite_sheet_volume_off = {
      .width = 13,
      .height = 10,
      .frames = 1,
      .scale = 1,
      &volume_off_icon_pixel_index_data[0][0]};

  AnimationController anim_controller = {
      .current_animation = 0,
      .current_frame = 0,
      .animations = {sprite_sheet_volume_on, sprite_sheet_volume_off},
      .on_playback = _dummy_playback};
  AnimatedSprite sprite = {
      {0, 0}, anim_controller, 1, 0, .blend = DEFAULT_BLEND};

  Entity volume_icon = {.id = 15,
                        .tag = ImageUI,
                        .position = {MAX_SCREEN_X - 35, 2},
                        .sprite = sprite,
                        .text = DEFAULT_TEXT_COMPONENT,
                        .kinematic = DEFAULT_KINEMATIC_COMPONENT,
                        .script = DEFAULT_SCRIPT_COMPONENT,
                        .is_visible = 1};

  volume_icon.script.on_update = volume_ui_on_update;
  add_entity(&volume_icon);
}

void game_world_setup() {
  renderer_setup();
  world_reset();

  create_meal_entity();
  create_snack_entity();
  create_creature_entity();
  create_age_textui_entity();
  create_happiness_imageui_entities();
  create_hungriness_imageui_entities();
  create_meal_textui_entity();
  create_snack_textui_entity();
  create_menu_bar_logic_entity();
  create_reset_textui_entity();
  create_cuddle_sfx_entity();
  create_volume_ui_entity();

  // disable_timer(0);
  // disable_RIT();
  //
  // reset_RIT();
  // reset_timer(0);

  reset_clock();

  init_RIT(RIT_INTERVAL_IN_CLOCK_COUNT);
  // init_timer(0, 0, 0, 3, ONE_SECOND_FOR_TIMER_IN_CLOCK_COUNT);
  // /*init timer 0, use MatchReg 0, in interrupt only mode (1), with value
  // 25,000,000 (clock count to reach 1 second with 25MHz frequency)*/

  // enable_timer(0);
  enable_RIT();

  NVIC_SetPriority(RIT_IRQn, 3);
  // NVIC_SetPriority(TIMER0_IRQn, 2);
  NVIC_SetPriority(TIMER1_IRQn, 1);
  on_reset = game_world_setup;
}
