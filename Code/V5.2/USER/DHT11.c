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

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // ����ʱ��ʹ�ܺ���

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

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // ����ʱ��ʹ�ܺ���

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//u8 DHT11_Init(void)
//{
//	
//	DHT11_GPIO_Init_OUT();  // ��������Ϊ���ģʽ
//	DHT11_GPIO_Init_IN();  // ��������Ϊ���ģʽ
//}




//�������Ϳ�ʼ�ź�
void DHT11_Start(void)
{
	DHT11_GPIO_Init_OUT(); //���ģʽ
	
	dht11_high; //������
	delay_us(30);
	
	dht11_low; //���͵�ƽ����18us
	delay_ms(20);
	
	dht11_high; //���ߵ�ƽ20~40us
	delay_us(30);
	
	DHT11_GPIO_Init_IN(); //����ģʽ
}


//��ȡһ���ֽ�
char DHT11_Rec_Byte(void)
{
	unsigned char i = 0;
	unsigned char data;
	
	for(i=0;i<8;i++) //1�����ݾ���1���ֽ�byte��1���ֽ�byte��8λbit
	{
		while( Read_Data == 0); //��1bit��ʼ���͵�ƽ��ߵ�ƽ���ȴ��͵�ƽ����
		delay_us(30); //�ӳ�30us��Ϊ����������0������1��0ֻ��26~28us
		
		data <<= 1; //����
		
		if( Read_Data == 1 ) //�������30us���Ǹߵ�ƽ�Ļ���������1
		{
			data |= 1; //����+1
		}
		
		while( Read_Data == 1 ); //�ߵ�ƽ��͵�ƽ���ȴ��ߵ�ƽ����
	}
	
	return data;
}

//��ȡ����

void DHT11_REC_Data(void)
{
    float R_H, R_L, T_H, T_L;
    unsigned int  RH, RL, TH, TL, CHECK;

    DHT11_Start(); // ���������ź�
    dht11_high; // ���ߵ�ƽ

    if (Read_Data == 0) // �ж�DHT11�Ƿ���Ӧ
    {
        while (Read_Data == 0); // �ȴ��͵�ƽ����
        while (Read_Data == 1); // �ȴ��ߵ�ƽ����

        R_H = DHT11_Rec_Byte();
        R_L = DHT11_Rec_Byte();
        T_H = DHT11_Rec_Byte();
        T_L = DHT11_Rec_Byte();
        CHECK = DHT11_Rec_Byte(); // ����5������

        dht11_low; // ��������
        delay_us(55);
        dht11_high; // �������״̬

		if (R_H + R_L + T_H + T_L == CHECK) // У������
		{
            
			
			if ( R_H <= 100 && T_H <= 100 && R_L <= 100 && T_H <= 100)
			{
				
				last_RH = R_H + 10; // ��������ֵ
				last_TH = T_H - 7;
				last_R_L = R_L;
				last_T_L = T_L;
				rec_data[0] = R_H + 10 ; // ����ʪ�ȵ���������
				rec_data[2] = T_H - 7 ; // �����¶ȵ���������
				rec_data[1] = R_L; // ʪ�ȵ�С������
				rec_data[3] = T_L; // �¶ȵ�С������
			}
			else
			{
				// ���������Ч����ʹ���ϴε�ֵ
				rec_data[0] = last_RH;
				rec_data[2] = last_TH;
				rec_data[1] = last_R_L;
				rec_data[3] = last_T_L;
			}

        }
        else
        {
            // ���У��ʧ�ܣ�Ҳʹ���ϴε�ֵ
            rec_data[0] = last_RH;
            rec_data[2] = last_TH;
			rec_data[1] = last_R_L;
			rec_data[3] = last_T_L;
        }
	}
    else
    {
        // ���DHT11û����Ӧ��Ҳʹ���ϴε�ֵ
        rec_data[0] = last_RH;
        rec_data[2] = last_TH;
		rec_data[1] = last_R_L;
		rec_data[3] = last_T_L;
    }
}








