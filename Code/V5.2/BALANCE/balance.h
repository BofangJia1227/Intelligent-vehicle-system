#ifndef __BALANCE_H
#define __BALANCE_H			  	 
#include "sys.h"
#include "system.h"
#include "lidar.h"

#define Follow_Distance 1600  //�������
#define Keep_Follow_Distance 500  //���汣�־���


#define Avoid_Min_Distance 200  //������С����
#define Avoid_Distance 450     //���Ͼ���
#define forward_velocity 0.3  //Move_X�ٶ�

// #define corner_velocity 0.5    //ȫ����omniС�� Move_Y�ٶ�
#define other_corner_velocity 1.5    //Move_Y�ٶ�

#define amplitude_limiting 0.6   //�ٶ��޷�

#define limit_distance 100   //������ֱ��ģʽ���״�̽�����

#define BALANCE_TASK_PRIO		4     //Task priority //�������ȼ�
#define BALANCE_STK_SIZE 		512   //Task stack size //�����ջ��С

extern uint8_t Lidar_Detect;
extern uint8_t Mode;

#define Lidar_Detect_ON 1				//���Ѳ���Ƿ����״����ϰ���
#define Lidar_Detect_OFF 0
#define Barrier_Detected 1
#define No_Barrier 0

//С����ģʽ����
#define Environment_Patrol_Mode 0
#define Lidar_Avoid_Mode 1
#define Lidar_Follow_Mode 2
#define Lidar_Along_Mode 3		
#define CCD_Line_Patrol_Mode 4
#define OPENMV_Line_Patrol_Mode 5
#define APP_Control_Mode 6



//ָʾң�ؿ��ƵĿ���
#define RC_ON 1	
#define RC_OFF 0
//ң�ؿ���ǰ���ٶ����ֵ
#define MAX_RC_Velocity 1.230f
//ң�ؿ���ת���ٶ����ֵ
#define	MAX_RC_Turn_Bias 2.27f
//ң�ؿ���ǰ���ٶ���Сֵ
#define MINI_RC_Velocity 0.1f
//ң�ؿ���ת���ٶ���Сֵ
#define	MINI_RC_Turn_Velocity 0.2f
//ǰ���Ӽ��ٷ���ֵ��ÿ��ң�ؼӼ��Ĳ���ֵ
#define X_Step 0.08f
//ת��Ӽ��ٷ���ֵ
#define Z_Step 0.2f

extern float Mec_Car_x1,Mec_Car_x2;
extern float RC_Velocity_CCD, RC_Velocity_ELE,RC_Velocity_OPENMV; 
void  Get_RC_CCD(void);
void  Get_RC_OPENMV(void);
typedef struct {
    float temperature;
    float humidity;
    float gas_ppm;
    float light_intensity;
} EnvironmentData;
extern float light;

EnvironmentData Get_environment(void);

void Balance_task(void *pvParameters);
void Set_Pwm(int motor_a,int motor_b,int motor_c,int motor_d,int servo);
void Limit_Pwm(int amplitude);
float target_limit_float(float insert,float low,float high);
int target_limit_int(int insert,int low,int high);
uint8_t Turn_Off( int voltage);
uint32_t myabs(long int a);
int Incremental_PI_A (float Encoder,float Target);
int Incremental_PI_B (float Encoder,float Target);
int Incremental_PI_C (float Encoder,float Target);
int Incremental_PI_D (float Encoder,float Target);
void Get_RC(void);
void Drive_Motor(float Vx,float Vy,float Vz);
void Get_Velocity_Form_Encoder(void);
void Smooth_control(float vx,float vy,float vz);
float float_abs(float insert);
void ELE_Move_Detect(void);
uint8_t Detect_Barrier(void);
void Lidar_Avoid(void);
void Lidar_Follow(void);
void Lidar_along_wall(void);


#endif  

