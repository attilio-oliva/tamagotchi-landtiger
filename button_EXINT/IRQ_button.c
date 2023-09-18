#include "button.h"
#include "lpc17xx.h"

#include "../engine/manager/input.h"
#include "../custom_button/button_utils.h"

extern int btn_down_counter[];

void EINT0_IRQHandler (void)	  
{
	btn_down_counter[0] = 1;				/* alert that button has been pressed */
	disable_btn(EINT0_IRQn);				/* self disable the button (both interrupt and pin) */
	
  LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt int0 */
}


void EINT1_IRQHandler (void)	  
{
	btn_down_counter[1] = 1;
	disable_btn(EINT1_IRQn);
	
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt key1 */
	
}

void EINT2_IRQHandler (void)	  
{
	btn_down_counter[2] = 1;
	disable_btn(EINT2_IRQn);
	
	LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt key2 */   
}


