#include "led.h"
#include "sys.h"
#include <stdio.h>
#include "turang_exti.h"
#include "usart.h"
#include "usart3.h"
#include "delay.h"
// #include "guangmin_adc.h"
//#include "digital_tube.h"
//#include "adc.h"
#include "lsens.h"
#include "lcd12864.h"
#include "kem.h"
#include "beep.h"
#include "adc.h"
#include "key_scan.h"
#include "timer3.h"

#define LSENS_READ_TIMES	10		//定义光敏传感器读的次数

int lcd12864_up   = 16;           //lcd12864画长方形阴影上界
int lcd12864_down = 31;           //lcd12864画长方形阴影下界

int time_num[5] = {14400, 10800, 7200, 3600, 600};	//4小时 3小时 2小时 1小时 1分钟(Debug)

void itoa1(u8 * a,int x);
extern __IO uint16_t ADCConvertedValue;

void Cursor_move()
{
	LCD12864_clear();
  LCD12864_bar(0, lcd12864_up, 127, lcd12864_down, 1);   
  LCD12864_WriteCommand(0x36);    //设置绘图显示开
}

void Beep_ming(void)
{
	int i = 0;
	
	BEEP = BEEPON;
	for(i = 655350; i > 0; i--);
	for(i = 655350; i > 0; i--);
	for(i = 655350; i > 0; i--);
	BEEP = BEEPOFF;	
}

void Show_wendu_guangzhao(void)
{
	LCD12864_WriteCommand(0x01);	                          //清屏
	delay_ms(1);
	LCD12864_WriteCommand(0x30);
	LCD12864_ShowStr_Line("  光照强度：    ",1);
	LCD12864_ShowStr_Line("  土壤湿度：    ",3);	
}

void Show_success(void)
{
	LCD12864_WriteCommand(0x01);	                          //清屏
	delay_ms(1);
	LCD12864_WriteCommand(0x30);
	LCD12864_ShowStr_Line("开启成功        ",1);
}

int main(void)
{
	int i = 0;
	float value = 0.0;
	u16 tmp;
	int ivalue;
	char buf[1];
	u32 temp_val=0;
	u8 t;
	u8 adcx = 0;
	u8 key_value = 0;
	int temp_index = 0;
	int time_index = 0;
	int time_counter = 0;
	int time_num_index = 4;	//从一分钟开始

//	Lsens_Init();		//初始化光敏电阻传感器
	LED_Init();			//led灯初始化 E7
	KEM_Init();			//继电器初始化
	BEEP_Init();		//蜂鸣器
	delay_init();		//延迟函数初始化
	EXTI_Turang_init();					//土壤湿度中断初始化  D8
	LCD12864_init();            //LCD12864初始化
//		ADC_DMA_Configuration();			//使用DMA模式的ADC控制器的初始化
	USART1_Configuration();
	usart3_init(9600);
	Scan_Key_Configuration();		//独立按键初始化

	Lsens_Init(); 			//????????
	
	delay_ms(1000);
	LCD12864_WriteCommand(0x30);                            //打开基本指令集
	LCD12864_WriteCommand(0x0c);                            //关光标
	LCD12864_WriteCommand(0x01);	                          //清屏
 	LCD12864_WriteCommand(0x06);                            //进入模式设置

	//开机界面
	LCD12864_WriteCommand(0x01);	                          //清屏
	delay_ms(1);
	LCD12864_WriteCommand(0x30);
  LCD12864_ShowStr_Line("  太原师范学院  ",1);
  LCD12864_ShowStr_Line("    计算机系    ",2);
 	LCD12864_ShowStr_Line("  太原师范学院  ",3);
 	LCD12864_ShowStr_Line("  设计：刘强民  ",4);
	delay_ms(8000);
	//开机界面
	
	LCD12864_WriteCommand(0x01);	                          //清屏
	delay_ms(1);
	LCD12864_WriteCommand(0x30);
	LCD12864_ShowStr_Line("  光照强度：    ",1);
	LCD12864_ShowStr_Line("  土壤湿度：    ",3);	

	while(1)
	{
		delay_ms(10);
		
		if(time_counter > time_num[time_num_index])
		{//1为100ms 10 为1s 600为 1 3600 为 1h
			KEM2 = KEMOFF;
			time_counter = 0;		//重新计数
			time_index = 0;			//关闭计数
		}
		
		key_value = 0;  //未初始化时很危险
		key_value = KEY_Scan(0);	//得到键值

		if(key_value > 0)
		{
			Beep_ming();
			
			if(key_value == 1)
			{//OK
				LCD12864_WriteCommand(0x01);	                          //清屏
				delay_ms(1);
				LCD12864_WriteCommand(0x30);
				LCD12864_ShowStr_Line("1:开启水泵      ", 1);
				LCD12864_ShowStr_Line("2:开启补光灯    ", 2);
				LCD12864_ShowStr_Line("3:关闭水泵      ", 3);
				LCD12864_ShowStr_Line("4:关闭补光灯    ", 4);
				
				i = 1;
				while(i)
				{
					delay_ms(100);
					
					if(time_index)
					{
						time_counter++;	//记一次的运行
					}
					
					key_value = 0;  //未初始化时很危险
					key_value = KEY_Scan(0);	//重新得到键值
					if(key_value > 0)
					{
						Beep_ming();
					}
					if(key_value == 1)
					{//开启水泵
						KEM1 = KEMON;
						
						LCD12864_WriteCommand(0x01);	                          //清屏
						delay_ms(1);
						LCD12864_WriteCommand(0x30);
						LCD12864_ShowStr_Line("开启成功        ",1);
						
						delay_ms(600);			//人眼看得见
						
						LCD12864_WriteCommand(0x01);	                          //清屏
						delay_ms(1);
						LCD12864_WriteCommand(0x30);
						LCD12864_ShowStr_Line("  光照强度：    ",1);
						LCD12864_ShowStr_Line("  土壤湿度：    ",3);	
						
						i = 0;		//退出循环
					}
					else if(key_value == 2)
					{//开启补光灯
						LCD12864_WriteCommand(0x01);	                          //清屏
						delay_ms(1);
						LCD12864_WriteCommand(0x30);
						LCD12864_ShowStr_Line("1:木本 4小时    ", 1);
						LCD12864_ShowStr_Line("2:草本 3小时    ", 2);
						LCD12864_ShowStr_Line("3:藤本 2小时    ", 3);
						LCD12864_ShowStr_Line("4:多肉 1小时    ", 4);	
						
						i = 1;		//制造一个死循环
						while(i)
						{
							delay_ms(100);
					
							if(time_index)
							{
								time_counter++;	//记一次的运行
							}
							
							key_value = 0;  //未初始化时很危险
							key_value = KEY_Scan(0);	//重新得到键值
							if(key_value > 0)
							{
								Beep_ming();	//按键按下蜂鸣器响
							}
							
							if(key_value == 1)
							{
								KEM2 = KEMON;
								
								time_index = 1;		//启用程序定义的计时器
								time_num_index = 0;		//选择运行的时间4小时
								
								LCD12864_WriteCommand(0x01);	                          //清屏
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("开启成功        ",1);
								LCD12864_ShowStr_Line("距离自动关闭还有",2);
								LCD12864_ShowStr_Line("   4小时        ",3);
								
								delay_ms(600);			//人眼看得见
								
								LCD12864_WriteCommand(0x01);	                          //清屏
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("  光照强度：    ",1);
								LCD12864_ShowStr_Line("  土壤湿度：    ",3);
								
								i = 0;		//退出选择植物的循环								
							}
							else if(key_value == 2)
							{
								KEM2 = KEMON;
								
								time_index = 1;		//启用程序定义的计时器
								time_num_index = 1;
								
								LCD12864_WriteCommand(0x01);	                          //清屏
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("开启成功        ",1);
								LCD12864_ShowStr_Line("距离自动关闭还有",2);
								LCD12864_ShowStr_Line("   3小时        ",3);
								
								delay_ms(600);			//人眼看得见
								
								LCD12864_WriteCommand(0x01);	                          //清屏
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("  光照强度：    ",1);
								LCD12864_ShowStr_Line("  土壤湿度：    ",3);
								
								i = 0;		//退出选择植物的循环	
							}
							else if(key_value == 3)
							{
								KEM2 = KEMON;
								
								time_index = 1;		//启用程序定义的计时器
								time_num_index = 2;
								
								LCD12864_WriteCommand(0x01);	                          //清屏
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("开启成功        ",1);
								LCD12864_ShowStr_Line("距离自动关闭还有",2);
								LCD12864_ShowStr_Line("   2小时        ",3);
								
								delay_ms(600);			//人眼看得见
								
								LCD12864_WriteCommand(0x01);	                          //清屏
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("  光照强度：    ",1);
								LCD12864_ShowStr_Line("  土壤湿度：    ",3);
								
								i = 0;		//退出选择植物的循环	
							}
							else if(key_value == 4)
							{
								KEM2 = KEMON;
								
								time_index = 1;		//启用程序定义的计时器
								time_num_index = 3;
								
								LCD12864_WriteCommand(0x01);	                          //清屏
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("开启成功        ",1);
								LCD12864_ShowStr_Line("距离自动关闭还有",2);
								LCD12864_ShowStr_Line("   1小时        ",3);
								
								delay_ms(600);			//人眼看得见
								
								LCD12864_WriteCommand(0x01);	                          //清屏
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("  光照强度：    ",1);
								LCD12864_ShowStr_Line("  土壤湿度：    ",3);
								
								i = 0;		//退出选择植物的循环	
							}
							else if(key_value == 5)
							{//debug
								KEM2 = KEMON;
								
								time_index = 1;		//启用程序定义的计时器
								time_num_index = 4;
								
								LCD12864_WriteCommand(0x01);	                          //清屏
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("开启成功        ",1);
								LCD12864_ShowStr_Line("距离自动关闭还有",2);
								LCD12864_ShowStr_Line("   1分钟        ",3);
								
								delay_ms(600);			//人眼看得见
								
								LCD12864_WriteCommand(0x01);	                          //清屏
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("  光照强度：    ",1);
								LCD12864_ShowStr_Line("  土壤湿度：    ",3);
								
								i = 0;		//退出选择植物的循环	
							}
							else if(key_value == 6)
							{
								LCD12864_WriteCommand(0x01);	                          //清屏
								delay_ms(1);
								LCD12864_WriteCommand(0x30);
								LCD12864_ShowStr_Line("  光照强度：    ",1);
								LCD12864_ShowStr_Line("  土壤湿度：    ",3);	
								
								i = 0;		//退出循环
							}
						}
// 						KEM2 = KEMON;
// //						TIM3_Int_Init(4999,7199);//10Khz计数频率，计数到5000为500ms
// 						
// 						time_index = 1;		//启用程序定义的计时器
// 						
// 						LCD12864_WriteCommand(0x01);	                          //清屏
// 						delay_ms(1);
// 						LCD12864_WriteCommand(0x30);
// 						LCD12864_ShowStr_Line("开启成功        ",1);
// 						LCD12864_ShowStr_Line("距离自动关闭还有",2);
// 						LCD12864_ShowStr_Line("   2小时        ",3);
// 						
// 						delay_ms(600);			//人眼看得见
// 						
// 						LCD12864_WriteCommand(0x01);	                          //清屏
// 						delay_ms(1);
// 						LCD12864_WriteCommand(0x30);
// 						LCD12864_ShowStr_Line("  光照强度：    ",1);
// 						LCD12864_ShowStr_Line("  土壤湿度：    ",3);	
						
						i = 0;		//退出开启补光灯的循环
					}
					else if(key_value == 3)
					{
						if(KEM1 == KEMON)
						{
							KEM1 = KEMOFF;
							
							delay_ms(10);
							LCD12864_WriteCommand(0x01);	                          //清屏
							delay_ms(1);
							LCD12864_WriteCommand(0x30);
							LCD12864_ShowStr_Line("  水泵已经关闭  ",1);
							
							delay_ms(600);			//人眼看得见
						
							LCD12864_WriteCommand(0x01);	                          //清屏
							delay_ms(1);
							LCD12864_WriteCommand(0x30);
							LCD12864_ShowStr_Line("  光照强度：    ",1);
							LCD12864_ShowStr_Line("  土壤湿度：    ",3);
							
							i = 0;		//退出循环
						}
						else
						{
							delay_ms(10);
							LCD12864_WriteCommand(0x01);	                          //清屏
							delay_ms(1);
							LCD12864_WriteCommand(0x30);
							LCD12864_ShowStr_Line("  水泵关闭失败  ",1);
						}
					}
					else if(key_value == 4)
					{
						if(KEM2 == KEMON)
						{
							KEM2 = KEMOFF;
							
							delay_ms(10);
							LCD12864_WriteCommand(0x01);	                          //清屏
							delay_ms(1);
							LCD12864_WriteCommand(0x30);
							LCD12864_ShowStr_Line("  补光灯已关闭  ",1);
							
							delay_ms(600);			//人眼看得见
						
							LCD12864_WriteCommand(0x01);	                          //清屏
							delay_ms(1);
							LCD12864_WriteCommand(0x30);
							LCD12864_ShowStr_Line("  光照强度：    ",1);
							LCD12864_ShowStr_Line("  土壤湿度：    ",3);
							
							time_counter = 0;		//重新计数
							time_index = 0;			//关闭计数
							
							i = 0;		//退出循环
						}
						else
						{
							delay_ms(10);
							LCD12864_WriteCommand(0x01);	                          //清屏
							delay_ms(1);
							LCD12864_WriteCommand(0x30);
							LCD12864_ShowStr_Line("  补光灯关闭失败",1);
						}
					}
					else if(key_value == 6)
					{
						LCD12864_WriteCommand(0x01);	                          //清屏
						delay_ms(1);
						LCD12864_WriteCommand(0x30);
						LCD12864_ShowStr_Line("  光照强度：    ",1);
						LCD12864_ShowStr_Line("  土壤湿度：    ",3);	
						
						i = 0;		//退出循环
					}
				}
			}
		}

		delay_ms(100);			//应该是一个比较合理的时间
		if(time_index)
		{
			time_counter++;	//记一次的运行
		}
		temp_index++;				//0~9
		
//		value = (float)3.3 * ADCConvertedValue / 4096;  //电压

//		adcx = string[0] * (-20) + 205;	//公式
		if(temp_index == 9)
		{
			temp_index = 0;
			adcx = Lsens_Get_Val();
	// 		value = value * (-20) + 205;
	// 		i = (int)value;
	// 		adcx = i;
	//		itoa1(string, i);
			i = adcx;
	//		printf("  value: %d\r\n", i);
			
			sprintf(buf, "%d", i);		//将检测到的值放置显示缓冲区
			LCD12864_WriteCommand(0x30);
			LCD12864_ShowStr_Line(buf, 2);   //0~99
//			LCD12864_ShowStr_Line("buf             ", 2);
			
			if(adcx < 20 && adcx > 0)
			{//!0
				LED2 = LEDON;
				LED3 = LEDOFF;
				LED4 = LEDOFF;
			}
			else if(adcx < 50)
			{
				LED2 = LEDOFF;
				LED3 = LEDON;
				LED4 = LEDOFF;
			}
			else if(adcx < 100)
			{
				LED2 = LEDOFF;
				LED3 = LEDOFF;
				LED4 = LEDON;
			}
			else
			{
				LED2 = LEDOFF;
				LED3 = LEDOFF;
				LED4 = LEDOFF;
			}
//			string[3] = '\0';
// 		for( i = 0; i < 3; i++)
// 			LCD12864_PutString(2 + i, 2  ,  string +i );

// 		for(t=0;t<LSENS_READ_TIMES;t++)
// 		{
// 			temp_val += ADCConvertedValue;	//??ADC?
// 			delay_ms(5);
// 		}
// 		temp_val/=LSENS_READ_TIMES;//????? 
// 		if(temp_val>4000)temp_val=4000;
// 		string[0]	= (u8)(100-(temp_val/40));
// 		if(string[0])
// 		{
// 			LED2 = LEDON;
// 		}
// 		string[1] = '\0';
// 		
// 		LCD12864_PutString(2, 2 ,  string );
// 		//显示ADC转换结果，有效位数3位，小数位2位
// //		value *= 100; //数值扩大10000倍，显示数据保留小数点2位
// 		ivalue = (int)value;
// 		itoa1(string, ivalue);
// 		
// 		//状态显示
// //		LCD12864_WriteCommand(0x01);	                          //清屏
// 		
// 		if(ivalue >= 1000)
// 		{
// //			KEM1 = KEMOFF;
// 		}
// 		else
// 		{
// //			KEM1 = KEMON;
// 		}
			delay_ms(100);			//应该是一个比较合理的时间
			if(time_index)
			{
				time_counter++;	//记一次的运行
			}
		}
	}
}
	void itoa1(u8 * a,int x)
{
	int i,j = 0;
	int tmp;
	int shuzu[10];

	for(i = 0; x > 0; i ++)
	{

			tmp = x % 10;
			shuzu[i] = tmp;
			x = x / 10;
	}

	for(i = i - 1; i >= 0 ; i--,j++)
	{
			a[j] = shuzu[i] + 48;
	}
	a[++j] = '\0';
}
