/********************************************************************************/
/********************************************************************************/
/***   MARCIN OKARMA   ***   EDU_BOT   ***   VERSION  1.0   *********************/
/********************************************************************************/
/********************************************************************************/


#ifndef _ADC_H
#define _ADC_H

#include "board.h"

#define ADC_VREF_TYPE 0x40

void ADC_initialize(void);
//unsigned char read_adc(unsigned char adc_input);
unsigned int read_adc(unsigned char adc_input);

#endif
