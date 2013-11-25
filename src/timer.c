/********************************************************************************/
/********************************************************************************/
/***   MARCIN OKARMA   ***   BadWolf   ***   VERSION  2.0   *********************/
/********************************************************************************/
/********************************************************************************/



#include "timer.h"

void t0_initialize(void){
	// Timer/Counter 0 initialization
	// Clock source: System Clock
	// Clock value: 8000,000 kHz
	// Mode: Normal top=0xFF
	// OC0 output: Disconnected
	ASSR=0x00;
	TCCR0=0x01;
	TCNT0=176;
	OCR0=0x00;

	// Timer(s)/Counter(s) Interrupt(s) initialization
	TIMSK=0x01;

}


void t1_initialize(void){

PWM_DDR |= ENABLE_A;
PWM_DDR |= ENABLE_B;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 125,000 kHz
// Mode: Fast PWM top=0x00FF
// OC1A output: Non-Inv.
// OC1B output: Non-Inv.
// OC1C output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
// Compare C Match Interrupt: Off
TCCR1A=0xA1;
TCCR1B=0x0B;
TCNT1=0x0000;
ICR1=0x0000;

OCR1A=0x0000;
OCR1B=0x0000;
OCR1C=0x0000;

}

