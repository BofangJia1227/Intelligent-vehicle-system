/**************************************************************************
 * �ļ���  ��bh1750.c
 * ����    ����ǿ����ģ��     
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
��  �ܣ���ʼ��IIC
��  ��: ��
����ֵ: ��
**********************************************************/
void IICC_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//ʹ��GPIOA/Cʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	//ʹ��GPIOA/Cʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//��ʼ��

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);				//��ʼ��

    IICC_SCL(1);
    IICC_SDA(1);
}

/**************************************************************
*��  �ܣ���������˿�
*��  ��: ��
*����ֵ: ��
**************************************************************/
void SDAC_OUT(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;					//PA4�˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		 	//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);				 	//��ʼ��IO��
}

/**************************************************************
*��  �ܣ���������˿�
*��  ��: ��
*����ֵ: ��
**************************************************************/
void SDAC_IN(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //PA4�˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);				 	//��ʼ��IO��

}

/*********************************************************
��  �ܣ�����IIC��ʼ�ź�
��  ��: ��
����ֵ: ��
**********************************************************/
void IICC_Start(void)
{
    SDAC_OUT();     //sda�����
    IICC_SDA(1);
    IICC_SCL(1);
    delay_us(4);
    IICC_SDA(0);//START:when CLK is high,DATA change form high to low
    delay_us(4);
    IICC_SCL(0);//ǯסI2C���ߣ�׼�����ͻ��������
}

/*********************************************************
��  �ܣ�����IICֹͣ�ź�
��  ��: ��
����ֵ: ��
**********************************************************/
void IICC_Stop(void)
{
    SDAC_OUT();//sda�����
    IICC_SCL(0);
    IICC_SDA(0);//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    IICC_SCL(1);
    IICC_SDA(1);//����I2C���߽����ź�
    delay_us(4);
}

/*********************************************************
��  �ܣ��ȴ�Ӧ���źŵ���
��  ��: ��
����ֵ: 1������Ӧ��ʧ��
        0������Ӧ��ɹ�
**********************************************************/
uint8_t IICC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;

    SDAC_IN();      //SDA����Ϊ����
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
    IICC_SCL(0);//ʱ�����0
    return 0;
}

/*********************************************************
��  �ܣ�����ACKӦ��
��  ��: ��
����ֵ: ��
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
��  �ܣ�������ACKӦ��
��  ��: ��
����ֵ: ��
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
��  �ܣ�IIC����һ���ֽ�
��  ��: ��
����ֵ: �ӻ�����Ӧ��
		1����Ӧ��
		0����Ӧ��
**********************************************************/
void IICC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDAC_OUT();
    IICC_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0; t<8; t++)
    {
        IICC_SDA((txd&0x80)>>7);
        txd<<=1;
        delay_us(2);   //��TEA5767��������ʱ���Ǳ����
        IICC_SCL(1);
        delay_us(2);
        IICC_SCL(0);
        delay_us(2);
    }
}

/*********************************************************
��  �ܣ���1���ֽ�
��  ��: ack=1ʱ��ack=0ʱ
����ֵ: ����ACK������nACK
**********************************************************/
uint8_t IICC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDAC_IN();//SDA����Ϊ����
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
        IICC_NAck();//����nACK
    else
        IICC_Ack(); //����ACK
    return receive;
}




/**************************************************************
*��  �ܣ��˿ڳ�ʼ��
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void BH1750_Init(void)
{
	IICC_Init(); 
}

/**************************************************************
*��  �ܣ������豸��ַ
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void Cmd_Write_BH1750(uint8_t cmd)
{
    IICC_Start();						//��ʼ�ź�
    IICC_Send_Byte(BH1750_Addr+0);		//�����豸��ַ+д�ź�
	while(IICC_Wait_Ack());
    IICC_Send_Byte(cmd);				//�ڲ��Ĵ�����ַ
	while(IICC_Wait_Ack());
    IICC_Stop();						//����ֹͣ�ź�
	delay_ms(5);
}


/**************************************************************
*��  �ܣ�����һ��H�ֱ���ģʽ
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void Start_BH1750(void)
{
	Cmd_Write_BH1750(BH1750_ON);	 		//power on
	Cmd_Write_BH1750(BH1750_RSET);		//clear
	Cmd_Write_BH1750(BH1750_ONE);  		//һ��H�ֱ���ģʽ������120ms��֮���Զ��ϵ�ģʽ  
}

/**************************************************************
*��  �ܣ��������ź�
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void Read_BH1750(void)
{
    IICC_Start();							//��ʼ�ź�
    IICC_Send_Byte(BH1750_Addr+1);		//�����豸��ַ+���ź�
	while(IICC_Wait_Ack());
	BUF[0]=IICC_Read_Byte(1);				//����ACK
	BUF[1]=IICC_Read_Byte(0);				//����NACK
    IICC_Stop();							//ֹͣ�ź�
    delay_ms(5);
}

/**************************************************************
*��  �ܣ��ϳɹ�������
*��  ��: ��
*����ֵ: �� 
**************************************************************/
float Convert_BH1750(void)
{
	Start_BH1750();          // ���� BH1750 ����
    delay_ms(120);           // �ȴ��������
    Read_BH1750();           // ��ȡ BH1750 ����
	result1750=BUF[0];
	result1750=(result1750<<8)+BUF[1];		//�ϳ����ݣ�����������
	result1750=(double)result1750/1.2;
	if(result1750 >= 2000)
	{
		
		GPIO_SetBits(GPIOA,GPIO_Pin_8);//�øߵ�ƽ����������
	}
	return result1750;
}
