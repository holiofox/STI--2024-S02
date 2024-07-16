
#include "AD9833.h"

#include "stm32f4xx.h"





void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    /*Configure GPIO pin Output Level */
    GPIO_ResetBits(GPIOC, DDS_FSY1_Pin | DDS_CLK_Pin | DDS_DAT_Pin | DDS_FSY0_Pin);

    /*Configure GPIO pins : PEPin PEPin PEPin PEPin */
    GPIO_InitStruct.GPIO_Pin = DDS_FSY1_Pin | DDS_CLK_Pin | DDS_DAT_Pin | DDS_FSY0_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
}






//***************************
//		Pin assign	   	
//		STM32			AD9833
//		PF12 		---> FSYNC
//		PF1 		---> SCK
//		PG6 		---> DAT
//		PC9			---> FSY1
//***************************	

	/*�˿ڶ��� */ 
//	#define PORT_FSYNC	GPIOB
//	#define PIN_FSYNC	GPIO_Pin_15  //PC7

//	#define PORT_SCK	GPIOB
//	#define PIN_SCK		GPIO_Pin_14  //PC8

//	#define PORT_DAT	GPIOB
//	#define PIN_DAT		GPIO_Pin_13  //PC9

//	#define PORT_FSY1		GPIOB
//	#define PIN_FSY1		GPIO_Pin_12  //���ֵ�λ��Ƭѡ//PA8

//****************************************************************




#define FSY0_0()        GPIO_ResetBits(DDS_FSY0_GPIO_Port, DDS_FSY0_Pin)
#define FSY0_1()        GPIO_SetBits(DDS_FSY0_GPIO_Port, DDS_FSY0_Pin)

#define SCK_0()         GPIO_ResetBits(DDS_CLK_GPIO_Port, DDS_CLK_Pin)
#define SCK_1()         GPIO_SetBits(DDS_CLK_GPIO_Port, DDS_CLK_Pin)

#define DAT_0()         GPIO_ResetBits(DDS_DAT_GPIO_Port, DDS_DAT_Pin)
#define DAT_1()         GPIO_SetBits(DDS_DAT_GPIO_Port, DDS_DAT_Pin)

#define FSY1_0()        GPIO_ResetBits(DDS_FSY1_GPIO_Port, DDS_FSY1_Pin)
#define FSY1_1()        GPIO_SetBits(DDS_FSY1_GPIO_Port, DDS_FSY1_Pin)
//��ʼ��AD9833 GPIO

//void AD9833_Init_GPIO()
//{
//    GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

//	GPIO_InitStructure.GPIO_Pin = PIN_FSY0|PIN_SCK|PIN_DAT|PIN_FSY1; 
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init(PORT_SCK, &GPIO_InitStructure);



//}



/*
*********************************************************************************************************
*	�� �� ��: AD9833_Delay
*	����˵��: ʱ����ʱ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AD9833_Delay(void)
{
	uint16_t i;
	for (i = 0; i < 1; i++);
}



/*
*********************************************************************************************************
*	�� �� ��: AD9833_Write
*	����˵��: ��SPI���߷���16��bit����
*	��    ��: TxData : ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD9833_Write(unsigned int TxData, unsigned char channel)
{
	unsigned char i;

	SCK_1();
	//AD9833_Delay();
	if (channel) {
		FSY1_1();
		//AD9833_Delay();
		FSY1_0();
		//AD9833_Delay();
	}
	else {
		FSY0_1();
		//AD9833_Delay();
		FSY0_0();
		//AD9833_Delay();
	}
	for(i = 0; i < 16; i++)
	{
		if (TxData & 0x8000)
			DAT_1();
		else
			DAT_0();
		
		AD9833_Delay();
		SCK_0();
		AD9833_Delay();		
		SCK_1();
		
		TxData <<= 1;
	}
	FSY0_1();
	
} 



/*
*********************************************************************************************************
*	�� �� ��: AD9833_WaveSeting
*	����˵��: ��SPI���߷���16��bit����
*	��    ��: 1.Freq: Ƶ��ֵ, 0.1 hz - 12Mhz
			  2.Freq_SFR: 0 �� 1
			  3.WaveMode: TRI_WAVE(���ǲ�),SIN_WAVE(���Ҳ�),SQU_WAVE(����)
			  4.Phase : ���εĳ���λ�����12λΪ��λ���ݣ�13λ����ν��14λ��ʾ��д����λ�Ĵ���0����1,ʹ�õ��ǼĴ���0���������뷶ΧΪ0~4095
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void AD9833_WaveSeting(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase, unsigned char channel )
{

		int frequence_LSB,frequence_MSB,Phs_data;
		double   frequence_mid,frequence_DATA;
		long int frequence_hex;

		/*********************************����Ƶ�ʵ�16����ֵ***********************************/
		frequence_mid=268435456/25;//�ʺ�25M����
		//���ʱ��Ƶ�ʲ�Ϊ25MHZ���޸ĸô���Ƶ��ֵ����λMHz ��AD9833���֧��25MHz
		frequence_DATA=Freq;
		frequence_DATA=frequence_DATA/1000000;
		frequence_DATA=frequence_DATA*frequence_mid;
		frequence_hex=frequence_DATA;  //���frequence_hex��ֵ��32λ��һ���ܴ�����֣���Ҫ��ֳ�����14λ���д���
		frequence_LSB=frequence_hex&0x3fff;
		frequence_MSB=(frequence_hex>>14)&0x3fff;

		Phs_data=Phase|0xC000;	//��λֵ��D15��D14Ϊ1����ʾ����λ�Ĵ���д�����ݣ�ֻ����λ0�Ĵ���
		AD9833_Write(0x0100, channel); //��λAD9833,��RESETλΪ1
		AD9833_Write(0x2100, channel); //ѡ������һ��д�룬B28λ��RESETλΪ1������������ܵ�������MSB\LSB����Ҫ��������д��

		if(Freq_SFR==0)				  //���������õ�����Ƶ�ʼĴ���0
		{
		 	frequence_LSB=frequence_LSB|0x4000;//D15 D14 Ϊ01��ʾ��F0�Ĵ���д��
		 	frequence_MSB=frequence_MSB|0x4000;
			 //ʹ��Ƶ�ʼĴ���0�������
			AD9833_Write(frequence_LSB, channel); //L14��ѡ��Ƶ�ʼĴ���0�ĵ�14λ��������
			AD9833_Write(frequence_MSB, channel); //H14 Ƶ�ʼĴ����ĸ�14λ��������
			AD9833_Write(Phs_data, channel);	//������λ
			//AD9833_Write(0x2000); /**����FSELECTλΪ0��оƬ���빤��״̬,Ƶ�ʼĴ���0�������**/
	    }
		if(Freq_SFR==1)				//���������õ�����Ƶ�ʼĴ���1
		{
			 frequence_LSB=frequence_LSB|0x8000;//D15 D14 Ϊ10��ʾ��F0�Ĵ���д��
			 frequence_MSB=frequence_MSB|0x8000;
			//ʹ��Ƶ�ʼĴ���1�������
			AD9833_Write(frequence_LSB, channel); //L14��ѡ��Ƶ�ʼĴ���1�ĵ�14λ����
			AD9833_Write(frequence_MSB, channel); //H14 Ƶ�ʼĴ���1Ϊ
			AD9833_Write(Phs_data, channel);	//������λ
			//AD9833_Write(0x2800); /**����FSELECTλΪ0������FSELECTλΪ1����ʹ��Ƶ�ʼĴ���1��ֵ��оƬ���빤��״̬,Ƶ�ʼĴ���1�������**/
		}

		if(WaveMode==TRI_WAVE) //������ǲ�����
		{if(Freq_SFR==0)
		 	AD9833_Write(0x2002, channel); 
			else if(Freq_SFR==1)
			AD9833_Write(0x2802, channel); 
				
		}
		if(WaveMode==SQU_WAVE)	//�����������
		{if(Freq_SFR==0)
			AD9833_Write(0x2028, channel);
			else if(Freq_SFR==1)
			AD9833_Write(0x2828, channel);
				
		}
		if(WaveMode==SIN_WAVE)	//������Ҳ���
		{if(Freq_SFR==0)
			AD9833_Write(0x2000, channel);
			else if(Freq_SFR==1)
							AD9833_Write(0x2800, channel);
		}

}


/*
*********************************************************************************************************
*	�� �� ��: AD9833_WaveSeting_Double
*	����˵��: ��SPI���߷���16��bit����
*	��    ��: 1.Freq: Ƶ��ֵ, 0.1 hz - 12Mhz
			  2.Freq_SFR: 0 �� 1
			  3.WaveMode: TRI_WAVE(���ǲ�),SIN_WAVE(���Ҳ�),SQU_WAVE(����)
			  4.Phase : ����1�ĳ���λ,���ڸ�ֵ�ɵ�����������֮�����λ��(������360��һ��һ��Ӧ)
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void AD9833_WaveSeting_Double(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase)
{

		int frequence_LSB,frequence_MSB,Phs_data;
		double   frequence_mid,frequence_DATA;
		long int frequence_hex;

		/*********************************����Ƶ�ʵ�16����ֵ***********************************/
		frequence_mid=268435456/25;//�ʺ�25M����
		//���ʱ��Ƶ�ʲ�Ϊ25MHZ���޸ĸô���Ƶ��ֵ����λMHz ��AD9833���֧��25MHz
		frequence_DATA=Freq;
		frequence_DATA=frequence_DATA/1000000;
		frequence_DATA=frequence_DATA*frequence_mid;
		frequence_hex=frequence_DATA;  //���frequence_hex��ֵ��32λ��һ���ܴ�����֣���Ҫ��ֳ�����14λ���д���
		frequence_LSB=frequence_hex; //frequence_hex��16λ�͸�frequence_LSB
		frequence_LSB=frequence_LSB&0x3fff;//ȥ�������λ��16λ����ȥ����λ������14λ
		frequence_MSB=frequence_hex>>14; //frequence_hex��16λ�͸�frequence_HSB
		frequence_MSB=frequence_MSB&0x3fff;//ȥ�������λ��16λ����ȥ����λ������14λ

		Phs_data=Phase|0xC000;	//��λֵ
		AD9833_Write(0x0100, 0); //��λAD9833,��RESETλΪ1
		AD9833_Write(0x2100, 0); //ѡ������һ��д�룬B28λ��RESETλΪ1
		AD9833_Write(0x0100, 1); //��λAD9833,��RESETλΪ1
		AD9833_Write(0x2100, 1); //ѡ������һ��д�룬B28λ��RESETλΪ1

		if(Freq_SFR==0)				  //���������õ�����Ƶ�ʼĴ���0
		{
		 	frequence_LSB=frequence_LSB|0x4000;
		 	frequence_MSB=frequence_MSB|0x4000;
			 //ʹ��Ƶ�ʼĴ���0�������
			AD9833_Write(frequence_LSB, 0); //L14��ѡ��Ƶ�ʼĴ���0�ĵ�14λ��������
			AD9833_Write(frequence_MSB, 0); //H14 Ƶ�ʼĴ����ĸ�14λ��������
			AD9833_Write(Phs_data, 1);	//������λ
			AD9833_Write(frequence_LSB, 1); //L14��ѡ��Ƶ�ʼĴ���0�ĵ�14λ��������
			AD9833_Write(frequence_MSB, 1); //H14 Ƶ�ʼĴ����ĸ�14λ��������
			//AD9833_Write(0x2000); /**����FSELECTλΪ0��оƬ���빤��״̬,Ƶ�ʼĴ���0�������**/
	    }
		if(Freq_SFR==1)				//���������õ�����Ƶ�ʼĴ���1
		{
			 frequence_LSB=frequence_LSB|0x8000;
			 frequence_MSB=frequence_MSB|0x8000;
			//ʹ��Ƶ�ʼĴ���1�������
			AD9833_Write(frequence_LSB, 0); //L14��ѡ��Ƶ�ʼĴ���1�ĵ�14λ����
			AD9833_Write(frequence_MSB, 0); //H14 Ƶ�ʼĴ���1Ϊ
			AD9833_Write(Phs_data, 1);	//������λ
			AD9833_Write(frequence_LSB, 1); //L14��ѡ��Ƶ�ʼĴ���1�ĵ�14λ����
			AD9833_Write(frequence_MSB, 1); //H14 Ƶ�ʼĴ���1Ϊ
			//AD9833_Write(0x2800); /**����FSELECTλΪ0������FSELECTλΪ1����ʹ��Ƶ�ʼĴ���1��ֵ��оƬ���빤��״̬,Ƶ�ʼĴ���1�������**/
		}

		if(WaveMode==TRI_WAVE) //������ǲ�����
		{
			if(Freq_SFR == 0)
			{
			AD9833_Write(0x2002, 0); 
			AD9833_Write(0x2002, 1);
			}
			else if (Freq_SFR == 1)
			{
			AD9833_Write(0x2802, 0); 
			AD9833_Write(0x2802, 1);
			}
		}			
		if(WaveMode==SQU_WAVE)	//�����������
		{
			if(Freq_SFR == 0)
			{
			AD9833_Write(0x2028, 0); 
			AD9833_Write(0x2028, 1); 
			}
			else if(Freq_SFR == 1)
			{
			AD9833_Write(0x2828, 0); 
			AD9833_Write(0x2828, 1); 
			}
		}
		if(WaveMode==SIN_WAVE)	//������Ҳ���
		{
			if(Freq_SFR == 0)
			{
			AD9833_Write(0x2000, 0); 
			AD9833_Write(0x2000, 1);
			}
			else if(Freq_SFR == 1)
			{
			AD9833_Write(0x2800, 0); 
			AD9833_Write(0x2800, 1);
			}
		}

}



