//   Iman Gholizadeh
//===================================================
#include "LPC17xx.h"
#include "Serial.h"
#include <string.h>
#include <stdio.h>
#include "M45PE161.h"
//===================================================
#define  W25Q16_FLASH_ID    0xEF4015
volatile unsigned long FLASH_ID = 0;
char temp = '\0';
char temp1 = 'a';
//===================================================
//             delay 
//===================================================
void delay (uint32_t Time)
{
    uint32_t i;
    
    i = 0;
    while (Time--) {
        for (i = 0; i < 5000; i++);
    }
}
//===================================================
//           spi_init
//===================================================
void spi_init (void) {
 LPC_SC->PCONP |= (1 << 8);  /* Enable power to SSPI0 block */
	
LPC_GPIO0->FIODIR   |= (1<<16);            /* P0.16 is output             */
LPC_PINCON->PINSEL0 |= (3UL<<30);            // select p0.15  -->   SCK 
LPC_PINCON->PINSEL1 |= (3<<2) | (3<<4);    // select p0.17 , p0.18  -->  MISO , MOSI
    
	
LPC_SPI-> SPCR = (1<<5) | (0<<6);          //  (Master)(MSB) 
LPC_SPI->SPCCR = 0x8;


					 
}
//===================================================
//           spi_init2
//===================================================
void spi_init2 (void) {	
LPC_GPIO0->FIODIR   |= (1<<6);            /* P0.6 is output    cs         */
LPC_GPIO0->FIODIR   |= (1<<7);            /* P0.7 is output     sck        */
//LPC_GPIO0->FIODIR   |= (0<<8);            /* P0.8 is input     miso        */
LPC_GPIO0->FIODIR   |= (1<<9);            /* P0.9 is output      mosi       */	
}
//===================================================
//===================================================
unsigned int SPI0(unsigned int val)
{
  /* Write and Read a byte on SPI interface. */
	unsigned int received = 0;
  LPC_SPI->SPDR = val;
  while( !(LPC_SPI->SPSR & 0x80));//wait for transfer to be completed
   received=LPC_SPI->SPDR;
   return(received);
}
//===================================================


int main(){	
	uint16_t i=0;
	SystemInit();
	SER_Init( );
	printf(" -----start------\r\n");
	LPC_GPIO0->FIOSET |= (1<<6);	
	spi_init (); 	
	for(i=0;i<0xff;i++);
	LPC_GPIO0->FIOCLR |= (1<<6);
 	//SPIx_Init();           
	FLASH_ID = SPI_Flash_ReadID();	
	while(1){
			LPC_GPIO0->FIOCLR |= (1<<6);
			printf("FLASH_ID : %x\r\n",FLASH_ID);
			FLASH_ID = 0;
			FLASH_ID = SPI_Flash_ReadID();		
			SPI_FLASH_Write_Enable();
			SPI_Flash_Write(&temp1,0,1);
			SPI_FLASH_Write_Disable();
			SPI_Flash_Read(&temp,0,1); 
			printf("Temp Data : %c\r\n",temp);
			LPC_GPIO0->FIOSET |= (1<<6);
			delay(1000);	
	}
}

