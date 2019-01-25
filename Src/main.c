#include "stm32f1xx_hal.h"
#include "main.h"
#include "uart/bsp_uartx.h"
#include "bsp_key.h"
#include "bsp_dac.h"
#include "gpio.h"
#include "mlx90614.h"
#include "TM1650.h"
#include "string.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 系统时钟配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  // Initializes the CPU, AHB and APB busses clocks 
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  // Initializes the CPU, AHB and APB busses clocks 
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
	
	// HAL_RCC_GetHCLKFreq()/1000    1ms中断一次
	// HAL_RCC_GetHCLKFreq()/100000	 10us中断一次
	// HAL_RCC_GetHCLKFreq()/1000000 1us中断一次
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);  // 配置并启动系统滴答定时器
  // 系统滴答定时器时钟源 
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  // 系统滴答定时器中断优先级配置 
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
/*void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  // 外部晶振，8MHz
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;  // 9倍频，得到72MHz主时钟
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;       // 系统时钟：72MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;              // AHB时钟：72MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;               // APB1时钟：36MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;               // APB2时钟：72MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

 	// HAL_RCC_GetHCLKFreq()/1000    1ms中断一次
	// HAL_RCC_GetHCLKFreq()/100000	 10us中断一次
	// HAL_RCC_GetHCLKFreq()/1000000 1us中断一次
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);  // 配置并启动系统滴答定时器
  // 系统滴答定时器时钟源 
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  // 系统滴答定时器中断优先级配置 
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}*/

/**
  * 函数功能: 主函数.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
int main(void)
{
	Module_Init();
  
  while (1)
  {		
		if(flag_commOver == 1)   // command detect
		{
			if(strncmp((char*)comm, "set,1", 5) == 0)
			{
					begin = true;
					
			}
			else if(strncmp((char*)comm, "set,0", 5) == 0)
			{
				begin = false;
			}
			else
			{
				HAL_UART_Transmit(&huartx, (uint8_t*)"Commamd Wrong!\n",strlen("Commamd Wrong!\n"),1000);
			}
			comm[0] = '\0';
			flag_commOver = 0;
		}
		if(begin == 1)  // working status
		{
			HAL_Delay(cycle_ms);
			tempF = SMBus_ReadTemp();
			error = tempSet - tempF;
			integral += error * ((float)cycle_ms / 1000);
			derivative = (error - preErr)/cycle_ms;
			PID_Out = PID_P*error + PID_I*integral + PID_D*derivative;
			preErr = error;
			outputV = PID_Out/2 + INIT_VOL;
			if(outputV >= VOL_MAX)
				outputV = VOL_MAX;
			if(outputV <= VOL_MIN)
				outputV = VOL_MIN;
			HAL_DAC_SetValue(&hdac, DACx_CHANNEL, DAC_ALIGN_8B_R, outputV);
			if(LED_Status == TEMP)
				ShowVal_float(tempF);
			else if(LED_Status == VOL_OUT)
				ShowVal_int(outputV);
			
			sprintf((char*)format_out, "%.2f, %3d, %.2f,\r\n", tempSet, outputV, tempF);
			HAL_UART_Transmit(&huartx, format_out,strlen((char*)format_out),1000);
		}
		
		if(0)			// DAC test
		{
			if(outputV > VOL_MAX)
				outputV = VOL_MAX;
			else if(outputV < VOL_MIN)
				outputV = VOL_MIN;
			HAL_DAC_SetValue(&hdac, DACx_CHANNEL, DAC_ALIGN_8B_R, outputV);
			ShowVal_int(outputV);
		}
		if(0)
		//if(flag_commOver == 1)
		{
			if(strncmp((char*)comm, "temp", 4) == 0)
			{
				tempF = SMBus_ReadTemp();
				
				if(LED_Status == TEMP)		// show temperature
					ShowVal_float(tempF);
				sprintf((char*)tempS, "%f\n", tempF);
				HAL_UART_Transmit(&huartx, tempS,strlen((char*)tempS),1000);
			}
			else
			{
				HAL_UART_Transmit(&huartx, (uint8_t*)"Commamd Wrong!\n",strlen("Commamd Wrong!\n"),1000);
			}
			comm[0] = '\0';
			flag_commOver = 0;
		}	
  }
}

/**
  * 函数功能: 按键外部中断服务函数
  * 输入参数: GPIO_Pin：中断引脚
  * 返 回 值: 无
  * 说    明: 无
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin==KEY1_GPIO_PIN)
  {
    HAL_Delay(20);/* 延时一小段时间，消除抖动 */
    if(HAL_GPIO_ReadPin(KEY_GPIO,KEY1_GPIO_PIN)==KEY1_DOWN_LEVEL)
    {
			if(LED_Status == VOL_OUT)		// if the LED is show the DA's output, add and refresh it
			{
				HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
				outputV++;
				HAL_DAC_SetValue(&hdac, DACx_CHANNEL, DAC_ALIGN_8B_R, outputV);
				ShowVal_int(outputV);
			}
			else if(LED_Status == SET_T)
			{
				tempSet++;
				ShowVal_float(tempSet);
			}
    }
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_GPIO_PIN);
  }
  else if(GPIO_Pin==KEY2_GPIO_PIN)
  {
    HAL_Delay(20);/* 延时一小段时间，消除抖动 */
    if(HAL_GPIO_ReadPin(KEY_GPIO,KEY2_GPIO_PIN)==KEY2_DOWN_LEVEL)
    {
			if(LED_Status == VOL_OUT)		// if the LED is show the DA's output, sub and refresh it
			{
				HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
				outputV--;
				HAL_DAC_SetValue(&hdac, DACx_CHANNEL, DAC_ALIGN_8B_R, outputV);
				ShowVal_int(outputV);
			}
			else if(LED_Status == SET_T)
			{
				tempSet--;
				ShowVal_float(tempSet);
			}
    }
    __HAL_GPIO_EXTI_CLEAR_IT(KEY2_GPIO_PIN);
  }
	else if(GPIO_Pin==KEY3_GPIO_PIN)
  {
    HAL_Delay(20);/* 延时一小段时间，消除抖动 */
    if(HAL_GPIO_ReadPin(KEY_GPIO,KEY3_GPIO_PIN)==KEY3_DOWN_LEVEL)
    {
			if(LED_Status == TEMP)
			{
				LED_Status = VOL_OUT;
				ShowVal_int(outputV);
			}
			else if(LED_Status == VOL_OUT)
			{
				LED_Status = SET_T;
				ShowVal_float(tempSet);
			}
			else if(LED_Status == SET_T)
			{
				LED_Status = TEMP;
				ShowVal_float(tempF);
			}
      HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
    }
    __HAL_GPIO_EXTI_CLEAR_IT(KEY3_GPIO_PIN);
  }
	else if(GPIO_Pin==KEY4_GPIO_PIN)
  {
    HAL_Delay(20);/* 延时一小段时间，消除抖动 */
    if(HAL_GPIO_ReadPin(KEY_GPIO,KEY4_GPIO_PIN)==KEY4_DOWN_LEVEL)
    {
			outputV = 94;			// reset the outputV
			tempSet = 25.0;
			HAL_DAC_SetValue(&hdac, DACx_CHANNEL, DAC_ALIGN_8B_R, outputV);
			LED_Status = VOL_OUT;
			ShowVal_int(outputV);
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
    }
    __HAL_GPIO_EXTI_CLEAR_IT(KEY4_GPIO_PIN);
  }
}


/**
  * 函数功能: 串口接收完成回调函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if((aRxBuffer != '\n') & (count_commChr < 20))
	{
		count_commChr++;
		strncat((char*)comm, (char*)&aRxBuffer, 1);
	}
	else
	{
		flag_commOver = 1;
		count_commChr = 0;
	}
  //HAL_UART_Transmit(&huartx,&aRxBuffer,1,0);
  HAL_UART_Receive_IT(&huartx,&aRxBuffer,1);
}

/**
  * 函数功能: show int value in LED segments
  * 输入参数: val
  * 返 回 值: none
  * 说    明：none
  */
void ShowVal_int(uint8_t val)
{
	Display_L1(num[val/1000]);
	Display_L2(num[val%1000/100]);
	Display_L3(num[val%100/10]);
	Display_L4(num[val%10]);
}

/**
  * 函数功能: show float value in LED segments
  * 输入参数: val in float
  * 返 回 值: none
  * 说    明：none
  */
void ShowVal_float(float val)
{
	uint32_t temp = 0.0;
	if(val >= 100 && val < 1000)
	{
		temp = val * 10;
		Display_L1(num[temp/1000]);
		Display_L2(num[temp%1000/100]);
		Display_L3(num[temp%100/10] | 0x80);
		Display_L4(num[temp%10]);
	}
	else if(val >= 10)
	{
		temp = val * 100;
		Display_L1(num[temp/1000]);
		Display_L2(num[temp%1000/100] | 0x80);
		Display_L3(num[temp%100/10]);
		Display_L4(num[temp%10]);
	}
	else if(val >= 0)
	{
		temp = val * 1000;
		Display_L1(num[temp/1000] | 0x80);
		Display_L2(num[temp%1000/100]);
		Display_L3(num[temp%100/10]);
		Display_L4(num[temp%10]);
	}
	else if(val >= -0.01)
	{
		temp = val * -100;
		Display_L1(0x40);			// sign -
		Display_L2(num[0] | 0x80);	// 0.
		Display_L3(num[temp/10]);
		Display_L4(num[temp%10]);
	}
	else	// the temp is out of range
	{
		Display_L1(0x00);		// nothing
		Display_L2(0x79);		// E
		Display_L3(0x50);		// r
		Display_L4(0x50);		// r
	}
}

/**
  * 函数功能: Init all used modules 
  * 输入参数: none
  * 返 回 值: none
  * 说    明：none
  */
void Module_Init(void)
{
  HAL_Init();						/* 复位所有外设，初始化Flash接口和系统滴答定时器 */
  SystemClock_Config();	/* 配置系统时钟 */
  KEY_GPIO_Init();			/* 按键初始化 */
	
	MX_GPIO_Init();				// GPIO init
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);		// light the LED
	
  MX_DAC_Init();				// DAC init
  HAL_DAC_SetValue(&hdac, DACx_CHANNEL, DAC_ALIGN_8B_R, outputV);		// Init the DAC's output
  HAL_DAC_Start(&hdac, DACx_CHANNEL);		/* 启动DAC */
	
  MX_UARTx_Init();			/* 初始化串口并配置串口中断优先级 */
	HAL_UART_Transmit(&huartx, (uint8_t*)"\t\tPCR Temp Control System\t\t\n",strlen("\t\tPCR Temp Control System\t\t\n"),1000);
  HAL_UART_Receive_IT(&huartx,&aRxBuffer,1);		/* 使能接收，进入中断回调函数 */
	
  SMBus_Init();					// init mlx90614/5
	
	TM1650_Init();				// Init TM1650
	tempSet = SMBus_ReadTemp();
	ShowVal_float(tempSet);		// Init the segments' display to 0000
}
	
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{

}
