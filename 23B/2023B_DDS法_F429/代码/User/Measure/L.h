#ifndef __L_H
#define	__L_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#define AVG_N 1000 // 平均次数

#define MIN_L 100 // 最小允许测量长度，单位厘米
#define MAX_L 7000 // 最大允许测量长度，单位厘米

// 校准参数
#define Ce8 3*0.69950847*100000000// 铜中的光速


float getL(void);

#endif /* __L_H */
