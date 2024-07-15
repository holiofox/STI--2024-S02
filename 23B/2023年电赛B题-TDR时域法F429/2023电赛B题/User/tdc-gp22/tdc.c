#include "./tdc-gp22/tdc.h"
#include "./tim/bsp_general_tim.h"


static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   

static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

/*SPI通信-开头****************************/
void delay(uint8_t t) {
    volatile uint32_t delay = t*2;
    while (delay > 0) {
        delay--;
    }
}
void send1(void) {
    SCK(1);
    delay(1);
    SI(1);
    delay(1);
    SCK(0);
    delay(1);
    //delay(1);
}

void send0(void) {
    SCK(1);
    delay(1);
    SI(0);
    delay(1);
    SCK(0);
    delay(1);
    //delay(1);
}


void write8(uint8_t wbuf8) {
    uint8_t cnt;
    uint8_t MSB8 = 0x80;
    SPI_TDC_CS_LOW();
    for (cnt = 8; cnt > 0; cnt--) {
        if ((wbuf8 & MSB8) > 0)
            send1();
        else
            send0();
        MSB8 /= 2;
    }
    delay(1);//这里没有SPI关闭命令，在测试程序中代码关闭
}

uint8_t read8(void) {
    uint8_t cnt;
    uint8_t LSB8 = 0x80;
    uint8_t rbuf8 = 0x00;
    SPI_TDC_CS_LOW();
    delay(3);
    for (cnt = 8; cnt > 0; cnt--) {
        //rbuf8 <<= 1;
        SCK(1);
        delay(3);

        if (SO())
            rbuf8 |= LSB8;
        LSB8 /= 2;
        delay(1);
        SCK(0);
        delay(10);
    }
    delay(3);
    SPI_TDC_CS_HIGH();
    return rbuf8;
}

uint32_t read32(void) {
    uint8_t cnt;
    uint32_t LSB32 = 0x80000000;
    uint32_t rbuf32 = 0x00000000;
    SPI_TDC_CS_LOW();
    delay(1);
    for (cnt = 32; cnt > 0; cnt--) {
        //rbuf32 <<=1;
        SCK(1);
        delay(3);
        if (SO())
            rbuf32 |= LSB32;
        LSB32 /= 2;
        delay(1);
        SCK(0);
        delay(3);
    }
    delay(3);
    SPI_TDC_CS_HIGH();
    return rbuf32;
}

 
void write32(uint32_t wbuf32) {
    uint8_t cnt;
    uint32_t MSB32 = 0x80000000;
    SPI_TDC_CS_LOW();
    delay(1);
    for (cnt = 32; cnt > 0; cnt--) {
        if ((wbuf32 & MSB32) > 0)
            send1();
        else
            send0();
        MSB32 /= 2;
    }
    delay(1);
    SPI_TDC_CS_HIGH();
}

void reset(void) {
    RTN(1);
    delay(1);
    RTN(0);
    delay(5);
    RTN(1);
    delay(1);
}

/*SPI通信-结尾****************************/

/**
  * @brief  SPI_TDC_GPIO_Init初始化
  * @param  无
  * @retval 无
  */
void SPI_TDC_GPIO_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* 使能 TDC_SPI 及GPIO 时钟 */
  /*!< SPI_TDC_SPI_CS_GPIO, SPI_TDC_SPI_MOSI_GPIO, 
       SPI_TDC_SPI_MISO_GPIO,SPI_TDC_SPI_SCK_GPIO 时钟使能 */
  RCC_AHB1PeriphClockCmd (TDC_SPI_SCK_GPIO_CLK | TDC_SPI_MISO_GPIO_CLK|TDC_SPI_MOSI_GPIO_CLK|TDC_CS_GPIO_CLK|TDC_INT_GPIO_CLK|TDC_RTN_GPIO_CLK, ENABLE);

  ;


  /*!< 配置 SPI_TDC_SPI 引脚: SCK */
  GPIO_InitStructure.GPIO_Pin = TDC_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
  
  GPIO_Init(TDC_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
  
	/*!< 配置 SPI_TDC_SPI 引脚: MISO */
  GPIO_InitStructure.GPIO_Pin = TDC_SPI_MISO_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
		
  GPIO_Init(TDC_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
  
	/*!< 配置 SPI_TDC_SPI 引脚: MOSI */
 GPIO_InitStructure.GPIO_Pin = TDC_SPI_MOSI_PIN;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
 
 GPIO_Init(TDC_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
	/*!< 配置 SPI_TDC_SPI 引脚: CS */
  GPIO_InitStructure.GPIO_Pin = TDC_CS_PIN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
  GPIO_Init(TDC_CS_GPIO_PORT, &GPIO_InitStructure);
	  /* 停止信号 TDC: CS引脚高电平*/
  SPI_TDC_CS_HIGH();

	/* 配置 SPI_TDC_SPI 引脚: INT */

  GPIO_InitStructure.GPIO_Pin = TDC_INT_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(TDC_INT_GPIO_Port, &GPIO_InitStructure);

/* 配置 SPI_TDC_SPI 引脚: RTN */
  GPIO_InitStructure.GPIO_Pin = TDC_RTN_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
  GPIO_Init(TDC_RTN_GPIO_Port, &GPIO_InitStructure);
	/* 配置 SPI_TDC_SPI 引脚: START */
 GPIO_InitStructure.GPIO_Pin = TDC_START_Pin;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 GPIO_Init(TDC_START_GPIO_Port, &GPIO_InitStructure);
 
 	 /*Configure GPIO pin Output Level */
  GPIO_WriteBit(TDC_SPI_SCK_GPIO_PORT, TDC_SPI_SCK_PIN|TDC_CS_PIN, 0);

 /*Configure GPIO pin Output Level */
  GPIO_WriteBit(TDC_SPI_MOSI_GPIO_PORT, TDC_SPI_MOSI_PIN, 0);

 /*Configure GPIO pin Output Level */
  GPIO_WriteBit(TDC_RTN_GPIO_Port, TDC_RTN_Pin, 0);
  GPIO_WriteBit(TDC_START_GPIO_Port, TDC_START_Pin, 1);
}
void TDC_Init() {
	
	  SPI_TDC_GPIO_Init();
    reset();
    write8(Power_On_Reset);
		delay(1);
	  SPI_TDC_CS_HIGH();
	  write32(TDC_REG0);  //校准陶瓷晶振时间为 8 个 32k 周期,244.14us.
//设置高速晶振上电后一直起振.设置测量范围 1,自动校准,上升沿敏感
    delay(1);
    write32(TDC_REG1);//stop1 和 stop2 通道分别接受一个脉冲， 定义计算方法.
//用stop2 的第一个脉冲减去 stop1 的第一个脉冲。
    delay(1);
    write32(TDC_REG2);//开启所有中断源
    delay(1);
    write32(TDC_REG3);
    delay(1);
    write32(TDC_REG4);
    delay(1);
    write32(TDC_REG5);
    delay(1);
	  write8(0x70);
    delay(1);
//		SPI_TDC_CS_HIGH();
}

uint32_t TDC_Get_Status_Reg()
{
    uint32_t reg;

//    while(INTN) //判断中断置位否
//        delay_us(1);

    write8(0xb4);
    delay(1);
    reg = read32();

    return reg;
}

uint8_t TDC_Test() {
    uint32_t test_reg;
SPI_TDC_CS_HIGH();
	  RTN(1);
    write32(0x81868642); //INTN的脉冲和这个有关。
    delay(100);
	//  SPI_TDC_CS_LOW();
    write8(0xB5);
    delay(10);
    test_reg = read8();

	  delay(10);
    SPI_TDC_CS_HIGH();

    return test_reg;
}

float fixed2float(uint32_t fixedPoint) {
    int16_t integerPart = (fixedPoint >> 16) & 0xFFFF;  // 获取高16位整数部分
    uint16_t fractionalPart = fixedPoint & 0xFFFF;      // 获取低16位小数部分

    float result = (float)integerPart + ((float)fractionalPart / 65536.0f);  // 将小数部分除以2^16转换为浮点数

    return result;
}

float TDC_to_ns(uint32_t val) {
    float val_f = fixed2float(val);
    return val_f / Fe9;
}

uint8_t TDC_Measure(uint32_t *result) {

    write8(0x70);
    delay(1);
    SPI_TDC_CS_HIGH();
   // write8(Start_Cal_Resonator);
/* 设置 TDC_START 引脚为低电平 */
GPIO_WriteBit(TDC_START_GPIO_Port, TDC_START_Pin, 0);

//for(int i=0;i<1;i++){}
/* 设置 TDC_START 引脚为高电平 */
GPIO_WriteBit(TDC_START_GPIO_Port, TDC_START_Pin, 1);
  
    while (INT()) {
        delay(1);
    }

    delay(5);
    write8(0xB0); //READ REG0
    delay(5);
    *result = read32();



    return 0; // 测量成功
}

//float convert(uint32_t val) {
//    unsigned int z, x;
//    float sum, num = 2.0;
//
//    z = (val >> 16);
//    x = (val & 0xffff);
//
//    for (int i = 0; i < 16; i++) {
//        num *= 2.0;
//        if (x & 0x8000) {
//            sum += (1.0 / num);
//        }
//        x <<= 1;
//    }
//
//    return ((z + sum) * 250.0); //使用的是4Mhz晶振    显示的时间单位是ns
//}
