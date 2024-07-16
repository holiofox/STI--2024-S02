#include "./Measure/all.h"
void switchX_GPIO_Init(){

			GPIO_InitTypeDef GPIO_InitStructure;
	
			RCC_AHB1PeriphClockCmd (SWITCH_R_GPIO_CLK |SWITCH_C_GPIO_CLK, ENABLE);
	
			GPIO_InitStructure.GPIO_Pin = SWITCH_R_Pin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_Init(SWITCH_R_GPIO_Port, &GPIO_InitStructure);
	
			GPIO_InitStructure.GPIO_Pin = SWITCH_C_Pin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_Init(SWITCH_C_GPIO_Port, &GPIO_InitStructure);
	 
			GPIO_InitStructure.GPIO_Pin = SWITCH_L_Pin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_Init(SWITCH_L_GPIO_Port, &GPIO_InitStructure);
			
			GPIO_WriteBit(SWITCH_R_GPIO_Port, SWITCH_R_Pin, 1);
			GPIO_WriteBit(SWITCH_C_GPIO_Port, SWITCH_C_Pin, 1);
			GPIO_WriteBit(SWITCH_L_GPIO_Port, SWITCH_L_Pin, 0);
}
void switchX(char x) {
    switch (x) {
        case 'R':
            GPIO_WriteBit(SWITCH_C_GPIO_Port, SWITCH_C_Pin, 1);
            GPIO_WriteBit(SWITCH_R_GPIO_Port, SWITCH_R_Pin, 0);
						GPIO_WriteBit(SWITCH_L_GPIO_Port, SWITCH_L_Pin, 0);
            break;
        case 'C':
            GPIO_WriteBit(SWITCH_R_GPIO_Port, SWITCH_R_Pin, 1);
            GPIO_WriteBit(SWITCH_C_GPIO_Port, SWITCH_C_Pin, 0);
						GPIO_WriteBit(SWITCH_L_GPIO_Port, SWITCH_L_Pin, 0);
            break;
				case 'L':
            GPIO_WriteBit(SWITCH_R_GPIO_Port, SWITCH_R_Pin, 1);
            GPIO_WriteBit(SWITCH_C_GPIO_Port, SWITCH_C_Pin, 1);
						GPIO_WriteBit(SWITCH_L_GPIO_Port, SWITCH_L_Pin, 1);
            break;
        default:
            GPIO_WriteBit(SWITCH_R_GPIO_Port, SWITCH_R_Pin, 1);
            GPIO_WriteBit(SWITCH_C_GPIO_Port, SWITCH_C_Pin, 1);
						GPIO_WriteBit(SWITCH_L_GPIO_Port, SWITCH_L_Pin, 0);
            break;
    }
}