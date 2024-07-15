#include "./Measure/L.h"
#include "./tdc-gp22/tdc.h"



// ��ȡTDC����ԭʼ���
float getTDC() {
    uint32_t tdc;
    uint8_t flag;

    flag = TDC_Measure(&tdc);
    if (flag == 1) // ������ʱ
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
         printf("����l = %f\n", l);
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
        l = ns * Ce8 / 2 * 10;//��λΪ����

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
         printf("������ԭʼ����tdc = %u\n", tdc);
         printf("ת��Ϊns = %f\n", ns);
         printf("����l = %f\n", l);

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