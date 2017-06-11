#include "lcd12864.h"
#include "delay.h"
#include <stdio.h>

void LCD12864_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*对12864屏数据信号进行配置*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	    
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/*对12864屏控制信号进行配置*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	    
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
}

void LCD12864_busy(void)
{
	unsigned int timeout = 10;
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);               //EN 失能 
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);               //RS=L
	GPIO_SetBits(GPIOB,GPIO_Pin_13);                 //RW=H
	GPIO_SetBits(GPIOB,GPIO_Pin_14);                 //EN 使能=H 
	while((GPIOD->ODR & 0x80) && --timeout != 0);    //忙状态检测
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);               //EN 失能
	
//     uint16_t flag = 0x00;
// 	  GPIO_ResetBits(GPIOB,GPIO_Pin_12);    //RS=L 
// 	  GPIO_SetBits(GPIOB,GPIO_Pin_13);     //RW 拉高 
// 	  do
// 		{
// 			  GPIO_ResetBits(GPIOB,GPIO_Pin_14);    //EN 失能 
// 			  Delay_us(5);
// 			  GPIO_SetBits(GPIOB,GPIO_Pin_14);    //EN 使能=H 
// 			  Delay_us(5);
// 			  flag = (uint16_t)LCD_BF;
//     }while(flag);
// 		GPIO_ResetBits(GPIOB,GPIO_Pin_14);               //EN 失能
}

void LCD12864_WriteCommand(u8 command) 
{ 
//	LCD12864_busy();
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);    //RW=L   
  GPIO_ResetBits(GPIOB,GPIO_Pin_12);    //RS=L 
	delay_us(15);
  GPIO_SetBits(GPIOB,GPIO_Pin_14);    //EN 使能=H 
//	DATAUOT |= command;
  delay_us(15);	
  GPIOD->ODR = (( GPIOD->ODR & 0xff00 )| command ) ;  
  GPIO_ResetBits(GPIOB,GPIO_Pin_14);    //EN 失能 
} 
 
void LCD12864_WriteData(u8 data) 
{ 
//	LCD12864_busy();
	GPIO_SetBits(GPIOB,GPIO_Pin_12);    //RS 拉高 
  GPIO_ResetBits(GPIOB,GPIO_Pin_13);    //RW 拉低 
	delay_us(15);
  GPIO_SetBits(GPIOB,GPIO_Pin_14);    //EN 使能 
  GPIOD->ODR = (( GPIOD->ODR & 0xff00 )| data ) ; 
//	DATAUOT |= data;
  delay_us(15);
  GPIO_ResetBits(GPIOB,GPIO_Pin_14);    //EN 失能
} 

u8 LCD12864_ReadData(void)
{
	u8 temp;
//	LCD12864_busy();
	GPIOD->ODR = (( GPIOD->ODR & 0xff00 )| 0x00ff ) ;
	GPIO_SetBits(GPIOB,GPIO_Pin_12);      //RS 拉高
	GPIO_SetBits(GPIOB,GPIO_Pin_13);      //RW 拉高 
	GPIO_SetBits(GPIOB,GPIO_Pin_14);      //EN 使能 
	delay_us(15);
	temp = GPIOD->ODR;
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);    //EN 失能
	return temp;
}

void LCD12864_ShowStr(u8 *s)    //显示字符串的函数 
{ 
     while(*s != '\0') 
     { 
         LCD12864_WriteData(*s); 
         s++; 
         delay_ms(2); 
     } 
} 
 
void LCD12864_ShowStr_Line(u8 *s,u8 line)    //在第几行显示字符串的函数
{ 
     switch(line) 
    { 
      case 1: 
        LCD12864_WriteCommand(0x80); 
        LCD12864_ShowStr(s); 
         break; 
      case 2: 
        LCD12864_WriteCommand(0x90); 
			  LCD12864_ShowStr(s); 
         break; 
      case 3: 
        LCD12864_WriteCommand(0x88); 
        LCD12864_ShowStr(s); 
         break; 
      case 4: 
        LCD12864_WriteCommand(0x98); 
        LCD12864_ShowStr(s); 
         break; 
    } 
} 

void LCD12864_PutString(u8 x,u8 y,u8 *s)          //在X行Y列显示字符串的函数
{
	switch(y)
	{
		case 1:
			LCD12864_WriteCommand(0x80+x);
		  break;
		case 2:
			LCD12864_WriteCommand(0x90+x);
		  break;
		case 3:
			LCD12864_WriteCommand(0x88+x);
		  break;
		case 4:
			LCD12864_WriteCommand(0x98+x);
		  break;
		default:
			break;
  }
	
	while(*s != '\0')
	{
		LCD12864_WriteData(*s); 
    s++; 
    delay_ms(2);
  }
}
	

/*以下是光标定位函数，第一个参数为行坐标，第二个为列坐标，起始坐标是1行1列。
只能以16个点的宽度为单位移动。*/

void CursorLocate(int row, int col) 
{
	switch(row)
	{
		case 1: LCD12864_WriteCommand(0x80+col-1); break;
		case 2: LCD12864_WriteCommand(0x90+col-1); break;
		case 3: LCD12864_WriteCommand(0x88+col-1); break;
		case 4: LCD12864_WriteCommand(0x98+col-1); break;
	}
}

void drawdot_12864(u8 y,u8 x,u8 type)
{
	u8 X,Y,k;
	u8 DH,DL;
	if(y >= 0 && y <= 63 && x >= 0 && x <= 127)
	{
		if(y < 32)
		{
			X = 0x80 + (x >> 4);
			Y = 0x80 + y;
    }else
		{
			X = 0x88 + (x >> 4);
			Y = 0x80 + (y - 32);
    }
		LCD12864_WriteCommand(0x34);
		LCD12864_WriteCommand(Y);
		LCD12864_WriteCommand(X);
		DH = LCD12864_ReadData();
		DH = LCD12864_ReadData();
		DL = LCD12864_ReadData();
		
		k = x % 16;
		
		switch(type)
		{
			case 0: 
				if(k < 8)
				{
					DH &= ~(0x01 << (7 - k));
        }else
				{
          DL &= ~(0x01 << (7 - (k%8)));
				}
				break;
			case 1:
					if(k < 8)
					{
						DH |= (0x01 << (7 - k));
          }else
					{
						DL |= (0x01 << (7 - (k%8)));
          }
					break;
				default: break;	
    }
		LCD12864_WriteCommand(Y);
		LCD12864_WriteCommand(X);
		LCD12864_WriteData(DH);
		LCD12864_WriteData(DL);
		LCD12864_WriteCommand(0x30);
  }
}

void LCD12864_clear(void)
{
	u8 i,j;
	LCD12864_WriteCommand(0x34);
	for(i = 0;i < 32;i++)
	{
		LCD12864_WriteCommand(0x80 + i);
		LCD12864_WriteCommand(0x80);
		for(j = 0;j < 32;j++)
		{
			LCD12864_WriteData(0x00);
    }
  }
	LCD12864_WriteCommand(0x36);
}

void LCD12864_bar(u8 left,u8 up,u8 right,u8 down,u8 color)
{
	u8 temp;
	LCD12864_WriteCommand(0x34);             //打开扩展指令
	if(left > right)
	{
		temp = left;
		left = right;
		right = temp;
  }
	if(up > down)
	{
		temp = up;
		up = down;
		down = temp;
  }
	
	for(;up <= down;up++)
	{
	  for(temp = left;temp <= right;temp++)
		{
			drawdot_12864(up,temp,color);
    }
	}
	//LCD12864_WriteCommand(0x36);
}





