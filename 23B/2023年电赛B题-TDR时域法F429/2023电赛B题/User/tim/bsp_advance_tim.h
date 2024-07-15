#ifndef __GENERAL_TIM_H
#define	__GENERAL_TIM_H

#include "stm32f4xx.h"


/* 高级控制定时器PWM输入捕获 */
/* PWM输入捕获引脚 */
#define ADVANCE_ICPWM_PIN             GPIO_Pin_6            
#define ADVANCE_ICPWM_GPIO_PORT       GPIOC                      
#define ADVANCE_ICPWM_GPIO_CLK        RCC_AHB1Periph_GPIOC
#define ADVANCE_ICPWM_PINSOURCE				GPIO_PinSource6
#define ADVANCE_ICPWM_AF							GPIO_AF_TIM8
#define ADVANCE_IC1PWM_CHANNEL        TIM_Channel_1
#define ADVANCE_IC2PWM_CHANNEL        TIM_Channel_2

/* 高级控制定时器 */
#define ADVANCE_TIM           		    TIM8
#define ADVANCE_TIM_CLK       		    RCC_APB2Periph_TIM8

/* 捕获/比较中断 */
#define ADVANCE_TIM_IRQn					    TIM8_CC_IRQn
#define ADVANCE_TIM_IRQHandler        TIM8_CC_IRQHandler

void Stop_Measurement(void);
void Start_Measurement(void);
void TIMx_Configuration(void);

#endif /* __GENERAL_TIM_H */

