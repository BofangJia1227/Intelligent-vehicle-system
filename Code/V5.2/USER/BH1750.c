/**************************************************************************
 * 文件名  ：bh1750.c
 * 描述    ：光强传感模块     
****************************************************************************/
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"
#include "delay.h"
#include "BH1750.h"
#include "led.h"
float result_lx=0;
uint8_t BUF[2]={0};
uint16_t result1750;
extern unsigned char tmp_buf[16];
float temp;

/*********************************************************
功  能：初始化IIC
参  数: 无
返回值: 无
**********************************************************/
void IICC_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//使能GPIOA/C时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	//使能GPIOA/C时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//初始化

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);				//初始化

    IICC_SCL(1);
    IICC_SDA(1);
}

/**************************************************************
*功  能：配置输出端口
*参  数: 无
*返回值: 无
**************************************************************/
void SDAC_OUT(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;					//PA4端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		 	//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);				 	//初始化IO口
}

/**************************************************************
*功  能：配置输入端口
*参  数: 无
*返回值: 无
**************************************************************/
void SDAC_IN(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //PA4端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);				 	//初始化IO口

}

/*********************************************************
功  能：产生IIC起始信号
参  数: 无
返回值: 无
**********************************************************/
void IICC_Start(void)
{
    SDAC_OUT();     //sda线输出
    IICC_SDA(1);
    IICC_SCL(1);
    delay_us(4);
    IICC_SDA(0);//START:when CLK is high,DATA change form high to low
    delay_us(4);
    IICC_SCL(0);//钳住I2C总线，准备发送或接收数据
}

/*********************************************************
功  能：产生IIC停止信号
参  数: 无
返回值: 无
**********************************************************/
void IICC_Stop(void)
{
    SDAC_OUT();//sda线输出
    IICC_SCL(0);
    IICC_SDA(0);//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    IICC_SCL(1);
    IICC_SDA(1);//发送I2C总线结束信号
    delay_us(4);
}

/*********************************************************
功  能：等待应答信号到来
参  数: 无
返回值: 1，接收应答失败
        0，接收应答成功
**********************************************************/
uint8_t IICC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;

    SDAC_IN();      //SDA设置为输入
    IICC_SDA(1);
    delay_us(1);
    IICC_SCL(1);
    delay_us(1);

    while(READC_SDA())
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IICC_Stop();
            return 1;
        }
    }
    IICC_SCL(0);//时钟输出0
    return 0;
}

/*********************************************************
功  能：产生ACK应答
参  数: 无
返回值: 无
**********************************************************/
void IICC_Ack(void)
{
    IICC_SCL(0);
    SDAC_OUT();
    IICC_SDA(0);
    delay_us(2);
    IICC_SCL(1);
    delay_us(2);
    IICC_SCL(0);
}

/*********************************************************
功  能：不产生ACK应答
参  数: 无
返回值: 无
**********************************************************/
void IICC_NAck(void)
{
    IICC_SCL(0);
    SDAC_OUT();
    IICC_SDA(1);
    delay_us(2);
    IICC_SCL(1);
    delay_us(2);
    IICC_SCL(0);
}

/*********************************************************
功  能：IIC发送一个字节
参  数: 无
返回值: 从机有无应答
		1，有应答
		0，无应答
**********************************************************/
void IICC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDAC_OUT();
    IICC_SCL(0);//拉低时钟开始数据传输
    for(t=0; t<8; t++)
    {
        IICC_SDA((txd&0x80)>>7);
        txd<<=1;
        delay_us(2);   //对TEA5767这三个延时都是必须的
        IICC_SCL(1);
        delay_us(2);
        IICC_SCL(0);
        delay_us(2);
    }
}

/*********************************************************
功  能：读1个字节
参  数: ack=1时，ack=0时
返回值: 发送ACK，发送nACK
**********************************************************/
uint8_t IICC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDAC_IN();//SDA设置为输入
    for(i=0; i<8; i++ )
    {
        IICC_SCL(0);
        delay_us(2);
        IICC_SCL(1);
        receive<<=1;
        if(READC_SDA())receive++;
        delay_us(1);
    }
    if (!ack)
        IICC_NAck();//发送nACK
    else
        IICC_Ack(); //发送ACK
    return receive;
}




/**************************************************************
*功  能：端口初始化
*参  数: 无
*返回值: 无 
**************************************************************/
void BH1750_Init(void)
{
	IICC_Init(); 
}

/**************************************************************
*功  能：发送设备地址
*参  数: 无
*返回值: 无 
**************************************************************/
void Cmd_Write_BH1750(uint8_t cmd)
{
    IICC_Start();						//起始信号
    IICC_Send_Byte(BH1750_Addr+0);		//发送设备地址+写信号
	while(IICC_Wait_Ack());
    IICC_Send_Byte(cmd);				//内部寄存器地址
	while(IICC_Wait_Ack());
    IICC_Stop();						//发送停止信号
	delay_ms(5);
}


/**************************************************************
*功  能：开启一次H分辨率模式
*参  数: 无
*返回值: 无 
**************************************************************/
void Start_BH1750(void)
{
	Cmd_Write_BH1750(BH1750_ON);	 		//power on
	Cmd_Write_BH1750(BH1750_RSET);		//clear
	Cmd_Write_BH1750(BH1750_ONE);  		//一次H分辨率模式，至少120ms，之后自动断电模式  
}

/**************************************************************
*功  能：读光照信号
*参  数: 无
*返回值: 无 
**************************************************************/
void Read_BH1750(void)
{
    IICC_Start();							//起始信号
    IICC_Send_Byte(BH1750_Addr+1);		//发送设备地址+读信号
	while(IICC_Wait_Ack());
	BUF[0]=IICC_Read_Byte(1);				//发送ACK
	BUF[1]=IICC_Read_Byte(0);				//发送NACK
    IICC_Stop();							//停止信号
    delay_ms(5);
}

/**************************************************************
*功  能：合成光照数据
*参  数: 无
*返回值: 无 
**************************************************************/
float Convert_BH1750(void)
{
	Start_BH1750();          // 启动 BH1750 测量
    delay_ms(120);           // 等待测量完成
    Read_BH1750();           // 读取 BH1750 数据
	result1750=BUF[0];
	result1750=(result1750<<8)+BUF[1];		//合成数据，即光照数据
	result1750=(double)result1750/1.2;
	if(result1750 >= 2000)
	{
		
		GPIO_SetBits(GPIOA,GPIO_Pin_8);//置高电平，蜂鸣器响
	}
	return result1750;
}
