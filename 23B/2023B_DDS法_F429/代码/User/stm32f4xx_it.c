/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "./usart/bsp_debug_usart.h"
#include "./exti/bsp_exti.h"
#include "./dwt_delay/core_delay.h" 
#include "./Measure/C.h"
#include "./Measure/all.h"
#include "./AD9959/AD9959.h"
#include "./tim/bsp_basic_tim.h"
#include "./adc/bsp_adc.h"
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup FMC_SDRAM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t Sweep_count = 0;
uint8_t SWP_TAG=0;
uint32_t FREQ = 2000000;
uint32_t STEP_FREQ = 1000000;
uint32_t ready = 0;
uint32_t last_tick = 0;
float freq = 0.0f; // NE555震荡频率
uint8_t i = 0;
/* Private function prototypes -----------------------------------------------*/
extern void LTDC_ISR_Handler(void);
extern void DMA2D_ISR_Handler(void);
extern SemaphoreHandle_t BinarySem_Handle_C;
extern SemaphoreHandle_t BinarySem_Handle_Tim;
extern __IO uint16_t ADC_ConvertedValue;
extern uint32_t AMP[2000];
extern uint32_t AMP_NEXT[4000];
extern uint16_t L_tag;
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
	printf("hardfault!!!\n");
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

///**
//  * @brief  This function handles SVCall exception.
//  * @param  None
//  * @retval None
//  */
//void SVC_Handler(void)
//{}

///**
//  * @brief  This function handles PendSV_Handler exception.
//  * @param  None
//  * @retval None
//  */
//void PendSV_Handler(void)
//{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern void xPortSysTickHandler(void);

void SysTick_Handler(void)
{
	#if (INCLUDE_xTaskGetSchedulerState == 1 )
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
	#endif /* INCLUDE_xTaskGetSchedulerState */
	xPortSysTickHandler();
	#if (INCLUDE_xTaskGetSchedulerState == 1 )
	}
	#endif /* INCLUDE_xTaskGetSchedulerState */
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).                         */
/******************************************************************************/
/**
  * @brief  This function handles LTDC global interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_IRQHandler(void)
{
	uint32_t ulReturn;
	/* 进入临界段 */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	/* 行中断处理 */
  LTDC_ISR_Handler();
	/* 退出临界段 */
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);
}

/**
  * @brief  This function handles DMA2D global interrupt request.
  * @param  None
  * @retval None
  */
void DMA2D_IRQHandler(void)
{
	uint32_t ulReturn;
	/* 进入临界段 */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	
  DMA2D_ISR_Handler();
	/* 退出临界段 */
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);
}
/*********************************************************************************
  * @ 函数名  ： KEY1_IRQHandler
  * @ 功能说明： 中断服务函数
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  ********************************************************************************/
void KEY1_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken;
  //确保是否产生了EXTI Line中断
  uint32_t ulReturn;
	  /* 进入临界段，临界段可以嵌套 */
  ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
 uint32_t tick = Get_Tick();

 // 第一次触发
 if (ready == 0) {
     ready = 1;
     last_tick = tick;
	 			 		  		//清除中断标志位
	EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);  
  /* 退出临界段 */
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
	 return;
 }

 // 计算两次中断间系统时钟数的差值
 uint32_t tick_diff = Tick_Diff(last_tick, tick);

 // 计算外部信号的频率
 freq = (float) SystemCoreClock / (float) tick_diff;
 push(freq);
 last_tick = tick;
  i++;

		if(i >= 200) {
			i = 0;
				 // 处理完毕，禁用进一步的中断
        EXTI->IMR &= ~EXTI_Line13;  // 禁用中断线路
        NVIC_DisableIRQ(EXTI15_10_IRQn); // 禁用NVIC中的中断
     //释放二值信号量，发送接收到新数据标志，供前台程序查询
   xSemaphoreGiveFromISR(BinarySem_Handle_C,&
   pxHigherPriorityTaskWoken);

		}
			 		  		//清除中断标志位
	EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);  
  /* 退出临界段 */
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
 portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);		
	}

}

void  BASIC_TIM_IRQHandler (void)
{
	  BaseType_t pxHigherPriorityTaskWoken;
	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
	{
		
		if(L_tag == 0)//粗测（分辨率10KHz）
		{
		Write_frequence(0,FREQ);
		Start_ADC_Conversion();
		FREQ+=STEP_FREQ;
	if(SWP_TAG == 0)
	{
		AMP[Sweep_count] = (uint32_t)ADC_ConvertedValue;

		if (Sweep_count>=5&&(AMP[Sweep_count]>=AMP[Sweep_count-1])&&(AMP[Sweep_count-1]>=AMP[Sweep_count-2])&&(AMP[Sweep_count-2]>=AMP[Sweep_count-3]))
		SWP_TAG = 1;
		else
		Sweep_count++;
		TIM_ClearITPendingBit(BASIC_TIM , TIM_IT_Update); 
	}		
	else {
		SWP_TAG = Sweep_count;
		Sweep_count = 0;
		  //给出二值信号量 ，发送接收到新数据标志，供前台程序查询
			// 使能定时器
	TIM_ClearITPendingBit(BASIC_TIM , TIM_IT_Update); 
	TIM_Cmd(BASIC_TIM, DISABLE);	
  xSemaphoreGiveFromISR(BinarySem_Handle_Tim,&pxHigherPriorityTaskWoken);	//释放二值信号量
  //如果需要的话进行一次任务切换，系统会判断是否需要进行切换
  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
	}
	
	}
		else if (L_tag ==1)//精测(分辨率100Hz)
		{
		Write_frequence(0,FREQ);
		Start_ADC_Conversion();
		FREQ+=STEP_FREQ;
	if(SWP_TAG == 0)
	{
		AMP_NEXT[Sweep_count] = (uint32_t)ADC_ConvertedValue;

		if ((AMP_NEXT[Sweep_count]>=AMP_NEXT[Sweep_count-1])&&(AMP_NEXT[Sweep_count-1]>=AMP_NEXT[Sweep_count-2])&&(AMP_NEXT[Sweep_count-2]>=AMP_NEXT[Sweep_count-3]))
		SWP_TAG = 1;
		else
		Sweep_count++;
		TIM_ClearITPendingBit(BASIC_TIM , TIM_IT_Update); 
	}		
	else {
		SWP_TAG = Sweep_count;
		Sweep_count = 0;
		  //给出二值信号量 ，发送接收到新数据标志，供前台程序查询
			// 使能定时器
	TIM_ClearITPendingBit(BASIC_TIM , TIM_IT_Update); 
	TIM_Cmd(BASIC_TIM, DISABLE);	
  xSemaphoreGiveFromISR(BinarySem_Handle_Tim,&pxHigherPriorityTaskWoken);	//释放二值信号量
  //如果需要的话进行一次任务切换，系统会判断是否需要进行切换
  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
	}
	
		}

	}
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
