#ifndef __AD9959_H__
#define __AD9959_H__

#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"

extern uint8_t 	FR1_DATA[3] ;
extern uint8_t 	LSRR_DATA[2];
extern uint8_t 	RDW_DATA[4] ; 						
extern uint8_t 	FDW_DATA[4] ;


#define SCK_Pin 						GPIO_Pin_5
#define SCK_GPIO_Port 			GPIOC
#define SCK_GPIO_CLK 				RCC_AHB1Periph_GPIOC


#define SD1_Pin 						GPIO_Pin_12
#define SD1_GPIO_Port 			GPIOB
#define SD1_GPIO_CLK 				RCC_AHB1Periph_GPIOB

#define CS_Pin 							GPIO_Pin_2
#define CS_GPIO_Port 				GPIOA
#define CS_GPIO_CLK 				RCC_AHB1Periph_GPIOA

#define SD0_Pin 						GPIO_Pin_5
#define SD0_GPIO_Port 			GPIOD
#define SD0_GPIO_CLK 				RCC_AHB1Periph_GPIOD

#define UPDATE_Pin 					GPIO_Pin_3
#define UPDATE_GPIO_Port 		GPIOB
#define UPDATE_GPIO_CLK 		RCC_AHB1Periph_GPIOB

#define P0_Pin 							GPIO_Pin_15
#define P0_GPIO_Port 				GPIOA
#define P0_GPIO_CLK 				RCC_AHB1Periph_GPIOA

#define RST_Pin							GPIO_Pin_1
#define RST_GPIO_Port 			GPIOA
#define RST_GPIO_CLK 				RCC_AHB1Periph_GPIOA

#define PDC_Pin 						GPIO_Pin_4
#define PDC_GPIO_Port 			GPIOB
#define PDC_GPIO_CLK 				RCC_AHB1Periph_GPIOB

#define P1_Pin 							GPIO_Pin_2
#define P1_GPIO_Port 				GPIOB
#define P1_GPIO_CLK 				RCC_AHB1Periph_GPIOB

#define P2_Pin 							GPIO_Pin_7
#define P2_GPIO_Port 				GPIOA
#define P2_GPIO_CLK 				RCC_AHB1Periph_GPIOA

#define P3_Pin 							GPIO_Pin_4
#define P3_GPIO_Port 				GPIOC
#define P3_GPIO_CLK 				RCC_AHB1Periph_GPIOC

#define SD2_Pin 						GPIO_Pin_2
#define SD2_GPIO_Port 			GPIOC
#define SD2_GPIO_CLK 				RCC_AHB1Periph_GPIOC

#define SD3_Pin 						GPIO_Pin_3
#define SD3_GPIO_Port 			GPIOI
#define SD3_GPIO_CLK 				RCC_AHB1Periph_GPIOI


//AD9959寄存器地址定义
#define CSR_ADD   0x00   //CSR 通道选择寄存器
#define FR1_ADD   0x01   //FR1 功能寄存器1
#define FR2_ADD   0x02   //FR2 功能寄存器2
#define CFR_ADD   0x03   //CFR 通道功能寄存器
#define CFTW0_ADD 0x04   //CTW0 通道频率转换字寄存器
#define CPOW0_ADD 0x05   //CPW0 通道相位转换字寄存器
#define ACR_ADD   0x06   //ACR 幅度控制寄存器
#define LSRR_ADD  0x07   //LSR 通道线性扫描寄存器
#define RDW_ADD   0x08   //RDW 通道线性向上扫描寄存器
#define FDW_ADD   0x09   //FDW 通道线性向下扫描寄存器


//AD9959管脚宏定义
#define CS_H					GPIO_SetBits(CS_GPIO_Port,CS_Pin);		
#define SCLK_H				GPIO_SetBits(SCK_GPIO_Port,SCK_Pin);				
#define UPDATE_H			GPIO_SetBits(UPDATE_GPIO_Port,UPDATE_Pin);	
#define PS0_H					GPIO_SetBits(P0_GPIO_Port,P0_Pin);
#define PS1_H					GPIO_SetBits(P1_GPIO_Port,P1_Pin);			
#define PS2_H					GPIO_SetBits(P2_GPIO_Port,P2_Pin);			
#define PS3_H					GPIO_SetBits(P3_GPIO_Port,P3_Pin);			
#define SDIO0_H				GPIO_SetBits(SD0_GPIO_Port,SD0_Pin);	
#define SDIO1_H				GPIO_SetBits(SD1_GPIO_Port,SD1_Pin);
#define SDIO2_H				GPIO_SetBits(SD2_GPIO_Port,SD2_Pin);	
#define SDIO3_H				GPIO_SetBits(SD3_GPIO_Port,SD3_Pin);	
#define AD9959_PWR_H	GPIO_SetBits(PDC_GPIO_Port,PDC_Pin);
#define Reset_H				GPIO_SetBits(RST_GPIO_Port,RST_Pin);



#define CS_L						GPIO_ResetBits(CS_GPIO_Port,CS_Pin);
#define SCLK_L					GPIO_ResetBits(SCK_GPIO_Port,SCK_Pin);		
#define UPDATE_L				GPIO_ResetBits(UPDATE_GPIO_Port,UPDATE_Pin);			
#define PS0_L						GPIO_ResetBits(P0_GPIO_Port,P0_Pin);
#define PS1_L						GPIO_ResetBits(P1_GPIO_Port,P1_Pin);			
#define PS2_L						GPIO_ResetBits(P2_GPIO_Port,P2_Pin);			
#define PS3_L						GPIO_ResetBits(P3_GPIO_Port,P3_Pin);			
#define SDIO0_L					GPIO_ResetBits(SD0_GPIO_Port,SD0_Pin);
#define SDIO1_L					GPIO_ResetBits(SD1_GPIO_Port,SD1_Pin);
#define SDIO2_L					GPIO_ResetBits(SD2_GPIO_Port,SD2_Pin);	
#define SDIO3_L					GPIO_ResetBits(SD3_GPIO_Port,SD3_Pin);	
#define AD9959_PWR_L		GPIO_ResetBits(PDC_GPIO_Port,PDC_Pin);
#define Reset_L					GPIO_ResetBits(RST_GPIO_Port,RST_Pin);



void AD9959_GPIO_Init(void);
void delay1 (uint32_t length);
void IntReset(void);	  //AD9959复位
void IO_Update(void);   //AD9959更新数据
void Intserve(void);		   //IO口初始化
void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData,uint8_t temp);
void Init_AD9959(void);
void Write_frequence(uint8_t Channel,uint32_t Freq);
void Write_Amplitude(uint8_t Channel, uint16_t Ampli);
void Write_Phase(uint8_t Channel,uint16_t Phase);
void Sweep_Frequence(uint32_t S0,uint32_t E0,uint8_t Channel,uint8_t time,uint32_t step,uint8_t direct,uint8_t nodwell);
void Sweep_Amplitude(uint16_t S0,uint32_t E0,uint8_t Channel,uint8_t time,uint32_t step,uint8_t direct,uint8_t nodwell);
void Sweep_Phase(uint16_t S0,uint32_t E0,uint8_t Channel,uint8_t time,uint32_t step,uint8_t direct,uint8_t nodwell);
void ReadData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData);

#endif
