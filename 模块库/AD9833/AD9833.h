
#include "stm32f4xx.h"


#ifndef _AD9833_H
#define _AD9833_H

#define TRI_WAVE 	0  		//输出三角波
#define SIN_WAVE 	1		//输出正弦波
#define SQU_WAVE 	2		//输出方波



#define DDS_FSY1_Pin GPIO_Pin_2
#define DDS_FSY1_GPIO_Port GPIOC
#define DDS_CLK_Pin GPIO_Pin_4
#define DDS_CLK_GPIO_Port GPIOC
#define DDS_DAT_Pin GPIO_Pin_5
#define DDS_DAT_GPIO_Port GPIOC
#define DDS_FSY0_Pin GPIO_Pin_6
#define DDS_FSY0_GPIO_Port GPIOC



void AD9833_WaveSeting(double frequence,unsigned int frequence_SFR,unsigned int WaveMode,unsigned int Phase, unsigned char channel );
void AD9833_WaveSeting_Double(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase);
void MX_GPIO_Init(void);
#endif



