#ifndef __STMFLASH_H__
#define __STMFLASH_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
/************************** STM32 �ڲ� FLASH ���� *****************************/
#define STM32_FLASH_SIZE        256  // ��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN        1    // stm32оƬ����FLASH д��ʹ��(0������;1��ʹ��)

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
/* ��չ���� ------------------------------------------------------------------*/
extern int VOL_MAX, VOL_MIN, VOL_INIT;
extern uint16_t cycle_ms;
extern float PID_P, PID_I, PID_D;
extern float lowTemp, midTemp, higTemp;

/* �������� ------------------------------------------------------------------*/
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  //��������

void STMFLASH_WriteLenByte(uint32_t WriteAddr, uint32_t DataToWrite, uint16_t Len );	      //ָ����ַ��ʼд��ָ�����ȵ�����
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr, uint16_t Len );					                    	//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite );		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read( uint32_t ReadAddr, uint16_t * pBuffer, uint16_t NumToRead );   	//��ָ����ַ��ʼ����ָ�����ȵ�����
int SaveParam(void);
int GetParam(void);

#endif /* __STMFLASH_H__ */

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
