#include "turang_exti.h"
#include "stm32f10x.h"

/*
 * 函数名：EXTI_Key_NVIC_Configuration
 * 描述  ：配置嵌套向量中断控制器NVIC
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void EXTI_Turang_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);   			//把NVIC中断优先级分组设置为第一组
  
  /* 配置P[A|B|C|D|E]的中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;            //配置的为EXTI第5~9的中断向量
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;			//先占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;            //配置的为EXTI第10~15的中断向量
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;				//从优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

/*
 * 函数名：EXTI_Key_GPIO_Configuration
 * 描述  ：配置独立按键所使用的GPIO端口及模式
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void EXTI_Turang_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* EXTI line gpio config(PD8,PD9,PD10,PD11) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;// |GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11      
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // 上拉输入   
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);  //定义PA8为外部中断8输入通道
//     GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource9);  //定义PD9为外部中断9输入通道
//     GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource10); //定义PD10为外部中断10输入通道
//     GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource11); //定义PD11为外部中断11输入通道
     
}

/*
 * 函数名：EXTI_Key_init
 * 描述  ：初始化独立按键，配置所需的中断端口及模式
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void EXTI_Turang_init(void)
{	
	EXTI_InitTypeDef EXTI_InitStructure;												

    /* config the extiline(PA) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);	//使用中断务必要开启复用时钟
    /* config the GPIO(PA) */
    EXTI_Turang_GPIO_Configuration();
    /* config the NVIC(PA) */
	EXTI_Turang_NVIC_Configuration();

	/* EXTI line mode config */
	/*设置外部中断8、9、10、11在下降沿时触发中断*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;//|EXTI_Line9|EXTI_Line10|EXTI_Line11
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升和下降沿
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;               //使能中断
	EXTI_Init(&EXTI_InitStructure); 
}
