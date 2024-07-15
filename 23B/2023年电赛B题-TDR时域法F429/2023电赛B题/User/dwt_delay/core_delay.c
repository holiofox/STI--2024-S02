/************************************************************
  * @brief   core_delay.c
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    使用内核寄存器精确延时
  ***********************************************************/
  
#include "./dwt_delay/core_delay.h"   


/*
**********************************************************************
*         时间戳相关寄存器定义
**********************************************************************
*/
/*
 在Cortex-M里面有一个外设叫DWT(Data Watchpoint and Trace)，
 该外设有一个32位的寄存器叫CYCCNT，它是一个向上的计数器，
 记录的是内核时钟运行的个数，最长能记录的时间为：
 23.8s=2的32次方/180000000
 (假设内核频率为180M，内核跳一次的时间大概为1/180M=5.5ns)
 当CYCCNT溢出之后，会清0重新开始向上计数。
 使能CYCCNT计数的操作步骤：
 1、先使能DWT外设，这个由另外内核调试寄存器DEMCR的位24控制，写1使能
 2、使能CYCCNT寄存器之前，先清0
 3、使能CYCCNT寄存器，这个由DWT_CTRL(代码上宏定义为DWT_CR)的位0控制，写1使能
 */
 
#if USE_DWT_DELAY


#define  DWT_CR      *(__IO uint32_t *)0xE0001000
#define  DWT_CYCCNT  *(__IO uint32_t *)0xE0001004
#define  DEM_CR      *(__IO uint32_t *)0xE000EDFC


#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)


/**
  * @brief  初始化时间戳
  * @param  无
  * @retval 无
  * @note   使用延时函数前，必须调用本函数
  */
void DWT_Init(void)
{
    /* 使能DWT外设 */
    DEM_CR |= (uint32_t)DEM_CR_TRCENA;                

    /* DWT CYCCNT寄存器计数清0 */
    DWT_CYCCNT = (uint32_t)0u;

    /* 使能Cortex-M DWT CYCCNT寄存器 */
    DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
}

/**
  * @brief  读取当前时间戳
  * @param  无
  * @retval 当前时间戳，即DWT_CYCCNT寄存器的值
  */
uint32_t Get_Tick(void)
{        
  return ((uint32_t)DWT_CYCCNT);
}

///**
//  * @brief  读取当前时间戳
//  * @param  无
//  * @retval 当前时间戳，即DWT_CYCCNT寄存器的值
//	* 				此处给HAL库替换HAL_GetTick函数，用于os
//  */
//uint32_t HAL_GetTick(void)
//{        
//  return ((uint32_t)DWT_CYCCNT*1000/SysClockFreq);
//}
uint32_t Tick_Diff(uint32_t tick1, uint32_t tick2) {
    uint32_t diff1, diff2;
    if (tick1 >= tick2) {
        diff1 = tick1 - tick2;
        diff2 = UINT32_MAX - tick1 + tick2;//防止溢出
    } else {
        diff1 = tick2 - tick1;
        diff2 = UINT32_MAX - tick2 + tick1;//防止溢出
    }
    if (diff1 <= diff2)
        return diff1;
    else
        return diff2;
	}
#endif

/*********************************************END OF FILE**********************/
