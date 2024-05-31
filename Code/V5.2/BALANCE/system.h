#ifndef __SYSTEM_H
#define __SYSTEM_H


//����������Ҫ�õ���ͷ�ļ�
#include "FreeRTOSConfig.h"
//FreeRTOS���ͷ�ļ� 
//FreeRTOS related header files
#include "FreeRTOS.h"
#include "stm32f4xx.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

//��������ͷ�ļ�
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "balance.h"
#include "led.h"
#include "oled.h"
#include "usart.h"
#include "usartx.h"
#include "adc.h"
#include "motor.h"
#include "encoder.h"
#include "show.h"								   
#include "key.h"
#include "lidar.h"
#include "robot_select_init.h"
//#include "I2C.h"
//#include "MPU6050.h"


//С���ͺŵ�ö�ٶ���
typedef enum 
{
	Mec_Car = 0,
	Akm_Car, 
	Diff_Car, 
	FourWheel_Car
} CarMode;


//����ٶȿ�����ز����ṹ��
typedef struct  
{
	float Encoder;     		//��������ֵ����ȡ���ʵʱ�ٶ�
	float Motor_Pwm;   		//���PWM��ֵ�����Ƶ��ʵʱ�ٶ�
	float Target;      		//���Ŀ���ٶ�ֵ�����Ƶ��Ŀ���ٶ�
	float Velocity_KP; 		//�ٶȿ���PID����
	float	Velocity_KI;	//�ٶȿ���PID����
}Motor_parameter;


//ƽ�������������ٶ�
typedef struct  
{
	float VX;
	float VY;
	float VZ;
}Smooth_Control;


/****** �ⲿ�������壬������c�ļ�����system.hʱ��Ҳ����ʹ��system.c����ı��� ******/
extern uint8_t Flag_Stop;
extern int Divisor_Mode;
extern uint8_t Car_Mode;
extern int Servo;
extern float RC_Velocity;
extern float Move_X, Move_Y, Move_Z; 
extern float Velocity_KP, Velocity_KI;	
extern Smooth_Control smooth_control;
extern Motor_parameter MOTOR_A, MOTOR_B, MOTOR_C, MOTOR_D;
extern float Encoder_precision;
extern float Wheel_perimeter;
extern float Wheel_spacing; 
extern float Axle_spacing; 
extern uint8_t CCD_ON_Flag, APP_ON_Flag, Avoid_ON_Flag, Follow_ON_Flag, Along_wall;
extern uint8_t Flag_Left, Flag_Right, Flag_Direction, Turn_Flag; 
extern uint8_t PID_Send; 
extern float CCD_KP,CCD_KI;
extern float OPENMV_KP,OPENMV_KI;
extern float ELE_KP,ELE_KI;

void systemInit(void);

/***�궨��***/

//����(1000/100hz=10)�����������С�������˶�
#define CONTROL_DELAY		1000

//�������ͺ�����������Divisor_Mode��ֵ��Ŀǰ��6��С������   
#define RATE_1_HZ		  1
#define RATE_5_HZ		  5
#define RATE_10_HZ		10
#define RATE_20_HZ		20
#define RATE_25_HZ		25
#define RATE_50_HZ		50
#define RATE_100_HZ		100
#define RATE_200_HZ 	200
#define RATE_250_HZ 	250
#define RATE_500_HZ 	500
#define RATE_1000_HZ 	1000


//C library function related header file
//C�⺯�������ͷ�ļ�
#include <stdio.h> 
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "stdarg.h"
#endif 
