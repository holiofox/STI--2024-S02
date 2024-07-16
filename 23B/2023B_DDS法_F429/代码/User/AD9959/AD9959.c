#include "./AD9959/AD9959.h"

extern void delay_us(u32 nus);

uint8_t CSR_DATA0[1] = {0x10};     // �� CH0
uint8_t CSR_DATA1[1] = {0x20};      // �� CH1
uint8_t CSR_DATA2[1] = {0x40};      // �� CH2
uint8_t CSR_DATA3[1] = {0x80};      // �� CH3		
																	
 
uint8_t 	FR2_DATA[2] = {0x20,0x00};//default Value = 0x0000
uint8_t 	CFR_DATA[3] = {0x00,0x03,0x00};//default Value = 0x000302	   
																	
uint8_t 	CPOW0_DATA[2] = {0x00,0x00};//default Value = 0x0000   @ = POW/2^14*360
			


uint32_t SinFre[4] = {0, 1000000, 1000000, 1000000};
uint32_t SinAmp[4] = {1023, 1023, 1023, 1023};
uint32_t SinPhr[4] = {0, 0, 180, 360};
//ɨƵ����
uint32_t SweepMinFre = 1000;
uint32_t SweepMaxFre = 10000;
uint32_t SweepStepFre = 100;
uint16_t SweepTime = 1;//ms
uint8_t SweepFlag = 0;



uint8_t 	FR1_DATA[3] = {0xD0,0x00,0x00};//20��Ƶ Charge pump control = 75uA FR1<23> -- VCO gain control =0ʱ system clock below 160 MHz;
uint8_t 	LSRR_DATA[2] = {0x00,0x00};//default Value = 0x----
																	
uint8_t 	RDW_DATA[4] = {0x00,0x00,0x00,0x00};//default Value = 0x--------
																	
uint8_t 	FDW_DATA[4] = {0x00,0x00,0x00,0x00};//default Value = 0x--------

void AD9959_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct ;

  /* GPIO Ports Clock Enable */

	RCC_AHB1PeriphClockCmd(SCK_GPIO_CLK|SD1_GPIO_CLK|CS_GPIO_CLK|SD0_GPIO_CLK|SD2_GPIO_CLK|SD3_GPIO_CLK|UPDATE_GPIO_CLK|P0_GPIO_CLK|P1_GPIO_CLK|P2_GPIO_CLK|P3_GPIO_CLK|RST_GPIO_CLK|PDC_GPIO_CLK, ENABLE);

  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.GPIO_Pin = SCK_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(SCK_GPIO_Port, &GPIO_InitStruct);
	
	  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.GPIO_Pin = SD1_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(SD1_GPIO_Port, &GPIO_InitStruct);
	
		  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.GPIO_Pin = SD2_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(SD2_GPIO_Port, &GPIO_InitStruct);
	
		  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.GPIO_Pin = SD3_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(SD3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
	GPIO_InitStruct.GPIO_Pin = CS_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.GPIO_Pin = SD0_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(SD0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.GPIO_Pin = UPDATE_Pin;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(UPDATE_GPIO_Port, &GPIO_InitStruct);

	  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.GPIO_Pin = P0_Pin;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(P0_GPIO_Port, &GPIO_InitStruct);
	
		  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.GPIO_Pin = P1_Pin;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(P1_GPIO_Port, &GPIO_InitStruct);
	
		  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.GPIO_Pin = P2_Pin;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(P2_GPIO_Port, &GPIO_InitStruct);
	
		  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.GPIO_Pin = P3_Pin;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(P3_GPIO_Port, &GPIO_InitStruct);
	
  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.GPIO_Pin = RST_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.GPIO_Pin = PDC_Pin;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(PDC_GPIO_Port, &GPIO_InitStruct);
	

	  /*Configure GPIO pin Output Level */
	GPIO_ResetBits(SCK_GPIO_Port,SCK_Pin);
	  /*Configure GPIO pin Output Level */
	GPIO_ResetBits(SD1_GPIO_Port,SD1_Pin);
  /*Configure GPIO pin Output Level */
	GPIO_SetBits(CS_GPIO_Port,CS_Pin);
	  /*Configure GPIO pin Output Level */
	GPIO_ResetBits(SD0_GPIO_Port,SD0_Pin);
  /*Configure GPIO pin Output Level */
	GPIO_ResetBits(UPDATE_GPIO_Port,UPDATE_Pin);
	  /*GPIO_ResetBits GPIO pin Output Level */
	GPIO_ResetBits(P0_GPIO_Port,P0_Pin);
		  /*Configure GPIO pin Output Level */
	GPIO_ResetBits(RST_GPIO_Port,RST_Pin);
  /*Configure GPIO pin Output Level */
	GPIO_ResetBits(PDC_GPIO_Port,PDC_Pin);

}




//AD9959��ʼ��
void Init_AD9959(void)  
{
	AD9959_GPIO_Init();
	Intserve();  //IO�ڳ�ʼ��
	IntReset();  //AD9959��λ  
	
	WriteData_AD9959(FR1_ADD,3,FR1_DATA,1);//д���ܼĴ���1
	WriteData_AD9959(CFR_ADD,3,CFR_DATA,1);
//		WriteData_AD9959(FR2_ADD,2,FR2_DATA,1);
	//д���ʼƵ��
	Write_frequence(0,SinFre[0]);
//	Write_frequence(1,SinFre[1]); 
//	Write_frequence(2,SinFre[2]);
//	Write_frequence(3,SinFre[3]);

	Write_Phase(0, SinPhr[0]);
//	Write_Phase(1, SinPhr[1]);
//	Write_Phase(2, SinPhr[2]);
//	Write_Phase(3, SinPhr[3]);
	
	Write_Amplitude(0, SinAmp[0]);
//	Write_Amplitude(1, SinAmp[1]);
//	Write_Amplitude(2, SinAmp[2]);
//	Write_Amplitude(3, SinAmp[3]);
} 
//��ʱ
void delay1 (uint32_t length)
{
//	delay_us(length);
	length *=12;
	while(length--);
}
//IO�ڳ�ʼ��
void Intserve(void)		   
{   
	AD9959_PWR_L;
    CS_H;
    SCLK_L;
    UPDATE_L;
    PS0_L;
    PS1_L;
    PS2_L;
    PS3_L;
    SDIO0_L;
    SDIO1_L;
    SDIO2_L;
    SDIO3_L;
}
//AD9959��λ
void IntReset(void)	  
{
	Reset_L;
	delay1(1);
	Reset_H;
	delay1(30);
	Reset_L;
}
 //AD9959��������
void IO_Update(void)  
{
	UPDATE_L;
	delay1(2);
	UPDATE_H;
	delay1(4);
	UPDATE_L;
}
/*--------------------------------------------
�������ܣ�������ͨ��SPI��AD9959д����
RegisterAddress: �Ĵ�����ַ
NumberofRegisters: �����ֽ���
*RegisterData: ������ʼ��ַ
temp: �Ƿ����IO�Ĵ���
----------------------------------------------*/
void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData,uint8_t temp)
{
	uint8_t	ControlValue = 0;
	uint8_t	ValueToWrite = 0;
	uint8_t	RegisterIndex = 0;
	uint8_t	i = 0;

	ControlValue = RegisterAddress;

//д���ַ
	SCLK_L;
 	CS_L;//����Ƭѡ                                                                                                                                                            	CS_L;	
	delay1(1);	
	for(i=0; i<8; i++)
	{
		SCLK_L; 
		
		if(0x80 == (ControlValue & 0x80))
		{SDIO0_H;	}  
		else
		{SDIO0_L;	}  
		
		delay1(1);
		
		
		SCLK_H;
		delay1(1);
		
		
		ControlValue <<= 1;
	}
	SCLK_L;
	delay1(10);
	
	//д������
	for (RegisterIndex=0; RegisterIndex<NumberofRegisters; RegisterIndex++)
	{
		ValueToWrite = RegisterData[RegisterIndex];
		for (i=0; i<8; i++)
		{
			SCLK_L;
			if(0x80 == (ValueToWrite & 0x80))
			{
				SDIO0_H;	
			}				
			else
			{
				SDIO0_L;
			}
			
			delay1(1);
			
			SCLK_H;
			delay1(1);
			
			ValueToWrite <<= 1;
		}
		SCLK_L;	
		delay1(5);
	}	
	if(temp != 0)
	IO_Update();	
	CS_H;
	delay1(25);
} 

/*---------------------------------------
�������ܣ�����ͨ�����Ƶ��
Channel:  ���ͨ��
Freq:     ���Ƶ�ʣ���Χ0~200 000 000 Hz
---------------------------------------*/
void Write_frequence(uint8_t Channel,uint32_t Freq)
{	 
	  uint8_t 	CFTW0_DATA[4] ={0x00,0x00,0x00,0x00};	//�м����
	  
	  uint32_t 	FTW;            
	  FTW=(uint32_t)Freq*8.589934592;	   //������Ƶ�����ӷ�Ϊ�ĸ��ֽ�  8.589934592=(2^32)/500000000 ����500M=25M*20(��Ƶ���ɱ��)
	  
	  CFTW0_DATA[3]=(uint8_t)FTW;
	  CFTW0_DATA[2]=(uint8_t)(FTW>>8);
	  CFTW0_DATA[1]=(uint8_t)(FTW>>16);
	  CFTW0_DATA[0]=(uint8_t)(FTW>>24);
	  
	  if(Channel==0)	  
	  {
		  WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1);//���ƼĴ���д��CH0ͨ��
		  WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.���CH0�趨Ƶ��
		}
	  else if(Channel==1)	
	  {
		  WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1);//���ƼĴ���д��CH1ͨ��
		  WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.���CH1�趨Ƶ��	
	  }
	  else if(Channel==2)	
	  {
		  WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1);//���ƼĴ���д��CH2ͨ��
	      WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.���CH2�趨Ƶ��	
	  }
	  else if(Channel==3)	
	  {
		  WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1);//���ƼĴ���д��CH3ͨ��
          WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.���CH3�趨Ƶ��	
	  }																																																																										 
	
} 
/*---------------------------------------
�������ܣ�����ͨ���������
Channel:  ���ͨ��
Ampli:    �������,��Χ0~1023������ֵ0~1023��Ӧ�������0~530mV����
---------------------------------------*/
void Write_Amplitude(uint8_t Channel, uint16_t Ampli)
{ 
	uint16_t A_temp;//=0x23ff;
	uint8_t ACR_DATA[3] = {0x00,0x00,0x00};//default Value = 0x--0000 Rest = 18.91/Iout 
	
	A_temp=Ampli|0x1000;
	ACR_DATA[2] = (uint8_t)A_temp;  //��λ����
	ACR_DATA[1] = (uint8_t)(A_temp>>8); //��λ����
	if(Channel==0)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1); 
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1); 
	}
	else if(Channel==1)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1); 
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
	}
	else if(Channel==2)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1); 
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1); 
	}
	else if(Channel==3)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1); 
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1); 
	}
}
/*---------------------------------------
�������ܣ�����ͨ�������λ
Channel:  ���ͨ��
Phase:    �����λ,��Χ��0-360
---------------------------------------*/
void Write_Phase(uint8_t Channel,uint16_t Phase)
{
	uint16_t P_temp=0;
	P_temp=(uint16_t)Phase/360*(2^14);
	CPOW0_DATA[1]=(uint8_t)P_temp;
	CPOW0_DATA[0]=(uint8_t)(P_temp>>8);
	if(Channel==0)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1); 
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,1);
	}
	else if(Channel==1)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1); 
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,1);
	}
	else if(Channel==2)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1); 
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,1);
	}
	else if(Channel==3)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1); 
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,1);
	}
}

/*---------------------------------------
�������ܣ�Ƶ������ɨ��(32λ)
S0:  Ƶ�����
E0:  Ƶ���յ�
��Χ0~200 000 000 Hz��200MHz��
Channel:ͨ��
time:ÿ����ʱ�䣨8λ����Χ0-255
step��ɨ��Ĳ�����32λ��
direct������ 0��S0-E0 1:E0-S0
nodwell:��פ��λ
---------------------------------------*/
void Sweep_Frequence(uint32_t S0,uint32_t E0,uint8_t Channel,uint8_t time,uint32_t step,uint8_t direct,uint8_t nodwell)
{
	uint8_t 	CFR_DATA_Temp[3] = {0x00,0x00,0x00};
	uint8_t 	FR1_DATA_Temp[3] = {0x00,0x00,0x00};

	uint32_t 	DW,S0_Temp,E0_Temp;
	uint8_t S0_Temp_Array[4],E0_Temp_Array[4],DW_Temp_Array[4],Time_Temp_Array[2];	
	
	ReadData_AD9959(CFR_ADD,3,CFR_DATA_Temp);
	ReadData_AD9959(FR1_ADD,3,FR1_DATA_Temp);
	CFR_DATA_Temp[0] = CFR_DATA_Temp[0]|0x80;
	CFR_DATA_Temp[1] = CFR_DATA_Temp[1]|0x40;
	if(nodwell)
	CFR_DATA_Temp[1] = CFR_DATA_Temp[1]|0x80;	
	FR1_DATA_Temp[1] = FR1_DATA_Temp[1]&0xFC;
	
	DW = (uint32_t)step*8.589934592;
	S0_Temp = (uint32_t)S0*8.589934592;
	E0_Temp = (uint32_t)E0*8.589934592;

	S0_Temp_Array[3] = (uint8_t)S0_Temp;         // ��λ�ֽ�
	S0_Temp_Array[2] = (uint8_t)(S0_Temp >> 8);
	S0_Temp_Array[1] = (uint8_t)(S0_Temp >> 16);
	S0_Temp_Array[0] = (uint8_t)(S0_Temp >> 24); // ��λ�ֽ�
	
	E0_Temp_Array[3] = (uint8_t)E0_Temp;         // ��λ�ֽ�
	E0_Temp_Array[2] = (uint8_t)(E0_Temp >> 8);
	E0_Temp_Array[1] = (uint8_t)(E0_Temp >> 16);
	E0_Temp_Array[0] = (uint8_t)(E0_Temp >> 24); // ��λ�ֽ�
	
	DW_Temp_Array[3] = (uint8_t)DW;         // ��λ�ֽ�
	DW_Temp_Array[2] = (uint8_t)(DW >> 8);
	DW_Temp_Array[1] = (uint8_t)(DW >> 16);
	DW_Temp_Array[0] = (uint8_t)(DW >> 24); // ��λ�ֽ�
	
	Time_Temp_Array[0] = (uint8_t)time;
	Time_Temp_Array[1] = (uint8_t)time;
	
	if(direct == 0)
	{
		switch(Channel)
		{
			case 0:
				PS0_L;
				break;
			case 1:
				PS1_L;
				break;
			case 2:
				PS2_L;
				break;
			case 3:
				PS3_L;
				break;
			}
	}
	if(direct == 1)
	{
		switch(Channel)
		{
			case 0:
				PS0_H;
				break;
			case 1:
				PS1_H;
				break;
			case 2:
				PS2_H;
				break;
			case 3:
				PS3_H;
				break;
			}
	}
	if(Channel == 0)
	{
	WriteData_AD9959(CSR_ADD,1,CSR_DATA0,0); 
	WriteData_AD9959(CFR_ADD,3,CFR_DATA_Temp,1); 
	WriteData_AD9959(FR1_ADD,3,FR1_DATA_Temp,1); 
	WriteData_AD9959(CFTW0_ADD,4,S0_Temp_Array,0);
	WriteData_AD9959(0x0A,4,E0_Temp_Array,0);
		if(direct == 0)//S0-E0
		{
				WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,0);
				PS0_H;
		}
			else//E0-S0
			{
					WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,0);
					PS0_L;
			}
	}
	if(Channel == 1)
	{
	WriteData_AD9959(CSR_ADD,1,CSR_DATA1,0); 
	WriteData_AD9959(CFR_ADD,3,CFR_DATA_Temp,1); 
	WriteData_AD9959(FR1_ADD,3,FR1_DATA_Temp,1); 
	WriteData_AD9959(CFTW0_ADD,4,S0_Temp_Array,0);
	WriteData_AD9959(0x0A,4,E0_Temp_Array,0);
		if(direct == 0)//S0-E02
		{
				WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
				PS1_H;
		}
			else//E0-S0
			{
					WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);	
					WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
					PS1_L;
			}
	}
	if(Channel == 2)
	{
	WriteData_AD9959(CSR_ADD,1,CSR_DATA2,0); 
	WriteData_AD9959(CFR_ADD,3,CFR_DATA_Temp,1); 
	WriteData_AD9959(FR1_ADD,3,FR1_DATA_Temp,1); 
	WriteData_AD9959(CFTW0_ADD,4,S0_Temp_Array,0);
	WriteData_AD9959(0x0A,4,E0_Temp_Array,0);
		if(direct == 0)//S0-E0
		{
				WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
				PS2_H;
		}
			else//E0-S0
			{
					WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
					PS2_L;
			}
	}
	if(Channel == 3)
	{
	WriteData_AD9959(CSR_ADD,1,CSR_DATA3,0); 
	WriteData_AD9959(CFR_ADD,3,CFR_DATA_Temp,1); 
	WriteData_AD9959(FR1_ADD,3,FR1_DATA_Temp,1); 
	WriteData_AD9959(CFTW0_ADD,4,S0_Temp_Array,0);
	WriteData_AD9959(0x0A,4,E0_Temp_Array,0);
		if(direct == 0)//S0-E0
		{
				WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
				PS3_H;
		}
			else//E0-S0
			{
					WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
					PS3_L;
			}
	}
}
/*---------------------------------------
�������ܣ���������ɨ�裨10λ��
S0:  �������
E0:  �����յ�
��Χ0~1023
Channel:ͨ��
time:ÿ����ʱ�䣨8λ��
step��ɨ��Ĳ�����10λ��
direct������ 0��S0-E0 1:E0-S0
nodwell:��פ��λ
---------------------------------------*/
void Sweep_Amplitude(uint16_t S0,uint32_t E0,uint8_t Channel,uint8_t time,uint32_t step,uint8_t direct,uint8_t nodwell)
{
	uint8_t 	CFR_DATA_Temp[3] = {0x00,0x00,0x00};
	uint8_t 	FR1_DATA_Temp[3] = {0x00,0x00,0x00};

	uint32_t 	DW,S0_Temp,E0_Temp;
	uint8_t S0_Temp_Array[3],E0_Temp_Array[4],DW_Temp_Array[4],Time_Temp_Array[2];	
	
	ReadData_AD9959(CFR_ADD,3,CFR_DATA_Temp);
	ReadData_AD9959(FR1_ADD,3,FR1_DATA_Temp);
	CFR_DATA_Temp[0] = CFR_DATA_Temp[0]|0x40;
	CFR_DATA_Temp[1] = CFR_DATA_Temp[1]|0x40;
	if(nodwell)
	CFR_DATA_Temp[1] = CFR_DATA_Temp[1]|0x80;	
	FR1_DATA_Temp[1] = FR1_DATA_Temp[1]&0xFC;
	if (S0>1023)
	S0_Temp = 1023;
	else 
	S0_Temp = S0;
	if (E0>1023)
	E0_Temp = (uint32_t)1023<<22;
	else 
	E0_Temp = (uint32_t)E0<<22;
	if (step>1023)
	DW = (uint32_t)1023<<22;
	else 
	DW = (uint32_t)step<<22;
	
	
	S0_Temp_Array[2] = (uint8_t)S0_Temp;         // ��λ�ֽ�
	S0_Temp_Array[1] = (uint8_t)(S0_Temp >> 8);
	S0_Temp_Array[0] = ((uint8_t)(S0_Temp >> 16))&0x3F;
	
	E0_Temp_Array[3] = (uint8_t)E0_Temp;         // ��λ�ֽ�
	E0_Temp_Array[2] = (uint8_t)(E0_Temp >> 8);
	E0_Temp_Array[1] = (uint8_t)(E0_Temp >> 16);
	E0_Temp_Array[0] = (uint8_t)(E0_Temp >> 24); // ��λ�ֽ�
	
	DW_Temp_Array[3] = (uint8_t)DW;         // ��λ�ֽ�
	DW_Temp_Array[2] = (uint8_t)(DW >> 8);
	DW_Temp_Array[1] = (uint8_t)(DW >> 16);
	DW_Temp_Array[0] = (uint8_t)(DW >> 24); // ��λ�ֽ�
	
	Time_Temp_Array[0] = (uint8_t)time;
	Time_Temp_Array[1] = (uint8_t)time;
	
	if(direct == 0)
	{
		switch(Channel)
		{
			case 0:
				PS0_L;
				break;
			case 1:
				PS1_L;
				break;
			case 2:
				PS2_L;
				break;
			case 3:
				PS3_L;
				break;
			}
	}
	if(direct == 1)
	{
		switch(Channel)
		{
			case 0:
				PS0_H;
				break;
			case 1:
				PS1_H;
				break;
			case 2:
				PS2_H;
				break;
			case 3:
				PS3_H;
				break;
			}
	}
	if(Channel == 0)
	{
	WriteData_AD9959(CSR_ADD,1,CSR_DATA0,0); 
	WriteData_AD9959(CFR_ADD,3,CFR_DATA_Temp,1); 
	WriteData_AD9959(FR1_ADD,3,FR1_DATA_Temp,1); 
	WriteData_AD9959(ACR_ADD,3,S0_Temp_Array,0);
	WriteData_AD9959(0x0A,4,E0_Temp_Array,0);
		if(direct == 0)//S0-E0
		{
				WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,0);
				PS0_H;
		}
			else//E0-S0
			{
					WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,0);
					PS0_L;
			}
	}
	if(Channel == 1)
	{
	WriteData_AD9959(CSR_ADD,1,CSR_DATA1,0); 
	WriteData_AD9959(CFR_ADD,3,CFR_DATA_Temp,1); 
	WriteData_AD9959(FR1_ADD,3,FR1_DATA_Temp,1); 
	WriteData_AD9959(ACR_ADD,3,S0_Temp_Array,0);
	WriteData_AD9959(0x0A,4,E0_Temp_Array,0);
		if(direct == 0)//S0-E02
		{
				WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
				PS1_H;
		}
			else//E0-S0
			{
					WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);	
					WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
					PS1_L;
			}
	}
	if(Channel == 2)
	{
	WriteData_AD9959(CSR_ADD,1,CSR_DATA2,0); 
	WriteData_AD9959(CFR_ADD,3,CFR_DATA_Temp,1); 
	WriteData_AD9959(FR1_ADD,3,FR1_DATA_Temp,1); 
	WriteData_AD9959(ACR_ADD,3,S0_Temp_Array,0);
	WriteData_AD9959(0x0A,4,E0_Temp_Array,0);
		if(direct == 0)//S0-E0
		{
				WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
				PS2_H;
		}
			else//E0-S0
			{
					WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
					PS2_L;
			}
	}
	if(Channel == 3)
	{
	WriteData_AD9959(CSR_ADD,1,CSR_DATA3,0); 
	WriteData_AD9959(CFR_ADD,3,CFR_DATA_Temp,1); 
	WriteData_AD9959(FR1_ADD,3,FR1_DATA_Temp,1); 
	WriteData_AD9959(ACR_ADD,3,S0_Temp_Array,0);
	WriteData_AD9959(0x0A,4,E0_Temp_Array,0);
		if(direct == 0)//S0-E0
		{
				WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
				PS3_H;
		}
			else//E0-S0
			{
					WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
					PS3_L;
			}
	}
}
/*---------------------------------------
�������ܣ���λ����ɨ�裨14λ��
S0:  ��λ���
E0:  ��λ�յ�
��Χ0-16383
Channel:ͨ��
time:ÿ����ʱ�䣨8λ��
step��ɨ��Ĳ�����14λ��
direct������ 0��S0-E0 1:E0-S0
nodwell:��פ��λ
---------------------------------------*/
void Sweep_Phase(uint16_t S0,uint32_t E0,uint8_t Channel,uint8_t time,uint32_t step,uint8_t direct,uint8_t nodwell)
{

		uint8_t 	CFR_DATA_Temp[3] = {0x00,0x00,0x00};
	uint8_t 	FR1_DATA_Temp[3] = {0x00,0x00,0x00};

	uint32_t 	DW,S0_Temp,E0_Temp;
	uint8_t S0_Temp_Array[2],E0_Temp_Array[4],DW_Temp_Array[4],Time_Temp_Array[2];	
	
	ReadData_AD9959(CFR_ADD,3,CFR_DATA_Temp);
	ReadData_AD9959(FR1_ADD,3,FR1_DATA_Temp);
	CFR_DATA_Temp[0] = CFR_DATA_Temp[0]|0xC0;
	CFR_DATA_Temp[1] = CFR_DATA_Temp[1]|0x40;
	if(nodwell)
	CFR_DATA_Temp[1] = CFR_DATA_Temp[1]|0x80;	
	FR1_DATA_Temp[1] = FR1_DATA_Temp[1]&0xFC;
	if (S0>16383)
	S0_Temp = 16383;
	else 
	S0_Temp = S0;
	if (E0>16383)
	E0_Temp = (uint32_t)16383<<18;
	else 
	E0_Temp = (uint32_t)E0<<18;
	if (step>16383)
	DW = (uint32_t)16383<<18;
	else 
	DW = (uint32_t)step<<18;

	S0_Temp_Array[1] = (uint8_t)S0_Temp;         // ��λ�ֽ�
	S0_Temp_Array[0] = (uint8_t)(S0_Temp >> 8);

	E0_Temp_Array[3] = (uint8_t)E0_Temp;         // ��λ�ֽ�
	E0_Temp_Array[2] = (uint8_t)(E0_Temp >> 8);
	E0_Temp_Array[1] = (uint8_t)(E0_Temp >> 16);
	E0_Temp_Array[0] = (uint8_t)(E0_Temp >> 24); // ��λ�ֽ�
	
	DW_Temp_Array[3] = (uint8_t)DW;         // ��λ�ֽ�
	DW_Temp_Array[2] = (uint8_t)(DW >> 8);
	DW_Temp_Array[1] = (uint8_t)(DW >> 16);
	DW_Temp_Array[0] = (uint8_t)(DW >> 24); // ��λ�ֽ�
	
	Time_Temp_Array[0] = (uint8_t)time;
	Time_Temp_Array[1] = (uint8_t)time;
	
	if(direct == 0)
	{
		switch(Channel)
		{
			case 0:
				PS0_L;
				break;
			case 1:
				PS1_L;
				break;
			case 2:
				PS2_L;
				break;
			case 3:
				PS3_L;
				break;
			}
	}
	if(direct == 1)
	{
		switch(Channel)
		{
			case 0:
				PS0_H;
				break;
			case 1:
				PS1_H;
				break;
			case 2:
				PS2_H;
				break;
			case 3:
				PS3_H;
				break;
			}
	}
	if(Channel == 0)
	{
	WriteData_AD9959(CSR_ADD,1,CSR_DATA0,0); 
	WriteData_AD9959(CFR_ADD,3,CFR_DATA_Temp,1); 
	WriteData_AD9959(FR1_ADD,3,FR1_DATA_Temp,1); 
	WriteData_AD9959(CPOW0_ADD,2,S0_Temp_Array,0);
	WriteData_AD9959(0x0A,4,E0_Temp_Array,0);
		if(direct == 0)//S0-E0
		{
				WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,0);
				PS0_H;
		}
			else//E0-S0
			{
					WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,0);
					PS0_L;
			}
	}
	if(Channel == 1)
	{
	WriteData_AD9959(CSR_ADD,1,CSR_DATA1,0); 
	WriteData_AD9959(CFR_ADD,3,CFR_DATA_Temp,1); 
	WriteData_AD9959(FR1_ADD,3,FR1_DATA_Temp,1); 
	WriteData_AD9959(CPOW0_ADD,2,S0_Temp_Array,0);
	WriteData_AD9959(0x0A,4,E0_Temp_Array,0);
		if(direct == 0)//S0-E02
		{
				WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
				PS1_H;
		}
			else//E0-S0
			{
					WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);	
					WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
					PS1_L;
			}
	}
	if(Channel == 2)
	{
	WriteData_AD9959(CSR_ADD,1,CSR_DATA2,0); 
	WriteData_AD9959(CFR_ADD,3,CFR_DATA_Temp,1); 
	WriteData_AD9959(FR1_ADD,3,FR1_DATA_Temp,1); 
	WriteData_AD9959(CPOW0_ADD,2,S0_Temp_Array,0);
	WriteData_AD9959(0x0A,4,E0_Temp_Array,0);
		if(direct == 0)//S0-E0
		{
				WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
				PS2_H;
		}
			else//E0-S0
			{
					WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
					PS2_L;
			}
	}
	if(Channel == 3)
	{
	WriteData_AD9959(CSR_ADD,1,CSR_DATA3,0); 
	WriteData_AD9959(CFR_ADD,3,CFR_DATA_Temp,1); 
	WriteData_AD9959(FR1_ADD,3,FR1_DATA_Temp,1); 
	WriteData_AD9959(CPOW0_ADD,2,S0_Temp_Array,0);
	WriteData_AD9959(0x0A,4,E0_Temp_Array,0);
		if(direct == 0)//S0-E0
		{
				WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
				WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
				PS3_H;
		}
			else//E0-S0
			{
					WriteData_AD9959(RDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(FDW_ADD,4,DW_Temp_Array,0);
					WriteData_AD9959(LSRR_ADD,2,Time_Temp_Array,1);
					PS3_L;
			}
	}
}


uint32_t Get_Freq(void)
{
    uint8_t data[4] = {0};
    ReadData_AD9959(CFTW0_ADD, 4, data);
    int Freq = (data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3]) / 8.589934592;
    return Freq;
}

uint8_t Get_Amp(void)
{
    uint8_t data[3] = {0};
    ReadData_AD9959(ACR_ADD, 3, data);
    return data[2];
}
/*---------------------------------------
�������ܣ���ȡ����
RegisterAddress:�Ĵ�����ַ
NumberofRegisters:��ȡ��λ��
RegisterData:���ݴ洢��ַ
---------------------------------------*/
void ReadData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData)
{
    uint8_t ControlValue = 0;
    uint8_t ValueToRead = 0;
    uint8_t RegisterIndex = 0;
    uint8_t i = 0;
    ControlValue = RegisterAddress | 0x80;
    // Write address
    CS_L;
    SCLK_L;
    delay1(0x20);
    for (i = 0; i < 8; i++)
    {
        SCLK_L;
        if(0x80 == (ControlValue & 0x80))
            SDIO0_H;
        if(0x80 != (ControlValue & 0x80))	
           SDIO0_L;
        SCLK_H;
        ControlValue <<= 1;
        delay1(2);
    }

    SCLK_L

    // �л�������ģʽ
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.GPIO_Pin = SD0_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SD0_GPIO_Port, &GPIO_InitStruct);
    delay1(0x20);
    // Read data
    // ÿ���ֽڵ����ݶ��ǴӸ�λ��ʼ����
    // ��һ��������ֽ������λ���ֽ�
    for (RegisterIndex = 0; RegisterIndex < NumberofRegisters; RegisterIndex++)
    {
        ValueToRead = 0;
        for (i = 0; i < 8; i++)
        {
            ValueToRead <<= 1;
            SCLK_H;
            if (GPIO_ReadOutputDataBit(SD0_GPIO_Port,SD0_Pin) == 1)
                ValueToRead |= 0x01;
            else
                ValueToRead &= 0xFE;
            SCLK_L;
        }
        SCLK_L;
        RegisterData[RegisterIndex] = ValueToRead;
    }
    delay1(0x20);
    CS_H;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(SD0_GPIO_Port, &GPIO_InitStruct);
}









