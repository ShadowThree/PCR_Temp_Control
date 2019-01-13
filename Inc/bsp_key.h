#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* 类型定义 --------------------------------------------------------------*/
typedef enum
{
  KEY_UP   = 0,
  KEY_DOWN = 1,
}KEYState_TypeDef;

/* 宏定义 --------------------------------------------------------------------*/
#define KEY_RCC_CLK_ENABLE           __HAL_RCC_GPIOC_CLK_ENABLE
#define KEY_GPIO                     GPIOC

#define KEY1_GPIO_PIN                 GPIO_PIN_0
#define KEY1_DOWN_LEVEL               0  /* 根据原理图设计，KEY1按下时引脚为高电平，所以这里设置为1 */
#define KEY1_EXTI_IRQn                EXTI0_IRQn
#define KEY1_EXTI_IRQHandler          EXTI0_IRQHandler

#define KEY2_GPIO_PIN                 GPIO_PIN_1
#define KEY2_DOWN_LEVEL               0  /* 根据原理图设计，KEY1按下时引脚为低电平，所以这里设置为0 */
#define KEY2_EXTI_IRQHandler          EXTI1_IRQHandler
#define KEY2_EXTI_IRQn                EXTI1_IRQn

#define KEY3_GPIO_PIN                 GPIO_PIN_2
#define KEY3_DOWN_LEVEL               0  /* 根据原理图设计，KEY1按下时引脚为低电平，所以这里设置为0 */
#define KEY3_EXTI_IRQHandler          EXTI2_IRQHandler
#define KEY3_EXTI_IRQn                EXTI2_IRQn

#define KEY4_GPIO_PIN                 GPIO_PIN_3
#define KEY4_DOWN_LEVEL               0  /* 根据原理图设计，KEY1按下时引脚为低电平，所以这里设置为0 */
#define KEY4_EXTI_IRQHandler          EXTI3_IRQHandler
#define KEY4_EXTI_IRQn                EXTI3_IRQn

/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void KEY_GPIO_Init(void);
KEYState_TypeDef KEY1_StateRead(void);
KEYState_TypeDef KEY2_StateRead(void);
KEYState_TypeDef KEY3_StateRead(void);
KEYState_TypeDef KEY4_StateRead(void);

#endif  // __BSP_KEY_H__

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
