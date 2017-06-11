#ifndef __LCD1602_H
#define __LCD1602_H
#include "stm32f10x.h"

void LCD1602_WriteCommand(u8 command);
void LCD1602_WriteData(u8 data);
void LCD1602_Configuration(void);


#endif


