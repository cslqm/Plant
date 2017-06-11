#include "adc.h"
#include "stm32f10x.h"

#define ADC1_DR_Address    ((uint32_t)0x4001244C)

__IO uint16_t ADCConvertedValue = 0;

/**
  * @brief  配置ADC1和GPIO时钟
  * @param  None
  * @retval None
  */
static void ADC_RCC_Configuration(void)
{
	/* ADCCLK = PCLK2/8 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);

	/* 使能DMA1时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
	
    /* 使能ADC1和GPIOB的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
}

/**
  * @brief  配置ADC输入IO口
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* 配置PB.0做为模拟输入 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  配置DMA
  * @param  None
  * @retval None
  */
static void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	/* DMA1 通道1(channel1) 初始化配置 -------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;    //DMA传输的外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;   //DMA传输结果保存的内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;     //DMA方向是外设为数据源
	DMA_InitStructure.DMA_BufferSize = 1;    //DMA传输数据的总大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   //外设地址固定不增长
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;        //内存地址固定不增长
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;   //DMA传输中外设数据大小为半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;   //内存数据大小为半字
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;       //循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;   //DMA优先级为高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;     //禁止内存到内存的数据传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
	/* 使能 DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);	
}

void ADC_DMA_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	/*ADC1及GPIO时钟初始化*/
	ADC_RCC_Configuration();
	/*GPIO模式配置*/
	GPIO_Configuration();
	/*DMA模式配置*/
	DMA_Configuration();
	
	/* ADC1 初始化配置------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;    // ADC独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;         //禁止使用扫描转换模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;    //使能连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //不使用外部触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;    //数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;      //启用的ADC通道数目
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 通道及采样时间配置，PB.0使用ADC通道8 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);
    
	/* 使能 ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/* 使能ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* 使能 ADC1 复位校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/* 等待ADC1 复位校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* 启动 ADC1 校准 */
	ADC_StartCalibration(ADC1);
	/* 等待 ADC1 校准完成 */
	while(ADC_GetCalibrationStatus(ADC1));
     
	/* 启动 ADC1 软件触发转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
	
	
	
	
/******************* (C) 版权所有 2013 太原联航科技有限公司 *******************/
