/********************************************************************************/
/********************************************************************************/
/***   MARCIN OKARMA   ***   EDU_BOT   ***   VERSION  1.0   *********************/
/********************************************************************************/
/********************************************************************************/

#include "adc.h"


void ADC_initialize(void){
	// ADC initialization
	// ADC Clock frequency: 250,000 kHz
	// ADC Voltage Reference: AVCC pin
	ADMUX=ADC_VREF_TYPE & 0xff;
	ADCSRA=0x85;
}

/*unsigned char read_adc(unsigned char adc_input){
ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
_delay_us(10);
ADCSRA|=0x40;
while ((ADCSRA & 0x10)==0);
ADCSRA|=0x10;
return ADCH;
}*/

unsigned int read_adc(unsigned char adc_input){
ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
_delay_us(10);
ADCSRA|=0x40;
while ((ADCSRA & 0x10)==0);
ADCSRA|=0x10;
return ADCW;
}
