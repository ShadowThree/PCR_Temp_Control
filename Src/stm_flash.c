/**
  ******************************************************************************
  * �ļ�����: stm_flash.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: �ڲ�Falsh��дʵ��
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm_flash.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
#if STM32_FLASH_SIZE < 256
  #define STM_SECTOR_SIZE  1024 //�ֽ�
#else 
  #define STM_SECTOR_SIZE	 2048
#endif


/* ˽�б��� ------------------------------------------------------------------*/
#if STM32_FLASH_WREN	//���ʹ����д 
static uint16_t STMFLASH_BUF [ STM_SECTOR_SIZE / 2 ];//�����2K�ֽ�
static FLASH_EraseInitTypeDef EraseInitStruct;
#endif

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ��ȡָ����ַ�İ���(16λ����)
  * �������: faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
  * �� �� ֵ: ����ֵ:��Ӧ����.
  * ˵    ������
  */
uint16_t STMFLASH_ReadHalfWord ( uint32_t faddr )
{
	return *(__IO uint16_t*)faddr; 
}

#if STM32_FLASH_WREN	//���ʹ����д   
/**
  * ��������: ������д��
  * �������: WriteAddr:��ʼ��ַ
  *           pBuffer:����ָ��
  *           NumToWrite:����(16λ)��
  * �� �� ֵ: ��
  * ˵    ������
  */
void STMFLASH_Write_NoCheck ( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite )   
{ 			 		 
	uint16_t i;	
	
	for(i=0;i<NumToWrite;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,WriteAddr,pBuffer[i]);
	  WriteAddr+=2;                                    //��ַ����2.
	}  
} 

/**
  * ��������: ��ָ����ַ��ʼд��ָ�����ȵ�����
  * �������: WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
  *           pBuffer:����ָ��
  *           NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
  * �� �� ֵ: ��
  * ˵    ������
  */
void STMFLASH_Write ( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite )	
{
  uint32_t SECTORError = 0;
	uint16_t secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	uint16_t secremain; //������ʣ���ַ(16λ�ּ���)	   
 	uint16_t i;    
	uint32_t secpos;	   //������ַ
	uint32_t offaddr;   //ȥ��0X08000000��ĵ�ַ
	
	if(WriteAddr<FLASH_BASE||(WriteAddr>=(FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	
	HAL_FLASH_Unlock();						//����
	
	offaddr=WriteAddr-FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
      //�����������
      /* Fill EraseInit structure*/
      EraseInitStruct.TypeErase     = FLASH_TYPEERASE_PAGES;
      EraseInitStruct.PageAddress   = secpos*STM_SECTOR_SIZE+FLASH_BASE;
      EraseInitStruct.NbPages       = 1;
      HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError);

			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}
    else
    {
      STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		}
    if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain;	//д��ַƫ��	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	HAL_FLASH_Lock();//����
}
#endif

/**
  * ��������: ��ָ����ַ��ʼ����ָ�����ȵ�����
  * �������: ReadAddr:��ʼ��ַ
  *           pBuffer:����ָ��
  *           NumToRead:����(16λ)��
  * �� �� ֵ: ��
  * ˵    ������
  */
void STMFLASH_Read ( uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead )   	
{
	uint16_t i;
	
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

int SaveParam(void)
{
	uint16_t wdata[1], rdata[1];
	wdata[0] = TestValue;
	STMFLASH_Write(Test_Addr, wdata, 1);
	wdata[0] = VOL_MAX;
	STMFLASH_Write(VOL_MAX_Addr, wdata, 1);
	wdata[0] = VOL_MIN;
	STMFLASH_Write(VOL_MIN_Addr, wdata, 1);
	wdata[0] = VOL_INIT;
	STMFLASH_Write(VOL_INIT_Addr, wdata, 1);
	
	wdata[0] = (uint16_t)(PID_P * 100);
	STMFLASH_Write(Pid_P_Addr, wdata, 1);
	wdata[0] = (uint16_t)(PID_I * 100);
	STMFLASH_Write(Pid_I_Addr, wdata, 1);
	wdata[0] = (uint16_t)(PID_D * 100);
	STMFLASH_Write(Pid_D_Addr, wdata, 1);
	
	wdata[0] = cycle_ms;
	STMFLASH_Write(CycleT_Addr, wdata, 1);
	
	wdata[0] = (uint16_t)(lowTemp * 100);
	STMFLASH_Write(LowTemp_Addr, wdata, 1);
	wdata[0] = (uint16_t)(midTemp * 100);
	STMFLASH_Write(MidTemp_Addr, wdata, 1);
	wdata[0] = (uint16_t)(higTemp * 100);
	STMFLASH_Write(HigTemp_Addr, wdata, 1);
	
	STMFLASH_Read(Test_Addr, rdata, 1);
	return (rdata[0] == TestValue);
}

int GetParam(void)
{
	uint16_t rdata[1];
	STMFLASH_Read(VOL_MAX_Addr, rdata, 1);
	VOL_MAX = rdata[0];
	STMFLASH_Read(VOL_MIN_Addr, rdata, 1);
	VOL_MIN = rdata[0];
	STMFLASH_Read(VOL_INIT_Addr, rdata, 1);
	VOL_INIT = rdata[0];
	
	STMFLASH_Read(Pid_P_Addr, rdata, 1);
	PID_P = (float)rdata[0] / 100;
	STMFLASH_Read(Pid_I_Addr, rdata, 1);
	PID_I = (float)rdata[0] / 100;
	STMFLASH_Read(Pid_D_Addr, rdata, 1);
	PID_D = (float)rdata[0] / 100;
	
	STMFLASH_Read(CycleT_Addr, rdata, 1);
	cycle_ms = rdata[0];
	
	STMFLASH_Read(LowTemp_Addr, rdata, 1);
	lowTemp = (float)rdata[0] / 100;
	STMFLASH_Read(MidTemp_Addr, rdata, 1);
	midTemp = (float)rdata[0] / 100;
	STMFLASH_Read(HigTemp_Addr, rdata, 1);
	higTemp = (float)rdata[0] / 100;
	
	STMFLASH_Read(Test_Addr, rdata, 1);
	return (rdata[0] == TestValue);
}
