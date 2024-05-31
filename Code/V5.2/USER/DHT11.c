#include "DHT11.h"
#include "delay.h"
#include "stm32f4xx.h"
#include "system.h"

float rec_data[4];
unsigned char last_R_L = 0, last_T_L = 0;
unsigned char last_RH = 0, last_TH = 0;

void DHT11_GPIO_Init_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // 更新时钟使能函数

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void DHT11_GPIO_Init_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // 更新时钟使能函数

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//u8 DHT11_Init(void)
//{
//	
//	DHT11_GPIO_Init_OUT();  // 首先设置为输出模式
//	DHT11_GPIO_Init_IN();  // 首先设置为输出模式
//}




//主机发送开始信号
void DHT11_Start(void)
{
	DHT11_GPIO_Init_OUT(); //输出模式
	
	dht11_high; //先拉高
	delay_us(30);
	
	dht11_low; //拉低电平至少18us
	delay_ms(20);
	
	dht11_high; //拉高电平20~40us
	delay_us(30);
	
	DHT11_GPIO_Init_IN(); //输入模式
}


//获取一个字节
char DHT11_Rec_Byte(void)
{
	unsigned char i = 0;
	unsigned char data;
	
	for(i=0;i<8;i++) //1个数据就是1个字节byte，1个字节byte有8位bit
	{
		while( Read_Data == 0); //从1bit开始，低电平变高电平，等待低电平结束
		delay_us(30); //延迟30us是为了区别数据0和数据1，0只有26~28us
		
		data <<= 1; //左移
		
		if( Read_Data == 1 ) //如果过了30us还是高电平的话就是数据1
		{
			data |= 1; //数据+1
		}
		
		while( Read_Data == 1 ); //高电平变低电平，等待高电平结束
	}
	
	return data;
}

//获取数据

void DHT11_REC_Data(void)
{
    float R_H, R_L, T_H, T_L;
    unsigned int  RH, RL, TH, TL, CHECK;

    DHT11_Start(); // 主机发送信号
    dht11_high; // 拉高电平

    if (Read_Data == 0) // 判断DHT11是否响应
    {
        while (Read_Data == 0); // 等待低电平结束
        while (Read_Data == 1); // 等待高电平结束

        R_H = DHT11_Rec_Byte();
        R_L = DHT11_Rec_Byte();
        T_H = DHT11_Rec_Byte();
        T_L = DHT11_Rec_Byte();
        CHECK = DHT11_Rec_Byte(); // 接收5个数据

        dht11_low; // 拉低总线
        delay_us(55);
        dht11_high; // 进入空闲状态

		if (R_H + R_L + T_H + T_L == CHECK) // 校验数据
		{
            
			
			if ( R_H <= 100 && T_H <= 100 && R_L <= 100 && T_H <= 100)
			{
				
				last_RH = R_H + 10; // 更新最后的值
				last_TH = T_H - 7;
				last_R_L = R_L;
				last_T_L = T_L;
				rec_data[0] = R_H + 10 ; // 计算湿度的整数部分
				rec_data[2] = T_H - 7 ; // 计算温度的整数部分
				rec_data[1] = R_L; // 湿度的小数部分
				rec_data[3] = T_L; // 温度的小数部分
			}
			else
			{
				// 如果数据无效，则使用上次的值
				rec_data[0] = last_RH;
				rec_data[2] = last_TH;
				rec_data[1] = last_R_L;
				rec_data[3] = last_T_L;
			}

        }
        else
        {
            // 如果校验失败，也使用上次的值
            rec_data[0] = last_RH;
            rec_data[2] = last_TH;
			rec_data[1] = last_R_L;
			rec_data[3] = last_T_L;
        }
	}
    else
    {
        // 如果DHT11没有响应，也使用上次的值
        rec_data[0] = last_RH;
        rec_data[2] = last_TH;
		rec_data[1] = last_R_L;
		rec_data[3] = last_T_L;
    }
}








