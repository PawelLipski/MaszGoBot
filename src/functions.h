/********************************************************************************/
/********************************************************************************/
/***   MARCIN OKARMA   ***   EDU_BOT   ***   VERSION  1.0   *********************/
/********************************************************************************/
/********************************************************************************/


#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <stdio.h>
#include "board.h"
#include "HD44780.h"
#include "motor.h"
#include "timer.h"
#include "adc.h"

//#include "usart.h"

/********************************************************************************/
void initialize(void);
/********************************************************************************/
void button_initialize(void);
void led_initialize(void);
void switch_initialize(void);
void speaker_initialize(void);

void remote_initialize(void);

void input_initialize(void);
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
void Beep(unsigned int frequency, unsigned int duration);
/********************************************************************************/


void print0(const char* msg);
void print1(const char* msg);

void all_leds_on(void);
void all_leds_off(void);
void middle_leds_on(void);
void middle_leds_off(void);
void left_led_on(void);
void left_led_off(void);
void right_led_on(void);
void right_led_off(void);
void leftmost_middle_leds_on(unsigned c);

#endif
