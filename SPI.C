#include "LPC17XX.H"
#include "M45PE161.H"
#include "sys.h"

/* Private typedef -----------------------------------------------------------*/
#define SPI_FLASH_PageSize    0x210

/* Private define ------------------------------------------------------------*/
#define READ       0xD2  /* Read from Memory instruction */
#define WRITE      0x82  /* Write to Memory instruction */

#define RDID       0x9F  /* Read identification */
#define RDSR       0xD7  /* Read Status Register instruction  */

#define SE         0x7C  /* Sector Erase instruction */
#define PE         0x81  /* Page Erase instruction */

#define RDY_Flag   0x80  /* Ready/busy(1/0) status flag */

#define Dummy_Byte 0x00

void delay_us(int count)
{
  while(count--);
}

/*******************************************************************************************															
** ����˵������ʼ��SPI��				
** ��ڲ�������																
** ���ڲ�������
** ��    ע: ��																
*******************************************************************************************/
void  SPIx_Init(void)
{  
	/* ����M45PE161�Ŀ������� */
	LPC_PINCON->PINSEL0 	=	0;		
   	LPC_GPIO0->FIODIR2 	|=  	1<<0;
	/* ����Ӳ��SPI��ͨѶ��	*/ 
   	LPC_PINCON->PINSEL0	|= 	0xc0000000;				// ����P0.15��ΪSCK��
   	LPC_PINCON->PINSEL1	 = (0x03 << 2) | (0x03 << 4);	// ����P0.17��P0.18����ΪSPI����
				               
   	LPC_SPI->SPCCR =	0x40;		       		// ����SPIʱ�ӷ�Ƶ,�ɰ���������	72MHz/64*4		
 	/* ����SPI�Ĺ�����ʽ */ 
 	LPC_SPI->SPCR  = 	(0 << 2) |				// SPI������ÿ�δ��䷢�ͺͽ���8λ���ݡ�
 				(0 << 3) |				// CPHA = 0, ������SCK �ĵ�һ��ʱ���ز���
 				(0 << 4) |				// CPOL = 0, SCK Ϊ����Ч
 				(1 << 5) |				// MSTR = 1, SPI ������ģʽ
 				(0 << 6) |				// LSBF = 0, SPI ���ݴ���MSB (λ7)����
 				(0 << 7);				// SPIE = 0, SPI �жϱ���ֹ	
}
 
/************************************************************************													
** �������ܣ�ͨ��Ӳ��SPI����һ���ֽڵ�M45PE161					
** ��ڲ���: TxData  ������ַ															
** ���ڲ���: ���յ�������
** ��    ע: ��																
************************************************************************/
u8 SPIx_ReadWriteByte(u8 TxData)
{		
    INT8U temp;
	LPC_SPI->SPDR = TxData;							// ���͸��������Բ���ʱ��,���ݱ���û���ô�
    while ( 0 == (LPC_SPI->SPSR & 0x80));			// �ȴ�SPIF��λ�����ȴ����ݷ������
    temp = LPC_SPI->SPSR;								// ���SPIF��־
    temp=temp;
    return (INT8U)(LPC_SPI->SPDR);					// ���ؽ��յ�������				    
}

/************************************************************************																										
** ��������: M45PE161�Ķ�ID����				
** ��ڲ���: ��
** ���ڲ���: ID		
** ��    ע: ��
************************************************************************/
u32 SPI_Flash_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* Select the FLASH: Chip Select low */
	CE_Low();				    

  /* Send "RDID " instruction */
  SPIx_ReadWriteByte(M45P_ManufactDeviceID);
	  SPIx_ReadWriteByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  SPIx_ReadWriteByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  SPIx_ReadWriteByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  Temp0 = SPIx_ReadWriteByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  Temp1 = SPIx_ReadWriteByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  Temp2 = SPIx_ReadWriteByte(Dummy_Byte);
 
  
  /* Deselect the FLASH: Chip Select high */
  CE_High();				    

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}
/************************************************************************																										
** ��������: ��ȡSPI_FLASH��״̬�Ĵ���				
** ��ڲ���: ��
** ���ڲ���: ��	
** ��    ע: 
			BIT7  6   5   4   3   2   1   0
			SPR   RV  TB BP2 BP1 BP0 WEL BUSY
			SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
			TB,BP2,BP1,BP0:FLASH����д��������
			WEL:дʹ������
			BUSY:æ���λ(1,æ;0,����)
			Ĭ��:0x00
************************************************************************/
u8 SPI_Flash_ReadSR(void)   
{  
	u8 byte=0;   
	CE_Low();                            //ʹ������   
	SPIx_ReadWriteByte(M45P_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPIx_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
	CE_High();                            //ȡ��Ƭѡ     
	return byte;   
}
/************************************************************************																										
** ��������: SPI_FLASHдʹ�� ��WEL��λ				
** ��ڲ���: ��
** ���ڲ���: ��		
** ��    ע: ��
************************************************************************/  
void SPI_FLASH_Write_Enable(void)   
{
	CE_Low();                            //ʹ������   
    SPIx_ReadWriteByte(M45P_WriteEnable);      //����дʹ��  
	CE_High();                            //ȡ��Ƭѡ     	      
}
/************************************************************************																										
** ��������: SPI_FLASHд��ֹ ��WEL����				
** ��ڲ���: ��
** ���ڲ���: ��		
** ��    ע: ��
************************************************************************/ 
void SPI_FLASH_Write_Disable(void)   
{  
	CE_Low();                            //ʹ������   
    SPIx_ReadWriteByte(M45P_WriteDisable);     //����д��ָֹ��    
	CE_High();                            //ȡ��Ƭѡ     	      
}
/************************************************************************																										
** ��������: ��ȡSPI FLASH	��ָ����ַ��ʼ��ȡָ�����ȵ�����		
** ��ڲ���: pBuffer:���ݴ洢��
**			 ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
**			 NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
** ���ڲ���: ��		
** ��    ע: ��
************************************************************************/
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;    												    
	CE_Low();                            //ʹ������   
    SPIx_ReadWriteByte(M45P_ReadData);         //���Ͷ�ȡ���� 
    SPIx_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ    
    SPIx_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPIx_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPIx_ReadWriteByte(0XFF);   //ѭ������  
    }
	CE_High();                            //ȡ��Ƭѡ     	      
}
/************************************************************************																										
** ��������: �ȴ�����		
** ��ڲ���: ��
** ���ڲ���: ��		
** ��    ע: ��
************************************************************************/
void SPI_Flash_Wait_Busy(void)   
{
  	//SPI_FLASH_CS=0;                            //ʹ������   
   
	while ((SPI_Flash_ReadSR()&0x01)!=0);   // �ȴ�BUSYλ���

	//SPI_FLASH_CS=1;                            //ȡ��Ƭѡ     	      
} 
/************************************************************************																										
** ��������: SPI��һҳ(0~65535)��д������256���ֽڵ����ݣ���ָ����ַ��ʼ
**			 д�����256�ֽڵ�����		
** ��ڲ���: pBuffer:���ݴ洢��
**			 WriteAddr:��ʼд��ĵ�ַ(24bit)
**			 NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
** ���ڲ���: ��		
** ��    ע: ��
************************************************************************/ 	 
void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
    SPI_FLASH_Write_Enable();                  //SET WEL 
	CE_Low();                            //ʹ������   
    SPIx_ReadWriteByte(M45P_PageProgram);      //����дҳ����   
	SPIx_ReadWriteByte((u8)((WriteAddr)>>16)); //����24bit��ַ    
    SPIx_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPIx_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPIx_ReadWriteByte(pBuffer[i]);//ѭ��д��  
	CE_High();                            //ȡ��Ƭѡ 
	SPI_Flash_Wait_Busy();					   //�ȴ�д�����
}
/************************************************************************																										
** ��������: �޼���дSPI FLASH	�����Զ���ҳ����	
** ��ڲ���: pBuffer:���ݴ洢��
**			 WriteAddr:��ʼд��ĵ�ַ(24bit)
**			 NumByteToWrite:Ҫд����ֽ���(���65535)
** ���ڲ���: ��		
** ��    ע: ��
************************************************************************/
unsigned char sbuf_date[256];
void SPI_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		SPI_Flash_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
}
/************************************************************************																										
** ��������: ����һ������		
** ��ڲ���: Dst_Addr:������ַ 0~511 for M45P16
** ���ڲ���: ��		
** ��    ע: ��
************************************************************************/ 
void SPI_Flash_Erase_Sector(u32 Dst_Addr)   
{   
	Dst_Addr*=4096;
    SPI_FLASH_Write_Enable();                  //SET WEL 	 
  	CE_Low();                            //ʹ������   
    SPIx_ReadWriteByte(M45P_SectorErase);      //������������ָ�� 
    SPIx_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ    
    SPIx_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPIx_ReadWriteByte((u8)Dst_Addr);  
	CE_High();                            //ȡ��Ƭѡ     	      
    SPI_Flash_Wait_Busy();   				   //�ȴ��������
}
/*******************************************************************************
* Function Name  : SPI_FLASH_PageErase
* Description    : Erases the specified FLASH Page.
* Input          : PageAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_PageErase(u32 PageAddr)
{
  /* Enable the write access to the FLASH */
  SPI_FLASH_Write_Enable();

  /* Sector Erase */
  /* Select the FLASH: Chip Select low */
  CE_Low();                            //ʹ������   
  /* Send Sector Erase instruction */
  SPIx_ReadWriteByte(M45P_PageErase);
  /* Send SectorAddr high nibble address byte */
  SPIx_ReadWriteByte((PageAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  SPIx_ReadWriteByte((PageAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  SPIx_ReadWriteByte(PageAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  CE_High();                            //ʹ������   

  /* Wait the end of Flash writing */
  SPI_Flash_Wait_Busy();
}
/************************************************************************																										
** ��������: дSPI FLASH	��ָ����ַ��ʼд��ָ�����ȵ�����	
** ��ڲ���: pBuffer:���ݴ洢��
**			 WriteAddr:��ʼд��ĵ�ַ(24bit)
**			 NumByteToWrite:Ҫд����ֽ���(���65535)
** ���ڲ���: ��		
** ��    ע: ��
************************************************************************/ 	   
u8 SPI_FLASH_BUF[4096];
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    

	secpos=WriteAddr/4096;//������ַ 0~511 for M45P16
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		SPI_Flash_Read(SPI_FLASH_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)
			{
				break;//��Ҫ����
			}
			  	  
		}
		if(i<secremain)//��Ҫ����
		{
			SPI_Flash_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);//д���������� 		 

		}else SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 					   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	} 	 
}
/************************************************************************																										
** ��������: �������ģʽ		
** ��ڲ���: ��
** ���ڲ���: ��		
** ��    ע: ��
************************************************************************/
void SPI_Flash_PowerDown(void)   
{ 
  	CE_Low();                            //ʹ������   
    SPIx_ReadWriteByte(M45P_PowerDown);        //���͵�������  
	CE_High();                            //ȡ��Ƭѡ     	      
    delay_us(3);                               //�ȴ�TPD  
}

/************************************************************************																										
** ��������: ����		
** ��ڲ���: ��
** ���ڲ���: ��		
** ��    ע: ��
************************************************************************/
void SPI_Flash_WAKEUP(void)   
{  
  	CE_Low();                            //ʹ������   
    SPIx_ReadWriteByte(M45P_ReleasePowerDown);   //  send M45P_PowerDown command 0xAB    
	CE_High();                            //ȡ��Ƭѡ     	      
    delay_us(3);                               //�ȴ�TRES1
}










