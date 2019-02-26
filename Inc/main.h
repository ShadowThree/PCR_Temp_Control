#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stdbool.h"
#include "uart/bsp_uartx.h"
#include "bsp_key.h"
#include "bsp_dac.h"
#include "gpio.h"
#include "mlx90614.h"
#include "TM1650.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "limits.h"
#include "rt_heap.h"
#include "stm_flash.h"
	
/* Exported macro ------------------------------------------------------------*/
extern uint32_t __heap_base;
#define HEAP_BASE __heap_base							//0x20002558
#define HEAP_SIZE 0x00000050
#define HEAP_END  HEAP_BASE+HEAP_SIZE
	
#define COMM_BUFSIZE 64
#define TOK_DELIM " ,\t\r\n="

/* Private includes ----------------------------------------------------------*/
int VOL_MAX = 180;
int VOL_MIN =	0;
int VOL_INIT = 90;
uint8_t position = 0;
bool STA_COMM = false;
uint8_t aRxBuffer;
uint8_t flag_commOver = 0;
//uint8_t *comm = NULL;
uint8_t comm[COMM_BUFSIZE] = {'\0'};
uint8_t count_commChr = 0;
float tempF = 0.0;
uint8_t tempS[10] = {'\0'};
static uint8_t num[16] = {0x3f, 0x06, 0x5b, 0x4f, 
													0x66, 0x6d, 0x7d, 0x07,
													0x7f, 0x6f, 0x77, 0x7c,
													0x39, 0x5e, 0x79, 0x71};
/* DAC输出对应值：可设置0~255，对应引脚输出0~3.3V，该值越大，引脚输出电压越高*/
uint8_t outputV;
uint8_t format_out[30] = {'\0'};

float lowTemp = 35.0;
float midTemp = 50.0;
float higTemp = 90.0;
float tempSet = 25.0;		// current temp
													
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

enum {
	LOOP_OFF = 0,
	LOOP_ON
} STATUS = LOOP_OFF;

enum {
	CONFIG_FAIL = 0,
	CONFIG_OK
} ReadConfig_STA = CONFIG_FAIL;
/* Exported constants --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void SystemClock_Config(void);
void ShowVal_float(float val);
void ShowVal_int(uint8_t val);
void Module_Init(void);
float strTof(const char *args);
void ReadConfigFile(void);

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM 	" ,\t\r\n"

char **args;
int cmd_status;

/*** command ***/
char *cmd_str[] = {
  "PS",
	"ST",
	"VR",
	"HP",
	"GS"
};

/*** cmdFunc ***/
int cmd_PS(char **args);
int cmd_ST(char **args);
int cmd_VR(char **args);
int cmd_HP(char **args);
int cmd_GS(char **args);

/*** cmdFunc point ***/
int (*cmd_funcP[]) (char **) = {
  &cmd_PS,
	&cmd_ST,
	&cmd_VR,
	&cmd_HP,
	&cmd_GS
};

char **cmd_split(char *line, char *delim);
int cmd_execute(char**);
int cmd_count(void);






/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
