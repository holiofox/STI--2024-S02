#include "./Measure/C.h"
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "./dwt_delay/core_delay.h"   
#include "./exti/bsp_exti.h"
extern SemaphoreHandle_t BinarySem_Handle_C;
// ά��һ����������
float fq[Q_SIZE];
uint8_t head = 0;
uint8_t tail = 1;
extern __IO float Frequency;
extern uint16_t tag_C;



void push(float f) {
    fq[tail] = f;
    tail = (tail + 1) % Q_SIZE;
    if ((tail + Q_SIZE - head + 1) % Q_SIZE == 0) { // ������
        head = (head + 1) % Q_SIZE;
    }
}

float getF() {
    float sum = 0;
    float f_avg;


    for (int i = 0; i < Q_SIZE; i++) {
        sum += fq[i];
    }
    f_avg = sum / Q_SIZE;

    f_avg = f_avg * F_K + F_B; // У׼Ƶ��
		printf("Ƶ��f = %f\n", f_avg);
    return f_avg;
}

float getC() {
	float f,c12;
	  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
   EXTI_Config();
	printf("�ȴ��ź�!\r\n");
	  xReturn = xSemaphoreTake(BinarySem_Handle_C,/* ��ֵ�ź������ */
                              portMAX_DELAY); /* �ȴ�ʱ�� */
    if(pdTRUE == xReturn){
     f = getF();
     c12 = 1e6 * 1.442695f / ((R1M + 2 * R2M) * f);
    c12 = C_K * c12 + C_B; // У׼����
    if (c12 < 0)
        c12 = 0;

    return c12;
	}
}


float getBaseC(float l) {
    return BASE_C_K * l + BASE_C_B;
}

void calFTask() {
    float f;
    while (1) {
        f = getF();
    // UART_printf("%0.1f Hz\n", f);


    }
}

void calCTask() {
    float c12;
    while (1) {
        c12 = getC();
    //    UART_printf("%0.1f pF\n", c12);


    }
}