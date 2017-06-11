
#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
 
#define LEDON 0      		//µ„¡¡
#define LEDOFF 1			//œ®√

#define LED2 PBout(9)	// PB9
#define LED3 PBout(8)	// PB8
#define LED4 PBout(7)	// PB7
#define LED5 PBout(6)	// PB6
#define LED6 PBout(5)	// PB5

void LED_Init(void);//≥ı ºªØ
		 				    
#endif