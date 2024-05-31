#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
#include "system.h"

void KEY_Init(void);
u8 click(void);
void Delay_ms(void);
u8 click_N_Double (u8 time);
u8 click_N_Double_MPU6050 (u8 time);
u8 Long_Press(void);
u8  select(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#define KEY_ON	0
#define KEY_OFF	1
	
/*--------KEY control pin--------*/
#define KEY_PORT	GPIOE
#define KEY_PIN		GPIO_Pin_0
#define KEY			PEin(0) 
/*----------------------------------*/

#endif 
