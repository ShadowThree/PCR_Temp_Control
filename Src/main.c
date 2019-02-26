#include "main.h"

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

	uint16_t rdata[1];
	STMFLASH_Read(Test_Addr, rdata, 1);	
	while(rdata[0] != TestValue)		// 测试之前是否写入过数据，若写入过则直接读取数据，否则先写入数据。
	{
		SaveParam();
		STMFLASH_Read(Test_Addr, rdata, 1);
	}
	
	while(0)
	{
		if(timer_count==1)
		{
			timer_count=0;
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
		}
	}
  while(1)
	{	
		// if comm is NOT null and already over.
		if((comm[0] != '\0') && (comm[strlen((char*)comm)-1] == '\n'))
		{
			args = cmd_split((char*)comm, LSH_TOK_DELIM);
			if(cmd_execute(args)) 		// if execute succssful, send OK
				HAL_UART_Transmit(&huartx, (uint8_t*)"OK\r\n\r\n", strlen("OK\r\n\r\n"), 1000);
			for(int i = 0; i < COMM_BUFSIZE; i++)  		// reset comm[]
				comm[i] = '\0';
			free(args);
		}
		HAL_Delay(10);
		
		if(TEMP_CTRL_STA != TEMP_CTRL_STOP)  // working status
		{
			tempF = SMBus_ReadTemp();
			
			if((tempF >= higTemp-1) && (TIM_STA == TIM_STA_OFF) &&(TEMP_CTRL_STA == TEMP_CTRL_PRE_HEAT))
			{
				HAL_TIM_Base_Start_IT(&htimx); 		// 启动定时器
				TIM_STA = TIM_STA_ON;
			}
			else if((tempF >= higTemp-1) && (TIM_STA == TIM_STA_OFF) &&(TEMP_CTRL_STA == TEMP_CTRL_HIGH))
			{
				HAL_TIM_Base_Start_IT(&htimx); 		// 启动定时器
				TIM_STA = TIM_STA_ON;
			}
			else if((tempF <= lowTemp+1) && (TIM_STA == TIM_STA_OFF) &&(TEMP_CTRL_STA == TEMP_CTRL_LOW))
			{
				HAL_TIM_Base_Start_IT(&htimx); 		// 启动定时器
				TIM_STA = TIM_STA_ON;
			}
			else if((tempF >= midTemp-1) && (TIM_STA == TIM_STA_OFF) &&(TEMP_CTRL_STA == TEMP_CTRL_MID))
			{
				HAL_TIM_Base_Start_IT(&htimx); 		// 启动定时器
				TIM_STA = TIM_STA_ON;
			}
			
			error = tempSet - tempF;
			integral += error * ((float)cycle_ms / 1000);
			derivative = (error - preErr)/cycle_ms;
			PID_Out = PID_P*error + PID_I*integral + PID_D*derivative;
			preErr = error;
			outputV = PID_Out/2 + VOL_INIT;
			
			if(outputV >= VOL_MAX)
				outputV = VOL_MAX;
			if(outputV <= VOL_MIN)
				outputV = VOL_MIN;
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, (uint8_t)outputV);
			if(LED_Status == TEMP)
				ShowVal_float(tempF);
			else if(LED_Status == VOL_OUT)
				ShowVal_int(outputV);
			
			sprintf((char*)format_out, "%.2f, %3d, %.2f,\r\n", tempSet, outputV, tempF);
			HAL_UART_Transmit(&huartx, format_out,strlen((char*)format_out),1000);
			
			HAL_Delay(cycle_ms);		// 采样间隔时间
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
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, outputV);
				//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, outputV);
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
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, outputV);
				//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, outputV);
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
			//tempSet = 25.0;
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, VOL_INIT);
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, VOL_INIT);
			LED_Status = VOL_OUT;
			ShowVal_int(VOL_INIT);
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
			
			if(TIM_STA != TIM_STA_OFF)
			{
				HAL_TIM_Base_Stop_IT(&htimx);// 停止计时
				TIM_STA = TIM_STA_OFF;
			}
			cycle_count = 0;
			integral = 0;
			TEMP_CTRL_STA = TEMP_CTRL_STOP;
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
	if(aRxBuffer != '\r')
	{
		strncat((char*)comm, (char*)&aRxBuffer, 1);
		count_commChr++;
	}
	else
	{
		strncat((char*)comm, (char*)"\n", 1);
	}
	
  //HAL_UART_Transmit(&huartx,&aRxBuffer,1,0);
  HAL_UART_Receive_IT(&huartx,&aRxBuffer,1);
}

/**
  * 函数功能: 非阻塞模式下定时器的回调函数(每500ms进入一次)
  * 输入参数: htim：定时器句柄
  * 返 回 值: 无
  * 说    明: 无
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	timer_count++;
	if(TEMP_CTRL_STA == TEMP_CTRL_PRE_HEAT)
	{
		if(timer_count >= preHeat_T*2)
		{
			timer_count = 0;
			integral = 0;
			TEMP_CTRL_STA  = TEMP_CTRL_HIGH;
			HAL_TIM_Base_Stop_IT(&htimx);// 停止计时
			TIM_STA = TIM_STA_OFF;
			tempSet = higTemp;
		}
	}
	else if(TEMP_CTRL_STA == TEMP_CTRL_HIGH)
	{
		if(timer_count >= higTemp_T*2)
		{
			timer_count = 0;
			integral = 0;
			TEMP_CTRL_STA = TEMP_CTRL_LOW;
			HAL_TIM_Base_Stop_IT(&htimx);// 停止计时
			TIM_STA = TIM_STA_OFF;
			tempSet = lowTemp;
		}
	}
	else if(TEMP_CTRL_STA == TEMP_CTRL_LOW)
	{
		if(timer_count >= lowTemp_T*2)
		{
			timer_count = 0;
			integral = 0;
			TEMP_CTRL_STA = TEMP_CTRL_MID;
			HAL_TIM_Base_Stop_IT(&htimx);// 停止计时
			TIM_STA = TIM_STA_OFF;
			tempSet = midTemp;
		}
	}
	else if(TEMP_CTRL_STA == TEMP_CTRL_MID)
	{
		if(timer_count >= midTemp_T*2)
		{
			timer_count = 0;
			HAL_TIM_Base_Stop_IT(&htimx);// 停止计时
			TIM_STA = TIM_STA_OFF;
			integral = 0;
			if(++cycle_count >= Cycle_Num)		// 达到了PCR要求的循环次数
			{	
				TEMP_CTRL_STA = TEMP_CTRL_STOP;
				cycle_count = 0;
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, VOL_INIT);
			}
			else
			{
				TEMP_CTRL_STA = TEMP_CTRL_HIGH;
				tempSet = higTemp;
			}
		}
	}
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
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, VOL_INIT);		// Init the DAC's output
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1); 			// 启动DAC channel 1 
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, VOL_INIT);		// Init the DAC's output
  HAL_DAC_Start(&hdac, DAC_CHANNEL_2);		 // 启动DAC channel 2 
	
  MX_UARTx_Init();			 // 初始化串口并配置串口中断优先级 
	HAL_UART_Transmit(&huartx, (uint8_t*)"\t\tPCR Temp Control System\r\n",strlen("\t\tPCR Temp Control System\r\n"),1000);
  HAL_UART_Receive_IT(&huartx,&aRxBuffer,1);  	 // 使能接收，进入中断回调函数 
	
  SMBus_Init();					// init mlx90614/5
	
	TM1650_Init();				// Init TM1650
	tempSet = SMBus_ReadTemp();
	ShowVal_float(tempSet);		// Init the segments' display to 0000

	GENERAL_TIMx_Init();		// init the timer2, 50ms
}
	
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{}

/***
		ST,1 		// start
		ST,0 		// stop
	***/
int cmd_ST(char **args)
{
	if ((args[1] == NULL) || (args[2] != NULL)) 		// 参数不正确
	{
		HAL_UART_Transmit(&huartx, (uint8_t*)"Amount of Parameter Error!\r\n", strlen("Amount of Patameter Error!\r\n"), 1000);
		return 0;
	}
	else
	{
		int num = atoi(args[1]);
		
		if(num == 0)    		// command = "ST 0"
		{
			TEMP_CTRL_STA = TEMP_CTRL_STOP;
			integral = 0;
			if(TIM_STA != TIM_STA_OFF)
			{
				HAL_TIM_Base_Stop_IT(&htimx);// 停止计时
				TIM_STA = TIM_STA_OFF;
			}
			cycle_count = 0;
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, VOL_INIT);
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, VOL_INIT);
		}
		else if(num == 1)		// command = "ST 1"
		{
			TEMP_CTRL_STA = TEMP_CTRL_PRE_HEAT;
			tempSet = higTemp;
		}
		else								// comm
		{
			HAL_UART_Transmit(&huartx, (uint8_t*)"Parameter Wrong!\r\n", strlen("Parameter Wrong!\r\n"), 1000);
			return 0;
		}
	}
	return 1;
}

/*
PS,1,25.0		// set standby temp
PS,2,10.0		// set low temp
PS,3,45.0 	// set mid temp
PS,4,90.0		// set hign temp
PS,5,30 		// set cycle count
PS,6,1000		// 
PS,7,25.0		// set target temp	

PS,11,0.1 	// set PID_P value
PS,12,0.1 	// set PID_I value
PS,13,0.1 	// set PID_D value
*/
int cmd_PS(char **args)
{
	if (args[1] == NULL) 		// 参数不足
	{
		HAL_UART_Transmit(&huartx, (uint8_t*)"Expected argument to \"PS\"\r\n", strlen("Expected argument to \"PS\"\r\n"), 1000);
		return 0;
	} 
	else if(args[2] == NULL)	// 查询命令
	{
		int num = atoi(args[1]);
		char paramStr[10] = {'\0'};
		switch(num)
		{
			case 1:
				break;
			case 2:				// command = "PS 2"
				HAL_UART_Transmit(&huartx, (uint8_t*)"Check lowTemp: ", strlen("Check lowTemp:　"), 1000);
				sprintf(paramStr, "%.1f\r\n", lowTemp);
				HAL_UART_Transmit(&huartx, (uint8_t*)paramStr, strlen(paramStr), 1000);
				break;
			case 3:				// command = "PS 3"
				HAL_UART_Transmit(&huartx, (uint8_t*)"Check midTemp: ", strlen("Check midTemp:　"), 1000);
				sprintf(paramStr, "%.1f\r\n", midTemp);
				HAL_UART_Transmit(&huartx, (uint8_t*)paramStr, strlen(paramStr), 1000);
				break;
			case 4:				// command = "PS 4"
				HAL_UART_Transmit(&huartx, (uint8_t*)"Check higTemp: ", strlen("Check higTemp:　"), 1000);
				sprintf(paramStr, "%.1f\r\n", higTemp);
				HAL_UART_Transmit(&huartx, (uint8_t*)paramStr, strlen(paramStr), 1000);
				break;
			case 7:				// command = "PS 7"
				HAL_UART_Transmit(&huartx, (uint8_t*)"Check tempSet: ", strlen("Check tempSet:　"), 1000);
				sprintf(paramStr, "%.1f\r\n", tempSet);
				HAL_UART_Transmit(&huartx, (uint8_t*)paramStr, strlen(paramStr), 1000);
				break;
			case 11:				// command = "PS 11"
				HAL_UART_Transmit(&huartx, (uint8_t*)"Check PID_P value: ", strlen("Check PID_P value:　"), 1000);
				sprintf(paramStr, "%.1f\r\n", PID_P);
				HAL_UART_Transmit(&huartx, (uint8_t*)paramStr, strlen(paramStr), 1000);
				break;
			case 12:				// command = "PS 12"
				HAL_UART_Transmit(&huartx, (uint8_t*)"Check PID_I value: ", strlen("Check PID_I value:　"), 1000);
				sprintf(paramStr, "%.1f\r\n", PID_I);
				HAL_UART_Transmit(&huartx, (uint8_t*)paramStr, strlen(paramStr), 1000);
				break;
			case 13:				// command = "PS 13"
				HAL_UART_Transmit(&huartx, (uint8_t*)"Check PID_D value: ", strlen("Check PID_D value:　"), 1000);
				sprintf(paramStr, "%.1f\r\n", PID_D);
				HAL_UART_Transmit(&huartx, (uint8_t*)paramStr, strlen(paramStr), 1000);
				break;
			default:
				HAL_UART_Transmit(&huartx, (uint8_t*)"Parameter Wrong!\r\n", strlen("Parameter Wrong!\r\n"), 1000);
				return 0;
		}
  }
	else		// 设置命令  PS 7 23.0
	{
		int num = atoi(args[1]);
		char str[30] = {'\0'};
		switch(num)
		{
			case 1:
				break;
			case 2:
				lowTemp = strTof(args[2]);
				sprintf(str, "Set lowTemp value to %.1f\r\n", lowTemp);
				HAL_UART_Transmit(&huartx, (uint8_t*)str, strlen(str), 1000);
				break;
			case 3:
				midTemp = strTof(args[2]);
				sprintf(str, "Set midTemp value to %.1f\r\n", midTemp);
				HAL_UART_Transmit(&huartx, (uint8_t*)str, strlen(str), 1000);
				break;
			case 4:
				higTemp = strTof(args[2]);
				sprintf(str, "Set higTemp value to %.1f\r\n", higTemp);
				HAL_UART_Transmit(&huartx, (uint8_t*)str, strlen(str), 1000);
				break;
			case 7:
				tempSet = strTof(args[2]);
				sprintf(str, "Set tempSet value to %.1f\r\n", tempSet);
				HAL_UART_Transmit(&huartx, (uint8_t*)str, strlen(str), 1000);
				break;
			case 11:
				PID_P = strTof(args[2]);
				sprintf(str, "Set PID_P value to %.1f\r\n", PID_P);
				HAL_UART_Transmit(&huartx, (uint8_t*)str, strlen(str), 1000);
				break;
			case 12:
				PID_I = strTof(args[2]);
				sprintf(str, "Set PID_I value to %.1f\r\n", PID_I);
				HAL_UART_Transmit(&huartx, (uint8_t*)str, strlen(str), 1000);
				break;
			case 13:
				PID_D = strTof(args[2]);
				sprintf(str, "Set PID_D value to %.1f\r\n", PID_D);
				HAL_UART_Transmit(&huartx, (uint8_t*)str, strlen(str), 1000);
				break;
			default:
				HAL_UART_Transmit(&huartx, (uint8_t*)"Parameter Wrong!\r\n", strlen("Parameter Wrong!\r\n"), 1000);
				return 0;
		}
	}
  return 1;
}

int cmd_GS(char **args)
{
	if ((args[1] == NULL) || (args[2] != NULL)) 		// 参数不正确
	{
		HAL_UART_Transmit(&huartx, (uint8_t*)"Amount of Parameter Error!\r\n", strlen("Amount of Patameter Error!\r\n"), 1000);
		return 0;
	}
	else
	{
		int num = atoi(args[1]);
		
		if(num == 1)    		// command = "GS 1"
		{
			if(TEMP_CTRL_STA != TEMP_CTRL_STOP)
				HAL_UART_Transmit(&huartx, (uint8_t*)"Running\r\n", strlen("Running\r\n"), 1000);
			else
				HAL_UART_Transmit(&huartx, (uint8_t*)"Stop\r\n", strlen("Stop\r\n"), 1000);
		}
		else if(num == 2)		// command = "GS 2"
		{
			char str[30] = {'\0'};
			sprintf(str, "Current Temp is %.1f\r\n", SMBus_ReadTemp());
			HAL_UART_Transmit(&huartx, (uint8_t*)str, strlen(str), 1000);
		}
		else								// comm
		{
			HAL_UART_Transmit(&huartx, (uint8_t*)"Parameter Wrong!\r\n", strlen("Parameter Wrong!\r\n"), 1000);
			return 0;
		}
	}
	return 1;
}

int cmd_VR(char **args)
{
	return 1;
}
	
int cmd_HP(char **args)
{
	return 1;
}

char **cmd_split(char *line, char *delim)
{
  int position = 0;
  char **tokens = malloc(8 * sizeof(char*));
	char *token;
	
	if (!tokens) 
	{
		HAL_UART_Transmit(&huartx, (uint8_t*)"Allocation error!\r\n", strlen("Allocation error!\r\n"), 1000);
    return NULL;
  }

	HAL_UART_Transmit(&huartx, (uint8_t*)"Comm = \"", strlen("Comm = \""), 1000);
  token = strtok(line, delim);
	HAL_UART_Transmit(&huartx, (uint8_t*)token, strlen(token), 1000);
	
  while (token != NULL) 
	{		
    tokens[position] = token;
    position++;

    token = strtok(NULL, delim);
		if(token)
		{
			HAL_UART_Transmit(&huartx, (uint8_t*)" ", strlen(" "), 1000);
			HAL_UART_Transmit(&huartx, (uint8_t*)token, strlen(token), 1000);
		}
  }
	HAL_UART_Transmit(&huartx, (uint8_t*)"\"\r\n", strlen("\"\r\n"), 1000);
  tokens[position] = NULL;
  return tokens;
}

int cmd_execute(char **args)
{
  int i;

  if (args[0] == NULL) 	// An empty command was entered.
    return 1;

  for (i = 0; i < cmd_count(); i++) 
	{
    if (strcmp(args[0], cmd_str[i]) == 0) 
      return (*cmd_funcP[i])(args);
  }
  return 0;
}

int cmd_count(void) {
  return sizeof(cmd_str) / sizeof(char *);
}

float strTof(const char *args)
{
  float result = 0.0;
  unsigned int i, quan = 1;
  unsigned char afterP = 0;
  int sign = 1;

	for(i = 0; args[i] != '\0'; i++)
  {
    if(i == 0)
    {
			if((args[0] == '-') || (args[0] == '+'))
      {
				continue;
      }
    }
    if(args[i] == '.')
    {
			if(++afterP > 1)
				HAL_UART_Transmit(&huartx, (uint8_t*)"The str's format is NOT correct!\r\n", strlen("The str's format is NOT correct!\r\n"), 1000);
    }
		else if(args[i] <= '9' && args[i] >= '0')
		{
		 if(afterP == 0)
		 {
			 result = result * 10 + args[i] - '0';
		 }
		 else
		 {
			 quan *= 10;
			 if(quan > UINT_MAX/10)
			 {
				 HAL_UART_Transmit(&huartx, (uint8_t*)"Insufficient precision!\r\n", strlen("Insufficient precision!\r\n"), 1000);
				 return result;
			 }
			 result = result + (float)(args[i] - '0') / quan;
		 }
		}
		else
		{
			HAL_UART_Transmit(&huartx, (uint8_t*)"The str's format is NOT correct!\r\n", strlen("The str's format is NOT correct!\r\n"), 1000);
			return 0.0;
		}
	}

	sign = (args[0] == '-')? -1 : 1;
	return (sign * result);
}
