#include "key_scan.h"
#include "delay.h"

/*
 * 函数名：Scan_Key_Configuration
 * 描述  ：配置独立按键所使用的GPIO端口及模式
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
void Scan_Key_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//使能时钟
	
	/* gpio config(PC10,PC11,PC12) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入   
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);   //设置高电平
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);//使能时钟
	/* gpio config(PE2,PE3,PE4)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入   
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOE, GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4);   //设置高电平
}

/*
 * 函数名：KEY_Scan
 * 描述  ：配置独立按键所使用的GPIO端口及模式,GPIO设置为低电平
 * 输入  ：按键模式，0->不支持连按，1->支持连按
 * 输出  ：按键键值KEY1->1，KEY2->2，KEY3->3，KEY4->4，KEY5->5，KEY6->6
 * 调用  ：外部调用
 * 注意  ：此函数有响应优先级,KEY1>KEY2>KEY3>KEY4>KEY5>KEY6!!
 */
u8 KEY_Scan(u8 mode)
{
	static u8 key_up = 1;	//按键按松开标志
	
	if(mode)
	{
		key_up = 1;  	//支持连按	
	}
	
	if(key_up && (KEY1 == 0||KEY2 == 0||KEY3 == 0||KEY4 == 0||KEY5 == 0||KEY6 == 0))
	{
		delay_ms(10);	 //去抖动 
		key_up = 0;
		
		if(KEY1 == 0)   		 return 1;//KVALUE1;
		else if(KEY2 == 0)   return 2;//KVALUE2;
		else if(KEY3 == 0)   return 3;//KVALUE3;
		else if(KEY4 == 0)   return 4;//KVALUE4;
		else if(KEY5 == 0)   return 5;//KVALUE5;
		else if(KEY6 == 0)   return 6;//KVALUE6;
		
	}else if((KEY1 == 1) && (KEY2 == 1) && (KEY3 == 1) && (KEY4 == 1) && (KEY5 == 1) && (KEY6 == 1))	
	{	
		key_up = 1;
	}
	
 	return 0;			// 无按键按下
}
	


