#include "./Measure/R.h"
#include "./adc/bsp_adc.h"
#define AVG_N 100
#define R 55.9 // 分压电阻

// 标定参数
// ADC电压标定
#define V_K 1.0072
#define V_B (-0.0007)
// 电阻标定
#define R_K 0.8615
#define R_B (-0.1298)
// 线缆寄生电阻
#define BASE_R_K 0.0014
#define BASE_R_B 0.0328
// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue;


uint16_t getADC() {
		Start_ADC_Conversion();
		return ADC_ConvertedValue;
}

// 多次进行ADC测量，并转换为电压值
float getV() {
    uint32_t sum = 0;
    float avg_adc, v;

    // 获取多次ADC值取平均
    for (int i = 0; i < AVG_N; i++) {
        sum += getADC();
    }
    avg_adc = (float) sum / AVG_N;

    v = (float) avg_adc/4096*(float)3.3; // 转换为电压值

    v = v * V_K + V_B; // 校准电压

    return v;
}

float getR() {
    float v = getV();
    float r = v / (3.3f - v) * R; // 分压法测电阻
  //  r = R_K * r + R_B; // 电阻标定

    return r;
}

float getBaseR(float l) {
    return BASE_R_K * l + BASE_R_B;
}

void calVTask() {
    float v;
    while (1) {
        v = getV();
        printf("%0.3f V\n", v);

         delay(10); 
    }
}

void calRTask() {
    float r;
    while (1) {
        r = getR();
        printf("%0.1f R\n", r);

        delay(10);
    }
}