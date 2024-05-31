#ifndef _DHT11_H
#define _DHT11_H
#include "stm32f4xx.h"

#define dht11_high GPIO_SetBits(GPIOC, GPIO_Pin_10)
#define dht11_low GPIO_ResetBits(GPIOC, GPIO_Pin_10)
#define Read_Data GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10)

void DHT11_GPIO_Init_OUT(void);
void DHT11_GPIO_Init_IN(void);
u8 DHT11_Init(void);
void DHT11_Start(void);
unsigned char DHT11_REC_Byte(void);
//void DHT11_Read_Data(u8 *temp, u8 *humi);
void DHT11_REC_Data(void);
extern float  rec_data[4];
#endif

