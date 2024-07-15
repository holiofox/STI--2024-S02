#ifndef __ALL_H
#define	__ALL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
//*********** 继电器开始 ****************
#define SWITCH_R_GPIO_Port              GPIOG  
#define SWITCH_R_Pin                    GPIO_Pin_9
#define SWITCH_R_GPIO_CLK               RCC_AHB1Periph_GPIOG


#define SWITCH_C_GPIO_Port              GPIOB
#define SWITCH_C_Pin                    GPIO_Pin_13
#define SWITCH_C_GPIO_CLK               RCC_AHB1Periph_GPIOB

#define SWITCH_L_GPIO_Port              GPIOI
#define SWITCH_L_Pin                    GPIO_Pin_8
#define SWITCH_L_GPIO_CLK               RCC_AHB1Periph_GPIOI
//*********** 继电器结束 ****************
void switchX(char x);
void switchX_GPIO_Init(void);
#endif /* __ALL_H */