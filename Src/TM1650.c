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

//��ʼ��IIC
void IIC_Init(void)
{	
	GPIO_InitTypeDef    GPIO_InitStructure;

	/* Enable I2C clocks */
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*����SCK��SDAΪ�������*/
	GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//����IIC��ʼ���ź�
void IIC_Start(void)
{
	//SDA_OUT();     //sda����Ϊ�������
	SDA_H;	  	  
	SCL_H;
	delay_us(4);
 	SDA_L;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	SCL_L;//ǯסIIC���ߣ�׼����ʼ�շ����� 
}

//����IICֹͣ�ź�
void IIC_Stop(void)
{
	//SDA_OUT(); //sda����Ϊ�������
	SCL_L;
	SDA_L;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	SCL_H; 
	SDA_H;//����IIC���߽����ź�
	delay_us(4);							   	
}

//�ȴ�Ӧ���ź���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	//SDA_IN();      //SDA����Ϊ����  
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
	SCL_L;//ʱ�����Ϊ0	   
	return 0;  
} 

//����AckӦ��
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

//������ACKӦ��	    
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

//IIC����һ���ֽ�
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

//��ȡһ���ֽڣ�ack=1ʱ������ACK��ack=0;����nAck  
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	//SDA_IN();				//SDA����Ϊ����
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
		IIC_NAck();		//����nACK
	else
		IIC_Ack(); 		//����ACK   
	return receive;
}

void TM1650_Set(uint8_t add,uint8_t dat) //�������ʾ
{
	//д�Դ����Ӹߵ�ַ��ʼд
	IIC_Start();
	IIC_Send_Byte(add); //��һ���ִ��ַ
	IIC_Ack();
	IIC_Send_Byte(dat); //������ʾ����
	IIC_Ack();
	IIC_Stop();
}

//������ʾ4������ܵ�����
void display(uint8_t first_data,uint8_t second_data,uint8_t third_data,uint8_t fourth_data)
{    
	//TM1650_Set(0x48,0x51);				//��ʼ��Ϊ5���Ҷȣ�����ʾ
	TM1650_Set(0x68,first_data);
	TM1650_Set(0x6A,second_data);
	TM1650_Set(0x6C,third_data);
	TM1650_Set(0x6E,fourth_data);
}

void TM1650_Init(void)
{
	//д�Դ����Ӹߵ�ַ��ʼд
	IIC_Start();
	IIC_Send_Byte(0x48); // ����5���Ҷ�
	IIC_Ack();
	IIC_Send_Byte(0x51); // ����ʾ
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
