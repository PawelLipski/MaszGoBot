/********************************************************************************/
/********************************************************************************/
/***   MARCIN OKARMA   ***   EDU_BOT   ***   VERSION  1.0   *********************/
/********************************************************************************/
/********************************************************************************/


#include "motor.h"

/********************************************************************************/
void motor_initialize(void){
	MOTOR_DDR |= MOTOR_1;
	MOTOR_PORT |= MOTOR_1;
	MOTOR_DDR |= MOTOR_2;
	MOTOR_PORT |= MOTOR_2;
	MOTOR_DDR |= MOTOR_3;
	MOTOR_PORT |= MOTOR_3;
	MOTOR_DDR |= MOTOR_4;
	MOTOR_PORT |= MOTOR_4;

	t1_initialize();
}
/********************************************************************************/
/********************************************************************************/
inline void jedz_a(void){
	CLR(MOTOR1);
	SET(MOTOR2);
}

inline void cofaj_a(void){
	SET(MOTOR1);
	CLR(MOTOR2);
}

inline void stop_a(void){
	CLR(MOTOR2);
	CLR(MOTOR1);
}
/********************************************************************************/
inline void jedz_b(void){
	SET(MOTOR3);
	CLR(MOTOR4);
}

inline void cofaj_b(void){
	CLR(MOTOR3);
	SET(MOTOR4);
}

inline void stop_b(void){
	CLR(MOTOR3);
	CLR(MOTOR4);
}
/********************************************************************************/
/********************************************************************************/
void Jedz(void){
	jedz_a();
	jedz_b();
}

void Cofaj(void){
	cofaj_a();
	cofaj_b();
}

void Prawo(void){
	jedz_a();
	cofaj_b();
}

void Lewo(void){
	cofaj_a();
	jedz_b();
}

void Stop(void){
	stop_a();
	stop_b();
}
/********************************************************************************/
/********************************************************************************/
void Predkosc(uint8_t a , uint8_t b){
	//int motor_a = 0;
	//int motor_b = 0;

	//int min = 0;
	//int max = 255;
	//int r = max - min;

	//motor_a = r*a/100;
	//motor_a += min;
	//motor_b = r*b/100;
	//motor_b += min;

	//PWM_A = motor_a;
	//PWM_B = motor_b;

	PWM_A = a;
	PWM_B = b;
}
/********************************************************************************/
/********************************************************************************/
