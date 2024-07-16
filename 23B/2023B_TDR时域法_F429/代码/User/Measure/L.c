#include "./Measure/L.h"
#include "./tdc-gp22/tdc.h"



// 获取TDC测量原始结果
float getTDC() {
    uint32_t tdc;
    uint8_t flag;

    flag = TDC_Measure(&tdc);
    if (flag == 1) // 测量超时
        tdc = 0;

    return tdc;
}

float getL() {
	    uint32_t tdc;
    uint32_t sum = 0;
    uint32_t cnt = 0;
    float ns,l,len;

    for (uint32_t i = 0; i < AVG_N; i++) {
        TDC_Measure(&tdc);
        ns = TDC_to_ns(tdc);
        len = ns * Ce8 / 2 * 10 ;
        if (len < MIN_L || len > MAX_L)
            continue;
        sum += tdc;
        cnt++;
    }

    if (cnt != 0) {
        uint32_t avg = (uint32_t) (sum / cnt );
        ns = TDC_to_ns(avg);
        l = ns * Ce8 / 2 * 10;
        if (l < 0)
            l = 0;
    }
    else
        l = 0;
         printf("长度l = %f\n", l);
    return l;
}

void calTTask() {
    uint32_t tdc;
	    uint32_t sum = 0;
    uint32_t cnt = 0;
    float ns, l;
//    while (1) {
//    for (uint32_t i = 0; i < AVG_N; i++) {
        TDC_Measure(&tdc);
        ns = TDC_to_ns(tdc);
        l = ns * Ce8 / 2 * 10;//单位为厘米

//        sum += tdc;
//        cnt++;
//    }
//    if (cnt != 0) {
//        uint32_t avg = (uint32_t) (sum / cnt );
//       ns = TDC_to_ns(avg);
//        l = ns * Ce8 / 2 * 10;
//        if (l < 0)
//            l = 0;
//    }
         printf("测量的原始数据tdc = %u\n", tdc);
         printf("转换为ns = %f\n", ns);
         printf("长度l = %f\n", l);

       delay(10);
//    }
}

void calLTask() {
    float l;
    while(1) {
        l = getL();
        //UART_printf("%.1f cm\n", l);

       delay(10);
    }
}