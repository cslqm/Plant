/**
  ******************************************************************************
  * @file    adc.c
  * @author  联航精英训练营   allen@unigress.com
  * @version V1.0
  * @date    2013-10-20
  * @brief   SMART103中使用PD8、PD9、PD10、PD11做为独立按键，PB6、PB7、PB8、PB9
	*          做为步进电机控制端口，使用这些独立按键控制步进电机的启动、停止、正
	*          转和反转。
  ******************************************************************************
  * @attention
  *
  * 本程序所提供的源代码均为本程序作者或由网络收集整理而来，仅供学习和研究使用。
  * 如有侵犯的地方，请及时联系，我们会立即修改。使用本程序源码的用户必须明白，
  * 我们不能保证所提供的源码及其它资源的准确性、安全性和完整性，因此我们将不负责
  * 承担任何直接，间接因使用这些源码对自己和他人造成任何形式的损失或伤害。任何人
  * 在使用本代码时，应注明作者和出处。
  *
  * <h2><center>版权所有(C) 2013 太原联航科技有限公司</center></h2>
  ******************************************************************************
  */  
#include "step_motor.h"
#include "delay.h"
#include "stm32f10x.h"

volatile u8 Motor_Stop = 1;   //步进电机启动或停止状态 1->停止，0->启动
volatile u8 Motor_Forward = 1;    //步进电机转动方向，默认为正转。  1->正转，0->反转
#define STEP_EIGHT_SHOT     //使用四相八拍模式，如果需要使用四相四拍模式，请注释掉这行代码

#ifdef STEP_EIGHT_SHOT
/*  八拍电机正转码表  A-->AB-->B-->BC-->C-->CD-->D-->DA
Value	Step    A   B   C   D
0x08	 1      1   0   0   0
0x0C	 2	    1   1   0   0
0x04	 3      0   1   0   0 	
0x06	 4	    0   1	  1   0
0x02	 5	    0   0	  1   0
0x03	 6	    0   0	  1   1
0x01	 7	    0   0	  0   1
0x09	 8	    1   0	  0   1
*/
u16 step_forward[] = {0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09};
/*  八拍电机反转码表   A-->AD-->D-->DC-->C-->CB-->B-->BA
Value	Step    A   B   C   D
0x08	 1      1   0   0   0
0x09	 2	    1   0	  0   1
0x01	 3      0   0   0   1 	
0x03	 4	    0   0   1   1
0x02	 5	    0   0   1   0
0x06	 6	    0   1   1   0
0x04	 7	    0   1   0   0
0x0C	 8	    1   1   0   0
*/
u16 step_backward[] = {0x08,0x09,0x01,0x03,0x02,0x06,0x04,0x0c};
#else
/*  四拍电机正转码表  AB-->BC-->CD-->DA
Value	Step    A   B   C   D
0x03	 1      1   1   0   0
0x06	 2	    0   1   1   0
0x0C	 3      0   0   1   1 	
0x09	 4	    1   0	  0   1
*/
u16 step_forward[] = {0x3,0x6,0xc,0x9};
/*  四拍电机反转码表   AB-->AD-->CD-->CB
Value	Step    A   B   C   D
0x03	 1      1   1   0   0
0x09	 2	    1   0	  0   1
0x0C	 3      0   0   1   1 	
0x06	 4	    0   1   1   0
*/
u16 step_backward[] = {0x3,0x9,0xc,0x6};
#endif

/**
  * @brief  配置GPIOB时钟
  * @param  None
  * @retval None
  */
static void STEP_MOTOR_RCC_Configuration(void)
{	
    /* 使能GPIOB的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
}  
 
/**
  * @brief  配置GPIOB相应端口为输出模式
  * @param  None
  * @retval None
  */
static void STEP_MOTOR_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 配置PB6 PB7 PB8 PB9为推挽输出模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure. GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure. GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  步进电机初始化配置函数
  * @param  None
  * @retval None
  */
void STEP_MOTOR_Configuration(void)
{
	/* 系统延时初始化函数 */
	delay_init();
	/* 步进电机控制线时钟使能 */
	STEP_MOTOR_RCC_Configuration();
	/* 步进电机控制GPIO端口初始化配置 */
	STEP_MOTOR_GPIO_Configuration();
}

/**
  * @brief  步进电机启动函数
  * @param  None
  * @retval None
  */
void STEP_MOTOR_Start(void)
{
	int i;
	while(!Motor_Stop)
	{
		if(Motor_Forward)
		{
			for(i = 0; i < sizeof(step_forward)/sizeof(u16); i++)
			{
				GPIOB->ODR = step_forward[i] << 6;  //控制端口为6、7、8、9因此需要将数值左移6位
				delay_ms(1800);  //脉冲信号间间隔时间
			}
		}
		else if(!Motor_Forward)
		{
			for(i = 0; i < sizeof(step_backward)/sizeof(u16); i++)
			{
				GPIOB->ODR = step_backward[i] << 6;  //控制端口为6、7、8、9因此需要将数值左移6位
				delay_ms(1800);  //脉冲信号间间隔时间
			}
		}		
	}
}

/******************* (C) 版权所有 2013 太原联航科技有限公司 *******************/
