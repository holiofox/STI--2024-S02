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
/* FreeRTOSͷ�ļ� */
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
float freq = 0.0f; // NE555��Ƶ��
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
	/* �����ٽ�� */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	/* ���жϴ��� */
  LTDC_ISR_Handler();
	/* �˳��ٽ�� */
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
	/* �����ٽ�� */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	
  DMA2D_ISR_Handler();
	/* �˳��ٽ�� */
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);
}
/*********************************************************************************
  * @ ������  �� KEY1_IRQHandler
  * @ ����˵���� �жϷ�����
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  ********************************************************************************/
void KEY1_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken;
  //ȷ���Ƿ������EXTI Line�ж�
  uint32_t ulReturn;
	  /* �����ٽ�Σ��ٽ�ο���Ƕ�� */
  ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
 uint32_t tick = Get_Tick();

 // ��һ�δ���
 if (ready == 0) {
     ready = 1;
     last_tick = tick;
	 			 		  		//����жϱ�־λ
	EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);  
  /* �˳��ٽ�� */
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
	 return;
 }

 // ���������жϼ�ϵͳʱ�����Ĳ�ֵ
 uint32_t tick_diff = Tick_Diff(last_tick, tick);

 // �����ⲿ�źŵ�Ƶ��
 freq = (float) SystemCoreClock / (float) tick_diff;
 push(freq);
 last_tick = tick;
  i++;

		if(i >= 200) {
			i = 0;
				 // ������ϣ����ý�һ�����ж�
        EXTI->IMR &= ~EXTI_Line13;  // �����ж���·
        NVIC_DisableIRQ(EXTI15_10_IRQn); // ����NVIC�е��ж�
     //�ͷŶ�ֵ�ź��������ͽ��յ������ݱ�־����ǰ̨�����ѯ
   xSemaphoreGiveFromISR(BinarySem_Handle_C,&
   pxHigherPriorityTaskWoken);

		}
			 		  		//����жϱ�־λ
	EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);  
  /* �˳��ٽ�� */
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
 portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);		
	}

}

void  BASIC_TIM_IRQHandler (void)
{
	  BaseType_t pxHigherPriorityTaskWoken;
	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
	{
		
		if(L_tag == 0)//�ֲ⣨�ֱ���10KHz��
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
		  //������ֵ�ź��� �����ͽ��յ������ݱ�־����ǰ̨�����ѯ
			// ʹ�ܶ�ʱ��
	TIM_ClearITPendingBit(BASIC_TIM , TIM_IT_Update); 
	TIM_Cmd(BASIC_TIM, DISABLE);	
  xSemaphoreGiveFromISR(BinarySem_Handle_Tim,&pxHigherPriorityTaskWoken);	//�ͷŶ�ֵ�ź���
  //�����Ҫ�Ļ�����һ�������л���ϵͳ���ж��Ƿ���Ҫ�����л�
  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
	}
	
	}
		else if (L_tag ==1)//����(�ֱ���100Hz)
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
		  //������ֵ�ź��� �����ͽ��յ������ݱ�־����ǰ̨�����ѯ
			// ʹ�ܶ�ʱ��
	TIM_ClearITPendingBit(BASIC_TIM , TIM_IT_Update); 
	TIM_Cmd(BASIC_TIM, DISABLE);	
  xSemaphoreGiveFromISR(BinarySem_Handle_Tim,&pxHigherPriorityTaskWoken);	//�ͷŶ�ֵ�ź���
  //�����Ҫ�Ļ�����һ�������л���ϵͳ���ж��Ƿ���Ҫ�����л�
  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
	}
	
		}

	}
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
