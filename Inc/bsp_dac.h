#ifndef __DAC_H__
#define	__DAC_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define DACx                             DAC
#define DACx_CHANNEL_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define DACx_CHANNEL_GPIO_CLK_DISABLE()  __HAL_RCC_DAC_CLK_DISABLE()
#define DACx_CLK_ENABLE()                __HAL_RCC_DAC_CLK_ENABLE()
#define DACx_FORCE_RESET()               __HAL_RCC_DAC_FORCE_RESET()
#define DACx_RELEASE_RESET()             __HAL_RCC_DAC_RELEASE_RESET()

// ����DACͨ������
#define DACx_CHANNEL_PIN                 GPIO_PIN_4
#define DACx_CHANNEL_GPIO_PORT           GPIOA

// ����DACͨ��
#define DACx_CHANNEL                     DAC_CHANNEL_1

/* ��չ���� ------------------------------------------------------------------*/
extern DAC_HandleTypeDef hdac;
/* �������� ------------------------------------------------------------------*/
void MX_DAC_Init(void);

#endif /* __DAC_H */
