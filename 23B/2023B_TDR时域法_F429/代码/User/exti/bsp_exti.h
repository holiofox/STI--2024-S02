#ifndef __EXTI_H
#define	__EXTI_H

#include "stm32f4xx.h"

//Òý½Å¶¨Òå
/*******************************************************/
#define KEY1_INT_GPIO_PORT                GPIOC
#define KEY1_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOC
#define KEY1_INT_GPIO_PIN                 GPIO_Pin_6
#define KEY1_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOC
#define KEY1_INT_EXTI_PINSOURCE           EXTI_PinSource6
#define KEY1_INT_EXTI_LINE                EXTI_Line6
#define KEY1_INT_EXTI_IRQ                 EXTI9_5_IRQn

#define KEY1_IRQHandler                   EXTI9_5_IRQHandler


/*******************************************************/


void EXTI_Config(void);

#endif /* __EXTI_H */
