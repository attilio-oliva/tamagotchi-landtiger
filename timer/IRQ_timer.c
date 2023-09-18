/*********************************************************************************************************
**--------------File
*Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler(void) {
  /* Match register 0 interrupt service routine */
  if (LPC_TIM0->IR & 01) {
    LPC_TIM0->IR = 1; /* clear interrupt flag */
  }
  /* Match register 1 interrupt service routine */
  else if (LPC_TIM0->IR & 02) {
    LPC_TIM0->IR = 2; /* clear interrupt flag */
  }
  /* Match register 2 interrupt service routine */
  else if (LPC_TIM0->IR & 4) {
    LPC_TIM0->IR = 4; /* clear interrupt flag */
  }
  /* Match register 3 interrupt service routine */
  else if (LPC_TIM0->IR & 8) {
    LPC_TIM0->IR = 8; /* clear interrupt flag */
  }

  return;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
const uint16_t SinTable[45] = {
    410, 467, 523, 576, 627, 673, 714, 749, 778, 799, 813, 819, 817, 807, 789,
    764, 732, 694, 650, 602, 550, 495, 438, 381, 324, 270, 217, 169, 125, 87,
    55,  30,  12,  2,   0,   6,   20,  41,  70,  105, 146, 193, 243, 297, 353};

extern volatile float current_volume_level;

void TIMER1_IRQHandler(void) {
  /* DAC management */
  volatile static int sineticks = 0;
  volatile static int currentValue;
  currentValue = SinTable[sineticks] * current_volume_level;
  currentValue -= 410;
  currentValue /= 1;
  currentValue += 410;

  LPC_DAC->DACR = currentValue << 6;
  sineticks++;
  if (sineticks == 45) sineticks = 0;

  LPC_TIM1->IR = 1; /* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler(void) {
  disable_timer(1);
  LPC_TIM2->IR = 1; /* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
