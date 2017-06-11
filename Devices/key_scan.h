#ifndef __KEY_SCAN_H__
#define __KEY_SCAN_H__

#include "stm32f10x.h"

#if 1

#define KEY1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)  //OK
#define KEY2 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)   //ÉÏ 
#define KEY3 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)   //ÏÂ 
#define KEY4 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)  //×ó 
#define KEY5 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)  //ÓÒ
#define KEY6 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)   //·µ»Ø

#else

#define KEY1 PDin(8)  // PD8 
#define KEY2 PDin(9)  // PD9 
#define KEY3 PDin(10) // PD10 
#define KEY4 PDin(11) // PD11

#endif

#define KVALUE6  6 
#define KVALUE5  5 
#define KVALUE4  4 
#define KVALUE3  3 
#define KVALUE2  2 
#define KVALUE1  1

u8 KEY_Scan(u8 mode);
void Scan_Key_Configuration(void);

#endif
