#ifndef __L_H
#define	__L_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#define AVG_N 1000 // ƽ������

#define MIN_L 100 // ��С����������ȣ���λ����
#define MAX_L 7000 // �������������ȣ���λ����

// У׼����
#define Ce8 3*0.69950847*100000000// ͭ�еĹ���


float getL(void);

#endif /* __L_H */
