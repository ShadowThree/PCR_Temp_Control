#include "TM1650.h"

/*******************************************************************************
* Function Name  : delay_us
* Description    : 1us per cycle (stm32 HSI 64M, if the something change, rewrite this function)
* Input          : time in us
* Output         : None
* Return         : None
*******************************************************************************/
/*void delay_us(uint32_t us)		// maybe need modify
{
	us = (us<<2) + (us<<1) + us;
	while(us--);
}*/

//初始化IIC
void IIC_Init(void)
{	
	GPIO_InitTypeDef    GPIO_InitStructure;

	/* Enable I2C clocks */
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*配置SCK、SDA为上拉输出*/
	GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//产生IIC初始化信号
void IIC_Start(void)
{
	//SDA_OUT();     //sda设置为输出方向
	SDA_H;	  	  
	SCL_H;
	delay_us(4);
 	SDA_L;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	SCL_L;//钳住IIC总线，准备开始收发数据 
}

//产生IIC停止信号
void IIC_Stop(void)
{
	//SDA_OUT(); //sda设置为输出方向
	SCL_L;
	SDA_L;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	SCL_H; 
	SDA_H;//发送IIC总线结束信号
	delay_us(4);							   	
}

//等待应答信号来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	//SDA_IN();      //SDA设置为输入  
	SDA_H;delay_us(1);	   
	SCL_H;delay_us(1);	 
	while(SDA_read)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	SCL_L;//时钟输出为0	   
	return 0;  
} 

//产生Ack应答
void IIC_Ack(void)
{
	SCL_L;
	//SDA_OUT();
	SDA_L;
	delay_us(2);
	SCL_H;
	delay_us(2);
	SCL_L;
}

//不产生ACK应答	    
void IIC_NAck(void)
{
	SCL_L;
	//SDA_OUT();
	SDA_H;
	delay_us(2);
	SCL_H;
	delay_us(2);
	SCL_L;
}					 				     

//IIC发送一个字节
void IIC_Send_Byte(uint8_t txd)
{                        
	uint8_t t;   
	//SDA_OUT(); 	    
	SCL_L;
	for(t=0;t<8;t++)
	{              
		//IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			SDA_H;
		else
			SDA_L;
		txd<<=1; 	  
		delay_us(2);   
		SCL_H;
		delay_us(2); 
		SCL_L;	
		delay_us(2);
	}	 
}

//读取一个字节，ack=1时，发送ACK，ack=0;发送nAck  
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	//SDA_IN();				//SDA设置为输入
  for(i=0;i<8;i++ )
	{
		SCL_L; 
		delay_us(2);
		SCL_H;
		receive<<=1;
		if(SDA_read)receive++;   
		delay_us(1); 
  }					 
	if (!ack)
		IIC_NAck();		//发送nACK
	else
		IIC_Ack(); 		//发送ACK   
	return receive;
}

void TM1650_Set(uint8_t add,uint8_t dat) //数码管显示
{
	//写显存必须从高地址开始写
	IIC_Start();
	IIC_Send_Byte(add); //第一个现存地址
	IIC_Ack();
	IIC_Send_Byte(dat); //发送显示数据
	IIC_Ack();
	IIC_Stop();
}

//用于显示4个数码管的数据
void display(uint8_t first_data,uint8_t second_data,uint8_t third_data,uint8_t fourth_data)
{    
	//TM1650_Set(0x48,0x51);				//初始化为5级灰度，开显示
	TM1650_Set(0x68,first_data);
	TM1650_Set(0x6A,second_data);
	TM1650_Set(0x6C,third_data);
	TM1650_Set(0x6E,fourth_data);
}

void TM1650_Init(void)
{
	//写显存必须从高地址开始写
	IIC_Start();
	IIC_Send_Byte(0x48); // 设置5级灰度
	IIC_Ack();
	IIC_Send_Byte(0x51); // 开显示
	IIC_Ack();
	IIC_Stop();
}

void Display_L1(uint8_t l1)
{
	IIC_Start();
	IIC_Send_Byte(0x68); // address
	IIC_Ack();
	IIC_Send_Byte(l1); // display num
	IIC_Ack();
	IIC_Stop();
}

void Display_L2(uint8_t l2)
{
	IIC_Start();
	IIC_Send_Byte(0x6A); // address
	IIC_Ack();
	IIC_Send_Byte(l2); // display num
	IIC_Ack();
	IIC_Stop();
}

void Display_L3(uint8_t l3)
{
	IIC_Start();
	IIC_Send_Byte(0x6C); // address
	IIC_Ack();
	IIC_Send_Byte(l3); // display num
	IIC_Ack();
	IIC_Stop();
}

void Display_L4(uint8_t l4)
{
	IIC_Start();
	IIC_Send_Byte(0x6E); // address
	IIC_Ack();
	IIC_Send_Byte(l4); // display num
	IIC_Ack();
	IIC_Stop();
}
