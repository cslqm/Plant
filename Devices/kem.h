#ifndef __KEM_H
#define __KEM_H

#include "sys.h"

#define KEMON 1			//继电器开
#define KEMOFF 0		//继电器关

#define KEM1 PAout(11)			//继电器1水泵
#define KEM2 PAout(12)			//继电器2灯

void KEM_Init(void);

#endif