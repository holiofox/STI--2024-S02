#ifndef __TDC_H
#define	__TDC_H
//����������ڲ���׼ʱ�����ڵı������ڲ���׼ʱ�ӵ����ⲿ��׼ʱ�ӳ��� DIV_
//CLKHS��DIV_CLKHS��1��2��4����У׼ֵ�� 32 λ���������� 16 λ������ 16 λС�����
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>
/* Private define ------------------------------------------------------------*/
/*�����-��ͷ*******************************/
#define TDC_REG0		      0x80009420 //У׼�մɾ���ʱ��Ϊ 8 �� 32k ����,244.14us.
//���ٲο�ʱ��2��Ƶ�����ø��پ����ϵ��һֱ����.���ò�����Χ 1,�Զ�У׼,����������
#define TDC_REG1		      0x81010100 //����ڶ��� stop1-start���رտ��ٳ�ʼ����
//stop2ͨ���رգ�stop1ͨ��һ������
#define TDC_REG2		      0x82E00000 // ���������ж�Դ������Ϊ����������
#define TDC_REG3		      0x83080000 
#define TDC_REG4			    0x84200000 
#define TDC_REG5		      0x85080000 //�ر�������Ԫ
#define TDC_REG6		      0x86000010 //����˫����
#define Init                 0x70
#define Power_On_Reset       0x50
#define Start_TOF            0x01
#define Start_Temp           0x02
#define Start_Cal_Resonator  0x03   // Resonator г����
#define Start_Cal_TDC        0x04
#define Start_TOF_Restart    0x05
#define Start_Temp_Restart   0x06
/*�����-��β*******************************/

/*SPI�ӿڶ���-��ͷ****************************/
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
/*SPI�ӿڶ���-��β****************************/
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
#define Fe9 (4e6/2/1e9)//����ʱ��Ϊ4M,������2��Ƶ����У׼ģʽ�£������õ���ʱ�ӱ�������������ĵ�������Ϊns��

/*�ȴ���ʱʱ��*/
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
