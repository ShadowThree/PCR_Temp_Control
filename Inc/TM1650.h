#ifndef __TM1650_H_
#define __TM1650_H_

#include "stm32f1xx_hal.h"
#include "public.h"

//#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;} //pa9上拉或下拉输入
//#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;} //pa9输出模式，最大50M


#define SCL_H         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET)
#define SCL_L         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET)
         
#define SDA_H         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET)
#define SDA_L         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET)
      
#define SDA_read			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)

//void delay_us(uint32_t us);
void IIC_Init(void);				//初始化IIC
void IIC_Start(void);				//产生IIC初始化信号
void IIC_Stop(void);				//产生IIC停止信号
uint8_t IIC_Wait_Ack(void);	// 等待应答信号来
void IIC_Ack(void);					//产生Ack应答
void IIC_NAck(void);				//不产生ACK应答	    
void IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节		  
uint8_t IIC_Read_Byte(unsigned char ack);			//读取一个字节，ack=1时，发送ACK，ack=0;发送nAck  
void TM1650_Set(uint8_t add,uint8_t dat);			//数码管显示
void display(uint8_t first_data,uint8_t second_data,uint8_t third_data,uint8_t fourth_data);		//用于显示4个数码管的数据
void TM1650_Init(void);
void Display_L1(uint8_t l1);
void Display_L2(uint8_t l2);
void Display_L3(uint8_t l3);
void Display_L4(uint8_t l4);

#endif
