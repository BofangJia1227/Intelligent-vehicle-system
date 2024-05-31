#ifndef __PID_H
#define __PID_H
/* 
#define   KP    2
#define   KI    0.15
#define   KD    2
*/
//位置环PID参数
#define   KPx_angle    2
#define   KIx_angle    0.15
#define   KDx_angle    2
#define   KPy_angle    2
#define   KIy_angle    0.15
#define   KDy_angle    2
//速度环PID参数
#define   KPx_speed    2
#define   KIx_speed    0.15
#define   KDx_speed    2
#define   KPy_speed    2
#define   KIy_speed    0.15
#define   KDy_speed    2
 

//在云台转动的控制系统下，采用角度环以及速度环两级PID闭环系统
/**************************************************************************
函数功能：角度环，将nano传来的坐标转换为云台需要移动的角度，外加PID控制
入口参数：无
返回  值：无
**************************************************************************/
/*采用抗积分饱和的变积分PID控制，积分离散化后转换为梯形积分
采用抗积分饱和的PID控制
当前序误差error_angle 大于最大值umax_angle时，积分累计
当前序误差error_angle小于最小值umin_angle时，积分累计
**************************************************************************
采用积分分离式PID控制
当被控制量与设定值偏差较大时，取消积分作用，以避免由于积分作用使系统稳定性降低，超调量增大；
当被控量接近给定值时，引入积分控制，以便消除静差，提高控制精度。
本系统中设定阈值为thr = 0.3cm
当误差的绝对值大于thr时，采用PD控制，可以避免产生过大的超调，又使系统有较快的响应
当误差的绝对值小于等于thr时，采用PID控制，以保证系统的控制精度
*/
typedef struct
{
float Set_angle; // 定义设定值
float Actual_angle; // 定义实际值
float umax_angle;//角度式PID的极限范围
float umin_angle;//	角度式PID的极限范围
float error_angle;//当前偏差值
float error_last_angle;//	上一个偏差值
float thr_angle;//设定阈值
float integral_angle;//定义积分值
float pox;//PID调整后的坐标
} PID_Para_angle;




/**************************************************************************
函数功能：速度环，将角度信息转化为PWM，外加PID控制
入口参数：无
返回  值：无
**************************************************************************/
typedef struct
{
//float Set_speed; // 定义设定值
//float Actual_speed; // 定义实际值
//float umax_speed;//角度式PID的极限范围
//float umin_speed;//	角度式PID的极限范围
//float error_speed;//当前偏差值
//float error_last_speed;//	上一个偏差值
//float thr_speed;//设定阈值
//float integral_speed;//定义积分值
	
	float Target;//定义目标值
	float Encoder;//定义实际值
	float bias;//当前偏差值
	float last_bias;//上一个偏差值
	float vel;//设定阈值
	float umax;//速度环PID的极限范围
	float umin;//速度环PID的极限范围
	float integral;//目标积分制
	int pwm;
} PID_Para_speed;




/**************************************************************************/
void PID_angle_init(void);
float PID_angle_realize(float x,float y);//坐标转角度
void PID_realize_init(void);
int PID_speed_realize(float angle_x , float angle_y);//角度转pwm
void Set_Pwm_ServoX(float pos);
void Set_Pwm_ServoY(float pos);
void limit_PWM(void);
void limit_Speed(void);
void Set_Pwm_Increase(float incrsX,float incrsY);
float ServoY_PWM2Angle(float pwm_y);
float ServoY_Angle2PWM(float angle_y)	;
float ServoX_Angle2PWM(float angle_x)	;
float ServoX_PWM2Angle(float pwm_x)	;

#endif // __PID_H

