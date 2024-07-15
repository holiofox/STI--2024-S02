#include "./Measure/R.h"
#include "./adc/bsp_adc.h"
#define AVG_N 100
#define R 55.9 // ��ѹ����

// �궨����
// ADC��ѹ�궨
#define V_K 1.0072
#define V_B (-0.0007)
// ����궨
#define R_K 0.8615
#define R_B (-0.1298)
// ���¼�������
#define BASE_R_K 0.0014
#define BASE_R_B 0.0328
// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue;


uint16_t getADC() {
		Start_ADC_Conversion();
		return ADC_ConvertedValue;
}

// ��ν���ADC��������ת��Ϊ��ѹֵ
float getV() {
    uint32_t sum = 0;
    float avg_adc, v;

    // ��ȡ���ADCֵȡƽ��
    for (int i = 0; i < AVG_N; i++) {
        sum += getADC();
    }
    avg_adc = (float) sum / AVG_N;

    v = (float) avg_adc/4096*(float)3.3; // ת��Ϊ��ѹֵ

    v = v * V_K + V_B; // У׼��ѹ

    return v;
}

float getR() {
    float v = getV();
    float r = v / (3.3f - v) * R; // ��ѹ�������
  //  r = R_K * r + R_B; // ����궨

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