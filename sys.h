#ifndef __SYS_H
#define __SYS_H	

/*添加部分*/
typedef unsigned          char u8;
typedef unsigned short     int u16;
typedef unsigned       __int64 u64;
typedef signed   long     u32;

typedef unsigned char  BOOLEAN;                                         /*  布尔变量                    */
typedef unsigned char  INT8U;                                           /*  无符号8位整型变量           */
typedef signed   char  INT8S;                                           /*  有符号8位整型变量           */
typedef unsigned short INT16U;                                          /*  无符号16位整型变量          */
typedef signed   short INT16S;                                          /*  有符号16位整型变量          */
typedef unsigned long  INT32U;                                          /*  无符号32位整型变量          */
typedef signed   long  INT32S;                                          /*  有符号32位整型变量          */
typedef float          FP32;                                            /*  单精度浮点数（32位长度）    */
typedef double         FP64;                                            /*  双精度浮点数（64位长度）    */

#define   GPIOPIN_0	   (1 << 0)
#define   GPIOPIN_1	   (1 << 1)
#define   GPIOPIN_2	   (1 << 2)
#define   GPIOPIN_3	   (1 << 3)
#define   GPIOPIN_4	   (1 << 4)
#define   GPIOPIN_5	   (1 << 5)
#define   GPIOPIN_6	   (1 << 6)
#define   GPIOPIN_7	   (1 << 7)
#define   GPIOPIN_8	   (1 << 8)
#define   GPIOPIN_9	   (1 << 9)
#define   GPIOPIN_10   (1 << 10)
#define   GPIOPIN_11   (1 << 11)
#define   GPIOPIN_12   (1 << 12)
#define   GPIOPIN_13   (1 << 13)
#define   GPIOPIN_14   (1 << 14)
#define   GPIOPIN_15   (1 << 15)

#define   GPIO0_6(x)    ((x) ? (LPC_GPIO0->FIOSET = GPIOPIN_6) : (LPC_GPIO0->FIOCLR = GPIOPIN_6))
#define   GPIO0_7(x)    ((x) ? (LPC_GPIO0->FIOSET = GPIOPIN_7) : (LPC_GPIO0->FIOCLR = GPIOPIN_7))
#define   GPIO0_9(x)    ((x) ? (LPC_GPIO0->FIOSET = GPIOPIN_9) : (LPC_GPIO0->FIOCLR = GPIOPIN_9))
#define   GPIO2_13      (*((volatile unsigned long *) (0x22000000+ (0x2009C055-0x20000000)*32 + 5*4)))
#define   GPIO0_8       (*((volatile unsigned long *) (0x22000000+ (0X2009C015-0x20000000)*32 + 0*4)))

// #define   CE_Low()      LPC_GPIO0->FIOCLR2 = GPIOPIN_0
// #define   CE_High()     LPC_GPIO0->FIOSET2 = GPIOPIN_0

#define   CE_Low()      LPC_GPIO0->FIOCLR   |= (1<<6); 
#define   CE_High()     LPC_GPIO0->FIOSET   |= (1<<6); 

#define   SD_CS(x)      ((x) ? (LPC_GPIO1->FIOSET = (1<<21)) : (LPC_GPIO1->FIOCLR = (1<<21)))

#define SPI_SPEED_2   0
#define SPI_SPEED_4   1
#define SPI_SPEED_8   2
#define SPI_SPEED_16  3
#define SPI_SPEED_256 4





#endif

