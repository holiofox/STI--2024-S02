/************************************************************
  * @brief   core_delay.c
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    ʹ���ں˼Ĵ�����ȷ��ʱ
  ***********************************************************/
  
#include "./dwt_delay/core_delay.h"   


/*
**********************************************************************
*         ʱ�����ؼĴ�������
**********************************************************************
*/
/*
 ��Cortex-M������һ�������DWT(Data Watchpoint and Trace)��
 ��������һ��32λ�ļĴ�����CYCCNT������һ�����ϵļ�������
 ��¼�����ں�ʱ�����еĸ�������ܼ�¼��ʱ��Ϊ��
 23.8s=2��32�η�/180000000
 (�����ں�Ƶ��Ϊ180M���ں���һ�ε�ʱ����Ϊ1/180M=5.5ns)
 ��CYCCNT���֮�󣬻���0���¿�ʼ���ϼ�����
 ʹ��CYCCNT�����Ĳ������裺
 1����ʹ��DWT���裬����������ں˵��ԼĴ���DEMCR��λ24���ƣ�д1ʹ��
 2��ʹ��CYCCNT�Ĵ���֮ǰ������0
 3��ʹ��CYCCNT�Ĵ����������DWT_CTRL(�����Ϻ궨��ΪDWT_CR)��λ0���ƣ�д1ʹ��
 */
 
#if USE_DWT_DELAY


#define  DWT_CR      *(__IO uint32_t *)0xE0001000
#define  DWT_CYCCNT  *(__IO uint32_t *)0xE0001004
#define  DEM_CR      *(__IO uint32_t *)0xE000EDFC


#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)


/**
  * @brief  ��ʼ��ʱ���
  * @param  ��
  * @retval ��
  * @note   ʹ����ʱ����ǰ��������ñ�����
  */
void DWT_Init(void)
{
    /* ʹ��DWT���� */
    DEM_CR |= (uint32_t)DEM_CR_TRCENA;                

    /* DWT CYCCNT�Ĵ���������0 */
    DWT_CYCCNT = (uint32_t)0u;

    /* ʹ��Cortex-M DWT CYCCNT�Ĵ��� */
    DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
}

/**
  * @brief  ��ȡ��ǰʱ���
  * @param  ��
  * @retval ��ǰʱ�������DWT_CYCCNT�Ĵ�����ֵ
  */
uint32_t Get_Tick(void)
{        
  return ((uint32_t)DWT_CYCCNT);
}

///**
//  * @brief  ��ȡ��ǰʱ���
//  * @param  ��
//  * @retval ��ǰʱ�������DWT_CYCCNT�Ĵ�����ֵ
//	* 				�˴���HAL���滻HAL_GetTick����������os
//  */
//uint32_t HAL_GetTick(void)
//{        
//  return ((uint32_t)DWT_CYCCNT*1000/SysClockFreq);
//}
uint32_t Tick_Diff(uint32_t tick1, uint32_t tick2) {
    uint32_t diff1, diff2;
    if (tick1 >= tick2) {
        diff1 = tick1 - tick2;
        diff2 = UINT32_MAX - tick1 + tick2;//��ֹ���
    } else {
        diff1 = tick2 - tick1;
        diff2 = UINT32_MAX - tick2 + tick1;//��ֹ���
    }
    if (diff1 <= diff2)
        return diff1;
    else
        return diff2;
	}
#endif

/*********************************************END OF FILE**********************/
