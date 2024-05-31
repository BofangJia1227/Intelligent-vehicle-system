#include "PID.h"
#include "math.h"
PID_Para_angle pid_x;   // 定义x轴pid参数
PID_Para_angle pid_y;   // 定义y轴pid参数
float Kpx_angle,Kix_angle,Kdx_angle;   // 定义比例、积分、微分系数
float Kpy_angle,Kiy_angle,Kdy_angle;   // 定义比例、积分、微分系数
PID_Para_speed pid_m;   // 定义x轴pid参数
PID_Para_speed pid_n;   // 定义y轴pid参数
float Kpx_speed,Kix_speed,Kdx_speed;   // 定义比例、积分、微分系数
float Kpy_speed,Kiy_speed,Kdy_speed;   // 定义比例、积分、微分系数
//extern unsigned int pwm_x;
//extern unsigned int pwm_y;

float Servo_Speed = 10;// 舵机每次最大允许转动的PWM值
float Target_x = 750, Target_y = 750;		// X和Y舵机目标PWM值
float position_x= 750, position_y = 750;	// X和Y舵机当前PWM值
float increase_x, increase_y;				// X和Y舵机增量PWM值

//在云台转动的控制系统下，采用角度环以及速度环两级PID闭环系统
/**************************************************************************
函数功能：角度环，将nano传来的坐标转换为云台需要移动的角度，外加PID控制
入口参数：无
返回  值：无
**************************************************************************/
/*采用抗积分饱和的变积分PID控制，积分离散化后转换为梯形积分
采用抗积分饱和的PID控制
当前序误差error_angle 大于最大值时，积分累计
当前序误差error_angle小于最小值时，积分累计
**************************************************************************
采用积分分离式PID控制
当被控制量与设定值偏差较大时，取消积分作用，以避免由于积分作用使系统稳定性降低，超调量增大；
当被控量接近给定值时，引入积分控制，以便消除静差，提高控制精度。
本系统中设定阈值为thr = 0.3cm
当误差的绝对值大于thr时，采用PD控制，可以避免产生过大的超调，又使系统有较快的响应
当误差的绝对值小于等于thr时，采用PID控制，以保证系统的控制精度
*/
void PID_angle_init(void)

{
	Kpx_angle=KPx_angle; // 使用宏定义，参数整定直接对pid.h中的宏定义进行修改即可
	Kix_angle=KIx_angle; //同上
	Kdx_angle=KDx_angle; //同上
	
	Kpy_angle=KPy_angle; // 使用宏定义，参数整定直接对pid.h中的宏定义进行修改即可
	Kiy_angle=KIy_angle; //同上
	Kdy_angle=KDy_angle; //同上
	

	pid_x.Set_angle=0.9; // x定义设定值
	pid_x.Actual_angle=0.0;// x定义实际值
	pid_x.error_angle=0.0;//当前x偏差值
	pid_x.error_last_angle=0.0;//上一个x偏差值
	pid_x.thr_angle=0.94;//x设定阈值
	pid_x.umax_angle=1.2;//x角度式PID的极限范围
	pid_x.umin_angle=-0.6;//x角度式PID的极限范围
	pid_x.integral_angle=0.0;
	pid_x.pox=0.0;
	
	pid_y.Set_angle=0.9;//y定义设定值
	pid_y.Actual_angle=0.0;// y定义实际值
	pid_y.error_angle=0.0;//上一个y偏差值
	pid_y.error_last_angle=0.0;//上一个y偏差值
	pid_y.thr_angle=0.63;//y设定阈值
	pid_y.umax_angle=1.2;//y角度式PID的极限范围
	pid_y.umin_angle=-0.6;//y角度式PID的极限范围
	pid_y.integral_angle=0.0;
	pid_y.pox=0.0;


}

float PID_angle_realize(float x,float y)
{
	
	int indexx_angle;
	int indexy_angle;
	pid_x.Actual_angle = x;
	pid_y.Actual_angle = y;
	pid_x.error_angle = pid_x.Actual_angle - pid_x.Set_angle;
	pid_y.error_angle = pid_y.Actual_angle - pid_y.Set_angle;
	
	//x轴
	if(pid_x.Actual_angle > pid_x.umax_angle)
	{
		if(abs(pid_x.error_angle) > 0.94)
		{
			indexx_angle = 0;
		}else{
			indexx_angle = 1;
			if(pid_x.error_angle<0)
			{
				pid_x.integral_angle+=pid_x.error_angle;
			}
		}
			
	}
	else if(pid_x.error_angle < pid_x.umin_angle)
	{
		if(abs(pid_x.error_angle) > 0.94)
		{
			indexx_angle = 0;
		}else{
			indexx_angle = 1;
			if(pid_x.error_angle>0)
			{
				pid_x.integral_angle+=pid_x.error_angle;
			}
		}
		
	}
	else{
		if(abs(pid_x.error_angle) > 0.94)
		{
			indexx_angle = 0;
		}else{
			indexx_angle = 1;
			pid_x.integral_angle+=pid_x.error_angle;
			}
		}
			
	//pid_x.pox = Kp_angle * pid_x.error_angle + indexx_angle * Ki_angle * pid_x.integral_angle + Kd_angle * (pid_x.error_angle - pid_x.error_last_angle）;
	pid_x.pox = Kpx_angle * pid_x.error_angle + indexx_angle * Kix_angle * pid_x.integral_angle + Kdx_angle * (pid_x.error_angle - pid_x.error_last_angle);
	//角度转换
	
	pid_x.error_last_angle = pid_x.error_angle;
		
	//然后通过control.c进行计算角度
		
		
		
	//y轴
	if(pid_y.Actual_angle > pid_y.umax_angle)
	{
		if(abs(pid_y.error_angle) > 0.63)
		{
			indexx_angle = 0;
		}else{
			indexx_angle = 1;
			if(pid_y.error_angle<0)
			{
				pid_y.integral_angle+=pid_y.error_angle;
			}
		}
			
	}
	else if(pid_y.error_angle < pid_y.umin_angle)
	{
		if(abs(pid_y.error_angle) > 0.63)
		{
			indexx_angle = 0;
		}else{
			indexx_angle = 1;
			if(pid_y.error_angle>0)
			{
				pid_y.integral_angle+=pid_y.error_angle;
			}
		}
		
	}
	else{
		if(abs(pid_y.error_angle) > 0.63)
		{
			indexx_angle = 0;
		}else{
			indexx_angle = 1;
			pid_y.integral_angle+=pid_y.error_angle;
			}
		}
			
	//pid_x.pox = Kp_angle * pid_x.error_angle + indexx_angle * Ki_angle * pid_x.integral_angle + Kd_angle * (pid_x.error_angle - pid_x.error_last_angle）;
	pid_y.pox = Kpy_angle * pid_y.error_angle + indexx_angle * Kiy_angle * pid_y.integral_angle + Kdy_angle * (pid_y.error_angle - pid_y.error_last_angle);
	//角度转换

	pid_y.error_last_angle = pid_y.error_angle;
		
		//然后通过control.c进行计算角度
	
	
	pid_x.Actual_angle  = pid_x.pox * 1.0;
	pid_y.Actual_angle  = pid_y.pox * 1.0;
	return pid_x.Actual_angle ,pid_y.Actual_angle;
		
}





/**************************************************************************
函数功能：速度环，将角度信息转化为PWM，外加PID控制
入口参数：无
返回  值：无
**************************************************************************/
//采用抗积分饱和的变积分PID控制，积分离散化后转换为梯形积分

void PID_speed_init(void)
{
	Kpx_speed=KPx_speed; // 使用宏定义，参数整定直接对pid.h中的宏定义进行修改即可
	Kix_speed=KIx_speed; //同上
	Kdx_speed=KDx_speed; //同上
	Kpy_speed=KPy_speed; // 使用宏定义，参数整定直接对pid.h中的宏定义进行修改即可
	Kiy_speed=KIy_speed; //同上
	Kdy_speed=KDy_speed; //同上
	
	
//	pid_m.Set_speed=0.0; // x定义设定值
//	pid_m.Actual_speed=0.0;// x定义实际值
//	pid_m.error_speed=0.0;//当前x偏差值
//	pid_m.error_last_speed=0.0;//上一个x偏差值
//	pid_m.thr_speed=0.94;//x设定阈值
//	pid_m.umax_speed=0.0;//x角度式PID的极限范围
//	pid_m.umin_speed=0.0;//x角度式PID的极限范围
//	
//	pid_n.Set_speed=0.0;//y定义设定值
//	pid_n.Actual_speed=0.0;// y定义实际值
//	pid_n.error_speed=0.0;//当前y偏差值
//	pid_n.error_last_speed=0.0;//上一个y偏差值
//	pid_n.thr_speed=0.63;//y设定阈值
//	pid_n.umax_speed=0.0;//y角度式PID的极限范围
//	pid_n.umin_speed=0.0;//y角度式PID的极限范围
//	
	pid_m.Target = Target_x;//定义目标值
	pid_m.Encoder = position_x;//定义实际值
	pid_m.bias = 0.0;//当前偏差值
	pid_m.last_bias = 0.0;//上一个偏差值
	pid_m.integral = 0.0;//目标积分制
	pid_m.umax = 1200.0;//速度环PID的极限范围
	pid_m.umin = 300.0;//速度环PID的极限范围
	pid_m.vel = 10.0;//设定阈值
	
	
	pid_n.Target = Target_y;//定义目标值
	pid_n.Encoder = position_y;//定义实际值
	pid_n.bias = 0.0;//当前偏差值
	pid_n.last_bias = 0.0;//上一个偏差值
	pid_n.integral = 0.0;//目标积分制
	pid_n.umax = 1200.0;//速度环PID的极限范围
	pid_n.umin = 300.0;//速度环PID的极限范围
	pid_n.vel = 10.0;//设定阈值
	
	
	
	
}

int  PID_speed_realize(float speed_x , float speed_y)
{
	
	int index_speed_x;
	int index_speed_y;
	pid_m.Encoder  = speed_x;
	pid_n.Encoder = speed_y;
	pid_m.bias = pid_m.Target - pid_m.Encoder;
	pid_n.bias = pid_n.Target - pid_n.Encoder;
	
	//x轴
	if(pid_m.Encoder > pid_m.umax)
	{
		if(abs(pid_m.bias) > 10)
		{
			index_speed_x = 0;
		}else{
			index_speed_x = 1;
			if(pid_m.bias < 0)
			{
				pid_m.integral += pid_m.bias;
			}
		}
	}
	else if(pid_m.Encoder < pid_m.umin)
	{
		if(abs(pid_m.bias) > 10)
		{
			index_speed_x = 0;
		}else {
			index_speed_x = 1;
			if(pid_m.bias > 0)
			{
			pid_m.integral += pid_m.bias;
			}
		}
	}
	else {
		if(abs(pid_m.bias ) > 10)
		{
			index_speed_x = 0;
		}else{
			index_speed_x = 1;
			pid_m.integral += pid_m.bias ;
		}
	}
	
	pid_m.pwm =  Kpx_speed * pid_m.bias + index_speed_x * Kix_speed * pid_m.integral + Kdx_speed * (pid_m.bias - pid_m.last_bias);
	
	pid_m.last_bias = pid_m.bias;
	
	
	
	//y轴
	if(pid_n.Encoder > pid_n.umax)
	{
		if(abs(pid_n.bias) > 10)
		{
			index_speed_y = 0;
		}else{
			index_speed_y = 1;
			if(pid_n.bias < 0)
			{
				pid_n.integral += pid_n.bias;
			}
		}
	}
	else if(pid_n.Encoder < pid_n.umin)
	{
		if(abs(pid_n.bias) > 10)
		{
			index_speed_y = 0;
		}else {
			index_speed_y = 1;
			if(pid_n.bias > 0)
			{
			pid_n.integral += pid_n.bias;
			}
		}
	}
	else {
		if(abs(pid_n.bias ) > 10)
		{
			index_speed_y = 0;
		}else{
			index_speed_y = 1;
			pid_n.integral += pid_n.bias ;
		}
	}
	
	pid_n.pwm =  Kpy_speed * pid_n.bias + index_speed_y * Kiy_speed * pid_n.integral + Kdy_speed * (pid_n.bias - pid_n.last_bias);
	
	pid_n.last_bias = pid_n.bias;
	
	pid_m.Encoder = pid_m.pwm * 1.0;
	pid_n.Encoder = pid_n.pwm * 1.0;
	return pid_m.Encoder,pid_n.Encoder;
}



/**************************************************************************
函数功能：底部舵机 角度与PWM值互换
通过这个函数速度环可以和角度（位置环联系起来），角度环返回的是坐标，借此通过舵机性质计算
进行求得舵机的动作，然后执行，就可以和速度环联系起来，两者共同控制
入口参数：angle 待换算角度
返回  值：无
**************************************************************************/
// 角度 -> PWM
float ServoX_Angle2PWM(float angle_x)	
{
	return angle_x*1000/270+250; 
}

// PWM -> 角度
float ServoX_PWM2Angle(float pwm_x)	
{
	return (pwm_x-250)/1000*270;
}


/**************************************************************************
函数功能：摇臂舵机 角度与PWM值互换
通过这个函数速度环可以和角度（位置环联系起来），角度环返回的是坐标，借此通过舵机性质计算
进行求得舵机的动作，然后执行，就可以和速度环联系起来，两者共同控制
入口参数：
返回  值：无
**************************************************************************/
// 角度 -> PWM
float ServoY_Angle2PWM(float angle_y)	
{
	return angle_y*1000/180+250; 
}

// PWM -> 角度
float ServoY_PWM2Angle(float pwm_y)	
{
	return (pwm_y-250)/1000*180;
}

/**************************************************************************
函数功能：增量设置舵机PWM值(在当前位置基础上增量移动)，用于舵机在平面XY轴合成运动 和 PID速度环
入口参数：舵机X 和 舵机Y 的增量调整值
返回  值：无
**************************************************************************/
void Set_Pwm_Increase(float incrsX,float incrsY)	
{	
	// 增量得到新PWM值
	position_x += incrsX;		   
	position_y += incrsY;
	
	// 设置PWM值
	Set_Pwm_ServoX(position_x);
	Set_Pwm_ServoY(position_y);
	
//	TIM8->CCR1 = positionX;    // 底部舵机 PC6 - TIM8_CH1
//	TIM8->CCR2 = positionY;    // 摇臂舵机 PC7 - TIM8_CH2
}

/**************************************************************************
函数功能：设置舵机PWM
入口参数：无
返回  值：无
**************************************************************************/
// 底部舵机 PC6 - TIM8_CH1
void Set_Pwm_ServoX(float pos)	
{	

	//TIM8->CCR1 = pos;   
}

// 摇臂舵机 PC7 - TIM8_CH2
void Set_Pwm_ServoY(float pos)	
{	
	//TIM8->CCR2 = pos;    
}

/**************************************************************************
函数功能：PWM限幅，防止PWM超出可设置范围导致烧毁舵机
入口参数：无
返回  值：无
**************************************************************************/
void limit_PWM(void)
{	
	int Amplitude_H=1200, Amplitude_L=300;       
   
	//舵机X脉宽极限值，即限制舵机转角 13.5° -256.5°
	if(Target_x<Amplitude_L)  Target_x=Amplitude_L;	
	if(Target_x>Amplitude_H)  Target_x=Amplitude_H;	
	
	//舵机Y脉宽极限值，即限制舵机转角    9° -171°
	if(Target_y<Amplitude_L)  Target_y=Amplitude_L;	
	if(Target_y>Amplitude_H)  Target_y=Amplitude_H;	
}
/**************************************************************************
函数功能：速度限幅 防止PWM超出可设置范围导致烧毁舵机
入口参数：无
返回  值：无
**************************************************************************/
void limit_Speed(void)
{	
	int Amplitude_H = Servo_Speed, Amplitude_L = -Servo_Speed;  
	
    if(increase_x < Amplitude_L)  increase_x = Amplitude_L;	
	if(increase_x > Amplitude_H)  increase_x = Amplitude_H;	
	
	if(increase_y < Amplitude_L)  increase_y = Amplitude_L;	
	if(increase_y > Amplitude_H)  increase_y = Amplitude_H;	
}






































