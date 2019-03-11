/****************************************Copyright (c)**************************************************
**                               ShenZhen ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.openmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name: 			AT45DB161Drv.h
** Last modified Date: 	2007-04-25
** Last Version: 		1.0
** Descriptions: 		M45PE161的操作函数
**
**------------------------------------------------------------------------------------------------------
** Created 	 By: 		OpenMCU
** Created date: 		2010-12-23
** Version: 1.0
** Descriptions: 		First version
**
**------------------------------------------------------------------------------------------------------
** Modified by: 	
** Modified date: 	
** Version:
** Descriptions:  	
**
********************************************************************************************************/

#ifndef __F016BDRV_H 
#define __F016BDRV_H
#include "stdint.h"
#include "sys.h"


#define M45P_WriteEnable		0x06 
#define M45P_WriteDisable		0x04 
#define M45P_ReadStatusReg		0x05 
#define M45P_WriteStatusReg		0x01 
#define M45P_ReadData			0x03 
#define M45P_FastReadData		0x0B 
#define M45P_FastReadDual		0x3B 
#define M45P_PageProgram		0x02 
#define M45P_PageErase			0xDB 
#define M45P_SectorErase		0x20 //0xD8
#define M45P_ChipErase		0x60//	0xC7 
#define M45P_PowerDown			0xB9 
#define M45P_ReleasePowerDown	0xAB 
#define M45P_ManufactDeviceID	0x90 
//#define M45P_JedecDeviceID		0x9F 

void SPI_Flash_Init(void);
u32  SPI_Flash_ReadID(void);  	    //读取FLASH ID
u8	 SPI_Flash_ReadSR(void);        //读取状态寄存器 
void SPI_FLASH_Write_SR(u8 sr);  	  //写状态寄存器
void SPI_FLASH_Write_Enable(void);  //写使能 
void SPI_FLASH_Write_Disable(void);	//写保护
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void SPI_FLASH_PageErase(u32 PageAddr);
void SPI_Flash_Erase_Sector(u32 Dst_Addr);//扇区擦除
void SPI_Flash_Wait_Busy(void);           //等待空闲
void SPI_Flash_PowerDown(void);           //进入掉电模式
void SPI_Flash_WAKEUP(void);			        //唤醒
void  SPIx_Init(void);
u8 SPIx_ReadWriteByte(u8 TxData);




#endif
