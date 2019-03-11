//   Iman Gholizadeh
//===================================================


#include "LPC17xx.h"
#include "Serial.h"
#include <string.h>
#include <stdio.h>
#include "M45PE161.h"


#define  W25Q16_FLASH_ID    0xEF4015
volatile unsigned long FLASH_ID = 0;
char temp [2048]= "";
char temp1[2048] = "iman gholizadeh";
void delay (uint32_t Time)
{
    uint32_t i;
    
    i = 0;
    while (Time--) {
        for (i = 0; i < 5000; i++);
    }
}
//SPI
void spi_init (void) {
 LPC_SC->PCONP |= (1 << 8);  /* Enable power to SSPI0 block */
	
LPC_GPIO0->FIODIR   |= (1<<16);            /* P0.16 is output             */
LPC_PINCON->PINSEL0 |= (3UL<<30);            // select p0.15  -->   SCK 
LPC_PINCON->PINSEL1 |= (3<<2) | (3<<4);    // select p0.17 , p0.18  -->  MISO , MOSI
    
	
LPC_SPI-> SPCR = (1<<5) | (0<<6);          //  (Master)(MSB) 
LPC_SPI->SPCCR = 0x8;


					 
}
//--------------------------------------------------------------------------
unsigned int SPI0(unsigned int val)
{
  /* Write and Read a byte on SPI interface. */
	unsigned int received = 0;
  LPC_SPI->SPDR = val;
  while( !(LPC_SPI->SPSR & 0x80));//wait for transfer to be completed
   received=LPC_SPI->SPDR;
   return(received);
}
//--------------------------------------------------------------------------


int main(){	
	uint16_t i=0;
	SystemInit();
				SER_Init( );
			printf(" -----start------\r\n");
	LPC_GPIO0->FIOSET |= (1<<16);	
	spi_init ();
	 	
	for(i=0;i<0xff;i++);
		LPC_GPIO0->FIOCLR |= (1<<16);
	
	
 		//SPIx_Init();           
	  FLASH_ID = SPI_Flash_ReadID();
	
while(1){
		LPC_GPIO0->FIOCLR |= (1<<16);
	
		printf(" -----start------\r\n");
		printf("FLASH_ID : %x\r\n",FLASH_ID);
		FLASH_ID = 0;
		FLASH_ID = SPI_Flash_ReadID();
		SPI_FLASH_Write_Enable();
		SPI_Flash_Write(&*temp1,0,15);
	  //SPI_Flash_Write(&temp1,0,1);
		SPI_FLASH_Write_Disable();
		SPI_Flash_Read(&*temp,0,15); 
		printf("read data from SPI FLASH in STRING : %s\r\n",temp);
    printf("read data from SPI FLASH in HEX    : %x\r\n",temp);
	
		LPC_GPIO0->FIOSET |= (1<<16);
		//	 printf("Data Tcp Reception : %d\r\n",flash_read);
		delay(1000);
}
}

