#include "usart.h"
#include "stm32f10x.h"
#include <stdio.h>

static void USART1_RCC_Configuration(void)
{//设置时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1|
	RCC_APB2Periph_AFIO,ENABLE);
}

static void USART1_GPIO_Configuration(void)
{
		//定义gpio初始化结构体
		GPIO_InitTypeDef GPIO_InitStructure;
		//初始化结构体
		GPIO_StructInit(&GPIO_InitStructure);
		//配置usart1的接受端口
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化PA10
		//配置usart1的发送端口
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure);// 初始化pa9
}

void USART1_Configuration()
{//初始化串口
		USART_InitTypeDef USART_InitStructure;
		//USART相关时钟初始化数设置
		USART1_RCC_Configuration();
		//usart相关gpio初始化设置
		USART1_GPIO_Configuration();
		
		USART_InitStructure.USART_BaudRate = 9600;//波特率
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
		//设置usart1的异步双工串行工作模式
		USART_Init(USART1, &USART_InitStructure);
		//使能usart1的接受中断
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
		//关闭usart1的发送中断
		USART_ITConfig(USART1,USART_IT_TXE, ENABLE);
		//使能usart1
		USART_Cmd(USART1,ENABLE);
}
#if 1
/**
  * @brief  重定向C语言中的printf函数到USART
  * @param  None
  * @retval None
  */
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (u8)ch);

  /* 循环直至发送结束 */
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {
  }
  return ch;
}
#endif
