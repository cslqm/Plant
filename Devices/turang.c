#include "turang.h"

void TURANG_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	                

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			 	//LED1-->PE.8 ...  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 			  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 				//IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 				//根据设定参数初始化GPIOB	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}