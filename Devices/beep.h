#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h"

//蜂鸣器端口定义
#define BEEP PEout(15)	// BEEP,蜂鸣器接口

#define BEEPON 1
#define BEEPOFF 0

void BEEP_Init(void);	//初始化
		 				    
#endif

