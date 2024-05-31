#ifndef __BH1750_H__
#define __BH1750_H__


#include "sys.h"


//IO��������	 
#define IICC_SCL(X)  GPIO_WriteBit(GPIOA,GPIO_Pin_8,(BitAction)X) 	//SCL
#define IICC_SDA(X)  GPIO_WriteBit(GPIOC,GPIO_Pin_9,(BitAction)X) 	//SDA	 
#define READC_SDA()  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)		//PCin(9)  //����SDA 
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

//IIC���в�������
void IICC_Init(void);                		//��ʼ��IIC��IO��	
void SDAC_OUT(void);						//PA4���
void SDAC_IN(void);  						//PA4����
void IICC_Start(void);					//����IIC��ʼ�ź�
void IICC_Stop(void);	  					//����IICֹͣ�ź�
void IICC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t IICC_Read_Byte(unsigned char ack);	//IIC��ȡһ���ֽ�
uint8_t IICC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IICC_Ack(void);						//IIC����ACK�ź�
void IICC_NAck(void);						//IIC������ACK�ź�

void IICC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IICC_Read_One_Byte(uint8_t daddr,uint8_t addr);
   

#define BH1750_Addr 0x46				//0100 0110   ����ַλ = 0��
#define BH1750_ON   0x01
#define BH1750_CON  0x10
#define BH1750_ONE  0x20
#define BH1750_RSET 0x07

void BH1750_Init(void);					//IO��ʼ����
void Start_BH1750(void);				//�ϵ磬����������ݼĴ���
void Read_BH1750(void);					//�����Ķ�ȡ�ڲ��Ĵ�������
float Convert_BH1750(void);
void BH1750_Test(void); 
extern float temp;
char ReadAndDisplayBH1750(void);
extern uint16_t result1750;
#endif




