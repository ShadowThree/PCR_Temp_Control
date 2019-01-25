/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stdbool.h"
/* Exported macro ------------------------------------------------------------*/
#define VOL_MAX 		145
#define VOL_MIN 		40
#define INIT_VOL 		94
	
/* Private includes ----------------------------------------------------------*/
#define COMM_BUFSIZE 32
bool begin = 0;
uint8_t aRxBuffer;
uint8_t flag_commOver = 0;
uint8_t comm[20] = {'\0'};
uint8_t count_commChr = 0;
float tempF = 0.0;
uint8_t tempS[10] = {'\0'};
static uint8_t num[16] = {0x3f, 0x06, 0x5b, 0x4f, 
													0x66, 0x6d, 0x7d, 0x07,
													0x7f, 0x6f, 0x77, 0x7c,
													0x39, 0x5e, 0x79, 0x71};
/* DAC输出对应值：可设置0~255，对应引脚输出0~3.3V，该值越大，引脚输出电压越高*/
uint8_t outputV = INIT_VOL;
uint8_t format_out[20] = {'\0'};
float tempSet = 35.0;
													
float PID_P = 0.9;
float PID_I = 1.0;
float PID_D = 0.0;

float preErr = 0;
float integral = 0;
float error = 0;
uint16_t cycle_ms = 1000;
float derivative = 0;			
float PID_Out = 0.0;
													
/* Exported types ------------------------------------------------------------*/
enum {
TEMP = 0,
VOL_OUT,
SET_P,
SET_I,
SET_D,
SET_T
} LED_Status = TEMP;
/* Exported constants --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void SystemClock_Config(void);
void ShowVal_float(float val);
void ShowVal_int(uint8_t val);
void Module_Init(void);
/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
