#ifndef __FLASH_H
#define __FLASH_H
  
/*
ÈäÇã ÎÏÇæäÏ ÈÎÔäÏå æ ãåÑÈÇä
˜ÊÇÈÎÇäå äæÔÊå ÔÏå ÈÑÇí ÇíÓí ÍÇİÙå
W25Q
V=1.1 
by sinaset
centralclubs.com
Data:1395.12.09
*/

//#define SIZE_PAGE_W25Q          16384       //W25QÈÑÇí ÇíÓí32
#define SIZE_PAGE_W25Q          32768       //W25QÈÑÇí ÇíÓí64

#define SIZE                    256       //ÇäÏÇÒå åÑ ÕİÍå ÇíÓí ÍÇİÙå
#define W25Q_WriteEnable		0x06      //İÚÇá ÓÇÒí æÖÚíÊ äæÔÊä
#define W25Q_WriteDisable	  	0x04      //ÛíÑ İÚÇá ÓÇÒí æÖÚíÊ äæÔÊä
#define W25Q_ReadStatusReg		0x05      //ÎæÇäÏä ÑÌíÓÊÑ æÖÚíÊ 1
#define W25Q_ReadStatusReg2		0x35      //ÎæÇäÏä ÑÌíÓÊÑ æÖÚíÊ 1
#define W25Q_WriteStatusReg		0x01      //äæÔÊä ÏÑ ÑíÌÓÊÑæÖÚíÊ
#define W25Q_ReadData		    0x03      //ÎæÇäÏä ÏíÊÇ
#define W25Q_FastReadData	  	0x0B      //ÎæÇäÏ ÓÑíÚ ÏíÊÇ
#define W25Q_FastReadDual	  	0x3B      //ÎæÇäÏ ÓÑíÚÊ ÏíÊÇ¡ÏæÈÑÇÈÑ
#define W25Q_PageProgram	  	0x02      //äæÔÊä ÏÑ í˜ ÕİÍå ÇíÓí
#define W25Q_BlockErase32	    0x52      //Ç˜ ˜ÑÏä í˜ Èáæ˜ ÇíÓí
#define W25Q_BlockErase64		0xD8      //Ç˜ ˜ÑÏä í˜ Èáæ˜ ÇíÓí
#define W25Q_SectorErase		0x20     //Ç˜ ˜ÑÏä í˜ Èáæ˜ ÇíÓí
#define W25Q_ChipErase		  	0x60      //Ç˜ ˜ÑÏä ˜á í
#define W25Q_PowerDown			0xB9      //í ÏÑ æÖÚíÊ ˜ã ãÕÑİ
#define W25Q_ReleasePowerDown	0xAB      //
#define W25Q_ManufactDeviceID	0x90      //ãÚÑİí ˜Ï ˜ÇÑÎÇäå æ ÇíÓí
#define FLASH_CS_HIGH() (LPC_GPIO0->FIOSET   |= (1<<16) )  //æÖÚíÊ ÛíÑ İÚÇá ÓÇÒí ÈÑÇí ÇíÓí             
#define FLASH_CS_LOW() (LPC_GPIO0->FIOCLR   |= (1<<16) )  // æÖÚíÊ İÚÇá ÓÇÒí ÈÑÇí ÍÇİÙå 

#pragma used+
unsigned char W25Q_read_StatustReg(unsigned char RG);           //ÎæÇäÏä æÖÚíÊ ÑíÌÓÊÑåÇ
void W25Q_Write_StatusReg(unsigned char Data);                  //äæÔÊä ÏÑ ÑíÌÓÊÑ æÖÚíÊ
void wait_busy(void);                                            //ÎæÇäÏä æÖÚíÊ ÑíÌÓÊÑåÇ
void W25Q_Write_Disable(void);                                   //ÛíÑ İÚÇá ÓÇÒí ŞÇÈáíÊ äæÔÊä ÈÑ Ñæí ÇíÓí
void W25Q_Write(void);                                           //İÚÇá ÓÇÒí äæÔÊä ÈÑ Ñæí ÇíÓí
unsigned char W25Q_busy(void);                                   //ÎæÇäÏä æÖÚíÊ ÈíÊ ãÔÛæáí
unsigned char W25Q_wel(void);                                    //ÎæÇäÏä æÖÚíÊ ÈíÊ äæÔÊä

void W25Q_Page_Program(unsigned long int  Number_PAGE, unsigned char *Page_Data);        //äæÔÊä ÏÑ í˜ ÕİÍå ÇíÓí
void W25Q_Page_Program_Res(unsigned int  *Number_PAGE_Res, unsigned char *Page_Data)  ; //äæÔÊä ÇÒ ÇÎÑíä ÕİÍå

void W25Q_Read_Data(unsigned long int  Number_PAGE,unsigned char *Page_Data);           //ÎæÇäÏä í˜ ÕİÍå ÇíÓí
void W25Q_Manufacturer_Device_ID(unsigned char *Manufacturer_id,unsigned char *Device_id);//äÇã æ˜ÇÑÎÇäå ÓÇÒäÏå ÇíÓí
void W25Q_Sector_Erase(unsigned long int address );              //Ç˜ ÓÇÒí Ó˜ÊæÑåÇí 4 ˜íáæÈÇíÊí
void W25Q_BlockErase_32(unsigned long int address );             //ÍĞİ Èáæ˜ åÇíí 32 ˜íáæÈÇíÊí ÍÇİÙå
void W25Q_BlockErase_64(unsigned long int address );             //ÍĞİ Èáæ˜ åÇíí 64 ˜íáæÈÇíÊí ÍÇİÙå
void W25Q_Chip_Erase(void );                                    //Ç˜ ˜ÑÏä ˜á ÍÇİÙå
void SPI_Flash_WAKEUP(void);                                    //ÈíÏÇÑ ÔÏä ÇÒ ãÏ ˜ã ãÕÑİ ÎÇãæÔí
void SPI_Flash_PowerDown(void);                                 //ÑİÊä Èå ãÏ ˜ã ãÕÑİ ÎÇãæÔí
void Factoty_rest_Reg(void);                                    //ãŞÏÇÑ Ïåí ˜ÇÑÎÇäå í ÈÑÇí ÑíÌÓÊÑ 1
void Format_ic(char Type_format);                               //İÑãÊ ˜ÑÏä ÍÇİÙå
unsigned char Check_page(long int Page);                        //ÈÑÑÓí Ñ íÇ ÎÇáí ÈæÏä í˜ ÕİÍå
void Info_data_ic( unsigned  int *write, unsigned  int *no_write,unsigned  int *KByte_Used,char *Percent_Used,char Status_usart); 
//ÇØáÇÚÇÊ ãíÒÇä ÇÓÊİÇÏå
//void W25Q_Page_Program_Erase(unsigned long int  Number_PAGE, unsigned char *Page_Data); //åãÒãÇä Ç˜ ˜ÑÏä æ äæÔÊä ÏÑ ÇíÓí


unsigned int spi(unsigned int val)
{
	

  /* Write and Read a byte on SPI interface. */
	unsigned int received = 0;
	 FLASH_CS_LOW();
  LPC_SPI->SPDR = val;
  while( !(LPC_SPI->SPSR & 0x80));//wait for transfer to be completed
   received=LPC_SPI->SPDR;
	FLASH_CS_HIGH() ;
   return(received);
}


unsigned char W25Q_read_StatustReg(unsigned char RG) //ÎæÇäÏä æÖÚíÊ ÑíÌÓÊÑåÇ
{
unsigned char Data=0; 
   // W25Q_Write(); 
    FLASH_CS_LOW();
    if (RG==1)spi(W25Q_ReadStatusReg);   //ÎæÇäÏä æÖÚíÊ ÑíÌÓÊÑ    
    if (RG==2)spi(W25Q_ReadStatusReg2);   //ÎæÇäÏä æÖÚíÊ ÑíÌÓÊÑ 
    Data= spi(0x00);
    spi(0x00);
    FLASH_CS_HIGH(); 
    return Data;                
}

void wait_busy(void) //ÎæÇäÏä æÖÚíÊ ÑíÌÓÊÑåÇ
{
//while ((W25Q_read_StatustReg(1)&0x01)==0x01);
}
void W25Q_Write(void)     //İÚÇá ÓÇÒí äæÔÊä ÈÑ Ñæí ÇíÓí
{
FLASH_CS_LOW(); 
spi(W25Q_WriteEnable); 
FLASH_CS_HIGH();  
}


void W25Q_Write_Disable(void)  //ÛíÑ İÚÇá ÓÇÒí ŞÇÈáíÊ äæÔÊä ÈÑ Ñæí ÇíÓí
{
FLASH_CS_LOW(); 
spi(W25Q_WriteDisable); 
FLASH_CS_HIGH();  
}

unsigned char W25Q_busy(void)    //ÎæÇäÏä æÖÚíÊ ÈíÊ ãÔÛæáí
{
unsigned char Data=0;

    FLASH_CS_LOW(); 
    spi(W25Q_ReadStatusReg);   
    Data= spi(0x00);
    spi(0x00);
    FLASH_CS_HIGH();  
    if(Data & 0x80) Data=1;    
    else Data=0; 
    return Data;
}

unsigned char W25Q_wel(void)   //ÎæÇäÏä æÖÚíÊ ÈíÊ äæÔÊä
{
unsigned char Data=0;

    FLASH_CS_LOW(); 
    spi(W25Q_ReadStatusReg);    
    Data= spi(0x00);
    spi(0x00);
    FLASH_CS_HIGH();  
    Data=Data<<1;
     if(Data & 0x80) Data=1;    
     else Data=0; 
     return Data;
}

void W25Q_Sector_Erase(unsigned long int address )   //Ç˜ ÓÇÒí Ó˜ÊæÑåÇí 4 ˜íáæÈÇíÊí
{
unsigned char Data1,Data2,Data3;

//ÔãÇÑå Ó˜ÊæÑ ÈÇíÏ Èíä 0 ÊÇ 1024 ÈÇÔÏ
//ÏÑ ãÏá64 ÊÚÏÇÏ Ó˜ÊæÑåÇ 2048 ãí ÈÇÔÏ

//åÑ Ó˜ÊæÑ 4 ˜íáæÈÇíÊ ÑÇ ÍĞİ ãí ˜äÏ
address=address*SIZE*16;

Data1= (address>>16);
Data2= (address>>8);
Data3= (address);

   W25Q_Write(); 
   wait_busy();
   FLASH_CS_LOW(); 
   spi(W25Q_SectorErase);        //Ç˜ ˜ÑÏä Ó˜ÊæÑ     
   spi(Data1); 
   spi(Data2);
   spi(Data3); 
   FLASH_CS_HIGH(); 
   wait_busy();                 
}

void W25Q_BlockErase_32(unsigned long int address )      //ÍĞİ Èáæ˜ åÇíí 32 ˜íáæÈÇíÊí ÍÇİÙå
{
unsigned char Data1,Data2,Data3;

//ÔãÇÑå Èáæ˜ ÈÇíÏ Èíä 0 ÊÇ 128 ÈÇÔÏ
//ÏÑ ãÏá 64 ÊÚÏÇÏ 256 ãí ÈÇÔÏ

//ÏÑ Çíä ÑæÔ 64 ˜íáæÈÇíÊ ÍĞİ ãíÔæÏ
address=address*SIZE*128;
Data1= (address>>16);
Data2= (address>>8);
Data3= (address);
   W25Q_Write();
   FLASH_CS_LOW(); 
   spi(W25Q_BlockErase32);        //Ç˜ ˜ÑÏä Ó˜ÊæÑ     
   spi(Data1); 
   spi(Data2);
   spi(Data3); 
   FLASH_CS_HIGH();
   wait_busy(); 
// delay_ms(900);                  
}

void W25Q_BlockErase_64(unsigned long int address )        //ÍĞİ Èáæ˜ åÇíí 64 ˜íáæÈÇíÊí ÍÇİÙå
{
//ÔãÇÑå Èáæ˜ ÈÇíÏ Èíä 0 ÊÇ 64 ÈÇÔÏ
//ÏÑ ãÏá 64 ÊÚÏÇÏ 128 ãí ÈÇÔÏ

//ÏÑ Çíä ÑæÔ 64 ˜íáæÈÇíÊ ÍĞİ ãíÔæÏ
unsigned char Data1,Data2,Data3;

address=address*SIZE*256;
Data1= (address>>16);
Data2= (address>>8);
Data3= (address);
   W25Q_Write(); 
   wait_busy();
   FLASH_CS_LOW();  
   
   spi(W25Q_BlockErase64);        //Ç˜ ˜ÑÏä Ó˜ÊæÑ     
   spi(Data1); 
   spi(Data2);
   spi(Data3); 
   
   FLASH_CS_HIGH(); 
   wait_busy();                   
}

void W25Q_Chip_Erase(void )     //Ç˜ ˜ÑÏä ˜á ÍÇİÙå
{
   W25Q_Write();  
   wait_busy();
   FLASH_CS_LOW();
   spi(W25Q_ChipErase);        //Ç˜ ˜ÑÏä Ó˜ÊæÑ   
   FLASH_CS_HIGH(); 
   wait_busy();                  
}

void W25Q_Write_StatusReg(unsigned char Data)     //äæÔÊä ÏÑ ÑíÌÓÊÑ æÖÚíÊ
{
 
   W25Q_Write(); 
   FLASH_CS_LOW(); 
   spi(W25Q_WriteStatusReg);        //äæÔÊä æÖÚíÊ
   spi(Data); 
   spi(0x00);
   FLASH_CS_HIGH();  
}

void W25Q_Manufacturer_Device_ID(unsigned char *Manufacturer_id,unsigned char *Device_id)//äÇã æ˜ÇÑÎÇäå ÓÇÒäÏå ÇíÓí
{ 
wait_busy();
    FLASH_CS_LOW(); 
    spi(W25Q_ManufactDeviceID);
    spi(0x00);
    spi(0x00);
    spi(0x00);
    *Manufacturer_id= spi(0x00);
    *Device_id= spi(0x00);
    FLASH_CS_HIGH(); 
    
  // delay_ms(10);    
}

void W25Q_Page_Program(unsigned long int  Number_PAGE, unsigned char *Page_Data)//äæÔÊä ÏÑ í˜ ÕİÍå ÇíÓí
{                                                                             
//ÔãÇÑå ÕİÍå ÈÇíÏ Èíä 0 ÊÇ 16384 ÈÇÔÏ
//ÊÚÏÇÏ ÈÇíÊ ÈÑÇí ĞÎíÑå äÈÇíÏ ÈíÔÊÑ ÇÒ 256 ÈÇíÊ ÈÇÔÏ
int g=0;
unsigned char Data1,Data2,Data3;

Number_PAGE=(Number_PAGE*256);      

Data1= (Number_PAGE>>16);
Data2= (Number_PAGE>>8);
Data3= (Number_PAGE);

    W25Q_Write();  
    FLASH_CS_LOW(); 
    spi(W25Q_PageProgram);        //äæÔÊä
    spi(Data1);
    spi(Data2);
    spi(Data3); 
    for(g=0;g<SIZE;g++)spi(Page_Data[g]); 
    FLASH_CS_HIGH(); 
    //delay_us(750); 
     wait_busy(); 
}
/*
void W25Q_Page_Program_Erase(unsigned long int  Number_PAGE, unsigned char *Page_Data)//åãÒãÇä Ç˜ ˜ÑÏä æ äæÔÊä ÏÑ ÇíÓí
{
//ÈÑÇí ÇÓÊİÇÏå ÇÒ Çíä ÊÇÈÚ¡ÇÏÑÓ Çæá ÕİÍå ÔãÇ ÈÇíÏ ÇÈÊÏÇ Ó˜ÊæÑ ÈÇÔÏ
//ÇíÓí İáÔ ãæÑÏ ÇÓÊİÇÏå ÌãÚÇ 1024 Ó˜ÊæÑ 4 ˜íáæ ÈÇíÊí ÏÇÑÏ ˜å ÏÑ åÑ Èáæ˜ 16 Ó˜ÊæÑ æÌæÏ ÏÇÑÏ
//ÈåÊÑ ÇÓÊ ÈÑÇí ÇÓÊİÇÏå ÇÒ  ÊÇÈÚ¡ÇÒ ÕİÍå 0 ÔÑæÚ ÈÇ ĞÎíÑ ÇØáÇÚÇÊ ˜äíÏ
unsigned int g,p;
unsigned char block=0,sector;
float M=0.0;
block=(Number_PAGE/256);
sector=(Number_PAGE/16);
M=(Number_PAGE/16)%10; 

if(M==1.00000 || M==2.00000 || M==3.00000 || M==4.00000 || M==5.00000 || M==6.00000 || M==7.00000 || M==8.00000 || M==9.00000 || M==0.00000 ){
W25Q_Sector_Erase(sector);  
}
W25Q_Page_Program(Number_PAGE,Page_Data);
} 
*/

void W25Q_Read_Data(unsigned long int  Number_PAGE,unsigned char *Page_Data)//ÎæÇäÏä í˜ ÕİÍå ÇíÓí
{
//ÔãÇÑå ÕİÍå ÈÇíÏ Èíä 0 ÊÇ 16384 ÈÇÔÏ
//ÊÚÏÇÏ ÈÇíÊ ÈÑÇí ĞÎíÑå äÈÇíÏ ÈíÔÊÑ ÇÒ 256 ÈÇíÊ ÈÇÔÏ
int g=0;
unsigned char Data1,Data2,Data3;
Number_PAGE=(Number_PAGE*256);      
Data1= (Number_PAGE>>16);
Data2= (Number_PAGE>>8);
Data3= (Number_PAGE);

  //  W25Q_Write();  
    FLASH_CS_LOW(); 
    spi(W25Q_ReadData);       //ÎæÇäÏä
    spi(Data1);
    spi(Data2);
    spi(Data3);   
    for(g=0;g<SIZE;g++)Page_Data[g]=spi(0x00);
    FLASH_CS_HIGH(); 
}

void SPI_Flash_PowerDown(void)  //Èå ÎæÇÈ ÑİÊä 
{ 
  	FLASH_CS_LOW();                           
    spi(W25Q_PowerDown);      
	FLASH_CS_HIGH();                              	      
    //delay_us(3);                              
}   

void SPI_Flash_WAKEUP(void)      //ÈíÏÇÑ ÔÏä ÇÒ ÎæÇÈ
{  
  	FLASH_CS_LOW();                          
    spi(W25Q_ReleasePowerDown);                                    	      
    FLASH_CS_HIGH();                                
} 

void Factoty_rest_Reg(void)        //ÑíÓÊ ˜ÑÏä ÑíÌÓÊÑ ÔãÇÑå 1
{  
  W25Q_Write_StatusReg(0x00);                              
} ;

void Format_ic(char Type_format)     //İÑãÊ ˜ÑÏä ˜á ÍÇİÙå
{
//ÚÏÏ 0 İÑãÊ Ó˜ÊæÑ Èå Ó˜ÊæÑ
//ÚÏÏ 1 İÑãÊ ÈáÇ˜ åÇí 32 ˜íáæÈÇíÊí
//ÚÏÏ 2 İÑãÊ ÈáÇ˜ åÇ 64 ˜íáæÈÇíÊí
int g;

  if(Type_format==0)for(g=0;g<1024;g++)W25Q_Sector_Erase(g);
  if(Type_format==1) for(g=0;g<128;g++)W25Q_BlockErase_32(g); 
  if(Type_format==2) for(g=0;g<64;g++)W25Q_BlockErase_64(g);
                                 
} ;

unsigned char Check_page(long int Page)     //ÇØáÇÚ ÇÒ Ñ ÈæÏä íÇ äÈæÏä ÇØáÇÚÇÊ ÏÑ í˜ ÕİÍå
{

//ÈÑÔÊ ÚÏÏ 0 íÚäí Çíä ÕİÍå ÎÇáí ãí ÈÇÔÏ æ íÒí ÈÑ Ñæí Çä äæÔÊå äÔÏå
//ÈÑÔÊ ÚÏÏ 1 íÚäí ÏÑ Çíä ÕİÍå ãŞÏÇÑí ÇØáÇÚÇÊ äæÔÊå ÔÏå ÇÓÊ
int g,c=0;
unsigned char Buff[256],DATA;
int No_write=0;

   W25Q_Read_Data(Page,Buff);

    for(g=0;g<256;g++)
    {
    DATA=Buff[g];
    if(DATA==0xff)c++;
    }
    
   if(c<250)No_write=1;                        
   else if(c>250)No_write=0;  
    
   return No_write ;                              
};

void W25Q_Page_Program_Res(unsigned int  *Number_PAGE_Res, unsigned char *Page_Data) 
//äæÔÊä ÏÑ ÇÎÑíä ÕİÍå ÎÇáí ÇíÓí
{
/*
Èå ˜ã˜ Çíä ÊÇÈÚ ãí ÊæÇäíÏ ÇÒ ÇÎÑíä ÕİÍå äæÔÊå  ÔÏå  Èå ÈÚÏ ¡ ÇíÓí ÇÓÊİÇÏå ˜äíÏ æ ÔÑæÚ Èå äæÔÊä È˜äíÏ
ÊÕæÑ ˜äíÏ ÇÎÑíä ÕİÍå äæÔÊå ÔÏå ÏÑ ÇíÓí499 ãí ÈÇÔÏ
ãÇ ÏÑ ãÊÛíÑ
Page_Data
ÏíÊÇ ÎæÏ ÑÇ ŞÑÇÑ ãíÏåíã
ÊÇÈÚ ÇÈÊÏÇ ÔÑæÚ ãí ˜äÏ ÇÎÑíä ÕİÍå äæÔÊå ÔÏå ÑÇ íÏÇ ãí ˜äÏ ÓÓ ÇÒ ÕİÍå ÈÚÏ ÇÒ Çä ÏíÊÇ ãÇ ÑÇ ĞÎíÑ ãí ˜äÏ
ÇÏÑÓ ÕİÍå äæÔÊå ÔÏå ÑÇ ÏÑ ãÊÛíÑ
Number_PAGE_Res
ŞÑÇÑ ãíÏåÏ ˜å ÔãÇ äíÒ ãí ÊæÇäíÏ  Çä ÑÇ Èå ˜ã˜ ãÊÛíÑ ÏÑæä ÈÑäÇãå ÎæÏ ÈÑÏÇÑíÏ
Èå ÚäæÇä ãËÇá ãÇ ÏÑ ÈÑäÇãå ÈÏíä ØÑíŞ ÏíÊÇ ÑÇ ãíÏåíã æ ÇÏÑÓ ÑÇ ÏÑíÇİÊ ãí ˜äíã

W25Q_Page_Program_Res(&ADDRESS,BUF)
ÈÑÇí ÎæÇäÏä
W25Q_Read_Data(ADDRESS,BUFF)
íÇ ãí ÊæÇäíÏ ÇÏÑÓ ÕİÍå ĞÎíÑå ÔÏå ÑÇ ÏÑ ãÊÛíÑ ÏíÑ ĞÎíÑå ˜äíÏ æ Çä ÑÇ ÏÇÔÊå ÈÇÔíÏ
*/
unsigned int g,Check;
unsigned long int Address;

      for(g=0;g<SIZE_PAGE_W25Q;g++){
      Check=Check_page(g); 
      if(Check==0)Address=g,g=SIZE_PAGE_W25Q; 
      } 
      *Number_PAGE_Res=Address;
      W25Q_Page_Program(Address,Page_Data);

}


void Info_data_ic( unsigned  int *write, unsigned  int *no_write,unsigned  int *KByte_Used,char *Percent_Used,char Status_usart) 
//ÇØáÇÚÇÊ ãÑÈæØ Èå ÍÇİÙå
{
// ãÊÛíÑ Çæá ÊÚÏÇÏ ÕİÍå åÇÊ äæÔÊå ÔÏå ÑÇ íÏÇ ãí ˜äÏ
//ãÊÛíÑ Ïæã ÊÚÏÇÏ ÕİÍå åÇÊ ÎÇã æ äæÔÊå äÔÏå ÑÇ íÏÇ ãí ˜äÏ
//ãÊÛíÑ Óæã ÍÌã ÇÓÊİÇÏå ÔÏå ÇÒ ÇíÓí ÑÇ Ğ˜Ñ ãí ˜äÏ
//ãÊÛíÑ åÇÑã äíÒ ÏÑÕÏ ÇÓÊİÇÏå ÔÏ ÇÒ ÇíÓí ÑÇ Ğ˜Ñ ãí ˜äÏ
unsigned  int g,c=0,WR_Y=0,WR_N=0,us=0;
char LD=0,LD2=0,str[15],str2[25];
int check=0;
    puts("Please Wait..\n\r"); 
    for(g=0;g<SIZE_PAGE_W25Q;g++)
    {
     check=Check_page(g);  
     if(check==1)WR_Y++;
     if(check==0)WR_N++; 
     
     
     us++;   
   //  if(Status_usart==1 && check==1)sprintf(str2,"NumberPage:%iwritten\n\r",g),puts(str2); 
     if(Status_usart==1 && us > (SIZE_PAGE_W25Q / 100))
       {
        us=0;
        LD++;
       // if(LD>10)LD=0,LD2+=10,sprintf(str,"Loading:%d\n\r",LD2),puts(str);
       }
    } 
    puts("Loading:100\n\r");
    *no_write=WR_N;
    *write=WR_Y;
    *KByte_Used=(WR_Y/4);
    if  (SIZE_PAGE_W25Q==16384)*Percent_Used= (WR_Y/4)/40; 
    else if (SIZE_PAGE_W25Q==32768)*Percent_Used= (WR_Y/4)/80;                          
};
#pragma used-

#endif
