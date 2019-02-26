#ifndef __STMFLASH_H__
#define __STMFLASH_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
/************************** STM32 内部 FLASH 配置 *****************************/
#define STM32_FLASH_SIZE        256  // 所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN        1    // stm32芯片内容FLASH 写入使能(0，禁用;1，使能)

#define Test_Addr		         	  0x0803F800
#define VOL_MAX_Addr						0x0803F802
#define VOL_MIN_Addr						0x0803F804
#define VOL_INIT_Addr						0x0803F806
#define Pid_P_Addr 							0x0803F820
#define Pid_I_Addr							0x0803F822
#define Pid_D_Addr							0x0803F824
#define CycleT_Addr							0x0803F830
#define LowTemp_Addr						0x0803F840
#define MidTemp_Addr						0x0803F842
#define HigTemp_Addr 						0x0803F844
#define TestValue 							145
/* 扩展变量 ------------------------------------------------------------------*/
extern int VOL_MAX, VOL_MIN, VOL_INIT;
extern uint16_t cycle_ms;
extern float PID_P, PID_I, PID_D;
extern float lowTemp, midTemp, higTemp;

/* 函数声明 ------------------------------------------------------------------*/
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  //读出半字

void STMFLASH_WriteLenByte(uint32_t WriteAddr, uint32_t DataToWrite, uint16_t Len );	      //指定地址开始写入指定长度的数据
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr, uint16_t Len );					                    	//指定地址开始读取指定长度数据
void STMFLASH_Write( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite );		//从指定地址开始写入指定长度的数据
void STMFLASH_Read( uint32_t ReadAddr, uint16_t * pBuffer, uint16_t NumToRead );   	//从指定地址开始读出指定长度的数据
int SaveParam(void);
int GetParam(void);

#endif /* __STMFLASH_H__ */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
