#ifndef __BH1750_H__
#define __BH1750_H__


#include "sys.h"


//IO操作函数	 
#define IICC_SCL(X)  GPIO_WriteBit(GPIOA,GPIO_Pin_8,(BitAction)X) 	//SCL
#define IICC_SDA(X)  GPIO_WriteBit(GPIOC,GPIO_Pin_9,(BitAction)X) 	//SDA	 
#define READC_SDA()  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)		//PCin(9)  //输入SDA 
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

//IIC所有操作函数
void IICC_Init(void);                		//初始化IIC的IO口	
void SDAC_OUT(void);						//PA4输出
void SDAC_IN(void);  						//PA4输入
void IICC_Start(void);					//发送IIC开始信号
void IICC_Stop(void);	  					//发送IIC停止信号
void IICC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t IICC_Read_Byte(unsigned char ack);	//IIC读取一个字节
uint8_t IICC_Wait_Ack(void); 				//IIC等待ACK信号
void IICC_Ack(void);						//IIC发送ACK信号
void IICC_NAck(void);						//IIC不发送ACK信号

void IICC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IICC_Read_One_Byte(uint8_t daddr,uint8_t addr);
   

#define BH1750_Addr 0x46				//0100 0110   即地址位 = 0；
#define BH1750_ON   0x01
#define BH1750_CON  0x10
#define BH1750_ONE  0x20
#define BH1750_RSET 0x07

void BH1750_Init(void);					//IO初始化，
void Start_BH1750(void);				//上电，设置清除数据寄存器
void Read_BH1750(void);					//连续的读取内部寄存器数据
float Convert_BH1750(void);
void BH1750_Test(void); 
extern float temp;
char ReadAndDisplayBH1750(void);
extern uint16_t result1750;
#endif




