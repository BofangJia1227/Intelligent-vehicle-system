#ifndef _MQ2_H
#define _MQ2_H
 
//C�ļ�����Ҫ��������ͷ�ļ�
#include <stm32f4xx.h>
#include "sys.h"
#include "delay.h"
#include "math.h"
 
#define SMOG_READ_TIMES	10				//��������������ȡ����,����ô���,Ȼ��ȡƽ��ֵ
 
//C�ļ��ж���ĺ���������
void MQ2_Init(void);
u16 Get_Adcc(u8 ch);
u16 Get_Adc_Average(u8 ch, u8 times);

float MeasureAirQuality(void);
extern float ppm;
#endif
