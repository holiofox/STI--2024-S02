#ifndef __CORE_DELAY_H
#define __CORE_DELAY_H

#include "stm32f4xx.h"

#define USE_DWT_DELAY			1	/* 使用dwt内核精确延时 */

#if USE_DWT_DELAY
#define USE_TICK_DELAY		0		/* 不使用SysTick延时 */
#else
#define USE_TICK_DELAY		1		/* 使用SysTick延时 */
#endif


/*简单任务管理*/
#define TASK_ENABLE 0
#define NumOfTask 3


#if USE_DWT_DELAY

#define Delay_ms(ms)  	CPU_TS_Tmr_Delay_MS(ms)
#define Delay_us(us)  	CPU_TS_Tmr_Delay_US(us)
/* 最大延时 23.8s=2的32次方/180000000 */
#define Delay_s(s)  	  CPU_TS_Tmr_Delay_S(s)

/* 获取内核时钟频率 */
#define GET_CPU_ClkFreq()       (SystemCoreClock)
#define SysClockFreq            (SystemCoreClock)
/* 为方便使用，在延时函数内部调用CPU_TS_TmrInit函数初始化时间戳寄存器，
   这样每次调用函数都会初始化一遍。
   把本宏值设置为0，然后在main函数刚运行时调用CPU_TS_TmrInit可避免每次都初始化 */  

#define CPU_TS_INIT_IN_DELAY_FUNCTION   0


/*******************************************************************************
 * 							函数声明
 ******************************************************************************/
uint32_t Get_Tick(void);
void DWT_Init(void);
uint32_t Tick_Diff(uint32_t tick1, uint32_t tick2);
//使用以下函数前必须先调用CPU_TS_TmrInit函数使能计数器，或使能宏CPU_TS_INIT_IN_DELAY_FUNCTION
void CPU_TS_Tmr_Delay_US(uint32_t us);

#define CPU_TS_Tmr_Delay_MS(ms)     CPU_TS_Tmr_Delay_US(ms*1000)
#define CPU_TS_Tmr_Delay_S(s)       CPU_TS_Tmr_Delay_MS(s*1000)

#endif

#endif /* __CORE_DELAY_H */
