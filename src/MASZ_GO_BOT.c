/********************************************************************************/
/********************************************************************************/
/**   ALEKSANDER KSIAZEK i PAWEL LIPSKI   *   MASZ_GO_BOT   *   VERSION  1.0   **/
/********************************************************************************/
/********************************************************************************/

/*
 JAK PODLACZYC CZUJNIKI (opis wejsc od LEWEJ do PRAWEJ patrz�c na sterownik od strony wyswietlacza):
 PORTY ANALOGOWE:
 A1 <= LEWY CZUJNIK SWIATLA
 A2
 A3 <= LEWY CZUJNIK ODLEGLOSCI
 A4 <= SRODKOWY CZUJNIK ODLEGLOSCI
 A5 <= PRAWY CZUJNIK ODLEGLOSCI
 A6
 A7 <= PRAWY CZUJNIK SWIATLA
 A8

 PORTY CYFORWE:
 I1 <= LEWY ZDERZAK
 I2 <= PRAWY ZDERZAK
 I3 <= czujnik LINII
 I4 <= czujnik LINII (jesli robot ucieka z linii: zamien wtyczki z I3 i I4)

 */

#include "functions.h"

#define REMOTE_LEFT 88
#define REMOTE_OK 104
#define REMOTE_RIGHT 216

void Omin_pudelko_P(char silnik_a, char silnik_b, int obrot1, int prosta1,
		int obrot2, int prosta2, int obrot3, int prosta3, int obrot4) {
	Predkosc(silnik_a, silnik_b);
	Stop();
	Prawo();
	_delay_ms(obrot1);
	Jedz();
	_delay_ms(prosta1);
	Lewo();
	_delay_ms(obrot2);
	Jedz();
	_delay_ms(prosta2);
	Lewo();
	_delay_ms(obrot3);
	Jedz();
	_delay_ms(prosta3);
	Prawo();
	_delay_ms(obrot3);
	Jedz();
}

void Omin_pudelko_L(char silnik_a, char silnik_b, int obrot1, int prosta1,
		int obrot2, int prosta2, int obrot3, int prosta3, int obrot4) {
	Predkosc(silnik_a, silnik_b);
	Stop();
	Lewo();
	_delay_ms(obrot1);
	Jedz();
	_delay_ms(prosta1);
	Prawo();
	_delay_ms(obrot2);
	Jedz();
	_delay_ms(prosta2);
	Prawo();
	_delay_ms(obrot3);
	Jedz();
	_delay_ms(prosta3);
	Lewo();
	_delay_ms(obrot3);
	Jedz();
}

void Omin(char silnik_a, char silnik_b, int obrot1, int prosta1, int obrot2,
		int prosta2, int obrot3) {
	Predkosc(silnik_a, silnik_b);
	Stop();
	Lewo();
	_delay_ms(obrot1);
	Jedz();
	_delay_ms(prosta1);
	Prawo();
	_delay_ms(obrot2);
	Jedz();
	_delay_ms(prosta2);
	Prawo();
	_delay_ms(obrot3);
	Jedz();

	while (GET(INPUT3) && GET(INPUT4))
		;
}

#include "melodie.h"
void Play(int name[][2], char repeat);
void Play(int name[][2], char repeat) {
	int i;

	while (repeat--) {
		for (i = 0; name[i][0]; i++)
			Beep(name[i][1], (1200 / name[i][0]));
	}

}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
volatile int licznik_10us = 0;
volatile int licznik_1ms = 0;
volatile int licznik_1s = 0;

volatile char repeat = 0;
volatile char r_tx = 0;

void zeruj(void);
void zeruj(void) {
	licznik_10us = 0;
	licznik_1ms = 0;
	licznik_1s = 0;
}

ISR( TIMER0_OVF_vect) {
	TCNT0 = 197;
	licznik_10us++;
	if (licznik_10us == 100) {
		licznik_10us = 0;
		licznik_1ms++;
		if (repeat)
			repeat--;
		if (r_tx)
			r_tx--;
		if (licznik_1ms == 1000) {
			licznik_1ms = 0;
			licznik_1s++;
		}
	}
}
/********************************************************************************/
volatile char r_mode = 0;
volatile char pass = 0;
volatile char bit_licznik = 0;

volatile unsigned char address1 = 0;
volatile unsigned char address2 = 0;
volatile unsigned char command1 = 0;
volatile unsigned char command2 = 0;
volatile unsigned char pilot = 0;
volatile unsigned long ramka = 0;

void R_blink(void);
void R_blink(void) {
	CLR(LED_P);
	_delay_ms(30);
	SET(LED_P);
}

ISR( INT6_vect) {
	if ((r_mode == 0) && !r_tx) {
		zeruj();
		r_mode = 1;
	} else if ((r_mode == 1) && r_tx) {
		if ((licznik_1ms > 7) && (licznik_1ms < 11)) {
			r_mode = 2;
		} else {
			r_mode = 0;
		}
		zeruj();
	} else if ((r_mode == 2) && r_tx) {
		if ((licznik_1ms >= 4) && (licznik_1ms < 6)) {
			r_mode = 3;
		} else if (licznik_1ms >= 2) {
			r_mode = 10; // Repraet code
		} else {
			r_mode = 0;
		}
		zeruj();
	} else if ((r_mode == 3) && r_tx) {
		if (pass) {
			if (licznik_1ms >= 2)
				ramka |= 0x00000001;
			else
				ramka &= ~(0x00000001);
			if (bit_licznik < 31)
				ramka <<= 1;

			bit_licznik++;
			zeruj();
			pass = 0;
		} else {
			if (bit_licznik == 32) {
				command2 = ramka & 0x000000FF;
				ramka >>= 8;
				command1 = ramka & 0x000000FF;
				ramka >>= 8;
				address2 = ramka & 0x000000FF;
				ramka >>= 8;
				address1 = ramka & 0x000000FF;

				//if ((address1 + address2) == 0xFF){
				if ((command1 + command2) == 0xFF) {
					pilot = command1;
					repeat = 120;
					R_blink();
				}
				//}

				ramka = 0;
				r_mode = 0;
				bit_licznik = 0;
			} else
				pass = 1;
		}
	} else if ((r_mode == 10) && repeat && r_tx) {
		pilot = command1;
		repeat = 120;
		r_mode = 0;
		R_blink();
	} else {
		r_mode = 0;
		pass = 0;
		bit_licznik = 0;
		zeruj();
	}

	r_tx = 10;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

int running = 0;
void Blink(int czekaj);
void Blink(int czekaj) {

	sei();

	int leds = 0;

	unsigned char speed_A = 180;
	unsigned char speed_B = 180;

	while (running) {

		//LCD_GoTo(0, 1);
		//printf("%8d%8d",pilot,licznik_1s);

		if (pilot == 152) {
			Cofaj();
			pilot = 0;
		} else if (pilot == 24) {
			Jedz();
			pilot = 0;
		} else if (pilot == 216) {
			Prawo();
			pilot = 0;
		} else if (pilot == 88) {
			Lewo();
			pilot = 0;
		} else if (pilot == 176) {
			if (speed_A < 255)
				speed_A++;
			pilot = 0;
			LCD_GoTo(0, 1);
			printf("%3d          %3d", speed_A, speed_B);
		} else if (pilot == 66) {
			if (speed_A > 0)
				speed_A--;
			pilot = 0;
			LCD_GoTo(0, 1);
			printf("%3d          %3d", speed_A, speed_B);
		} else if (pilot == 136) {
			if (speed_B < 255)
				speed_B++;
			pilot = 0;
			LCD_GoTo(0, 1);
			printf("%3d          %3d", speed_A, speed_B);
		} else if (pilot == 98) {
			if (speed_B > 0)
				speed_B--;
			pilot = 0;
			LCD_GoTo(0, 1);
			printf("%3d          %3d", speed_A, speed_B);
		} else if (pilot != 0)
			Stop();

		Predkosc(speed_A, speed_B);

		if (!GET(BUTTON_L))
			running = 0;
		_delay_ms(czekaj);
		SET(LED1);
		SET(LED2);
		SET(LED3);
		SET(LED4);
		SET(LED5);
		SET(LED6);
		SET(LED7);
		SET(LED8);
		switch ((leds++) % 14) {
		case 0:
			CLR(LED1);
			break;
		case 1:
			CLR(LED2);
			break;
		case 2:
			CLR(LED3);
			break;
		case 3:
			CLR(LED4);
			break;
		case 4:
			CLR(LED5);
			break;
		case 5:
			CLR(LED6);
			break;
		case 6:
			CLR(LED7);
			break;
		case 7:
			CLR(LED8);
			break;
		case 8:
			CLR(LED7);
			break;
		case 9:
			CLR(LED6);
			break;
		case 10:
			CLR(LED5);
			break;
		case 11:
			CLR(LED4);
			break;
		case 12:
			CLR(LED3);
			break;
		case 13:
			CLR(LED2);
			break;
		default:
			break;
		}
	}
	SET(LED1);
	SET(LED2);
	SET(LED3);
	SET(LED4);
	SET(LED5);
	SET(LED6);
	SET(LED7);
	SET(LED8);

	cli();
}
void Test(void);
void Test(void) {
	while (running) {
		if (!GET(SWITCH1))
			CLR(LED1);
		else
			SET(LED1);
		if (!GET(SWITCH2))
			CLR(LED2);
		else
			SET(LED2);
		if (!GET(SWITCH3))
			CLR(LED3);
		else
			SET(LED3);
		if (!GET(SWITCH4))
			CLR(LED4);
		else
			SET(LED4);

		if (!GET(BUTTON_U))
			CLR(LED5);
		else
			SET(LED5);
		if (!GET(BUTTON_D))
			CLR(LED6);
		else
			SET(LED6);
		if (!GET(BUTTON_L))
			CLR(LED7);
		else
			SET(LED7);
		if (!GET(BUTTON_R))
			CLR(LED8);
		else
			SET(LED8);
	}
}
/********************************************************************************/
/********************************************************************************/
volatile char menu = 1;
void Wypisz_menu(char nr) {
	LCD_GoTo(0, 1);
	switch (nr) {
	case 2:
		printf(" <GRA W BERKA> ");
		break;
	case 3:
		printf("CZUJNIKI POSCIG");
		break;
	case 4:
		printf("   <HAMSTER>    ");
		break;
	case 5:
		printf("  <SWIATLOLUB>  ");
		break;
	case 6:
		printf("    <MANUAL>    ");
		break;
	case 7:
		printf("<TEST CZUJNIKOW>");
		break;
	case 8:
		printf(" <TEST PILOTA>  ");
		break;
	case 9:
		printf("  <PUDELKO L>   ");
		break;
	case 10:
		printf("  <PUDELKO P>   ");
		break;
	default:
		printf("     <MENU>     ");
		break;
	}
}
void Run(char nr) {
	LCD_GoTo(0, 1);
	switch (nr) {
	case 2:
		printf(">>GRA W BERKA<<");
		CLR(LED1);
		CLR(LED2);
		CLR(LED3);
		CLR(LED4);
		CLR(LED5);
		CLR(LED6);
		CLR(LED7);
		CLR(LED8);

		unsigned int lewo, prawo, srodek;
		unsigned int prev_lewo, prev_prawo, prev_srodek;

		pilot = 0;

		LCD_Clear();
		LCD_GoTo(0, 0);
		printf("R: START,L: BACK");
		LCD_GoTo(0, 1);
		printf("Pilot: STOP");
		_delay_ms(500);
		while (((lewo = GET(BUTTON_L)) == 1)&&GET(BUTTON_R))
			;
		if (!lewo) {
			SET(LED1);
			SET(LED2);
			SET(LED3);
			SET(LED4);
			SET(LED5);
			SET(LED6);
			SET(LED7);
			SET(LED8);
			break;
		}

		LCD_Clear();
		LCD_GoTo(0, 0);
		printf("STOPUJ Pilotem");

		/* inteligente zachowanie dojdzie */
		int exit = 0;
		while (!exit) {
			unsigned int i = 0;

			Predkosc(220, 180);
			Jedz();
			while (running) {
				//Beep(William[i][1],(1200/William[i][0]));
				sei();

				if (William[i][0] == 0)
					i = 0;

				unsigned int frequency = William[i][1];
				unsigned int duration = (1200 / William[i][0]);

				unsigned int j, t, n;
				t = F_CPU / (8 * frequency);
				n = (F_CPU / (4 * 1000)) * duration / t;

				for (j = 0; j < n * 5 / 4; j++) {
					FLIP(SPEAKER);
					_delay_loop_2(t / 7);
				}

				CLR(SPEAKER);

				i++;

				lewo = read_adc(2);
				srodek = read_adc(3);
				prawo = read_adc(4);

				LCD_GoTo(0, 0);
				printf("   OK => stop   ");
				LCD_GoTo(0, 1);
				printf("%4u  %4u  %4u", lewo, srodek, prawo);

				if (!GET(BUTTON_L))
					running = 0;
				if (pilot == REMOTE_OK) {
					running = 0;
					pilot = 0;

					Stop();
					Predkosc(0, 0);
				}
				_delay_ms(10);

				cli();
			}

			LCD_GoTo(0, 0);
			printf(" OK/R => resume ");
			LCD_GoTo(0, 1);
			printf("  Left => exit  ");

			sei();
			while (pilot != REMOTE_LEFT && pilot != REMOTE_OK && pilot != REMOTE_RIGHT) {
				//printf("pilot = %4u", pilot);
				_delay_ms(10);
			}
			cli();

			if (pilot == REMOTE_LEFT)
				exit = 1;
			else { // pilot == REMOTE_OK && pilot == REMOTE_RIGHT
				pilot = 0;
				running = 1;
			}
		}

		SET(LED1);
		SET(LED2);
		SET(LED3);
		SET(LED4);
		SET(LED5);
		SET(LED6);
		SET(LED7);
		SET(LED8);
		break;
	case 3:
		printf(">TEST CZUJNIKOW<");
		const int lspeed_def = 220, rspeed_def = 180;
		int lspeed = lspeed_def, rspeed = rspeed_def;
		Predkosc(lspeed, rspeed);
		Jedz();
		CLR(LED1);
		CLR(LED2);
		CLR(LED3);
		CLR(LED4);
		CLR(LED5);
		CLR(LED6);
		CLR(LED7);
		CLR(LED8);
		while (running) {

			_delay_ms(150);
			lewo = read_adc(2);
			srodek = read_adc(3);
			prawo = read_adc(4);
			LCD_GoTo(0, 0);
			printf("%4u  %4u  %4u", lewo, srodek, prawo);

			LCD_GoTo(0, 1);
			//unsigned int zderz1 = read_adc(6), zderz2 = read_adc(7);
			printf("%u %u", GET(INPUT1), GET(INPUT2));
			if (!GET(INPUT1) || !GET(INPUT2)) {
				Stop();
				break;
			}

			if (lewo > srodek - 20 && lewo > prawo)
				Predkosc(lspeed += 10, rspeed);
			else if (prawo > srodek - 20 && prawo > lewo)
				Predkosc(lspeed, rspeed += 10);
			else
				Predkosc(lspeed = lspeed_def, rspeed = rspeed_def);
			if (!GET(BUTTON_L))
				running = 0;
		}
		SET(LED1);
		SET(LED2);
		SET(LED3);
		SET(LED4);
		SET(LED5);
		SET(LED6);
		SET(LED7);
		SET(LED8);
		break;

		/*
		 printf(">LINEFOLLOWER E<");
		 CLR(LED3);CLR(LED6);
		 LCD_GoTo(0, 0);
		 printf("Nacisnij START  ");*/

		// 500,1200,500,2500,300
		volatile char silnik_a = 150;
		volatile char silnik_b = 150;
		volatile int obrot1 = 500;
		volatile int prosta1 = 1200;
		volatile int obrot2 = 500;
		volatile int prosta2 = 2500;
		volatile int obrot3 = 300;

		/*
		 sei();
		 while(GET(BUTTON_U) && GET(BUTTON_L)){


		 if(pilot == 176) { if(silnik_a<255) silnik_a++;
		 LCD_GoTo(0, 1);
		 printf("%3d          %3d",silnik_a,silnik_b);
		 pilot = 0;
		 }
		 if(pilot == 66) { if(silnik_a>0) silnik_a--;
		 LCD_GoTo(0, 1);
		 printf("%3d          %3d",silnik_a,silnik_b);
		 pilot = 0;
		 }
		 if(pilot == 136) { if(silnik_b<255) silnik_b++;
		 LCD_GoTo(0, 1);
		 printf("%3d          %3d",silnik_a,silnik_b);
		 pilot = 0;
		 }
		 if(pilot == 98) { if(silnik_b>0) silnik_b--;
		 LCD_GoTo(0, 1);
		 printf("%3d          %3d",silnik_a,silnik_b);
		 pilot = 0;
		 }

		 // obrot1
		 if(pilot == 128) { obrot1 += 50;
		 LCD_GoTo(0, 1);
		 printf("obrot1 = %4d   ",obrot1);
		 pilot = 0;
		 }
		 if(pilot == 32) { obrot1 -= 50;
		 LCD_GoTo(0, 1);
		 printf("obrot1 = %4d   ",obrot1);
		 pilot = 0;
		 }
		 // prosta1
		 if(pilot == 64) { prosta1 += 100;
		 LCD_GoTo(0, 1);
		 printf("prosta1 = %4d  ",prosta1);
		 pilot = 0;
		 }
		 if(pilot == 160) { prosta1 -= 100;
		 LCD_GoTo(0, 1);
		 printf("prosta1 = %4d  ",prosta1);
		 pilot = 0;
		 }
		 // obort2
		 if(pilot == 192) { obrot2 += 50;
		 LCD_GoTo(0, 1);
		 printf("obrot2 = %4d   ",obrot2);
		 pilot = 0;
		 }
		 if(pilot == 96) { obrot2 -= 50;
		 LCD_GoTo(0, 1);
		 printf("obrot2 = %4d   ",obrot2);
		 pilot = 0;
		 }
		 // prosta2
		 if(pilot == 224) { prosta2 += 100;
		 LCD_GoTo(0, 1);
		 printf("prosta2 = %4d  ",prosta2);
		 pilot = 0;
		 }
		 if(pilot == 208) { prosta2 -= 100;
		 LCD_GoTo(0, 1);
		 printf("prosta2 = %4d  ",prosta2);
		 pilot = 0;
		 }
		 // obrot3
		 if(pilot == 144) { obrot3 += 50;
		 LCD_GoTo(0, 1);
		 printf("obrot3 = %4d   ",obrot3);
		 pilot = 0;
		 }
		 if(pilot == 80) { obrot3 -= 50;
		 LCD_GoTo(0, 1);
		 printf("obrot3 = %4d   ",obrot3);
		 pilot = 0;
		 }


		 }
		 cli();

		 LCD_GoTo(0, 1);
		 printf(">LINEFOLLOWER E<");

		 Jedz();
		 LCD_GoTo(0, 0);
		 printf("Nacisnij STOP   ");*/
		int odleglosc = 0;
		/*while(running){
		 odleglosc = read_adc(3);
		 LCD_GoTo(0, 0);
		 printf("odleglosc = %4d",odleglosc);
		 if(odleglosc > 750) Omin(silnik_a,silnik_b,obrot1,prosta1,obrot2,prosta2,obrot3);

		 if(!GET(INPUT4) && GET(INPUT3)) { CLR(LED1); SET(LED8); Predkosc(50,200); while(GET(INPUT3));}
		 else if(!GET(INPUT3) && GET(INPUT4)) { SET(LED1); CLR(LED8); Predkosc(200,50); while(GET(INPUT4));}
		 else { Predkosc(200,200); if(GET(INPUT3)){ SET(LED1); SET(LED8); } else { CLR(LED1); CLR(LED8); }
		 if(!GET(BUTTON_L) || !GET(BUTTON_D)) running = 0; }
		 }
		 Stop();
		 Predkosc(0,0);
		 SET(LED1);SET(LED8);
		 SET(LED3);SET(LED6);*/
		break;
	case 4:
		printf("  >>HAMSTER<<   ");
		CLR(LED4);
		CLR(LED5);
		LCD_GoTo(0, 0);
		printf("Nacisnij START  ");
		while (GET(BUTTON_U) && GET(BUTTON_L))
			;
		Predkosc(150, 150);
		Jedz();
		LCD_GoTo(0, 0);
		printf("Nacisnij STOP   ");

		while (running) {
			if (!GET(INPUT1)) {
				CLR(LED1);
				Beep(1500, 200);
				Cofaj();
				_delay_ms(400);
				Prawo();
				_delay_ms(500);
				Jedz();
			} else
				SET(LED1);
			if (!GET(INPUT2)) {
				CLR(LED8);
				Beep(2000, 200);
				Cofaj();
				_delay_ms(400);
				Lewo();
				_delay_ms(500);
				Jedz();
			} else
				SET(LED8);
			if (!GET(BUTTON_L) || !GET(BUTTON_D))
				running = 0;
		}

		Stop();

		SET(LED1);
		SET(LED8);
		SET(LED4);
		SET(LED5);
		break;
	case 5:
		printf(" >>SWIATLOLUB<< ");
		CLR(LED2);
		CLR(LED7);

		volatile char czulosc = 230;
		volatile char czujnik_prawy = 0;
		volatile char czujnik_lewy = 0;
		volatile char na_lewe_kolo, na_prawe_kolo;
		volatile int predkosc_L, predkosc_P;

		LCD_GoTo(0, 0);
		printf("Nacisnij START  ");

		sei();
		while (GET(BUTTON_U) && GET(BUTTON_L)) {
			if (pilot == 24) {
				pilot = 0;
				if (czulosc != 255)
					czulosc += 1;
				LCD_GoTo(0, 1);
				printf("czulosc = %3d   ", czulosc);
			}
			if (pilot == 152) {
				pilot = 0;
				if (czulosc != 0)
					czulosc -= 1;
				LCD_GoTo(0, 1);
				printf("czulosc = %3d   ", czulosc);
			}
		}
		cli();

		LCD_GoTo(0, 1);
		printf(" >>SWIATLOLUB<< ");

		Predkosc(0, 0);
		Jedz();
		LCD_GoTo(0, 0);
		printf("Nacisnij STOP   ");

		while (running) {
			czujnik_prawy = read_adc(6);
			czujnik_lewy = read_adc(0);

			if (czujnik_prawy > czulosc)
				na_lewe_kolo = czujnik_prawy - czulosc;
			else
				na_lewe_kolo = 0;
			if (czujnik_lewy > czulosc)
				na_prawe_kolo = czujnik_lewy - czulosc;
			else
				na_prawe_kolo = 0;

			predkosc_L = (na_lewe_kolo * 255) / (255 - czulosc);
			predkosc_P = (na_prawe_kolo * 255) / (255 - czulosc);

			Predkosc(predkosc_L, predkosc_P);

			if (!GET(BUTTON_L) || !GET(BUTTON_D))
				running = 0;
		}
		Stop();
		SET(LED2);
		SET(LED7);
		break;
	case 6:
		printf("   >>MANUAL<<   ");
		Predkosc(180, 180);
		while (running) {
			if (!GET(BUTTON_D))
				Cofaj();
			else if (!GET(BUTTON_U))
				Jedz();
			else if (!GET(BUTTON_R))
				Prawo();
			else if (!GET(BUTTON_L))
				Lewo();
			else
				Stop();
		}
		break;
	case 7:
		printf(">TEST CZUJNIKOW<");
		CLR(LED1);
		CLR(LED2);
		CLR(LED3);
		CLR(LED4);
		CLR(LED5);
		CLR(LED6);
		CLR(LED7);
		CLR(LED8);
		while (running) {
			_delay_ms(150);
			lewo = read_adc(2);
			srodek = read_adc(3);
			prawo = read_adc(4);
			LCD_GoTo(0, 0);
			printf("%4u  %4u  %4u", lewo, srodek, prawo);
			if (!GET(BUTTON_L))
				running = 0;
		}
		SET(LED1);
		SET(LED2);
		SET(LED3);
		SET(LED4);
		SET(LED5);
		SET(LED6);
		SET(LED7);
		SET(LED8);
		break;
	case 8:
		printf(" >TEST PILOTA<  ");
		pilot = 0;
		CLR(LED1);
		CLR(LED2);
		CLR(LED3);
		CLR(LED4);
		CLR(LED5);
		CLR(LED6);
		CLR(LED7);
		CLR(LED8);
		sei();
		while (running) {
			_delay_ms(10);
			LCD_GoTo(0, 0);
			printf("KOMENDA = %3d   ", pilot);
			if (!GET(BUTTON_L))
				running = 0;
		}
		cli();
		SET(LED1);
		SET(LED2);
		SET(LED3);
		SET(LED4);
		SET(LED5);
		SET(LED6);
		SET(LED7);
		SET(LED8);
		break;

	case 9:
		printf("  >PUDELKO L<   ");

		silnik_a = 150;
		silnik_b = 150;
		obrot1 = 170;
		prosta1 = 500;
		obrot2 = 170;
		prosta2 = 900;
		obrot3 = 170;
		volatile int prosta3 = 500;
		volatile int obrot4 = 170;

		pilot = 0;

		sei();
		while (pilot != 104) {

			if (pilot == 176) {
				if (silnik_a < 255)
					silnik_a++;
				LCD_GoTo(0, 1);
				printf("%3d          %3d", silnik_a, silnik_b);
				pilot = 0;
			}
			if (pilot == 66) {
				if (silnik_a > 0)
					silnik_a--;
				LCD_GoTo(0, 1);
				printf("%3d          %3d", silnik_a, silnik_b);
				pilot = 0;
			}
			if (pilot == 136) {
				if (silnik_b < 255)
					silnik_b++;
				LCD_GoTo(0, 1);
				printf("%3d          %3d", silnik_a, silnik_b);
				pilot = 0;
			}
			if (pilot == 98) {
				if (silnik_b > 0)
					silnik_b--;
				LCD_GoTo(0, 1);
				printf("%3d          %3d", silnik_a, silnik_b);
				pilot = 0;
			}

			// obrot1
			if (pilot == 128) {
				obrot1 += 5;
				LCD_GoTo(0, 1);
				printf("obrot1 = %4d   ", obrot1);
				pilot = 0;
			}
			if (pilot == 32) {
				obrot1 -= 5;
				LCD_GoTo(0, 1);
				printf("obrot1 = %4d   ", obrot1);
				pilot = 0;
			}
			// prosta1
			if (pilot == 64) {
				prosta1 += 10;
				LCD_GoTo(0, 1);
				printf("prosta1 = %4d  ", prosta1);
				pilot = 0;
			}
			if (pilot == 160) {
				prosta1 -= 10;
				LCD_GoTo(0, 1);
				printf("prosta1 = %4d  ", prosta1);
				pilot = 0;
			}
			// obort2
			if (pilot == 192) {
				obrot2 += 5;
				LCD_GoTo(0, 1);
				printf("obrot2 = %4d   ", obrot2);
				pilot = 0;
			}
			if (pilot == 96) {
				obrot2 -= 5;
				LCD_GoTo(0, 1);
				printf("obrot2 = %4d   ", obrot2);
				pilot = 0;
			}
			// prosta2
			if (pilot == 224) {
				prosta2 += 10;
				LCD_GoTo(0, 1);
				printf("prosta2 = %4d  ", prosta2);
				pilot = 0;
			}
			if (pilot == 208) {
				prosta2 -= 10;
				LCD_GoTo(0, 1);
				printf("prosta2 = %4d  ", prosta2);
				pilot = 0;
			}
			// obrot3
			if (pilot == 144) {
				obrot3 += 5;
				LCD_GoTo(0, 1);
				printf("obrot3 = %4d   ", obrot3);
				pilot = 0;
			}
			if (pilot == 80) {
				obrot3 -= 5;
				LCD_GoTo(0, 1);
				printf("obrot3 = %4d   ", obrot3);
				pilot = 0;
			}
			// prosta3
			if (pilot == 56) {
				prosta3 += 10;
				LCD_GoTo(0, 1);
				printf("prosta3 = %4d  ", prosta3);
				pilot = 0;
			}
			if (pilot == 184) {
				prosta3 -= 10;
				LCD_GoTo(0, 1);
				printf("prosta3 = %4d  ", prosta3);
				pilot = 0;
			}
			// obrot4
			if (pilot == 194) {
				obrot4 += 5;
				LCD_GoTo(0, 1);
				printf("obrot4 = %4d   ", obrot4);
				pilot = 0;
			}
			if (pilot == 34) {
				obrot4 -= 5;
				LCD_GoTo(0, 1);
				printf("obrot4 = %4d   ", obrot4);
				pilot = 0;
			}

		}

		LCD_GoTo(0, 1);
		printf("  >PUDELKO L<   ");

		Jedz();
		odleglosc = 0;
		while (running) {
			Predkosc(silnik_a, silnik_b);
			odleglosc = read_adc(3);
			LCD_GoTo(0, 0);
			printf("odleglosc = %4d", odleglosc);
			if (odleglosc > 750)
				Omin_pudelko_L(silnik_a, silnik_b, obrot1, prosta1, obrot2,
						prosta2, obrot3, prosta3, obrot4);

			if (pilot == 8) {
				running = 0;
				pilot = 0;
			}
		}
		Stop();
		Predkosc(0, 0);
		cli();

		break;
	case 10:
		printf("  >PUDELKO P<   ");

		silnik_a = 150;
		silnik_b = 150;
		obrot1 = 170;
		prosta1 = 500;
		obrot2 = 170;
		prosta2 = 900;
		obrot3 = 170;
		prosta3 = 500;
		obrot4 = 170;

		pilot = 0;

		sei();
		while (pilot != 104) {

			if (pilot == 176) {
				if (silnik_a < 255)
					silnik_a++;
				LCD_GoTo(0, 1);
				printf("%3d          %3d", silnik_a, silnik_b);
				pilot = 0;
			}
			if (pilot == 66) {
				if (silnik_a > 0)
					silnik_a--;
				LCD_GoTo(0, 1);
				printf("%3d          %3d", silnik_a, silnik_b);
				pilot = 0;
			}
			if (pilot == 136) {
				if (silnik_b < 255)
					silnik_b++;
				LCD_GoTo(0, 1);
				printf("%3d          %3d", silnik_a, silnik_b);
				pilot = 0;
			}
			if (pilot == 98) {
				if (silnik_b > 0)
					silnik_b--;
				LCD_GoTo(0, 1);
				printf("%3d          %3d", silnik_a, silnik_b);
				pilot = 0;
			}

			// obrot1
			if (pilot == 128) {
				obrot1 += 5;
				LCD_GoTo(0, 1);
				printf("obrot1 = %4d   ", obrot1);
				pilot = 0;
			}
			if (pilot == 32) {
				obrot1 -= 5;
				LCD_GoTo(0, 1);
				printf("obrot1 = %4d   ", obrot1);
				pilot = 0;
			}
			// prosta1
			if (pilot == 64) {
				prosta1 += 10;
				LCD_GoTo(0, 1);
				printf("prosta1 = %4d  ", prosta1);
				pilot = 0;
			}
			if (pilot == 160) {
				prosta1 -= 10;
				LCD_GoTo(0, 1);
				printf("prosta1 = %4d  ", prosta1);
				pilot = 0;
			}
			// obort2
			if (pilot == 192) {
				obrot2 += 5;
				LCD_GoTo(0, 1);
				printf("obrot2 = %4d   ", obrot2);
				pilot = 0;
			}
			if (pilot == 96) {
				obrot2 -= 5;
				LCD_GoTo(0, 1);
				printf("obrot2 = %4d   ", obrot2);
				pilot = 0;
			}
			// prosta2
			if (pilot == 224) {
				prosta2 += 10;
				LCD_GoTo(0, 1);
				printf("prosta2 = %4d  ", prosta2);
				pilot = 0;
			}
			if (pilot == 208) {
				prosta2 -= 10;
				LCD_GoTo(0, 1);
				printf("prosta2 = %4d  ", prosta2);
				pilot = 0;
			}
			// obrot3
			if (pilot == 144) {
				obrot3 += 5;
				LCD_GoTo(0, 1);
				printf("obrot3 = %4d   ", obrot3);
				pilot = 0;
			}
			if (pilot == 80) {
				obrot3 -= 5;
				LCD_GoTo(0, 1);
				printf("obrot3 = %4d   ", obrot3);
				pilot = 0;
			}
			// prosta3
			if (pilot == 56) {
				prosta3 += 10;
				LCD_GoTo(0, 1);
				printf("prosta3 = %4d  ", prosta3);
				pilot = 0;
			}
			if (pilot == 184) {
				prosta3 -= 10;
				LCD_GoTo(0, 1);
				printf("prosta3 = %4d  ", prosta3);
				pilot = 0;
			}
			// obrot4
			if (pilot == 194) {
				obrot4 += 5;
				LCD_GoTo(0, 1);
				printf("obrot4 = %4d   ", obrot4);
				pilot = 0;
			}
			if (pilot == 34) {
				obrot4 -= 5;
				LCD_GoTo(0, 1);
				printf("obrot4 = %4d   ", obrot4);
				pilot = 0;
			}

		}

		LCD_GoTo(0, 1);
		printf("  >PUDELKO P<   ");

		Jedz();
		odleglosc = 0;
		while (running) {
			Predkosc(silnik_a, silnik_b);
			odleglosc = read_adc(3);
			LCD_GoTo(0, 0);
			printf("odleglosc = %4d", odleglosc);
			if (odleglosc > 750)
				Omin_pudelko_P(silnik_a, silnik_b, obrot1, prosta1, obrot2,
						prosta2, obrot3, prosta3, obrot4);

			if (pilot == 8) {
				running = 0;
				pilot = 0;
			}
		}
		Stop();
		Predkosc(0, 0);
		cli();

		break;

	default:
		printf("     <MENU>     ");
		Beep(1000, 200);
		break;
	}

	LCD_GoTo(0, 0);
	printf("MASZ GO!BOT v1.0");
}

/********************************************************************************/
int main(void) {

	initialize();

	LCD_Clear();
	LCD_GoTo(0, 0);
	printf("MASZ GO!BOT v1.0");
	LCD_GoTo(0, 1);
	printf("#KSIAZEK #LIPSKI");

	/* przywitaj dzwiekiem firmy Intel */

	/*
	 Play(Intel1, 1);
	 _delay_ms(500);
	 Play(Intel2, 1);
	 */

	/* info: jedz() i cofaj() z predkoscia 220, 180 aby by�o w miar� po linii */

	/*
	 while(GET(BUTTON_U)); // czekaj
	 Predkosc(220,180);
	 Jedz();
	 int i;
	 for(i = 0; William[i][0];i++) {
	 Beep(William[i][1],(1200/William[i][0]));
	 //Jedz();
	 //_delay_ms(10);
	 }
	 Stop();

	 //Play(William,1);
	 //while(1);
	 */

	running = 1;
	//Test();
	Blink(10);

	//running = 1;
	//Test();

/////////////////////////////////////////

	Wypisz_menu(menu);

	while (1) {
		_delay_ms(200);
		if (!GET(BUTTON_D)) {
			if (++menu == 11) {
				menu = 10;
				Beep(500, 200);
			}
			Wypisz_menu(menu);
			while (!GET(BUTTON_D))
				;
		}
		if (!GET(BUTTON_U)) {
			if (--menu == 0) {
				menu = 1;
				Beep(500, 200);
			}
			Wypisz_menu(menu);
			while (!GET(BUTTON_U))
				;
		}
		if (!GET(BUTTON_R)) {
			running = 1;
			Run(menu);
			Wypisz_menu(menu);
		}
	}

	return 0;
}
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
