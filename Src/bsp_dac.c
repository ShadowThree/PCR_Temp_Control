#include "bsp_dac.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
DAC_HandleTypeDef hdac;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: DAC初始化配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void MX_DAC_Init(void)
{
  DAC_ChannelConfTypeDef sConfig;

  /* DAC初始化 */
  hdac.Instance = DACx;
  HAL_DAC_Init(&hdac);

 /* DAC通道输出配置 */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);
	HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2);

}

/**
  * 函数功能: DAC初始化配置
  * 输入参数: hdac：DAC外设句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hdac->Instance==DACx)
  {
    /* DAC外设时钟使能 */
    DACx_CLK_ENABLE();
    
    /* DAC通道引脚端口时钟使能 */
    DACx_CHANNEL_GPIO_CLK_ENABLE();
    
    /* DAC通道引脚配置 */
    GPIO_InitStruct.Pin = DACx_CHANNEL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DACx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);
  }
}

/**
  * 函数功能: DAC反初始化配置
  * 输入参数: hdac：DAC外设句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{

  if(hdac->Instance==DAC)
  {
    /* 复位DAC */
    DACx_FORCE_RESET();
    DACx_RELEASE_RESET();
    
    /* 禁用DAC外设时钟 */
    DACx_CHANNEL_GPIO_CLK_DISABLE();
  
    /* 反初始化DAC通道引脚 */
    HAL_GPIO_DeInit(DACx_CHANNEL_GPIO_PORT, DACx_CHANNEL_PIN);
  }
} 
