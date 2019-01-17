#ifndef __TM1650_H_
#define __TM1650_H_

#include "stm32f1xx_hal.h"
#include "public.h"

//#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;} //pa9��������������
//#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;} //pa9���ģʽ�����50M


#define SCL_H         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET)
#define SCL_L         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET)
         
#define SDA_H         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET)
#define SDA_L         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET)
      
#define SDA_read			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)

//void delay_us(uint32_t us);
void IIC_Init(void);				//��ʼ��IIC
void IIC_Start(void);				//����IIC��ʼ���ź�
void IIC_Stop(void);				//����IICֹͣ�ź�
uint8_t IIC_Wait_Ack(void);	// �ȴ�Ӧ���ź���
void IIC_Ack(void);					//����AckӦ��
void IIC_NAck(void);				//������ACKӦ��	    
void IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�		  
uint8_t IIC_Read_Byte(unsigned char ack);			//��ȡһ���ֽڣ�ack=1ʱ������ACK��ack=0;����nAck  
void TM1650_Set(uint8_t add,uint8_t dat);			//�������ʾ
void display(uint8_t first_data,uint8_t second_data,uint8_t third_data,uint8_t fourth_data);		//������ʾ4������ܵ�����
void TM1650_Init(void);
void Display_L1(uint8_t l1);
void Display_L2(uint8_t l2);
void Display_L3(uint8_t l3);
void Display_L4(uint8_t l4);

#endif
