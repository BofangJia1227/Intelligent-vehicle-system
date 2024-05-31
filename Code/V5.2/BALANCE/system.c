
#include "system.h"

//���������ʧ�ܱ�־λ
u8 Flag_Stop=0;   

//ADCֵ�ֶα�����ȡ����С���ͺ�����
int Divisor_Mode;


//С�����ͱ���
//0=Mec_Car��1=Akm_Car��2=Diff_Car��3=FourWheel_Car
u8 Car_Mode=0; 

//�������PWMֵ��������С��ר��
int Servo;  

//ң��С����Ĭ���ٶȣ���λ��mm/s
float RC_Velocity=500; 


//CCD��ر���
float CCD_KP=37,CCD_KI=140;

//openmv��ر���
float OPENMV_KP=10,OPENMV_KI=55;

//С������Ŀ���˶��ٶȣ���λ��m/s
float Move_X, Move_Y, Move_Z;   


//�ٶȿ���PID����
float Velocity_KP=700,Velocity_KI=700; 


//ƽ�������м������ȫ���ƶ�С��ר��
Smooth_Control smooth_control;  


//����Ĳ����ṹ��
Motor_parameter MOTOR_A,MOTOR_B,MOTOR_C,MOTOR_D;  

/************ С���ͺ���ر��� **************************/


float Encoder_precision; 	//����������
float Wheel_perimeter;	 	//�����ܳ�����λ��m
float Wheel_spacing; 		//�������־࣬��λ��m
float Axle_spacing; 		//С��ǰ�������࣬��λ��m

/************ С���ͺ���ر��� **************************/

//����APP����ģ�ֱ���CANͨ�š�����1ͨ�ſ��Ʊ�־λ����5����־λĬ�϶�Ϊ0��������3����ģʽ
u8 CCD_ON_Flag=0, APP_ON_Flag=0, Avoid_ON_Flag=0, Follow_ON_Flag=0, Along_wall=0; 

//����ң����صı�־λ
u8 Flag_Left, Flag_Right, Flag_Direction=0, Turn_Flag; 

//������APP���Ͳ����ı�־λ
u8 PID_Send; 

void systemInit(void)
{       

	//�ж����ȼ���������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	//��ʱ������ʼ��
	delay_init(168);			                                            
	
	//��ʼ����LED�����ӵ�Ӳ���ӿ�
	LED_Init();                     
	    
	//��ʼ������������ӵ�Ӳ���ӿ�
	Buzzer_Init();  
	
	//��ʼ����ʹ�ܿ������ӵ�Ӳ���ӿ�
	Enable_Pin();

	//��ʼ����OLED��ʾ�����ӵ�Ӳ���ӿ�	
	OLED_Init();     
	
	//��ʼ�����û��������ӵ�Ӳ���ӿ�
	KEY_Init();	
	
	//����1��ʼ����ͨ�Ų�����115200
	uart1_init(115200);	  
	
	//����2��ʼ����ͨ�Ų�����9600������������APP��ͨ��
	uart2_init(9600);  
	
	//����3��ʼ����ͨ�Ų�����115200
	uart3_init(115200);	 
	
	//ADC���ų�ʼ�������ڶ�ȡ��ص�ѹ���λ����λ����λ����λ����С���������С�������ͺ�
 	Adc_Init();  

	//���ݵ�λ���ĵ�λ�ж���Ҫ���������һ���ͺŵ�С����Ȼ����ж�Ӧ�Ĳ�����ʼ��	
	Robot_Select();                 
	
	LIDAR_USART_Init();
         

	//������A��ʼ�������ڶ�ȡ���A��ʵʱ�ٶ�
	Encoder_Init_TIM2(); 

	//������B��ʼ�������ڶ�ȡ���B��ʵʱ�ٶ�	
	Encoder_Init_TIM3(); 

	//������C��ʼ�������ڶ�ȡ���C��ʵʱ�ٶ�	
	Encoder_Init_TIM4();

	//������D��ʼ�������ڶ�ȡ���D��ʵʱ�ٶ�	
	Encoder_Init_TIM5();  
	
	//��ʼ��������С���Ķ����������
	if(Car_Mode==Akm_Car)  Servo_PWM_Init(9999,167);


	//��ʼ������ٶȿ����Լ������ڿ��Ƶ���ٶȣ�PWMƵ��10KHZ
	MiniBalance_PWM_Init(16799,0);  //�߼���ʱ��TIM10��Ƶ��Ϊ168M����PWMΪ16799��Ƶ��=168M/((16799+1)*(0+1))=10k 
	
	MiniBalance_PWM11_Init(16799,0);  //�߼���ʱ��TIM11��Ƶ��Ϊ168M����PWMΪ16799��Ƶ��=168M/((16799+1)*(0+1))=10k
	
	MiniBalance_PWM9_Init(16799,0);  //�߼���ʱ��TIM9��Ƶ��Ϊ168M����PWMΪ16799��Ƶ��=168M/((16799+1)*(0+1))=10k 
	
	MiniBalance_PWM1_Init(16799,0);  //�߼���ʱ��TIM1��Ƶ��Ϊ168M����PWMΪ16799��Ƶ��=168M/((16799+1)*(0+1))=10k
	
  //IIC��ʼ��������MPU6050  
	I2C_GPIOInit();
	//Init_BH1750();
	 BH1750_Init();
  //MPU6050 ��ʼ�������ڶ�ȡС��������̬��������ٶȡ�������ٶ���Ϣ
   //MPU6050_initialize();  
	 	
}
