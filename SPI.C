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
** 功能说明：初始化SPI。				
** 入口参数：无																
** 出口参数：无
** 备    注: 无																
*******************************************************************************************/
void  SPIx_Init(void)
{  
	/* 设置M45PE161的控制引脚 */
	LPC_PINCON->PINSEL0 	=	0;		
   	LPC_GPIO0->FIODIR2 	|=  	1<<0;
	/* 设置硬件SPI的通讯脚	*/ 
   	LPC_PINCON->PINSEL0	|= 	0xc0000000;				// 设置P0.15脚为SCK脚
   	LPC_PINCON->PINSEL1	 = (0x03 << 2) | (0x03 << 4);	// 设置P0.17、P0.18引脚为SPI引脚
				               
   	LPC_SPI->SPCCR =	0x40;		       		// 设置SPI时钟分频,可按需求配置	72MHz/64*4		
 	/* 设置SPI的工作方式 */ 
 	LPC_SPI->SPCR  = 	(0 << 2) |				// SPI控制器每次传输发送和接收8位数据。
 				(0 << 3) |				// CPHA = 0, 数据在SCK 的第一个时钟沿采样
 				(0 << 4) |				// CPOL = 0, SCK 为低有效
 				(1 << 5) |				// MSTR = 1, SPI 处于主模式
 				(0 << 6) |				// LSBF = 0, SPI 数据传输MSB (位7)在先
 				(0 << 7);				// SPIE = 0, SPI 中断被禁止	
}
 
/************************************************************************													
** 函数功能：通过硬件SPI发送一个字节到M45PE161					
** 入口参数: TxData  命令或地址															
** 出口参数: 接收到的数据
** 备    注: 无																
************************************************************************/
u8 SPIx_ReadWriteByte(u8 TxData)
{		
    INT8U temp;
	LPC_SPI->SPDR = TxData;							// 发送该数据用以产生时钟,数据本身没有用处
    while ( 0 == (LPC_SPI->SPSR & 0x80));			// 等待SPIF置位，即等待数据发送完毕
    temp = LPC_SPI->SPSR;								// 清除SPIF标志
    temp=temp;
    return (INT8U)(LPC_SPI->SPDR);					// 返回接收到的数据				    
}

/************************************************************************																										
** 函数功能: M45PE161的读ID函数				
** 入口参数: 无
** 出口参数: ID		
** 备    注: 无
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
** 函数功能: 读取SPI_FLASH的状态寄存器				
** 入口参数: 无
** 出口参数: 无	
** 备    注: 
			BIT7  6   5   4   3   2   1   0
			SPR   RV  TB BP2 BP1 BP0 WEL BUSY
			SPR:默认0,状态寄存器保护位,配合WP使用
			TB,BP2,BP1,BP0:FLASH区域写保护设置
			WEL:写使能锁定
			BUSY:忙标记位(1,忙;0,空闲)
			默认:0x00
************************************************************************/
u8 SPI_Flash_ReadSR(void)   
{  
	u8 byte=0;   
	CE_Low();                            //使能器件   
	SPIx_ReadWriteByte(M45P_ReadStatusReg);    //发送读取状态寄存器命令    
	byte=SPIx_ReadWriteByte(0Xff);             //读取一个字节  
	CE_High();                            //取消片选     
	return byte;   
}
/************************************************************************																										
** 函数功能: SPI_FLASH写使能 将WEL置位				
** 入口参数: 无
** 出口参数: 无		
** 备    注: 无
************************************************************************/  
void SPI_FLASH_Write_Enable(void)   
{
	CE_Low();                            //使能器件   
    SPIx_ReadWriteByte(M45P_WriteEnable);      //发送写使能  
	CE_High();                            //取消片选     	      
}
/************************************************************************																										
** 函数功能: SPI_FLASH写禁止 将WEL清零				
** 入口参数: 无
** 出口参数: 无		
** 备    注: 无
************************************************************************/ 
void SPI_FLASH_Write_Disable(void)   
{  
	CE_Low();                            //使能器件   
    SPIx_ReadWriteByte(M45P_WriteDisable);     //发送写禁止指令    
	CE_High();                            //取消片选     	      
}
/************************************************************************																										
** 函数功能: 读取SPI FLASH	在指定地址开始读取指定长度的数据		
** 入口参数: pBuffer:数据存储区
**			 ReadAddr:开始读取的地址(24bit)
**			 NumByteToRead:要读取的字节数(最大65535)
** 出口参数: 无		
** 备    注: 无
************************************************************************/
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;    												    
	CE_Low();                            //使能器件   
    SPIx_ReadWriteByte(M45P_ReadData);         //发送读取命令 
    SPIx_ReadWriteByte((u8)((ReadAddr)>>16));  //发送24bit地址    
    SPIx_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPIx_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPIx_ReadWriteByte(0XFF);   //循环读数  
    }
	CE_High();                            //取消片选     	      
}
/************************************************************************																										
** 函数功能: 等待空闲		
** 入口参数: 无
** 出口参数: 无		
** 备    注: 无
************************************************************************/
void SPI_Flash_Wait_Busy(void)   
{
  	//SPI_FLASH_CS=0;                            //使能器件   
   
	while ((SPI_Flash_ReadSR()&0x01)!=0);   // 等待BUSY位清空

	//SPI_FLASH_CS=1;                            //取消片选     	      
} 
/************************************************************************																										
** 函数功能: SPI在一页(0~65535)内写入少于256个字节的数据，在指定地址开始
**			 写入最大256字节的数据		
** 入口参数: pBuffer:数据存储区
**			 WriteAddr:开始写入的地址(24bit)
**			 NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
** 出口参数: 无		
** 备    注: 无
************************************************************************/ 	 
void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
    SPI_FLASH_Write_Enable();                  //SET WEL 
	CE_Low();                            //使能器件   
    SPIx_ReadWriteByte(M45P_PageProgram);      //发送写页命令   
	SPIx_ReadWriteByte((u8)((WriteAddr)>>16)); //发送24bit地址    
    SPIx_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPIx_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPIx_ReadWriteByte(pBuffer[i]);//循环写数  
	CE_High();                            //取消片选 
	SPI_Flash_Wait_Busy();					   //等待写入结束
}
/************************************************************************																										
** 函数功能: 无检验写SPI FLASH	具有自动换页功能	
** 入口参数: pBuffer:数据存储区
**			 WriteAddr:开始写入的地址(24bit)
**			 NumByteToWrite:要写入的字节数(最大65535)
** 出口参数: 无		
** 备    注: 无
************************************************************************/
unsigned char sbuf_date[256];
void SPI_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		SPI_Flash_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
}
/************************************************************************																										
** 函数功能: 擦除一个扇区		
** 入口参数: Dst_Addr:扇区地址 0~511 for M45P16
** 出口参数: 无		
** 备    注: 无
************************************************************************/ 
void SPI_Flash_Erase_Sector(u32 Dst_Addr)   
{   
	Dst_Addr*=4096;
    SPI_FLASH_Write_Enable();                  //SET WEL 	 
  	CE_Low();                            //使能器件   
    SPIx_ReadWriteByte(M45P_SectorErase);      //发送扇区擦除指令 
    SPIx_ReadWriteByte((u8)((Dst_Addr)>>16));  //发送24bit地址    
    SPIx_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPIx_ReadWriteByte((u8)Dst_Addr);  
	CE_High();                            //取消片选     	      
    SPI_Flash_Wait_Busy();   				   //等待擦除完成
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
  CE_Low();                            //使能器件   
  /* Send Sector Erase instruction */
  SPIx_ReadWriteByte(M45P_PageErase);
  /* Send SectorAddr high nibble address byte */
  SPIx_ReadWriteByte((PageAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  SPIx_ReadWriteByte((PageAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  SPIx_ReadWriteByte(PageAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  CE_High();                            //使能器件   

  /* Wait the end of Flash writing */
  SPI_Flash_Wait_Busy();
}
/************************************************************************																										
** 函数功能: 写SPI FLASH	在指定地址开始写入指定长度的数据	
** 入口参数: pBuffer:数据存储区
**			 WriteAddr:开始写入的地址(24bit)
**			 NumByteToWrite:要写入的字节数(最大65535)
** 出口参数: 无		
** 备    注: 无
************************************************************************/ 	   
u8 SPI_FLASH_BUF[4096];
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    

	secpos=WriteAddr/4096;//扇区地址 0~511 for M45P16
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		SPI_Flash_Read(SPI_FLASH_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)
			{
				break;//需要擦除
			}
			  	  
		}
		if(i<secremain)//需要擦除
		{
			SPI_Flash_Erase_Sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);//写入整个扇区 		 

		}else SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 					   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	} 	 
}
/************************************************************************																										
** 函数功能: 进入掉电模式		
** 入口参数: 无
** 出口参数: 无		
** 备    注: 无
************************************************************************/
void SPI_Flash_PowerDown(void)   
{ 
  	CE_Low();                            //使能器件   
    SPIx_ReadWriteByte(M45P_PowerDown);        //发送掉电命令  
	CE_High();                            //取消片选     	      
    delay_us(3);                               //等待TPD  
}

/************************************************************************																										
** 函数功能: 唤醒		
** 入口参数: 无
** 出口参数: 无		
** 备    注: 无
************************************************************************/
void SPI_Flash_WAKEUP(void)   
{  
  	CE_Low();                            //使能器件   
    SPIx_ReadWriteByte(M45P_ReleasePowerDown);   //  send M45P_PowerDown command 0xAB    
	CE_High();                            //取消片选     	      
    delay_us(3);                               //等待TRES1
}











