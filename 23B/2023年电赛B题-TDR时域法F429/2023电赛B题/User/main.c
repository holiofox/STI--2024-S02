/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 工程模版
  *********************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
/*******************************************************************************
 * 包含的头文件
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "./rcc/bsp_clkconfig.h"
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* 开发板硬件bsp头文件 */
#include "./usart/bsp_debug_usart.h"
#include "./lcd/bsp_lcd.h"
#include "./touch/bsp_i2c_touch.h"
#include "./touch/gt9xx.h"
#include "./sdram/bsp_sdram.h"
#include "./tdc-gp22/tdc.h"
#include "./Measure/all.h"
#include "./adc/bsp_adc.h"
#include "./rcc/bsp_clkconfig.h"
#include "./exti/bsp_exti.h"
#include "./dwt_delay/core_delay.h"   
#include "./Measure/L.h"
#include "./Measure/R.h"
#include "./Measure/C.h"
/* STemWIN头文件 */
#include "GUI.h"
#include "DIALOG.h"
#include "MainTask.h"

/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
/* 创建任务句柄 */
static TaskHandle_t AppTaskCraete_Handle = NULL;
/* Touch任务句柄 */
static TaskHandle_t Touch_Task_Handle = NULL;
/* GUI任务句柄 */
static TaskHandle_t GUI_Task_Handle = NULL;
/* R任务句柄 */
static TaskHandle_t R_Task_Handle = NULL;
/* L任务句柄 */
static TaskHandle_t L_Task_Handle = NULL;
/* C任务句柄 */
static TaskHandle_t C_Task_Handle = NULL;
/* OP任务句柄 */
static TaskHandle_t OP_Task_Handle = NULL;
/* TR任务句柄 */
static TaskHandle_t TR_Task_Handle = NULL;
/* TL任务句柄 */
static TaskHandle_t TL_Task_Handle = NULL;
/* TC任务句柄 */
static TaskHandle_t TC_Task_Handle = NULL;
/********************************** 内核对象句柄 *********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */
 SemaphoreHandle_t BinarySem_Handle_R =NULL;
 SemaphoreHandle_t BinarySem_Handle_L =NULL;
 SemaphoreHandle_t BinarySem_Handle_C =NULL;
 SemaphoreHandle_t BinarySem_Handle_C_START =NULL;
 SemaphoreHandle_t BinarySem_Handle_OP =NULL;//负载开路,给屏幕
 SemaphoreHandle_t BinarySem_Handle_TR =NULL;//负载电阻,给屏幕
 SemaphoreHandle_t BinarySem_Handle_TC =NULL;//负载电容,给屏幕
 SemaphoreHandle_t BinarySem_Handle_TL =NULL;//测量长度,给屏幕
/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */

#define MAX_R 100
#define MIN_C 30
/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void AppTaskCreate(void);

static void Touch_Task(void* parameter);
static void GUI_Task(void* parameter);
static void R_Task(void* parameter);
static void L_Task(void* parameter);
static void C_Task(void* parameter);
static void OP_Task(void* parameter);
static void TR_Task(void* parameter);
static void TL_Task(void* parameter);
static void TC_Task(void* parameter);

static void BSP_Init(void);

uint32_t test1 =1;
float f,r,l,c,g_base_c,g_l;
char buffer_L[40],buffer_CR[40];

extern GUI_CONST_STORAGE GUI_FONT GUI_FontMicrosoftJhengHeiUI20;
extern WM_HWIN hFrame;
extern uint16_t tag_C;
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	
	/* 开发板硬件初始化 */
	BSP_Init();
	
	xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,/* 任务入口函数 */
											 (const char*    )"AppTaskCreate",/* 任务名称 */
											 (uint16_t       )512,					/* 任务栈大小 */
											 (void*          )NULL,					/* 任务入口函数参数 */
											 (UBaseType_t    )1,						/* 任务的优先级 */
											 (TaskHandle_t   )&AppTaskCraete_Handle);/* 任务控制块指针 */
	/* 启动任务调度 */
	if(pdPASS == xReturn)
		vTaskStartScheduler();/* 启动任务，开启调度 */
	else
		return -1;
	
	while(1);/* 正常不会执行到这里 */
}

/**
  * @brief 任务创建函数
  * @note 为了方便管理，所有的任务创建都放在这个函数里面
  * @param 无
  * @retval 无
  */
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	
	taskENTER_CRITICAL();//进入临界区
	
  /* 创建 BinarySem */
	  BinarySem_Handle_R = xSemaphoreCreateBinary();	 
  if(NULL != BinarySem_Handle_R)
    printf("BinarySem_Handle_R二值信号量创建成功!\r\n");
	
	  BinarySem_Handle_L = xSemaphoreCreateBinary();	 
  if(NULL != BinarySem_Handle_L)
    printf("BinarySem_Handle_L二值信号量创建成功!\r\n");
	
  BinarySem_Handle_C = xSemaphoreCreateBinary();	 
  if(NULL != BinarySem_Handle_C)
    printf("BinarySem_Handle_C二值信号量创建成功!\r\n");
	
		  BinarySem_Handle_C_START = xSemaphoreCreateBinary();	 
  if(NULL != BinarySem_Handle_C_START)
    printf("BinarySem_Handle_C_START二值信号量创建成功!\r\n");
	
	BinarySem_Handle_OP = xSemaphoreCreateBinary();	 
  if(NULL != BinarySem_Handle_OP)
  printf("BinarySem_Handle_OP二值信号量创建成功!\r\n");
	
	BinarySem_Handle_TR = xSemaphoreCreateBinary();	 
  if(NULL != BinarySem_Handle_TR)
    printf("BinarySem_Handle_TR二值信号量创建成功!\r\n");
	
		BinarySem_Handle_TL = xSemaphoreCreateBinary();	 
  if(NULL != BinarySem_Handle_TL)
    printf("BinarySem_Handle_TL二值信号量创建成功!\r\n");
	
	BinarySem_Handle_TC = xSemaphoreCreateBinary();	 
  if(NULL != BinarySem_Handle_TC)
    printf("BinarySem_Handle_TC二值信号量创建成功!\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t)Touch_Task,/* 任务入口函数 */
											 (const char*      )"Touch_Task",/* 任务名称 */
											 (uint16_t         )256,     /* 任务栈大小 */
											 (void*            )NULL,    /* 任务入口函数参数 */
											 (UBaseType_t      )7,       /* 任务的优先级 */
											 (TaskHandle_t     )&Touch_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建Touch_Task任务成功！\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t)GUI_Task,/* 任务入口函数 */
											 (const char*      )"GUI_Task",/* 任务名称 */
											 (uint16_t         )1024,      /* 任务栈大小 */
											 (void*            )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t      )6,         /* 任务的优先级 */
											 (TaskHandle_t     )&GUI_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建GUI_Task任务成功！\r\n");
	
		xReturn = xTaskCreate((TaskFunction_t)R_Task,/* 任务入口函数 */
											 (const char*      )"R_Task",/* 任务名称 */
											 (uint16_t         )1024,      /* 任务栈大小 */
											 (void*            )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t      )5,         /* 任务的优先级 */
											 (TaskHandle_t     )&R_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建R_Task任务成功！\r\n");
	
		xReturn = xTaskCreate((TaskFunction_t)L_Task,/* 任务入口函数 */
											 (const char*      )"L_Task",/* 任务名称 */
											 (uint16_t         )1024,      /* 任务栈大小 */
											 (void*            )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t      )5,         /* 任务的优先级 */
											 (TaskHandle_t     )&L_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建L_Task任务成功！\r\n");
	
		xReturn = xTaskCreate((TaskFunction_t)C_Task,/* 任务入口函数 */
											 (const char*      )"C_Task",/* 任务名称 */
											 (uint16_t         )1024,      /* 任务栈大小 */
											 (void*            )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t      )5,         /* 任务的优先级 */
											 (TaskHandle_t     )&C_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建C_Task任务成功！\r\n");
	
			xReturn = xTaskCreate((TaskFunction_t)OP_Task,/* 任务入口函数 */
											 (const char*      )"OP_Task",/* 任务名称 */
											 (uint16_t         )1024,      /* 任务栈大小 */
											 (void*            )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t      )3,         /* 任务的优先级 */
											 (TaskHandle_t     )&OP_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建OP_Task任务成功！\r\n");
	
			xReturn = xTaskCreate((TaskFunction_t)TR_Task,/* 任务入口函数 */
											 (const char*      )"TR_Task",/* 任务名称 */
											 (uint16_t         )1024,      /* 任务栈大小 */
											 (void*            )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t      )3,         /* 任务的优先级 */
											 (TaskHandle_t     )&TR_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建TR_Task任务成功！\r\n");
	
			xReturn = xTaskCreate((TaskFunction_t)TC_Task,/* 任务入口函数 */
											 (const char*      )"TC_Task",/* 任务名称 */
											 (uint16_t         )1024,      /* 任务栈大小 */
											 (void*            )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t      )3,         /* 任务的优先级 */
											 (TaskHandle_t     )&TC_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建TC_Task任务成功！\r\n");
	
			xReturn = xTaskCreate((TaskFunction_t)TL_Task,/* 任务入口函数 */
											 (const char*      )"TL_Task",/* 任务名称 */
											 (uint16_t         )1024,      /* 任务栈大小 */
											 (void*            )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t      )3,         /* 任务的优先级 */
											 (TaskHandle_t     )&TL_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建TL_Task任务成功！\r\n");

	vTaskDelete(AppTaskCraete_Handle);//删除AppTaskCreate任务
	
	taskEXIT_CRITICAL();//退出临界区
}

/**
  * @brief 触摸检测任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
static void Touch_Task(void* parameter)
{
	while(1)
	{
		GT9xx_GetOnePiont();
		vTaskDelay(20);
	}
}

/**
  * @brief GUI任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
static void GUI_Task(void* parameter)
{
	/* 初始化GUI */
	GUI_Init();
	/* 开启多缓冲 */
	WM_MULTIBUF_Enable(1);
	
	while(1)
	{
		MainTask();
	}
}

/**
  * @brief 电阻测量任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
static void R_Task(void* parameter)
{
	WM_HWIN  hItem;
BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	float r;
  while (1)
  {	
		printf("R_Task执行!\r\n");
    //获取二值信号量 xSemaphore,没获取到则一直等待
		xReturn = xSemaphoreTake(BinarySem_Handle_R,/* 二值信号量句柄 */
                              portMAX_DELAY); /* 等待时间 */
    if(pdTRUE == xReturn){
			
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_1);
    TEXT_SetText(hItem, "\xE6\xAD\xA3\xE5\x9C\xA8\xE6\xA3\x80\xE6\xB5\x8B");
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);
			
		printf("R_Task启动!\r\n");
		memset(buffer_CR, 0, sizeof(buffer_CR));
    // 先尝试检测电阻
    switchX('R');
    vTaskDelay(5);
    r = getR();
    switchX('N');

    r-= getBaseR(g_l);//减去负载带的电阻
    if (r <= MAX_R && r >= -MAX_R) {
        // LCD屏显示
			    snprintf(buffer_CR, sizeof(buffer_CR), "%.3f", r);
		snprintf(buffer_CR + strlen(buffer_CR), sizeof(buffer_CR) - strlen(buffer_CR), "\xCE\xA9");
      xReturn = xSemaphoreGive( BinarySem_Handle_TR );//给出二值信号量,屏幕显示
      if( xReturn == pdTRUE )
        printf("BinarySem_Handle_TR二值信号量释放成功!\r\n");
      else
        printf("BinarySem_Handle_TR二值信号量释放失败!\r\n");
    }
		else {
		    xReturn = xSemaphoreGive( BinarySem_Handle_C_START );//给出二值信号量，测量电容
      if( xReturn == pdTRUE )
        printf("BinarySem_Handle_C_START二值信号量释放成功!\r\n");
      else
        printf("BinarySem_Handle_C_START二值信号量释放失败!\r\n");
		}
		}
//测量R，如果大于某个阈值就测量C
  }
}
/**
  * @brief 电容测量任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
static void C_Task(void* parameter)
{
	
	WM_HWIN  hItem;
 BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	
  while (1)
  {
		printf("C_Task执行!\r\n");
		xReturn = xSemaphoreTake(BinarySem_Handle_C_START,/* 二值信号量句柄 */
                              portMAX_DELAY); /* 等待时间 */
				if(pdTRUE == xReturn){
					
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_1);
    TEXT_SetText(hItem, "\xE6\xAD\xA3\xE5\x9C\xA8\xE6\xA3\x80\xE6\xB5\x8B");
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);
					
					printf("C_Task启动!\r\n");
	memset(buffer_CR, 0, sizeof(buffer_CR));
		 // 再尝试检测电容
 switchX('C');
  vTaskDelay(5);
 c = getC();
 switchX('N');
 c -= g_base_c;
 snprintf(buffer_CR, sizeof(buffer_CR), "%.3f", c);
 snprintf(buffer_CR + strlen(buffer_CR), sizeof(buffer_CR) - strlen(buffer_CR), "pF");
 if (c >= MIN_C) {
     // LCD屏显示
      xReturn = xSemaphoreGive( BinarySem_Handle_TC );//给出二值信号量,屏幕显示
      if( xReturn == pdTRUE )
        printf("BinarySem_Handle_TC二值信号量释放成功!\r\n");
      else
        printf("BinarySem_Handle_TC二值信号量释放失败!\r\n");

 }
 		else {
		    xReturn = xSemaphoreGive( BinarySem_Handle_OP );//给出二值信号量，开路
      if( xReturn == pdTRUE )
        printf("BinarySem_Handle_OP二值信号量释放成功!\r\n");
      else
        printf("BinarySem_Handle_OP二值信号量释放失败!\r\n");
		}
		}


  }
}
/**
  * @brief 长度测量任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
static void L_Task(void* parameter)
{
	
		WM_HWIN  hItem;
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  while (1)
  {
		printf("L_Task执行!\r\n");
    //获取二值信号量 xSemaphore,没获取到则一直等待
		xReturn = xSemaphoreTake(BinarySem_Handle_L,/* 二值信号量句柄 */
                              portMAX_DELAY); /* 等待时间 */
    if(pdTRUE == xReturn){
			
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_1);
    TEXT_SetText(hItem, "\xE6\xAD\xA3\xE5\x9C\xA8\xE6\xA3\x80\xE6\xB5\x8B");
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);
			
			printf("L_Task启动!\r\n");
			  // 检测长度
		memset(buffer_L, 0, sizeof(buffer_L));
  switchX('L');
  vTaskDelay(5);
			printf("测量开始\r\n");
  l = getL();
			printf("测量结束\r\n");
  switchX('N');
  snprintf(buffer_L, sizeof(buffer_L), "%.3f", l);
	snprintf(buffer_L + strlen(buffer_L), sizeof(buffer_L) - strlen(buffer_L), "cm");
  g_l = l;

  // LCD屏显示
      xReturn = xSemaphoreGive( BinarySem_Handle_TL );//给出二值信号量,屏幕显示
      if( xReturn == pdTRUE )
        printf("BinarySem_Handle_TL二值信号量释放成功!\r\n");
      else
        printf("BinarySem_Handle_TL二值信号量释放失败!\r\n");


  // 测试下电容，给后面测电容作为基准
  switchX('C');
  vTaskDelay(5);
  g_base_c = getC();
  switchX('N');
		}
  }
}

static void OP_Task(void* parameter)
{ 
	WM_HWIN  hItem;
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  while (1)
  {
		printf("OP_Task执行!\r\n");
		//**************************按下“负载检测”但是开路**************************************
		xReturn = xSemaphoreTake(BinarySem_Handle_OP,/* 二值信号量句柄 */
                              portMAX_DELAY); /* 等待时间 */
    if(pdTRUE == xReturn){ 
			printf("OP_Task启动!\r\n");
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_5);
    TEXT_SetText(hItem, "\xE5\xBC\x80\xE8\xB7\xAF");
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);
			
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_7);
    TEXT_SetText(hItem,"");
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);
			
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_1);
    TEXT_SetText(hItem, "\xE7\xBB\x93\xE6\x9E\x9C\xE4\xBF\x9D\xE6\x8C\x81");
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);	
		}
  }
}

static void TR_Task(void* parameter)
{ 
	WM_HWIN  hItem;
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  while (1)
  {
		printf("TR_Task执行!\r\n");
		//**************************按下“负载检测”结果为电阻**************************************
				xReturn = xSemaphoreTake(BinarySem_Handle_TR,/* 二值信号量句柄 */
                              portMAX_DELAY); /* 等待时间 */
    if(pdTRUE == xReturn){ 
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_5);
    TEXT_SetText(hItem, "\xE7\x94\xB5\xE9\x98\xBB");
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);
			
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_7);
    TEXT_SetText(hItem,buffer_CR);
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);	
			
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_1);
    TEXT_SetText(hItem, "\xE7\xBB\x93\xE6\x9E\x9C\xE4\xBF\x9D\xE6\x8C\x81");
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);	
		}
  }
}

static void TL_Task(void* parameter)
{ 
	WM_HWIN  hItem;
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  while (1)
  {
		printf("TL_Task执行!\r\n");
		//**************************按下“长度检测”************************************************
				xReturn = xSemaphoreTake(BinarySem_Handle_TL,/* 二值信号量句柄 */
                              portMAX_DELAY); /* 等待时间 */
    if(pdTRUE == xReturn){ 
				printf("TL_Task启动!\r\n");
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_5);
    TEXT_SetText(hItem, "\xE5\xBC\x80\xE8\xB7\xAF");
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);
			
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_3);
    TEXT_SetText(hItem,buffer_L);
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);	
			
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_7);
    TEXT_SetText(hItem,"");
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);
			
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_1);
    TEXT_SetText(hItem, "\xE7\xBB\x93\xE6\x9E\x9C\xE4\xBF\x9D\xE6\x8C\x81");
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);	
		}

  }
}

static void TC_Task(void* parameter)
{ 
	WM_HWIN  hItem;
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  while (1)
  {
		printf("TC_Task执行!\r\n");
		//**************************按下“负载检测”结果为电容**************************************
		xReturn = xSemaphoreTake(BinarySem_Handle_TC,/* 二值信号量句柄 */
                              portMAX_DELAY); /* 等待时间 */
    if(pdTRUE == xReturn){ 
			printf("TC_Task启动!\r\n");
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_5);
    TEXT_SetText(hItem, "\xE7\x94\xB5\xE5\xAE\xB9");
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);

		hItem = WM_GetDialogItem(hFrame, ID_TEXT_7);
    TEXT_SetText(hItem,buffer_CR);
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);	
			
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_1);
    TEXT_SetText(hItem, "\xE7\xBB\x93\xE6\x9E\x9C\xE4\xBF\x9D\xE6\x8C\x81");
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);	
		}
  }
}

/**
  * @brief 板级外设初始化
  * @note 所有板子上的初始化均可放在这个函数里面
  * @param 无
  * @retval 无
  */
static void BSP_Init(void)
{
	
	/* CRC和emWin没有关系，只是他们为了库的保护而做的
   * 这样STemWin的库只能用在ST的芯片上面，别的芯片是无法使用的。
   */
		// 使用HSE，配置系统时钟为180M
	HSE_SetSysClock(25, 360, 2, 7);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	
	/*
	 * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
	 * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
	 * 都统一用同一个优先级分组，千万不要再分组，切记。
	 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/* DWT初始化	*/
	DWT_Init();
	/* 串口初始化	*/
	Debug_USART_Config();
	/* 触摸屏初始化 */
	GTP_Init_Panel();	
	/* SDRAM初始化 */
  SDRAM_Init();
	/* LCD初始化 */
	LCD_Init();
		/* 继电器初始化 */
  switchX_GPIO_Init();
/* TDC初始化 */
  TDC_Init();
/* ADC初始化 */
	Rheostat_Init();

	delay(10);  
	//-----------------------长度测试-----------
//	test1 = TDC_Test();
//	switchX('L');
//	delay(10);
//	calTTask();
  //-----------电容测试-------------
//		switchX('C');
//		delay(10);  
//		c=getC();
//		printf("电容c = %f\n", c);
////--------------电阻测试----------------
//		switchX('R');
//		r = getR();
//		printf("电阻r = %f\n", r);
}
/*********************************************END OF FILE**********************/


