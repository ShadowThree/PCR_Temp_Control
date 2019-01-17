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
  uint8_t txbuf[100];
  /* 复位所有外设，初始化Flash接口和系统滴答定时器 */
  HAL_Init();
  /* 配置系统时钟 */
  SystemClock_Config();
	
	/* 板子按键初始化 */
  KEY_GPIO_Init();

	MX_GPIO_Init();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	
	/****************************************************************************DAC**************************************************************/
  MX_DAC_Init();
	/* DAC输出对应值：可设置0~255，对应引脚输出0~3.3V，该值越大，引脚输出电压越高*/
	uint8_t dac_value=127;
	/* 设置DAC通道值 */
  HAL_DAC_SetValue(&hdac, DACx_CHANNEL, DAC_ALIGN_8B_R, dac_value);
  /* 启动DAC */
  HAL_DAC_Start(&hdac, DACx_CHANNEL);
  /* 初始化串口并配置串口中断优先级 */
	
	/****************************************************************************USART**************************************************************/
  MX_UARTx_Init();
  memcpy(txbuf,"这是一个串口中断接收回显实验\n",100);
  HAL_UART_Transmit(&huartx,txbuf,strlen((char *)txbuf),1000);
  memcpy(txbuf,"输入数据并以回车键结束\n",100);
  HAL_UART_Transmit(&huartx,txbuf,strlen((char *)txbuf),1000);
  /* 使能接收，进入中断回调函数 */
  HAL_UART_Receive_IT(&huartx,&aRxBuffer,1);
	
	/****************************************************************************I2C**************************************************************/
  SMBus_Init();		// init mlx90614/5
	
	TM1650_Init();	// Init TM1650
  
  while (1)
  {	
		/*
		if(0)			// DAC test
		{
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
			HAL_Delay(500);
			dac_value += 10;
			if(dac_value > 255)
				dac_value = 0;
			HAL_DAC_SetValue(&hdac, DACx_CHANNEL, DAC_ALIGN_8B_R, dac_value);
		}*/
		
		if(flag_commOver == 1)
		{
			if(strncmp((char*)comm, "temp", 4) == 0)
			{
				tempF = SMBus_ReadTemp();
				uint32_t temp = 0.0;
				if(tempF >= 100 && tempF < 1000)
				{
					temp = tempF * 10;
					Display_L1(num[temp/1000]);
					Display_L2(num[temp%1000/100]);
					Display_L3(num[temp%100/10] | 0x80);
					Display_L4(num[temp%10]);
				}
				else if(tempF >= 10)
				{
					temp = tempF * 100;
					Display_L1(num[temp/1000]);
					Display_L2(num[temp%1000/100] | 0x80);
					Display_L3(num[temp%100/10]);
					Display_L4(num[temp%10]);
				}
				else if(tempF >= 0)
				{
					temp = tempF * 1000;
					Display_L1(num[temp/1000] | 0x80);
					Display_L2(num[temp%1000/100]);
					Display_L3(num[temp%100/10]);
					Display_L4(num[temp%10]);
				}
				else if(tempF >= -0.01)
				{
					temp = tempF * -100;
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
				sprintf((char*)tempS, "%f\n", tempF);
				HAL_UART_Transmit(&huartx, tempS,strlen((char*)tempS),1000);
			}
			else
			{
				HAL_UART_Transmit(&huartx, (uint8_t*)"Commamd Wrong!\n",strlen("Commamd Wrong!\n"),1000);
			}
	    
			comm[0] = '\0';
			flag_commOver = 0;
			dac_value += 10;
			if(dac_value > 255)
			dac_value = 0;
			HAL_DAC_SetValue(&hdac, DACx_CHANNEL, DAC_ALIGN_8B_R, dac_value);
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
      HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
    }
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_GPIO_PIN);
  }
  else if(GPIO_Pin==KEY2_GPIO_PIN)
  {
    HAL_Delay(20);/* 延时一小段时间，消除抖动 */
    if(HAL_GPIO_ReadPin(KEY_GPIO,KEY2_GPIO_PIN)==KEY2_DOWN_LEVEL)
    {
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
    }
    __HAL_GPIO_EXTI_CLEAR_IT(KEY2_GPIO_PIN);
  }
	else if(GPIO_Pin==KEY3_GPIO_PIN)
  {
    HAL_Delay(20);/* 延时一小段时间，消除抖动 */
    if(HAL_GPIO_ReadPin(KEY_GPIO,KEY3_GPIO_PIN)==KEY3_DOWN_LEVEL)
    {
      HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
    }
    __HAL_GPIO_EXTI_CLEAR_IT(KEY3_GPIO_PIN);
  }
	else if(GPIO_Pin==KEY4_GPIO_PIN)
  {
    HAL_Delay(20);/* 延时一小段时间，消除抖动 */
    if(HAL_GPIO_ReadPin(KEY_GPIO,KEY4_GPIO_PIN)==KEY4_DOWN_LEVEL)
    {
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
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/



