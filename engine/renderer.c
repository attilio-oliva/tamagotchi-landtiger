#include "renderer.h"

#include "../GLCD/GLCD.h"
#include "LPC17xx.h"
#include "ecs/world.h"
#include "globals.h"
#include "stdlib.h"
#include "string.h"

#define BUFFER_WIDTH MAX_SCREEN_X
#define BUFFER_HEIGHT 12
#define BUFFER_DIMENSION \
  { BUFFER_WIDTH, BUFFER_HEIGHT }

#define DIRTY_AREA_FACTOR 6  // MAX_X / MAX_DIRTY_AREA_X

#define MAX_DIRTY_AREA_X (MAX_X / DIRTY_AREA_FACTOR)
#define MAX_DIRTY_AREA_Y (MAX_Y / DIRTY_AREA_FACTOR)

#define DIRTY_COORD_FROM_SCREEN(COORD) (COORD) / DIRTY_AREA_FACTOR

static uint8_t dirty_areas[MAX_DIRTY_AREA_X][MAX_DIRTY_AREA_Y] = {0};

static uint16_t render_buffer[BUFFER_HEIGHT][BUFFER_WIDTH] = {BACKGROUND_COLOR};

static uint16_t rand_color = WHITE_COLOR;  // used only to debug the renderer dirty section clearing

extern volatile const uint16_t index_color_map[];

void clear_buffer() {
  for (int i = 0; i < BUFFER_HEIGHT; i++) {
    for (int j = 0; j < BUFFER_WIDTH; j++) {
      render_buffer[i][j] = TRANSPARENT_COLOR;
    }
  }
}
void clear_dirty_areas() {
  for (int i = 0; i < MAX_DIRTY_AREA_Y; i++) {
    for (int j = 0; j < MAX_DIRTY_AREA_X; j++) {
      dirty_areas[i][j] = 0;
    }
  }
}

int are_rectangles_overlapped(Position rect_top_left1,
                              Position rect_bottom_right1,
                              Position rect_top_left2,
                              Position rect_bottom_right2) {
  // if rectangle has area 0, no overlap
  if (rect_top_left1.x == rect_bottom_right1.x ||
      rect_top_left1.y == rect_bottom_right1.y ||
      rect_bottom_right2.x == rect_top_left2.x ||
      rect_bottom_right2.y == rect_top_left2.y)
    return 0;

  // If one rectangle is on left side of other
  if (rect_top_left1.x > rect_bottom_right2.x ||
      rect_top_left2.x > rect_bottom_right1.x)
    return 0;

  // If one rectangle is above other
  if (rect_bottom_right1.y < rect_top_left2.y ||
      rect_bottom_right2.y < rect_top_left1.y)
    return 0;

  return 1;
}

int is_entity_inside_area(Entity *e, Position area_top_left,
                          Position dimensions) {
  Position entity_top_left = {e->position.x, e->position.y};

  Position sprite_top_left = entity_top_left;
  Position sprite_bottom_right = {e->position.x + get_sprite_width(e),
                                  e->position.y + get_sprite_height(e)};

  Position text_top_left = {entity_top_left.x + e->text.offset.x,
                            entity_top_left.y + e->text.offset.y};
  Position text_bottom_right = {text_top_left.x + TEXT_WIDTH(e->text),
                                text_top_left.y + TEXT_HEIGHT(e->text)};

  Position area_bottom_right = {area_top_left.x + dimensions.x,
                                area_top_left.y + dimensions.y};

  // sprite component is inside the area
  if (are_rectangles_overlapped(sprite_top_left, sprite_bottom_right,
                                area_top_left, area_bottom_right)) {
    return 1;
  }
  // text component is inside the area
  if (are_rectangles_overlapped(text_top_left, text_bottom_right, area_top_left,
                                area_bottom_right)) {
    return 1;
  }

  return 0;
}

void draw_buffer_on_screen(Position origin, Position buffer_dimension) {
  for (int y = 0; y < buffer_dimension.y; y++) {
    for (int x = 0; x < buffer_dimension.x; x++) {
      uint16_t color = render_buffer[y][x];
      Position screen_position = {origin.x + x, origin.y + y};
      if (color != TRANSPARENT_COLOR) {
        LCD_SetPoint(screen_position.x, screen_position.y, color);
      }
    }
  }
}

void draw_rect_on_buffer(Position pos, Position dimension, uint16_t color,
                         Position buffer_dimension) {
  for (int offset_x = 0; offset_x < dimension.x; offset_x++) {
    for (int offset_y = 0; offset_y < dimension.y; offset_y++) {
      int pixel_x = pos.x + offset_x;
      int pixel_y = pos.y + offset_y;
      if (pixel_x < buffer_dimension.x && pixel_y < buffer_dimension.y) {
        render_buffer[pixel_y][pixel_x] = color;
      }
    }
  }
}

void draw_dirty_areas_mask_on_buffer(Position buffer_origin,
                                     Position buffer_dimension) {
  Position buffer_pos = buffer_origin;
  for (int area_y = DIRTY_COORD_FROM_SCREEN(buffer_origin.y);
       area_y < DIRTY_COORD_FROM_SCREEN((buffer_origin.y + buffer_dimension.y));
       area_y++) {
    buffer_pos.x = buffer_origin.x;
    for (int area_x = DIRTY_COORD_FROM_SCREEN(buffer_origin.x);
         area_x <
         DIRTY_COORD_FROM_SCREEN((buffer_origin.y + buffer_dimension.x));
         area_x++) {
      if (dirty_areas[area_y]
                     [area_x]) {  // if this area is dirty from last frame
        Position local_buffer_pos = {buffer_pos.x - buffer_origin.x,
                                     buffer_pos.y - buffer_origin.y};
        Position dim = {DIRTY_AREA_FACTOR, DIRTY_AREA_FACTOR};
        draw_rect_on_buffer(local_buffer_pos, dim, BACKGROUND_COLOR,
                            buffer_dimension);  // rand_color can be used here
        dirty_areas[area_y][area_x] = 0;
      }
      buffer_pos.x += DIRTY_AREA_FACTOR;
    }
    buffer_pos.y += DIRTY_AREA_FACTOR;
  }
}

void draw_pixel_on_buffer(Position pos, Position local_pos, uint16_t color,
                          int scale, Position buffer_dimension) {
  local_pos.x *= scale;
  local_pos.y *= scale;

  for (int offset_x = 0; offset_x < scale; offset_x++) {
    for (int offset_y = 0; offset_y < scale; offset_y++) {
      int pixel_x = pos.x + local_pos.x + offset_x;
      int pixel_y = pos.y + local_pos.y + offset_y;
      if (pixel_x < 0 || pixel_y < 0) continue;
      if (pixel_x < buffer_dimension.x && pixel_y < buffer_dimension.y) {
        if (color != TRANSPARENT_COLOR) {
          render_buffer[pixel_y][pixel_x] = color;
        }
      }
    }
  }
}

void draw_indexed_frame_on_buffer(
    Position buffer_start_pos, Position buffer_origin,
    Position buffer_dimension, Position initial_frame_pos, Position dimension,
    const uint8_t *sprite_frame, int scale, Blend *blend, int flip_x) {
  Position current_buffer_pos = buffer_start_pos;
  for (int local_y = initial_frame_pos.y;
       local_y < dimension.y && current_buffer_pos.y < buffer_dimension.y;
       local_y++) {
    for (int local_x = initial_frame_pos.x;
         local_x < dimension.x && current_buffer_pos.x < buffer_dimension.x;
         local_x++) {
      int actual_x = (flip_x) ? (dimension.x - 1 - local_x) : local_x;
      Position transformed_local_pos = {actual_x, local_y};
      Position local_pos = {local_x, local_y};
      int byte_pos =
          transformed_local_pos.x + dimension.x * transformed_local_pos.y;
      uint8_t index_color = sprite_frame[byte_pos];
      uint16_t color = index_color_map[index_color];

      if (blend->enabled) {
        if (blend->mode == Normal) {
          if (color != TRANSPARENT_COLOR) {
            color = blend->color;
          }
        }
      }
      draw_pixel_on_buffer(buffer_start_pos, local_pos, color, scale,
                           buffer_dimension);
      for (int y = 0; y < scale; y++) {
        for (int x = 0; x < scale; x++) {
          dirty_areas[DIRTY_COORD_FROM_SCREEN(
              buffer_origin.y + current_buffer_pos.y + local_pos.y * scale + y)]
                     [DIRTY_COORD_FROM_SCREEN(buffer_origin.x +
                                              current_buffer_pos.x +
                                              local_pos.x * scale + x)] = 1;
        }
      }
      current_buffer_pos.x = buffer_start_pos.x + local_x;
    }
    current_buffer_pos.y = buffer_start_pos.y + local_y;
  }
}

void draw_frame_on_buffer(Position buffer_start_pos, Position buffer_origin,
                          Position buffer_dimension, Position initial_frame_pos,
                          Position dimension, const uint8_t *sprite_frame,
                          int scale, Blend *blend, int flip_x) {
  Position current_buffer_pos = buffer_start_pos;
  for (int local_y = initial_frame_pos.y;
       local_y < dimension.y && current_buffer_pos.y < buffer_dimension.y;
       local_y++) {
    for (int local_x = initial_frame_pos.x;
         local_x < dimension.x && current_buffer_pos.x < buffer_dimension.x;
         local_x++) {
      int actual_x = (flip_x) ? (dimension.x - 1 - local_x) : local_x;
      Position transformed_local_pos = {actual_x, local_y};
      Position local_pos = {local_x, local_y};
      int byte_pos = (transformed_local_pos.x * 2) +
                     (dimension.x * 2 * transformed_local_pos.y);
      uint8_t first_byte = sprite_frame[byte_pos];
      uint8_t second_byte = sprite_frame[byte_pos + 1];
      uint16_t color = ((uint16_t)second_byte << 8) | first_byte;

      if (blend->enabled) {
        if (blend->mode == Normal) {
          if (color != TRANSPARENT_COLOR) {
            color = blend->color;
          }
        }
      }
      draw_pixel_on_buffer(buffer_start_pos, local_pos, color, scale,
                           buffer_dimension);
      for (int y = 0; y < scale; y++) {
        for (int x = 0; x < scale; x++) {
          dirty_areas[DIRTY_COORD_FROM_SCREEN(
              buffer_origin.y + current_buffer_pos.y + local_pos.y * scale + y)]
                     [DIRTY_COORD_FROM_SCREEN(buffer_origin.x +
                                              current_buffer_pos.x +
                                              local_pos.x * scale + x)] = 1;
        }
      }
      current_buffer_pos.x = buffer_start_pos.x + local_x;
    }
    current_buffer_pos.y = buffer_start_pos.y + local_y;
  }
}
void draw_text_on_buffer(Position buffer_start_pos, Position buffer_origin,
                         Position buffer_dimension, Position initial_text_pos,
                         Position text_dimension, char *text_content,
                         uint16_t color, uint16_t highlight_color, int scale,
                         Blend *blend) {
  if (strlen(text_content) <= 0) {
    return;
  }

  GUI_TextOnMemoryBuffer(buffer_start_pos.x, buffer_start_pos.y,
                         (uint8_t *)text_content, color, highlight_color,
                         &render_buffer[0][0], buffer_origin.x, buffer_origin.y,
                         buffer_dimension.x, buffer_dimension.y);

  for (int y = 0; y < text_dimension.y; y++) {
    for (int x = 0; x < text_dimension.x; x++) {
      dirty_areas[DIRTY_COORD_FROM_SCREEN(buffer_origin.y + buffer_start_pos.y +
                                          initial_text_pos.y + y)]
                 [DIRTY_COORD_FROM_SCREEN(buffer_origin.x + buffer_start_pos.x +
                                          initial_text_pos.x + x)] = 1;
    }
  }
}
void draw_entity_on_buffer(Entity *e, Position buffer_origin,
                           Position buffer_dimension) {
  if (!e->is_visible) {
    return;
  }

  AnimationController *controller = &e->sprite.anim_controller;
  SpriteSheet *animation = get_current_animation(controller);
  Position sprite_dimension = {animation->width, animation->height};
  Position text_dimension = {TEXT_WIDTH(e->text), TEXT_HEIGHT(e->text)};
  Blend sprite_blend = e->sprite.blend;
  int scale = e->sprite.anim_controller.animations->scale;

  Position buffer_entity_pos = {e->position.x - buffer_origin.x,
                                e->position.y - buffer_origin.y};
  Position buffer_text_pos = {
      e->position.x + e->text.offset.x - buffer_origin.x,
      e->position.y + e->text.offset.y - buffer_origin.y};
  Position frame_start_pos = {0, 0};
  Position text_start_pos = {0, 0};

  const uint8_t *sprite_frame = get_current_frame(controller);

  if (buffer_entity_pos.x < 0) {
    frame_start_pos.x = -buffer_entity_pos.x / scale;
  }
  if (buffer_entity_pos.y < 0) {
    frame_start_pos.y = -buffer_entity_pos.y / scale;
  }

  if (buffer_text_pos.x < 0) {
    text_start_pos.x = -buffer_text_pos.x;
  }
  if (buffer_text_pos.y < 0) {
    text_start_pos.y = -buffer_text_pos.y;
  }
  draw_indexed_frame_on_buffer(buffer_entity_pos, buffer_origin,
                               buffer_dimension, frame_start_pos,
                               sprite_dimension, sprite_frame, animation->scale,
                               &sprite_blend, e->sprite.is_flip_x);
  // draw_frame_on_buffer(buffer_entity_pos, buffer_origin, buffer_dimension,
  // frame_start_pos, sprite_dimension, sprite_frame, animation->scale,
  // &sprite_blend, e->sprite.is_flip_x);

  draw_text_on_buffer(buffer_text_pos, buffer_origin, buffer_dimension,
                      text_start_pos, text_dimension, e->text.content,
                      e->text.color, e->text.highlight_color, 1, &sprite_blend);
}

void render_screen_section(int section_row, int section_col) {
  Position buffer_dim = BUFFER_DIMENSION;
  Position area_origin = {section_col * BUFFER_WIDTH,
                          section_row * BUFFER_HEIGHT};

  draw_dirty_areas_mask_on_buffer(area_origin, buffer_dim);

  for (int i = 0; i < get_num_entities(); i++) {
    Entity *e = get_entity_with_id(i);
    if (is_entity_inside_area(e, area_origin, buffer_dim)) {
      draw_entity_on_buffer(e, area_origin, buffer_dim);
    }
  }
  draw_buffer_on_screen(area_origin, buffer_dim);

  clear_buffer();
}

void renderer_setup(void) {
  // LCD_Initialization();
  LCD_Clear(BACKGROUND_COLOR);
  clear_buffer();
  clear_dirty_areas();
}

void renderer_draw(void) {
  int render_section_rows = MAX_SCREEN_Y / BUFFER_HEIGHT;
  int render_section_cols = MAX_SCREEN_X / BUFFER_WIDTH;
  rand_color =
      rand() % 65536;  // used only to debug the renderer dirty section clearing
  for (int row = 0; row < render_section_rows; row++) {
    for (int col = 0; col < render_section_cols; col++) {
      render_screen_section(row, col);
    }
  }

  for (int i = 0; i < get_num_entities(); i++) {
    Entity *e = get_entity_with_id(i);
    // draw_sprite(e);
    // draw_text(e);
    if (e->sprite.is_currently_playing) {
      play_next_frame(&e->sprite.anim_controller);
    }
  }
}
