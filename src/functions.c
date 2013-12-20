/********************************************************************************/
/********************************************************************************/
/***   MARCIN OKARMA   ***   EDU_BOT   ***   VERSION  1.0   *********************/
/********************************************************************************/
/********************************************************************************/

#include "functions.h"

/********************************************************************************/
void button_initialize(void) {
	BUTTON__U_DDR &= ~BUTTON__U;
	BUTTON__D_DDR &= ~BUTTON__D;
	BUTTON__L_DDR &= ~BUTTON__L;
	BUTTON__R_DDR &= ~BUTTON__R;

	BUTTON__U_PORT |= BUTTON__U;
	BUTTON__D_PORT |= BUTTON__D;
	BUTTON__L_PORT |= BUTTON__L;
	BUTTON__R_PORT |= BUTTON__R;
}
/********************************************************************************/
void switch_initialize(void) {
	SWITCH_DDR &= ~SWITCH_1;
	SWITCH_DDR &= ~SWITCH_2;
	SWITCH_DDR &= ~SWITCH_3;
	SWITCH_DDR &= ~SWITCH_4;

	SWITCH_PORT |= SWITCH_1;
	SWITCH_PORT |= SWITCH_2;
	SWITCH_PORT |= SWITCH_3;
	SWITCH_PORT |= SWITCH_4;
}
/********************************************************************************/
void led_initialize(void) {
	LED_1_DDR |= LED_1;
	LED_1_PORT |= LED_1;
	LED_2_DDR |= LED_2;
	LED_2_PORT |= LED_2;
	LED_3_DDR |= LED_3;
	LED_3_PORT |= LED_3;
	LED_4_DDR |= LED_4;
	LED_4_PORT |= LED_4;
	LED_5_DDR |= LED_5;
	LED_5_PORT |= LED_5;
	LED_6_DDR |= LED_6;
	LED_6_PORT |= LED_6;
	LED_7_DDR |= LED_7;
	LED_7_PORT |= LED_7;
	LED_8_DDR |= LED_8;
	LED_8_PORT |= LED_8;

	LED__P_DDR |= LED__P;
	LED__P_PORT |= LED__P;
}
/********************************************************************************/
void speaker_initialize(void) {
	_SPEAKER_DDR |= _SPEAKER;
	_SPEAKER_PORT &= ~_SPEAKER;
}
/********************************************************************************/
void remote_initialize(void) {
	// External Interrupt(s) initialization
	// INT0: Off
	// INT1: Off
	// INT2: Off
	// INT3: Off
	// INT4: Off
	// INT5: Off
	// INT6: On
	// INT6 Mode: Any change
	// INT7: Off
	EICRA = 0x00;
	EICRB = 0x10;
	EIMSK = 0x40;
	EIFR = 0x40;

	t0_initialize();
}
/********************************************************************************/
void input_initialize(void) {
	INPUT_DDR &= ~INPUT_1;
	INPUT_DDR &= ~INPUT_2;
	INPUT_DDR &= ~INPUT_3;
	INPUT_DDR &= ~INPUT_4;

	INPUT_PORT |= INPUT_1;
	INPUT_PORT |= INPUT_2;
	INPUT_PORT |= INPUT_3;
	INPUT_PORT |= INPUT_4;
}
/********************************************************************************/
void initialize(void) {
	led_initialize();
	button_initialize();
	switch_initialize();
	speaker_initialize();
	motor_initialize();

	input_initialize();
	ADC_initialize();

	remote_initialize();
	//usart0_initialize();
	//usart1_initialize();

	LCD_Initialize();
	fdevopen(LCD_WD, NULL );
}
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/*
 Funkcja generuje sygna� prostok�tny na wyprowadzeniu do kt�rego
 przy��czony jest  buzzer. Funkcja przyjmuje argumenty:
 cz�stotliwo��(Hz) sygna�u i du�go�� czasu trwania sygna�u (ms).
 */
void Beep(unsigned int frequency, unsigned int duration) {

	//cli();

	unsigned int i, t, n;
	t = F_CPU / (8 * frequency);
	n = (F_CPU / (4 * 1000)) * duration / t;

	for (i = 0; i < n; i++) {
		FLIP(SPEAKER);
		_delay_loop_2(t);
	}

	CLR(SPEAKER);

	//sei();
}

/********************************************************************************/

#include "HD44780.h"

void log0(const char* msg) {
	LCD_GoTo(0, 0);
	printf("%s", msg);
}

void log1(const char* msg) {
	LCD_GoTo(0, 1);
	printf("%s", msg);
}

void all_leds_off(void) {
	SET(LED1);
	SET(LED2);
	SET(LED3);
	SET(LED4);
	SET(LED5);
	SET(LED6);
	SET(LED7);
	SET(LED8);
}

void all_leds_on(void) {
	CLR(LED1);
	CLR(LED2);
	CLR(LED3);
	CLR(LED4);
	CLR(LED5);
	CLR(LED6);
	CLR(LED7);
	CLR(LED8);
}
