/**
		Custom library by Attilio Oliva
		
		Purpose: Easy handling for enabling and disabling buttons
**/
#include "button_utils.h"

void enable_btn(IRQn_Type type) {
	int offset = type - EINT0_IRQn;										//EINT0_IRQn, EINT1_IRQn, EINT2_IRQn values are 18,19,20
	int pinsel_bit_pos = 20 + offset*2;								//PINSEL bit positions should be respectively 20,22,24
	
	NVIC_EnableIRQ(type);													   //Enable the interrupt handler
	LPC_PINCON->PINSEL4  |= (1 << pinsel_bit_pos);   //Enable the pin connection for the button
}

void disable_btn(IRQn_Type type) {
	int offset = type - EINT0_IRQn;
	int pinsel_bit_pos = 20 + offset*2;
	
	NVIC_DisableIRQ(type);
	LPC_PINCON->PINSEL4  &= ~(1 << pinsel_bit_pos);
}
