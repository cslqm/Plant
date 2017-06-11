#include "sys.h"

//默认的优先级设置  
void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
}
/**
  * @brief  配置USART1、2、3的中断控制器
  * @param  None
  * @retval None
  */
void USARTx_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  /* 使能USART1中断*/
//   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
	
	  /* 使能USART3中断*/
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//IRQ
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化

}
