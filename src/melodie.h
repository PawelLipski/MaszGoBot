/********************************************************************************/
/********************************************************************************/
/**   ALEKSANDER KSIAZEK i PAWEL LIPSKI   *   MASZ_GO_BOT   *   VERSION  1.0   **/
/********************************************************************************/
/********************************************************************************/

#ifndef _MELODIE_H
#define _MELODIE_H

#include "tone_definitions.h"

/********************************************************************************/
//const char TEXT_SONG1[]       = "Intel Sound before Pause";
int Intel1[][2] = {
			{4,TONE_g0},
			{0,0}
        };
/********************************************************************************/
//const char TEXT_SONG1[]       = "Intel Sound after Pause";
int Intel2[][2] = {
			{6,TONE_d0},
			{6,TONE_g0},
			{6,TONE_d0},
			{3,TONE_a0},
			{0,0}
        };

/********************************************************************************/
//const char TEXT_SONG2[]       = "William Tell Overture";
// {duration, note}
// duration 8 means eighth note
int William[][2] = {
			{8,TONE_g},
			{8,TONE_g},
			{4,TONE_g},
			{8,TONE_g},
			{8,TONE_g},
			{4,TONE_g},
			{8,TONE_g},
			{8,TONE_g},
			{4,TONE_c0},
			{4,TONE_d0},
			{4,TONE_e0},
			{8,TONE_g},
			{8,TONE_g},
			{4,TONE_g},
			{8,TONE_g},
			{8,TONE_g},
			{4,TONE_c0},
			{8,TONE_e0},
			{8,TONE_e0},
			{4,TONE_d0},
			{4,TONE_b},
			{4,TONE_g},
			{8,TONE_g},
			{8,TONE_g},
			{4,TONE_g},
			{8,TONE_g},
			{8,TONE_g},
			{4,TONE_g},
			{8,TONE_g},
			{8,TONE_g},
			{4,TONE_c0},
			{4,TONE_d0},
			{4,TONE_e0},
			{8,TONE_c0},
			{8,TONE_e0},
			{2,TONE_g0},
			{8,TONE_f0},
			{8,TONE_e0},
			{8,TONE_d0},
			{4,TONE_c0},
			{4,TONE_e0},
			{4,TONE_c0},
			{0,0}
        };

#endif /* _MELODIE_H */
