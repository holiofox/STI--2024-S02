#ifndef __C_H
#define	__C_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#define R1M 0.01
#define R2M 1

// �궨����
#define F_K 0.9879
#define F_B 7.2989
//// ���ݱ궨
//#define C_K 1.003054914
//#define C_B (-36.6104)
// ���ݱ궨
#define C_K 1.0223
#define C_B 1.9123
// ���߼�������
#define BASE_C_K 0.9527
#define BASE_C_B 1.4487

#define Q_SIZE 200
float getC(void); // ��λpF
float getC_L(void); // ��λpF

float getBaseC(float l);

void calFTask(void);

void calCTask(void);

void push(float f);
#endif /* __C_H */