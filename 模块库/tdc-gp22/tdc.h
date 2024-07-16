#ifndef __TDC_H
#define	__TDC_H
//测量结果是内部基准时钟周期的倍数，内部基准时钟等于外部基准时钟除以 DIV_
//CLKHS（DIV_CLKHS＝1、2、4）。校准值是 32 位定点数，由 16 位整数和 16 位小数组成
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>
/* Private define ------------------------------------------------------------*/
/*命令定义-开头*******************************/
#define TDC_REG0		      0x80009420 //校准陶瓷晶振时间为 8 个 32k 周期,244.14us.
//高速参考时钟2分频，设置高速晶振上电后一直起振.设置测量范围 1,自动校准,上升沿敏感
#define TDC_REG1		      0x81010100 //计算第二个 stop1-start，关闭快速初始化。
//stop2通道关闭，stop1通道一个脉冲
#define TDC_REG2		      0x82E00000 // 开启所有中断源，设置为单边沿敏感
#define TDC_REG3		      0x83080000 
#define TDC_REG4			    0x84200000 
#define TDC_REG5		      0x85080000 //关闭噪声单元
#define TDC_REG6		      0x86000010 //开启双精度
#define Init                 0x70
#define Power_On_Reset       0x50
#define Start_TOF            0x01
#define Start_Temp           0x02
#define Start_Cal_Resonator  0x03   // Resonator 谐振器
#define Start_Cal_TDC        0x04
#define Start_TOF_Restart    0x05
#define Start_Temp_Restart   0x06
/*命令定义-结尾*******************************/

/*SPI接口定义-开头****************************/
#define TDC_SPI                           SPI1
#define TDC_SPI_CLK                       RCC_APB2Periph_SPI1
#define TDC_SPI_CLK_INIT                  RCC_APB2PeriphClockCmd

#define TDC_SPI_SCK_PIN                   GPIO_Pin_5                  
#define TDC_SPI_SCK_GPIO_PORT             GPIOA                       
#define TDC_SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define TDC_SPI_SCK_PINSOURCE             GPIO_PinSource5
#define TDC_SPI_SCK_AF                    GPIO_AF_SPI1

#define TDC_SPI_MISO_PIN                  GPIO_Pin_6                
#define TDC_SPI_MISO_GPIO_PORT            GPIOA                   
#define TDC_SPI_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define TDC_SPI_MISO_PINSOURCE            GPIO_PinSource6
#define TDC_SPI_MISO_AF                   GPIO_AF_SPI1

#define TDC_SPI_MOSI_PIN                  GPIO_Pin_7               
#define TDC_SPI_MOSI_GPIO_PORT            GPIOA                      
#define TDC_SPI_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define TDC_SPI_MOSI_PINSOURCE            GPIO_PinSource7
#define TDC_SPI_MOSI_AF                   GPIO_AF_SPI1

#define TDC_CS_PIN                        GPIO_Pin_4               
#define TDC_CS_GPIO_PORT                  GPIOA                     
#define TDC_CS_GPIO_CLK                   RCC_AHB1Periph_GPIOA



#define TDC_INT_Pin                       GPIO_Pin_2
#define TDC_INT_GPIO_Port                 GPIOA
#define TDC_INT_GPIO_CLK                  RCC_AHB1Periph_GPIOA

#define TDC_RTN_Pin                       GPIO_Pin_1
#define TDC_RTN_GPIO_Port                 GPIOA
#define TDC_RTN_GPIO_CLK                  RCC_AHB1Periph_GPIOA

#define TDC_START_Pin                       GPIO_Pin_0
#define TDC_START_GPIO_Port                 GPIOA
#define TDC_START_GPIO_CLK                  RCC_AHB1Periph_GPIOA
/*SPI接口定义-结尾****************************/
#define SPI_TDC_CS_LOW()      {TDC_CS_GPIO_PORT->BSRRH=TDC_CS_PIN;}
#define SPI_TDC_CS_HIGH()     {TDC_CS_GPIO_PORT->BSRRL=TDC_CS_PIN;}
#define SSN(x) GPIO_WriteBit(TDC_CS_GPIO_PORT, TDC_CS_PIN, x)
#define SCK(x) GPIO_WriteBit(TDC_SPI_SCK_GPIO_PORT, TDC_SPI_SCK_PIN, x)
#define SI(x)  GPIO_WriteBit(TDC_SPI_MOSI_GPIO_PORT, TDC_SPI_MOSI_PIN, x)
#define RTN(x) GPIO_WriteBit(TDC_RTN_GPIO_Port, TDC_RTN_Pin, x)

#define SO() GPIO_ReadInputDataBit(TDC_SPI_MISO_GPIO_PORT, TDC_SPI_MISO_PIN)
#define INT() GPIO_ReadInputDataBit(TDC_INT_GPIO_Port, TDC_INT_Pin)

#define OFF 0
#define ON 1
#define Fe9 (4e6/2/1e9)//高速时钟为4M,设置了2分频，在校准模式下，测量得到的时钟倍数，乘这个数的倒数，即为ns。

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

void SPI_TDC_GPIO_Init(void);
void TDC_Init(void);
uint32_t TDC_Get_Status_Reg(void);
uint8_t TDC_Test(void);
float fixed2float(uint32_t fixedPoint);
float TDC_to_ns(uint32_t val);
uint8_t TDC_Measure(uint32_t *result);

#endif /* __TDC_H */
