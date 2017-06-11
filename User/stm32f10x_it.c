/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "led.h"
#include "kem.h"
#include "delay.h"
#include "usart.h"
#include "lcd12864.h"
//#include "step_motor.h"


/*步进电机运行状态控制变量，通过修改变量状态使不仅电机启动或者停止，以及正转和反转*/
extern volatile u8 Motor_Stop;
extern volatile u8 Motor_Forward;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

void EXTI9_5_IRQHandler(void)
{//土壤中断
		
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{//通道8的终端
		u8 water;
		
		if( water = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8))
		{//就是赋值
			LED6 = LEDON;
//				KEM2 = KEMON;
		}
		else
		{
			LED6 = LEDOFF;
//			KEM2 = KEMOFF;
		}
		
		delay_ms(1);
		
		LCD12864_WriteCommand(0x30);

		if(water)
		{
			LCD12864_ShowStr_Line("    当前潮湿    ",4);
		}
		else
		{
			LCD12864_ShowStr_Line("    当前干燥    ",4);
		}
		
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line10) != RESET) //确保是否产生了EXTI Line中断
	{
		EXTI_ClearITPendingBit(EXTI_Line10);     //清除中断标志位
	}
	
	if(EXTI_GetITStatus(EXTI_Line11) != RESET) //确保是否产生了EXTI Line中断
	{
		EXTI_ClearITPendingBit(EXTI_Line11);     //清除中断标志位
	}
}

void USART1_IRQHandler(void)
{
	u8 Res;
	
	//usart1的接受中断
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
// 		LED2 = LEDON;
// 			Res = USART_ReceiveData(USART1);//最近USART1接受的数据
// 			if((Res == 0x31))
// 			{
// // 						//电机启动，按照停止前转动方向继续转动
// // 		       if(Motor_Stop)  Motor_Stop = 0;		
// // 		       EXTI_ClearITPendingBit(EXTI_Line8);     //清除中断标志位
// //					LED1 = LEDON;			//指示灯1亮
// //				KEM1 = KEMON;			//继电器1工作启动补光灯
// //				LED2 = LEDON;
// 			}
// 			else if(Res == 0x32)
// 			{
// // 					//电机停止
// // 					Motor_Stop = 1;
// // 					EXTI_ClearITPendingBit(EXTI_Line9);     //清除中断标志位
// //					LED1 = LEDOFF;		//指示灯1灭
// //				KEM1 = KEMOFF;		//继电器1停止工作
// 				LED2 = LEDOFF;
// 			}
// 			else if(Res == 0x33)
// 			{
// 				//电机正转
// // 					if(Motor_Stop) Motor_Stop = 0;	 //如果电机未启动，则启动电机	
// // 					Motor_Forward = 1;
// // 					EXTI_ClearITPendingBit(EXTI_Line10);     //清除中断标志位
// //					LED1 = LEDON;
// //				KEM2 = KEMON;			//继电器2工作启动水泵
// //				LED3 = LEDON;
// 			}
// 			else if(Res == 0x34)
// 			{
// 				//电机反转
// // 					if(Motor_Stop) Motor_Stop = 0;	 //如果电机未启动，则启动电机	
// // 					Motor_Forward = 0;
// // 					EXTI_ClearITPendingBit(EXTI_Line11);     //清除中断标志位
// //					LED1 = LEDOFF;
// //				KEM2 = KEMOFF;		//继电器2停止工作
// 				LED3 = LEDOFF;
// 			}
// 			else
// 			{
// 				
// 			}
			

	}

	/*USART1发送中断*/
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{ 
	}

}

// void USART3_IRQHandler(void)
// {
//     /*USART3????*/
//     if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//     {
//     }

//     /*USART3????*/
//     if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
//     { 
//     }
// }



/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
