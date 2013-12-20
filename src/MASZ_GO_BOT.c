/********************************************************************************/
/********************************************************************************/
/**   ALEKSANDER KSIAZEK i PAWEL LIPSKI   *   MASZ_GO_BOT   *   VERSION  1.1   **/
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

#define REMOTE_MENU 130
#define REMOTE_SOUND 200
#define REMOTE_UP 24
#define REMOTE_LEFT 88
#define REMOTE_RIGHT 216
#define REMOTE_DOWN 152

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

		if (!GET(BUTTON_L) || pilot == REMOTE_MENU)
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
		printf("    <MANUAL>    ");
		break;
	case 4:
		printf("<TEST CZUJNIKOW>");
		break;
	case 5:
		printf(" <TEST PILOTA>  ");
		break;
	default:
		printf("     <MENU>     ");
		break;
	}
}
void Run(char nr) {
	cli();
	LCD_GoTo(0, 1);
	switch (nr) {
	case 2:
		printf(">>GRA W BERKA<<");
		all_leds_off();

		unsigned int lewo, prawo, srodek;
		int sound_on = 0;
		int (*music)[][2] = &King;
		unsigned int i = 0;

		enum {
			PURSUIT_target_visible, PURSUIT_target_not_visible, PURSUIT_radar
		};
		unsigned int state = PURSUIT_target_not_visible;

		sei();
		pilot = 0;

		LCD_Clear();
		LCD_GoTo(0, 0);
		printf("R: START,L: BACK");
		LCD_GoTo(0, 1);
		printf("Pilot: STOP");
		_delay_ms(500);
		while (((lewo = GET(BUTTON_L)) == 1)&&GET(BUTTON_R)
		&& pilot != REMOTE_RIGHT && pilot != REMOTE_LEFT)
			;
		if (!lewo || pilot == REMOTE_LEFT) {
			all_leds_on();
			break;
		}

		pilot = 0;
		LCD_Clear();
		LCD_GoTo(0, 0);
		printf("STOPUJ Pilotem");

		int exit = 0;
		while (!exit) {

			const int lspeed_def = 230, rspeed_def = 170;
			int lspeed = lspeed_def, rspeed = rspeed_def;
			unsigned radar_interval_ticks = 0, invisibility_patience_ticks = 0,
					radar_to_do_ticks = 0;

			while (running) {
				Predkosc(lspeed, rspeed);
				Jedz();

				sei();

				if (sound_on) {
					if ((*music)[i][0] == 0) {
						// zmiana utworu
						if (music == &King)
							music = &William;
						else
							music = &King;
						i = 0; // gramy muzyke w kolko
					}

					// kod funkcji Beep przeniesiony tutaj, troche zmieniony,
					// zeby mimo wlaczonej obslugi przerwan
					// efekt byl taki sam, jak gdyby nie byla wlaczona

					unsigned int frequency = (*music)[i][1];
					unsigned int duration = (1200 / (*music)[i][0]);

					unsigned int j, t, n;
					t = F_CPU / (8 * frequency);
					n = (F_CPU / (4 * 1000)) * duration / t;

					for (j = 0; j < n * 5 / 4; j++) { // 1 zmiana: z n na (n * 5 / 4)
						FLIP(SPEAKER);
						_delay_loop_2(t / 7); // 2 zmiana: z t na t / 7
					}

					CLR(SPEAKER);
					i++;
				}

				lewo = read_adc(2);
				srodek = read_adc(3);
				prawo = read_adc(4);

				/*LCD_GoTo(0, 1);
				 printf("%4u  %4u  %4u", lewo, srodek, prawo);
				 _delay_ms(500);
				 continue;*/

				if (radar_interval_ticks > 0)
					radar_interval_ticks--;

				if (!GET(INPUT1) || !GET(INPUT2)) { // wykrywanie zderzenia
					Cofaj();
					_delay_ms(50);
					Stop();
					_delay_ms(20);
					Prawo();
					_delay_ms(700); // ! zamiast tego delay znalezc sposob jak mierzyc zegar
					Stop();
					_delay_ms(20);
					Jedz();
					_delay_ms(50);
					running = 0;
					Stop();
				} else if (pilot == REMOTE_SOUND) {
					sound_on = !sound_on;
					pilot = 0;
				} else if (pilot != 0 && pilot != REMOTE_SOUND) {
					running = 0;
					pilot = 0;
					Stop();
					//Predkosc(0, 0);
				} else if (!GET(BUTTON_L)) {
					running = 0;
					Stop();
				} else {
					/* zachowanie w trakcie zabawy */

					int target_visible = !(lewo < 130 && srodek < 130
							&& prawo < 130);

					switch (state) {

					case PURSUIT_target_visible:
						if (!target_visible) {
							state = PURSUIT_target_not_visible;
							invisibility_patience_ticks = 24;
						} else {
							log1("Target visible");
							CLR(LED_P);

							if (lewo > srodek - 30 && lewo > prawo) {
								Predkosc(lspeed += 15, rspeed);
								left_led_on();
							} else if (prawo > srodek - 30 && prawo > lewo) {
								Predkosc(lspeed, rspeed += 15);
								right_led_on();
							} else {
								Predkosc(lspeed = lspeed_def, rspeed =
										rspeed_def);
							}
						}
						break;

					case PURSUIT_target_not_visible:
						if (!target_visible) {
							log1("Target not visible");
							SET(LED_P);

							if (radar_interval_ticks == 0) {
								if (invisibility_patience_ticks > 0) {
									--invisibility_patience_ticks;
									//unsigned diodes_on = (24 - invisibility_patience_ticks) / 4;
									//leftmost_middle_leds_on(diodes_on);
								}

								if (invisibility_patience_ticks == 0) {
									log0("Radar launched");
									middle_leds_on();

									//Predkosc(0, rspeed_def);
									lspeed = 0;
									rspeed = rspeed_def;

									state = PURSUIT_radar;
									radar_to_do_ticks = 100;
								}
							}
						} else {
							state = PURSUIT_target_visible;
							middle_leds_off();
						}
						break;

					case PURSUIT_radar:
						if (target_visible || --radar_to_do_ticks == 0) {

							log0("Radar finished");
							middle_leds_off();

							//Predkosc(lspeed_def, rspeed_def);
							lspeed = lspeed_def;
							rspeed = rspeed_def;

							radar_interval_ticks = 120;
							state = PURSUIT_target_visible;
						}
						break;
					}

				}

				_delay_ms(10);

				left_led_off();
				right_led_off();
				cli();
			}

			LCD_GoTo(0, 0);
			printf(" Right => resume ");
			LCD_GoTo(0, 1);
			printf("  Left => exit  ");

			sei();
			while (pilot != REMOTE_LEFT && pilot != REMOTE_RIGHT
					&& GET(BUTTON_L) && GET(BUTTON_R)) {
				//printf("pilot = %4u", pilot);
				_delay_ms(10);
			}
			cli();

			if (pilot == REMOTE_LEFT || !GET(BUTTON_L)) // wyjscie do glownego menu
				exit = 1;
			else if (pilot == REMOTE_RIGHT || !GET(BUTTON_R)) {
				//  resume
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
		printf("   >>MANUAL<<   ");
		Blink(10);
		break;
	case 4:
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
	case 5:
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

	default:
		printf("     <MENU>     ");
		Beep(1000, 200);
		break;
	}

	sei();
	LCD_GoTo(0, 0);
	printf("MASZ GO!BOT v1.1");
}

/********************************************************************************/
int main(void) {

	initialize();

	LCD_Clear();
	LCD_GoTo(0, 0);
	printf("MASZ GO!BOT v1.1");
	LCD_GoTo(0, 1);
	printf("#KSIAZEK #LIPSKI");

	/* przywitaj dzwiekiem firmy Intel */

	/*
	 Play(Intel1, 1);
	 _delay_ms(500);
	 Play(Intel2, 1);
	 */

	running = 1;
	Blink(10);

	sei();
	pilot = 0;

/////////////////////////////////////////

	Wypisz_menu(menu);

	while (1) {
		_delay_ms(80);

		if (!GET(BUTTON_D) || pilot == REMOTE_DOWN) {
			pilot = 0;
			if (++menu == 6) {
				menu = 5;
				Beep(500, 200);
			}
			Wypisz_menu(menu);
		}
		if (!GET(BUTTON_U) || pilot == REMOTE_UP) {
			pilot = 0;
			if (--menu == 0) {
				menu = 1;
				Beep(500, 200);
			}
			Wypisz_menu(menu);
		}
		if (!GET(BUTTON_R) || pilot == REMOTE_RIGHT) {
			pilot = 0;
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
