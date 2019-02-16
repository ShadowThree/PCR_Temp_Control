#include "main.h"

//#include "command.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ϵͳʱ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
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
	
	// HAL_RCC_GetHCLKFreq()/1000    1ms�ж�һ��
	// HAL_RCC_GetHCLKFreq()/100000	 10us�ж�һ��
	// HAL_RCC_GetHCLKFreq()/1000000 1us�ж�һ��
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);  // ���ò�����ϵͳ�δ�ʱ��
  // ϵͳ�δ�ʱ��ʱ��Դ 
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  // ϵͳ�δ�ʱ���ж����ȼ����� 
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
/*void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  // �ⲿ����8MHz
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;  // 9��Ƶ���õ�72MHz��ʱ��
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;       // ϵͳʱ�ӣ�72MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;              // AHBʱ�ӣ�72MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;               // APB1ʱ�ӣ�36MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;               // APB2ʱ�ӣ�72MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

 	// HAL_RCC_GetHCLKFreq()/1000    1ms�ж�һ��
	// HAL_RCC_GetHCLKFreq()/100000	 10us�ж�һ��
	// HAL_RCC_GetHCLKFreq()/1000000 1us�ж�һ��
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);  // ���ò�����ϵͳ�δ�ʱ��
  // ϵͳ�δ�ʱ��ʱ��Դ 
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  // ϵͳ�δ�ʱ���ж����ȼ����� 
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}*/

/**
  * ��������: ������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
int main(void)
{
	Module_Init();	
  
  while (1)
  {	
		// if comm is NOT null and already over.
		if((comm[0] != '\0') && (comm[strlen((char*)comm)-1] == '\n'))
		{
			args = cmd_split((char*)comm);
			if(cmd_execute(args)) 		// if execute succssful, send OK
				HAL_UART_Transmit(&huartx, (uint8_t*)"OK\r\n\r\n", strlen("OK\r\n\r\n"), 1000);
			for(int i = 0; i < COMM_BUFSIZE; i++)  		// reset comm[]
				comm[i] = '\0';
			free(args);
		}
		HAL_Delay(10);
		
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
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, outputV);
			//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, outputV);
			if(LED_Status == TEMP)
				ShowVal_float(tempF);
			else if(LED_Status == VOL_OUT)
				ShowVal_int(outputV);
			
			sprintf((char*)format_out, "%.2f, %3d, %.2f,\r\n", tempSet, outputV, tempF);
			HAL_UART_Transmit(&huartx, format_out,strlen((char*)format_out),1000);
		}
  }
}

/**
  * ��������: �����ⲿ�жϷ�����
  * �������: GPIO_Pin���ж�����
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin==KEY1_GPIO_PIN)
  {
    HAL_Delay(20);/* ��ʱһС��ʱ�䣬�������� */
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
    HAL_Delay(20);/* ��ʱһС��ʱ�䣬�������� */
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
    HAL_Delay(20);/* ��ʱһС��ʱ�䣬�������� */
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
    HAL_Delay(20);/* ��ʱһС��ʱ�䣬�������� */
    if(HAL_GPIO_ReadPin(KEY_GPIO,KEY4_GPIO_PIN)==KEY4_DOWN_LEVEL)
    {
			outputV = INIT_VOL;			// reset the outputV
			tempSet = 25.0;
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, outputV);
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, outputV);
			LED_Status = VOL_OUT;
			ShowVal_int(outputV);
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
    }
    __HAL_GPIO_EXTI_CLEAR_IT(KEY4_GPIO_PIN);
  }
}


/**
  * ��������: ���ڽ�����ɻص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
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
  * ��������: show int value in LED segments
  * �������: val
  * �� �� ֵ: none
  * ˵    ����none
  */
void ShowVal_int(uint8_t val)
{
	Display_L1(num[val/1000]);
	Display_L2(num[val%1000/100]);
	Display_L3(num[val%100/10]);
	Display_L4(num[val%10]);
}

/**
  * ��������: show float value in LED segments
  * �������: val in float
  * �� �� ֵ: none
  * ˵    ����none
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
  * ��������: Init all used modules 
  * �������: none
  * �� �� ֵ: none
  * ˵    ����none
  */
void Module_Init(void)
{
  HAL_Init();						/* ��λ�������裬��ʼ��Flash�ӿں�ϵͳ�δ�ʱ�� */
  SystemClock_Config();	/* ����ϵͳʱ�� */
  KEY_GPIO_Init();			/* ������ʼ�� */
	
	MX_GPIO_Init();				// GPIO init
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);		// light the LED
	
  MX_DAC_Init();				// DAC init
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, INIT_VOL);		// Init the DAC's output
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);		/* ����DAC channel 1 */
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, INIT_VOL);		// Init the DAC's output
  HAL_DAC_Start(&hdac, DAC_CHANNEL_2);		/* ����DAC channel 2 */
	
  MX_UARTx_Init();			/* ��ʼ�����ڲ����ô����ж����ȼ� */
	HAL_UART_Transmit(&huartx, (uint8_t*)"\t\tPCR Temp Control System\t\t\n",strlen("\t\tPCR Temp Control System\t\t\n"),1000);
  HAL_UART_Receive_IT(&huartx,&aRxBuffer,1);  	/* ʹ�ܽ��գ������жϻص����� */
	
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
{}

/***
		ST,1 		// start
		ST,0 		// stop
	***/
int cmd_ST(char **args)
{
	if (args[1] == NULL) 		// ��������
	{
		HAL_UART_Transmit(&huartx, (uint8_t*)"expected argument to \"ST\"\r\n", strlen("expected argument to \"PS\"\r\n"), 1000);
	}
	else
	{
		int num = atoi(args[1]);
		
		if(num == 0)
			begin = 0;
		else if(num == 1)
			begin = 1;
		else
		{
			HAL_UART_Transmit(&huartx, (uint8_t*)"Argument Wrong!\r\n", strlen("Argument Wrong!\r\n"), 1000);
			return 0;
		}
	}
	return 1;
}

/*
PS,2,10.0		// set low temp
PS,4,90.0		// set hign temp
PS,7,25.0		// set current temp	

PS,2			// check low temp
PS,4			// check hign temp
*/
int cmd_PS(char **args)
{
	if (args[1] == NULL) 		// ��������
	{
		HAL_UART_Transmit(&huartx, (uint8_t*)"expected argument to \"PS\"\n", strlen("expected argument to \"PS\"\n"), 1000);
	} 
	else if(args[2] == NULL)	// ��ѯ����
	{
		int num = atoi(args[1]);
		char tempStr[10] = {'\0'};
		switch(num)
		{
			case 2:
				
				break;
			case 4:
				break;
			case 7:
				sprintf(tempStr, "%.1f\r\n", tempSet);
				HAL_UART_Transmit(&huartx, (uint8_t*)tempStr, strlen(tempStr), 1000);
				break;
			default:
				break;
		}
  }
	else		// ��������  PS 7 23.0
	{
		int num = atoi(args[1]);
		switch(num)
		{
			case 2:
				break;
			case 4:
				break;
			case 7:
				tempSet = strTof(args[2]);
				break;
			default:
				break;
		}
	}
  return 1;
}

char **cmd_split(char *line)
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
  token = strtok(line, LSH_TOK_DELIM);
	HAL_UART_Transmit(&huartx, (uint8_t*)token, strlen(token), 1000);
	
  while (token != NULL) 
	{		
    tokens[position] = token;
    position++;

    token = strtok(NULL, LSH_TOK_DELIM);
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
