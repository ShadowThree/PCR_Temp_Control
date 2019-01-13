/**
  ******************************************************************************
  * 文件名程: bsp_dac.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: DAC实现电压输出
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
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
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
  HAL_DAC_ConfigChannel(&hdac, &sConfig, DACx_CHANNEL);

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

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
