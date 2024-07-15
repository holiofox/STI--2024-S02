#include "./Measure/L.h"
#include "./tim/bsp_basic_tim.h"
#include "./AD9959/AD9959.h"
#include "./adc/bsp_adc.h"
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

extern SemaphoreHandle_t BinarySem_Handle_Tim;
extern int findMinIndex(uint32_t arr[], int start, int end);
extern uint16_t L_tag;
extern uint32_t AMP[2000];
extern uint32_t AMP_NEXT[4000];
extern uint32_t MAX_FREQ,MIN_FREQ,index1,index2,index3;
extern uint32_t FREQ;
extern uint32_t STEP_FREQ;
extern uint8_t SWP_TAG;
uint32_t temp_freq;
extern __IO uint16_t ADC_ConvertedValue;

float getL() {
	uint32_t xiabiao;
		BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	 float L;
		SWP_TAG = 0;
		L_tag = 0;//粗测表示(分辨率400k)
		STEP_FREQ = 200000;
		FREQ = 2000000;
		TIMx_Configuration();
	
		//等第一次粗测的结果
		xReturn = xSemaphoreTake(BinarySem_Handle_Tim,/* 二值信号量句柄 */
                              portMAX_DELAY); /* 等待时间 */
		xiabiao = findMinIndex(AMP,2,SWP_TAG);
		FREQ = 2000000+(xiabiao-1)*200000;
		temp_freq = FREQ;
		L_tag = 1;//精测表示（分辨率2000Hz）
		STEP_FREQ /=1000;
		SWP_TAG = 0;
		TIMx_Configuration();
	
				//等第二次测的结果
		xReturn = xSemaphoreTake(BinarySem_Handle_Tim,/* 二值信号量句柄 */
                              portMAX_DELAY); /* 等待时间 */
		xiabiao = findMinIndex(AMP_NEXT,0,SWP_TAG);
		FREQ = temp_freq+xiabiao*210;
		L = Ce8/4/FREQ;
		Write_frequence(0,0);
		return L;
}


