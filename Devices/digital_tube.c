
#include "stm32f10x.h"
#include "digital_tube.h"
#include "delay.h"

unsigned char code_table[] =
{
	0x3f,0x06,0x5b,0x4f,
	0x66,0x6d,0x7d,0x07,
	0x7f,0x6f,0x77,0x7c,
	0x39,0x5e,0x79,0x71
};

/**
  * @brief  8段位数码管初始化函数
  * @param  无
  * @retval 无
  */
void digital_tube_init(void)
{
	GPIO_InitTypeDef  GPIO_initStruct;

	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);

	//初始化GPIO
	GPIO_initStruct.GPIO_Pin =  GPIO_Pin_2 |GPIO_Pin_3;
	GPIO_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_initStruct.GPIO_Mode =  GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOD,&GPIO_initStruct);

	GPIO_initStruct.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_initStruct.GPIO_Mode =  GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOE,&GPIO_initStruct);
	
	GPIO_ResetBits(GPIOE, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
}

void disp_all_same(u8 num)
{					 
	GPIOE->ODR = 0xff00;	
	GPIO_SetBits(GPIOD,GPIO_Pin_2);	//位选	
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);

	GPIOE->ODR = code_table[num];  	  //如何显示小数点？   显示小数点 |0x80 ？
	GPIO_SetBits(GPIOD,GPIO_Pin_3);	  //段选
	GPIO_ResetBits(GPIOD,GPIO_Pin_3);
}

void disp_one_num(u8 num, int pos, u8 showPoint)
{
	unsigned char b_p = 1;
	
	GPIOE->ODR = ~(b_p << pos);     		//这样做的原理是什么？
	GPIO_SetBits(GPIOD,GPIO_Pin_2);			//位选	
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);

	if(showPoint)
		GPIOE->ODR = code_table[num] |0x80;  //显示小数点
    else
		GPIOE->ODR = code_table[num];   
	GPIO_SetBits(GPIOD,GPIO_Pin_3);	  		//段选
	GPIO_ResetBits(GPIOD,GPIO_Pin_3);
}

void disp_number(int num)
{
	int i = 0;
	int total = 100;
	int num_old = num;
	
	if (num > 0)
	{
		for (total = 500; total > 0;total--)
		{
			i = 0;
			num = num_old;
			while (num > 0)
			{
				disp_one_num(num%10,i++, 0);
				num = num / 10;
				delay_us(100);
			}
		}
	}
}

/******************* (C) 版权所有 2013 太原联航科技有限公司 *******************/
