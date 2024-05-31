#ifndef __MOTOR_H
#define __MOTOR_H

#include "system.h"


#define PWMA1   TIM10->CCR1  
#define PWMA2   TIM11->CCR1 

#define PWMB1   TIM9->CCR1 
#define PWMB2   TIM9->CCR2


#define PWMC1   TIM1->CCR1  
#define PWMC2   TIM1->CCR2 

#define PWMD1   TIM1->CCR3 
#define PWMD2   TIM1->CCR4

#define EN     PDin(3)  

#define Servo_PWM  TIM8->CCR4
#define SERVO_INIT 1500  //Servo zero point //¶æ»úÁãµã

void Enable_Pin(void);
void MiniBalance_PWM_Init(u16 arr,u16 psc);

void Servo_PWM_Init(u16 arr,u16 psc);
void MiniBalance_PWM11_Init(u16 arr,u16 psc);
void MiniBalance_PWM9_Init(u16 arr,u16 psc);
void MiniBalance_PWM1_Init(u16 arr,u16 psc);
#endif
