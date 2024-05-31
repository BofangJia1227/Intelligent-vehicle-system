
#include "system.h"

//机器人软件失能标志位
u8 Flag_Stop=0;   

//ADC值分段变量，取决于小车型号数量
int Divisor_Mode;


//小车类型变量
//0=Mec_Car，1=Akm_Car，2=Diff_Car，3=FourWheel_Car
u8 Car_Mode=0; 

//舵机控制PWM值，阿克曼小车专用
int Servo;  

//遥控小车的默认速度，单位：mm/s
float RC_Velocity=500; 


//CCD相关变量
float CCD_KP=37,CCD_KI=140;

//openmv相关变量
float OPENMV_KP=10,OPENMV_KI=55;

//小车三轴目标运动速度，单位：m/s
float Move_X, Move_Y, Move_Z;   


//速度控制PID参数
float Velocity_KP=700,Velocity_KI=700; 


//平滑控制中间变量，全向移动小车专用
Smooth_Control smooth_control;  


//电机的参数结构体
Motor_parameter MOTOR_A,MOTOR_B,MOTOR_C,MOTOR_D;  

/************ 小车型号相关变量 **************************/


float Encoder_precision; 	//编码器精度
float Wheel_perimeter;	 	//轮子周长，单位：m
float Wheel_spacing; 		//主动轮轮距，单位：m
float Axle_spacing; 		//小车前后轴的轴距，单位：m

/************ 小车型号相关变量 **************************/

//蓝牙APP、航模手柄、CAN通信、串口1通信控制标志位。这5个标志位默认都为0，代表串口3控制模式
u8 CCD_ON_Flag=0, APP_ON_Flag=0, Avoid_ON_Flag=0, Follow_ON_Flag=0, Along_wall=0; 

//蓝牙遥控相关的标志位
u8 Flag_Left, Flag_Right, Flag_Direction=0, Turn_Flag; 

//向蓝牙APP发送参数的标志位
u8 PID_Send; 

void systemInit(void)
{       

	//中断优先级分组设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	//延时函数初始化
	delay_init(168);			                                            
	
	//初始化与LED灯连接的硬件接口
	LED_Init();                     
	    
	//初始化与蜂鸣器连接的硬件接口
	Buzzer_Init();  
	
	//初始化与使能开关连接的硬件接口
	Enable_Pin();

	//初始化与OLED显示屏连接的硬件接口	
	OLED_Init();     
	
	//初始化与用户按键连接的硬件接口
	KEY_Init();	
	
	//串口1初始化，通信波特率115200
	uart1_init(115200);	  
	
	//串口2初始化，通信波特率9600，用于与蓝牙APP端通信
	uart2_init(9600);  
	
	//串口3初始化，通信波特率115200
	uart3_init(115200);	 
	
	//ADC引脚初始化，用于读取电池电压与电位器档位，电位器档位决定小车开机后的小车适配型号
 	Adc_Init();  

	//根据电位器的档位判断需要适配的是哪一种型号的小车，然后进行对应的参数初始化	
	Robot_Select();                 
	
	LIDAR_USART_Init();
         

	//编码器A初始化，用于读取电机A的实时速度
	Encoder_Init_TIM2(); 

	//编码器B初始化，用于读取电机B的实时速度	
	Encoder_Init_TIM3(); 

	//编码器C初始化，用于读取电机C的实时速度	
	Encoder_Init_TIM4();

	//编码器D初始化，用于读取电机D的实时速度	
	Encoder_Init_TIM5();  
	
	//初始化阿克曼小车的舵机控制引脚
	if(Car_Mode==Akm_Car)  Servo_PWM_Init(9999,167);


	//初始化电机速度控制以及，用于控制电机速度，PWM频率10KHZ
	MiniBalance_PWM_Init(16799,0);  //高级定时器TIM10的频率为168M，满PWM为16799，频率=168M/((16799+1)*(0+1))=10k 
	
	MiniBalance_PWM11_Init(16799,0);  //高级定时器TIM11的频率为168M，满PWM为16799，频率=168M/((16799+1)*(0+1))=10k
	
	MiniBalance_PWM9_Init(16799,0);  //高级定时器TIM9的频率为168M，满PWM为16799，频率=168M/((16799+1)*(0+1))=10k 
	
	MiniBalance_PWM1_Init(16799,0);  //高级定时器TIM1的频率为168M，满PWM为16799，频率=168M/((16799+1)*(0+1))=10k
	
  //IIC初始化，用于MPU6050  
	I2C_GPIOInit();
	//Init_BH1750();
	 BH1750_Init();
  //MPU6050 初始化，用于读取小车三轴姿态、三轴角速度、三轴加速度信息
   //MPU6050_initialize();  
	 	
}
