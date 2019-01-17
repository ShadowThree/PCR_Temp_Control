#ifndef __DAC_H__
#define	__DAC_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
#define DACx                             DAC
#define DACx_CHANNEL_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define DACx_CHANNEL_GPIO_CLK_DISABLE()  __HAL_RCC_DAC_CLK_DISABLE()
#define DACx_CLK_ENABLE()                __HAL_RCC_DAC_CLK_ENABLE()
#define DACx_FORCE_RESET()               __HAL_RCC_DAC_FORCE_RESET()
#define DACx_RELEASE_RESET()             __HAL_RCC_DAC_RELEASE_RESET()

// 定义DAC通道引脚
#define DACx_CHANNEL_PIN                 GPIO_PIN_4
#define DACx_CHANNEL_GPIO_PORT           GPIOA

// 定义DAC通道
#define DACx_CHANNEL                     DAC_CHANNEL_1

/* 扩展变量 ------------------------------------------------------------------*/
extern DAC_HandleTypeDef hdac;
/* 函数声明 ------------------------------------------------------------------*/
void MX_DAC_Init(void);

#endif /* __DAC_H */
