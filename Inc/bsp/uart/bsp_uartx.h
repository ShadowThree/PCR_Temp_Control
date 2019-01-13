#ifndef __BSP_UARTX_H__
#define __BSP_UARTX_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define UARTx                                 USART1
#define UARTx_BAUDRATE                        115200
#define UART_RCC_CLK_ENABLE()                 __HAL_RCC_USART1_CLK_ENABLE()
#define UART_RCC_CLK_DISABLE()                __HAL_RCC_USART1_CLK_DISABLE()

#define UARTx_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define UARTx_Tx_GPIO_PIN                     GPIO_PIN_9
#define UARTx_Tx_GPIO                         GPIOA
#define UARTx_Rx_GPIO_PIN                     GPIO_PIN_10
#define UARTx_Rx_GPIO                         GPIOA

#define UARTx_IRQHANDLER                      USART1_IRQHandler
#define UARTx_IRQn                            USART1_IRQn


/* ��չ���� ------------------------------------------------------------------*/
extern UART_HandleTypeDef huartx;

/* �������� ------------------------------------------------------------------*/
void MX_UARTx_Init(void);


#endif  /* __BSP_UARTX_H__ */

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
