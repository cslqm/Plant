#ifndef _LCD12864_H
#define _LCD12864_H

#include "stm32f10x.h"

#define LCD_CONTROL  GPIOC   //默认LCD12864的控制口在PC口
#define LCD_DATAPORT GPIOC   //默认LCD12864的数据口在PC口
#define DATAUOT  LCD_DATAPORT->ODR &= 0xff00;LCD_DATAPORT->ODR  //数据输出寄存器
#define DATAIN   LCD_DATAPORT->IDR                              //数据输入寄存器
#define LCD_BF   ((DATAIN)&0x0080)                              //忙状态

void LCD12864_init(void);
void LCD12864_WriteCommand(u8 command);
void LCD12864_WriteData(u8 data);
u8 LCD12864_ReadData(void);
void LCD12864_ShowStr(u8 *s);
void LCD12864_ShowStr_Line(u8 *s,u8 line);
void LCD12864_PutString(u8 x,u8 y,u8 *s);
void CursorLocate(int row, int col);
void LCD12864_bar(u8 left,u8 up,u8 right,u8 down,u8 color);
void LCD12864_clear(void);
void drawdot_12864(u8 y,u8 x,u8 type);

#endif  /*_LCD12864_H*/

