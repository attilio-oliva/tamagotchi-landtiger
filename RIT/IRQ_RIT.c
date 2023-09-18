/*********************************************************************************************************
**--------------File
*Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "../ADC/ADC.h"
#include "../GLCD/GLCD.h"
#include "../TouchPanel/TouchPanel.h"
#include "../engine/ecs/world.h"
#include "../engine/globals.h"
#include "../engine/manager/input.h"
#include "../engine/manager/music.h"
#include "../engine/manager/time.h"
#include "../engine/renderer.h"
#include "RIT.h"
#include "lpc17xx.h"

// divide the handling time in MAX_TICK intervals,
// where the render time is the last interval
#define MAX_TICK FRAME_TIME_IN_MILLISECONDS / RIT_INTERVAL_IN_MILLISECONDS

volatile Input last_detected_input;
volatile int tick = 1;

void RIT_IRQHandler(void) {
  Input input = get_debounced_input();
  last_detected_input = input;

  ADC_start_conversion();
  play_music();
  update_clock(RIT_INTERVAL_IN_MILLISECONDS);

  if (getDisplayPoint(&display, Read_Ads7846(), &matrix)) {
    if (display.y < MAX_SCREEN_Y) {
      Position p = {display.x, display.y};
      world_on_pressed(&p);
    }
  }

  if (input.key != None) {
    world_on_input(&input);
  }

  world_update();

  if (tick == MAX_TICK) {
    world_before_new_frame();
    renderer_draw();
    world_update_physics();
    tick = 1;
  } else {
    tick++;
  }

  disable_RIT();
  reset_RIT();
  enable_RIT();
  LPC_RIT->RICTRL |= 0x1; /* clear interrupt flag */

  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
