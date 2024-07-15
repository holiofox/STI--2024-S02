/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 ����ģ��
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
/*******************************************************************************
 * ������ͷ�ļ�
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "./rcc/bsp_clkconfig.h"
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* ������Ӳ��bspͷ�ļ� */
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
/* STemWINͷ�ļ� */
#include "GUI.h"
#include "DIALOG.h"
#include "MainTask.h"

/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
/* ���������� */
static TaskHandle_t AppTaskCraete_Handle = NULL;
/* Touch������ */
static TaskHandle_t Touch_Task_Handle = NULL;
/* GUI������ */
static TaskHandle_t GUI_Task_Handle = NULL;
/* R������ */
static TaskHandle_t R_Task_Handle = NULL;
/* L������ */
static TaskHandle_t L_Task_Handle = NULL;
/* C������ */
static TaskHandle_t C_Task_Handle = NULL;
/* OP������ */
static TaskHandle_t OP_Task_Handle = NULL;
/* TR������ */
static TaskHandle_t TR_Task_Handle = NULL;
/* TL������ */
static TaskHandle_t TL_Task_Handle = NULL;
/* TC������ */
static TaskHandle_t TC_Task_Handle = NULL;
/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */
 SemaphoreHandle_t BinarySem_Handle_R =NULL;
 SemaphoreHandle_t BinarySem_Handle_L =NULL;
 SemaphoreHandle_t BinarySem_Handle_C =NULL;
 SemaphoreHandle_t BinarySem_Handle_C_START =NULL;
 SemaphoreHandle_t BinarySem_Handle_OP =NULL;//���ؿ�·,����Ļ
 SemaphoreHandle_t BinarySem_Handle_TR =NULL;//���ص���,����Ļ
 SemaphoreHandle_t BinarySem_Handle_TC =NULL;//���ص���,����Ļ
 SemaphoreHandle_t BinarySem_Handle_TL =NULL;//��������,����Ļ
/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */

#define MAX_R 100
#define MIN_C 30
/*
*************************************************************************
*                             ��������
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
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	
	/* ������Ӳ����ʼ�� */
	BSP_Init();
	
	xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,/* ������ں��� */
											 (const char*    )"AppTaskCreate",/* �������� */
											 (uint16_t       )512,					/* ����ջ��С */
											 (void*          )NULL,					/* ������ں������� */
											 (UBaseType_t    )1,						/* ��������ȼ� */
											 (TaskHandle_t   )&AppTaskCraete_Handle);/* ������ƿ�ָ�� */
	/* ����������� */
	if(pdPASS == xReturn)
		vTaskStartScheduler();/* �������񣬿������� */
	else
		return -1;
	
	while(1);/* ��������ִ�е����� */
}

/**
  * @brief ���񴴽�����
  * @note Ϊ�˷���������е����񴴽������������������
  * @param ��
  * @retval ��
  */
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	
	taskENTER_CRITICAL();//�����ٽ���
	
  /* ���� BinarySem */
	  BinarySem_Handle_R = xSemaphoreCreateBinary();	 
  if(NULL != BinarySem_Handle_R)
    printf("BinarySem_Handle_R��ֵ�ź��������ɹ�!\r\n");
	
	  BinarySem_Handle_L = xSemaphoreCreateBinary();	 
  if(NULL != BinarySem_Handle_L)
    printf("BinarySem_Handle_L��ֵ�ź��������ɹ�!\r\n");
	
  BinarySem_Handle_C = xSemaphoreCreateBinary();	 
  if(NULL != BinarySem_Handle_C)
    printf("BinarySem_Handle_C��ֵ�ź��������ɹ�!\r\n");
	
		  BinarySem_Handle_C_START = xSemaphoreCreateBinary();	 
  if(NULL != BinarySem_Handle_C_START)
    printf("BinarySem_Handle_C_START��ֵ�ź��������ɹ�!\r\n");
	
	BinarySem_Handle_OP = xSemaphoreCreateBinary();	 
  if(NULL != BinarySem_Handle_OP)
  printf("BinarySem_Handle_OP��ֵ�ź��������ɹ�!\r\n");
	
	BinarySem_Handle_TR = xSemaphoreCreateBinary();	 
  if(NULL != BinarySem_Handle_TR)
    printf("BinarySem_Handle_TR��ֵ�ź��������ɹ�!\r\n");
	
		BinarySem_Handle_TL = xSemaphoreCreateBinary();	 
  if(NULL != BinarySem_Handle_TL)
    printf("BinarySem_Handle_TL��ֵ�ź��������ɹ�!\r\n");
	
	BinarySem_Handle_TC = xSemaphoreCreateBinary();	 
  if(NULL != BinarySem_Handle_TC)
    printf("BinarySem_Handle_TC��ֵ�ź��������ɹ�!\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t)Touch_Task,/* ������ں��� */
											 (const char*      )"Touch_Task",/* �������� */
											 (uint16_t         )256,     /* ����ջ��С */
											 (void*            )NULL,    /* ������ں������� */
											 (UBaseType_t      )7,       /* ��������ȼ� */
											 (TaskHandle_t     )&Touch_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����Touch_Task����ɹ���\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t)GUI_Task,/* ������ں��� */
											 (const char*      )"GUI_Task",/* �������� */
											 (uint16_t         )1024,      /* ����ջ��С */
											 (void*            )NULL,      /* ������ں������� */
											 (UBaseType_t      )6,         /* ��������ȼ� */
											 (TaskHandle_t     )&GUI_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����GUI_Task����ɹ���\r\n");
	
		xReturn = xTaskCreate((TaskFunction_t)R_Task,/* ������ں��� */
											 (const char*      )"R_Task",/* �������� */
											 (uint16_t         )1024,      /* ����ջ��С */
											 (void*            )NULL,      /* ������ں������� */
											 (UBaseType_t      )5,         /* ��������ȼ� */
											 (TaskHandle_t     )&R_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����R_Task����ɹ���\r\n");
	
		xReturn = xTaskCreate((TaskFunction_t)L_Task,/* ������ں��� */
											 (const char*      )"L_Task",/* �������� */
											 (uint16_t         )1024,      /* ����ջ��С */
											 (void*            )NULL,      /* ������ں������� */
											 (UBaseType_t      )5,         /* ��������ȼ� */
											 (TaskHandle_t     )&L_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����L_Task����ɹ���\r\n");
	
		xReturn = xTaskCreate((TaskFunction_t)C_Task,/* ������ں��� */
											 (const char*      )"C_Task",/* �������� */
											 (uint16_t         )1024,      /* ����ջ��С */
											 (void*            )NULL,      /* ������ں������� */
											 (UBaseType_t      )5,         /* ��������ȼ� */
											 (TaskHandle_t     )&C_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����C_Task����ɹ���\r\n");
	
			xReturn = xTaskCreate((TaskFunction_t)OP_Task,/* ������ں��� */
											 (const char*      )"OP_Task",/* �������� */
											 (uint16_t         )1024,      /* ����ջ��С */
											 (void*            )NULL,      /* ������ں������� */
											 (UBaseType_t      )3,         /* ��������ȼ� */
											 (TaskHandle_t     )&OP_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����OP_Task����ɹ���\r\n");
	
			xReturn = xTaskCreate((TaskFunction_t)TR_Task,/* ������ں��� */
											 (const char*      )"TR_Task",/* �������� */
											 (uint16_t         )1024,      /* ����ջ��С */
											 (void*            )NULL,      /* ������ں������� */
											 (UBaseType_t      )3,         /* ��������ȼ� */
											 (TaskHandle_t     )&TR_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����TR_Task����ɹ���\r\n");
	
			xReturn = xTaskCreate((TaskFunction_t)TC_Task,/* ������ں��� */
											 (const char*      )"TC_Task",/* �������� */
											 (uint16_t         )1024,      /* ����ջ��С */
											 (void*            )NULL,      /* ������ں������� */
											 (UBaseType_t      )3,         /* ��������ȼ� */
											 (TaskHandle_t     )&TC_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����TC_Task����ɹ���\r\n");
	
			xReturn = xTaskCreate((TaskFunction_t)TL_Task,/* ������ں��� */
											 (const char*      )"TL_Task",/* �������� */
											 (uint16_t         )1024,      /* ����ջ��С */
											 (void*            )NULL,      /* ������ں������� */
											 (UBaseType_t      )3,         /* ��������ȼ� */
											 (TaskHandle_t     )&TL_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����TL_Task����ɹ���\r\n");

	vTaskDelete(AppTaskCraete_Handle);//ɾ��AppTaskCreate����
	
	taskEXIT_CRITICAL();//�˳��ٽ���
}

/**
  * @brief ���������������
  * @note ��
  * @param ��
  * @retval ��
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
  * @brief GUI��������
  * @note ��
  * @param ��
  * @retval ��
  */
static void GUI_Task(void* parameter)
{
	/* ��ʼ��GUI */
	GUI_Init();
	/* �����໺�� */
	WM_MULTIBUF_Enable(1);
	
	while(1)
	{
		MainTask();
	}
}

/**
  * @brief ���������������
  * @note ��
  * @param ��
  * @retval ��
  */
static void R_Task(void* parameter)
{
	WM_HWIN  hItem;
BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	float r;
  while (1)
  {	
		printf("R_Taskִ��!\r\n");
    //��ȡ��ֵ�ź��� xSemaphore,û��ȡ����һֱ�ȴ�
		xReturn = xSemaphoreTake(BinarySem_Handle_R,/* ��ֵ�ź������ */
                              portMAX_DELAY); /* �ȴ�ʱ�� */
    if(pdTRUE == xReturn){
			
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_1);
    TEXT_SetText(hItem, "\xE6\xAD\xA3\xE5\x9C\xA8\xE6\xA3\x80\xE6\xB5\x8B");
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);
			
		printf("R_Task����!\r\n");
		memset(buffer_CR, 0, sizeof(buffer_CR));
    // �ȳ��Լ�����
    switchX('R');
    vTaskDelay(5);
    r = getR();
    switchX('N');

    r-= getBaseR(g_l);//��ȥ���ش��ĵ���
    if (r <= MAX_R && r >= -MAX_R) {
        // LCD����ʾ
			    snprintf(buffer_CR, sizeof(buffer_CR), "%.3f", r);
		snprintf(buffer_CR + strlen(buffer_CR), sizeof(buffer_CR) - strlen(buffer_CR), "\xCE\xA9");
      xReturn = xSemaphoreGive( BinarySem_Handle_TR );//������ֵ�ź���,��Ļ��ʾ
      if( xReturn == pdTRUE )
        printf("BinarySem_Handle_TR��ֵ�ź����ͷųɹ�!\r\n");
      else
        printf("BinarySem_Handle_TR��ֵ�ź����ͷ�ʧ��!\r\n");
    }
		else {
		    xReturn = xSemaphoreGive( BinarySem_Handle_C_START );//������ֵ�ź�������������
      if( xReturn == pdTRUE )
        printf("BinarySem_Handle_C_START��ֵ�ź����ͷųɹ�!\r\n");
      else
        printf("BinarySem_Handle_C_START��ֵ�ź����ͷ�ʧ��!\r\n");
		}
		}
//����R���������ĳ����ֵ�Ͳ���C
  }
}
/**
  * @brief ���ݲ�����������
  * @note ��
  * @param ��
  * @retval ��
  */
static void C_Task(void* parameter)
{
	
	WM_HWIN  hItem;
 BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	
  while (1)
  {
		printf("C_Taskִ��!\r\n");
		xReturn = xSemaphoreTake(BinarySem_Handle_C_START,/* ��ֵ�ź������ */
                              portMAX_DELAY); /* �ȴ�ʱ�� */
				if(pdTRUE == xReturn){
					
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_1);
    TEXT_SetText(hItem, "\xE6\xAD\xA3\xE5\x9C\xA8\xE6\xA3\x80\xE6\xB5\x8B");
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);
					
					printf("C_Task����!\r\n");
	memset(buffer_CR, 0, sizeof(buffer_CR));
		 // �ٳ��Լ�����
 switchX('C');
  vTaskDelay(5);
 c = getC();
 switchX('N');
 c -= g_base_c;
 snprintf(buffer_CR, sizeof(buffer_CR), "%.3f", c);
 snprintf(buffer_CR + strlen(buffer_CR), sizeof(buffer_CR) - strlen(buffer_CR), "pF");
 if (c >= MIN_C) {
     // LCD����ʾ
      xReturn = xSemaphoreGive( BinarySem_Handle_TC );//������ֵ�ź���,��Ļ��ʾ
      if( xReturn == pdTRUE )
        printf("BinarySem_Handle_TC��ֵ�ź����ͷųɹ�!\r\n");
      else
        printf("BinarySem_Handle_TC��ֵ�ź����ͷ�ʧ��!\r\n");

 }
 		else {
		    xReturn = xSemaphoreGive( BinarySem_Handle_OP );//������ֵ�ź�������·
      if( xReturn == pdTRUE )
        printf("BinarySem_Handle_OP��ֵ�ź����ͷųɹ�!\r\n");
      else
        printf("BinarySem_Handle_OP��ֵ�ź����ͷ�ʧ��!\r\n");
		}
		}


  }
}
/**
  * @brief ���Ȳ�����������
  * @note ��
  * @param ��
  * @retval ��
  */
static void L_Task(void* parameter)
{
	
		WM_HWIN  hItem;
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  while (1)
  {
		printf("L_Taskִ��!\r\n");
    //��ȡ��ֵ�ź��� xSemaphore,û��ȡ����һֱ�ȴ�
		xReturn = xSemaphoreTake(BinarySem_Handle_L,/* ��ֵ�ź������ */
                              portMAX_DELAY); /* �ȴ�ʱ�� */
    if(pdTRUE == xReturn){
			
		hItem = WM_GetDialogItem(hFrame, ID_TEXT_1);
    TEXT_SetText(hItem, "\xE6\xAD\xA3\xE5\x9C\xA8\xE6\xA3\x80\xE6\xB5\x8B");
		TEXT_SetFont(hItem, &GUI_FontMicrosoftJhengHeiUI20);
			
			printf("L_Task����!\r\n");
			  // ��ⳤ��
		memset(buffer_L, 0, sizeof(buffer_L));
  switchX('L');
  vTaskDelay(5);
			printf("������ʼ\r\n");
  l = getL();
			printf("��������\r\n");
  switchX('N');
  snprintf(buffer_L, sizeof(buffer_L), "%.3f", l);
	snprintf(buffer_L + strlen(buffer_L), sizeof(buffer_L) - strlen(buffer_L), "cm");
  g_l = l;

  // LCD����ʾ
      xReturn = xSemaphoreGive( BinarySem_Handle_TL );//������ֵ�ź���,��Ļ��ʾ
      if( xReturn == pdTRUE )
        printf("BinarySem_Handle_TL��ֵ�ź����ͷųɹ�!\r\n");
      else
        printf("BinarySem_Handle_TL��ֵ�ź����ͷ�ʧ��!\r\n");


  // �����µ��ݣ�������������Ϊ��׼
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
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  while (1)
  {
		printf("OP_Taskִ��!\r\n");
		//**************************���¡����ؼ�⡱���ǿ�·**************************************
		xReturn = xSemaphoreTake(BinarySem_Handle_OP,/* ��ֵ�ź������ */
                              portMAX_DELAY); /* �ȴ�ʱ�� */
    if(pdTRUE == xReturn){ 
			printf("OP_Task����!\r\n");
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
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  while (1)
  {
		printf("TR_Taskִ��!\r\n");
		//**************************���¡����ؼ�⡱���Ϊ����**************************************
				xReturn = xSemaphoreTake(BinarySem_Handle_TR,/* ��ֵ�ź������ */
                              portMAX_DELAY); /* �ȴ�ʱ�� */
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
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  while (1)
  {
		printf("TL_Taskִ��!\r\n");
		//**************************���¡����ȼ�⡱************************************************
				xReturn = xSemaphoreTake(BinarySem_Handle_TL,/* ��ֵ�ź������ */
                              portMAX_DELAY); /* �ȴ�ʱ�� */
    if(pdTRUE == xReturn){ 
				printf("TL_Task����!\r\n");
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
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  while (1)
  {
		printf("TC_Taskִ��!\r\n");
		//**************************���¡����ؼ�⡱���Ϊ����**************************************
		xReturn = xSemaphoreTake(BinarySem_Handle_TC,/* ��ֵ�ź������ */
                              portMAX_DELAY); /* �ȴ�ʱ�� */
    if(pdTRUE == xReturn){ 
			printf("TC_Task����!\r\n");
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
  * @brief �弶�����ʼ��
  * @note ���а����ϵĳ�ʼ�����ɷ��������������
  * @param ��
  * @retval ��
  */
static void BSP_Init(void)
{
	
	/* CRC��emWinû�й�ϵ��ֻ������Ϊ�˿�ı���������
   * ����STemWin�Ŀ�ֻ������ST��оƬ���棬���оƬ���޷�ʹ�õġ�
   */
		// ʹ��HSE������ϵͳʱ��Ϊ180M
	HSE_SetSysClock(25, 360, 2, 7);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��ͬһ�����ȼ����飬ǧ��Ҫ�ٷ��飬�мǡ�
	 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/* DWT��ʼ��	*/
	DWT_Init();
	/* ���ڳ�ʼ��	*/
	Debug_USART_Config();
	/* ��������ʼ�� */
	GTP_Init_Panel();	
	/* SDRAM��ʼ�� */
  SDRAM_Init();
	/* LCD��ʼ�� */
	LCD_Init();
		/* �̵�����ʼ�� */
  switchX_GPIO_Init();
/* TDC��ʼ�� */
  TDC_Init();
/* ADC��ʼ�� */
	Rheostat_Init();

	delay(10);  
	//-----------------------���Ȳ���-----------
//	test1 = TDC_Test();
//	switchX('L');
//	delay(10);
//	calTTask();
  //-----------���ݲ���-------------
//		switchX('C');
//		delay(10);  
//		c=getC();
//		printf("����c = %f\n", c);
////--------------�������----------------
//		switchX('R');
//		r = getR();
//		printf("����r = %f\n", r);
}
/*********************************************END OF FILE**********************/


