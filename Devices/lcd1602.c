#include "lcd1602.h"	
#include "delay.h"	

/**
  * @brief  配置GPIOC和GPIOD时钟
  * @param  None
  * @retval None
  */
static void LCD1602_RCC_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
}	

/**
  * @brief  配置LCD1602所需的GPIO引脚及模式
  * @param  None
  * @retval None
  */
static void LCD1602_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
								  GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |
	                              GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void LCD1602_WriteCommand(u8 command)//写指令
{
   GPIO_ResetBits(GPIOD,GPIO_Pin_6);	 //RW
   GPIO_ResetBits(GPIOD,GPIO_Pin_5);	//RS     拉低使能指令寄存器
   delay_ms(5); 
   GPIO_SetBits(GPIOD,GPIO_Pin_7);     //EN使能
   delay_ms(5);   
	 GPIOC->ODR = command;
   delay_ms(5);
   GPIO_ResetBits(GPIOD,GPIO_Pin_7);   //pin7为EN使能端 
}

void LCD1602_WriteData(u8 data)
{
  GPIO_SetBits(GPIOD,GPIO_Pin_5); 	//拉高使能数据寄存器
	GPIOC->ODR = data;
	GPIO_SetBits(GPIOD,GPIO_Pin_7);    //使能
	delay_ms(10);
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);  //失能
}

void LCD1602_Configuration(void)
{
	delay_init();//其他地方能用吗？
	
	LCD1602_RCC_Configuration();
	
	LCD1602_GPIO_Configuration();
	
	delay_ms(400);		//延时400ms
	
	LCD1602_WriteCommand(0x38);	  //复位

	LCD1602_WriteCommand(0x0C);	 //开显示，关闭光标，光标不闪烁

  LCD1602_WriteCommand(0x06);   //显示地址增加，即写一个数据后右移一位

	LCD1602_WriteCommand(0x01);	  //清屏
}

