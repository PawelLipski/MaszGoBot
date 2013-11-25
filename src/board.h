/********************************************************************************/
/********************************************************************************/
/***   MARCIN OKARMA   ***   EDU_BOT   ***   VERSION  1.0   *********************/
/********************************************************************************/
/********************************************************************************/

#ifndef _BOARD_H
#define _BOARD_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


/********************************************************************************/
// Makrodefinicje do ustawiania bitów w stan wysoki i niski.
 /********************************************************************************/
#define bit_set(port, bit)		((port) |= (1<<bit))
#define bit_clr(port, bit)		((port) &= ~(1<<bit))
#define bit_flip(port, bit)		(port ^= _BV(bit))
#define bit_get(port, bit)		((port & _BV(bit))>>bit)
#define CAT_PORT(x) 			(PORT ## x)
#define CAT_PIN(x) 				(PIN ## x)
#define _bit_set(port, bit)		(CAT_PORT(port) |= (1<<bit))
#define _bit_clr(port, bit)		(CAT_PORT(port) &= ~(1<<bit))
#define _bit_flip(port, bit)            (CAT_PORT(port) ^= _BV(bit))
#define _bit_get(port, bit)		((CAT_PIN(port) & _BV(bit))>>bit)

/********************************************************************************/
// Makrodefinicje do ustawiania pinów procesora w stan wysoki i niski.
/********************************************************************************/
#define SET(port)		_bit_set(port ## _PORT, port ## _PIN)
#define CLR(port)		_bit_clr(port ## _PORT, port ## _PIN)
#define FLIP(port)		_bit_flip(port ## _PORT, port ## _PIN)
#define GET(port)		_bit_get(port ## _PORT, port ## _PIN)


/********************************************************************************/
// LCD
/********************************************************************************/
#define LCD_DB4_PORT	PORTC
#define LCD_DB5_PORT	PORTC
#define LCD_DB6_PORT	PORTC
#define LCD_DB7_PORT	PORTC

#define LCD_DB4_DIR		DDRC
#define LCD_DB5_DIR		DDRC
#define LCD_DB6_DIR		DDRC
#define LCD_DB7_DIR		DDRC

#define LCD_DB4			0x20
#define LCD_DB5			0x10
#define LCD_DB6			0x08
#define LCD_DB7			0x04

#define LCD_RS_PORT		PORTA
#define LCD_RS_DIR		DDRA
#define LCD_RS			0x01

#define LCD_E_PORT		PORTA
#define LCD_E_DIR		DDRA
#define LCD_E			0x02

/********************************************************************************/
// LED
/********************************************************************************/
#define LED_1_PORT		PORTA
#define LED_1_DDR		DDRA
#define LED_2_PORT		PORTA
#define LED_2_DDR		DDRA
#define LED_3_PORT		PORTA
#define LED_3_DDR		DDRA
#define LED_4_PORT		PORTA
#define LED_4_DDR		DDRA
#define LED_5_PORT		PORTA
#define LED_5_DDR		DDRA
#define LED_6_PORT		PORTA
#define LED_6_DDR		DDRA
#define LED_7_PORT		PORTC
#define LED_7_DDR		DDRC
#define LED_8_PORT		PORTC
#define LED_8_DDR		DDRC

#define LED_1			0x04		// PA2
#define LED_2			0x08		// PA3
#define LED_3			0x10		// PA4
#define LED_4			0x20		// PA5
#define LED_5			0x40		// PA6
#define LED_6			0x80		// PA7
#define LED_7			0x80		// PC7
#define LED_8			0x40		// PC6

#define LED1_PORT		A
#define LED2_PORT		A
#define LED3_PORT		A
#define LED4_PORT		A
#define LED5_PORT		A
#define LED6_PORT		A
#define LED7_PORT		C
#define LED8_PORT		C

#define LED1_PIN		2
#define LED2_PIN		3
#define LED3_PIN		4
#define LED4_PIN		5
#define LED5_PIN		6
#define LED6_PIN		7
#define LED7_PIN		7
#define LED8_PIN		6

// PILOT LED
#define LED__P_PORT		PORTG
#define LED__P_DDR		DDRG
#define LED__P			0x10		// PG4

#define LED_P_PORT		G
#define LED_P_PIN		4

/********************************************************************************/
// SPEAKER
/********************************************************************************/
#define _SPEAKER_PORT	PORTG
#define _SPEAKER_DDR	DDRG
#define _SPEAKER		0x04

#define SPEAKER_PORT	G
#define SPEAKER_PIN		2

/********************************************************************************/
// SWITCH
/********************************************************************************/
#define SWITCH_PORT		PORTE
#define SWITCH_DDR		DDRE
#define SWITCH_PIN		PINE

#define SWITCH_1		0x04
#define SWITCH_2		0x08
#define SWITCH_3		0x10
#define SWITCH_4		0x20

#define SWITCH1_PORT	E
#define SWITCH2_PORT	E
#define SWITCH3_PORT	E
#define SWITCH4_PORT	E

#define SWITCH1_PIN		2
#define SWITCH2_PIN		3
#define SWITCH3_PIN		4
#define SWITCH4_PIN		5

/********************************************************************************/
// BUTTON
/********************************************************************************/
#define BUTTON__U_PORT	PORTC
#define BUTTON__U_DDR	DDRC
#define BUTTON__U		0x01		// PC0
#define BUTTON_U_ON		(~PINC & BUTTON_U)

#define BUTTON__D_PORT	PORTB
#define BUTTON__D_DDR	DDRB
#define BUTTON__D		0x80		// PB7
#define BUTTON_D_ON		(~PINB & BUTTON_D)

#define BUTTON__L_PORT	PORTC
#define BUTTON__L_DDR	DDRC
#define BUTTON__L		0x02		// PC1
#define BUTTON_L_ON		(~PINC & BUTTON_L)

#define BUTTON__R_PORT	PORTG
#define BUTTON__R_DDR	DDRG
#define BUTTON__R		0x08		// PG3
#define BUTTON_R_ON		(~PING & BUTTON_R)

#define BUTTON_U_PORT	C
#define BUTTON_D_PORT	B
#define BUTTON_L_PORT	C
#define BUTTON_R_PORT	G

#define BUTTON_U_PIN	0
#define BUTTON_D_PIN	7
#define BUTTON_L_PIN	1
#define BUTTON_R_PIN	3

/********************************************************************************/
// MOTOR
/********************************************************************************/
#define MOTOR_PORT		PORTD
#define MOTOR_DDR		DDRD

#define MOTOR_1			0x10		// PD4
#define MOTOR_2			0x20		// PD5
#define MOTOR_3			0x40		// PD6
#define MOTOR_4			0x80		// PD7

#define MOTOR1_PORT		D
#define MOTOR2_PORT		D
#define MOTOR3_PORT		D
#define MOTOR4_PORT		D

#define MOTOR1_PIN		4
#define MOTOR2_PIN		5
#define MOTOR3_PIN		6
#define MOTOR4_PIN		7

/********************************************************************************/
// PWM
/********************************************************************************/
#define PWM_PORT		PORTB
#define PWM_DDR			DDRB

#define ENABLE_A		0x20		// PB5/OC1A
#define ENABLE_B 		0x40		// PB6/OC1B

#define PWM_A			OCR1A
#define PWM_B			OCR1B

/********************************************************************************/
// INPUT
/********************************************************************************/
#define INPUT_PORT		PORTD
#define INPUT_DDR		DDRD

#define INPUT_1			0x01		// PD0
#define INPUT_2			0x02		// PD1
#define INPUT_3			0x04		// PD2
#define INPUT_4			0x08		// PD3

#define INPUT1_PORT		D
#define INPUT2_PORT		D
#define INPUT3_PORT		D
#define INPUT4_PORT		D

#define INPUT1_PIN		0
#define INPUT2_PIN		1
#define INPUT3_PIN		2
#define INPUT4_PIN		3

#endif
