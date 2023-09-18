/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../timer/timer.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

volatile float current_volume = 1;   
volatile float last_volume = 1;     /* Last converted value               */

volatile float current_volume_level = 1;  /* non linear mapped value */

/* k=1/f'*f/n  k=f/(f'*n) k=25MHz/(f'*45) */

//const int freqs[8]={4240,3779,3367,3175,2834,2525,2249,2120};
/* 
131Hz		k=4240 C3
147Hz		k=3779
165Hz		k=3367
175Hz		k=3175
196Hz		k=2834		
220Hz		k=2525
247Hz		k=2249
262Hz		k=2120 C4
*/

const int freqs[8]={2120,1890,1684,1592,1417,1263,1125,1062};
/*
262Hz	k=2120		c4
294Hz	k=1890		
330Hz	k=1684		
349Hz	k=1592		
392Hz	k=1417		
440Hz	k=1263		
494Hz	k=1125		
523Hz	k=1062		c5

*/

#define MAX_ADC_VALUE 0xFFF

volatile float min_volume = 0.3;

void ADC_IRQHandler(void) {
  	
  float adc_val = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
	current_volume = adc_val/MAX_ADC_VALUE;
	current_volume_level = (current_volume >= min_volume) ? current_volume : 0;
  if(current_volume != last_volume){		
		last_volume = current_volume;
  }
	
}
