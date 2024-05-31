#include "PID.h"
#include "math.h"
PID_Para_angle pid_x;   // ����x��pid����
PID_Para_angle pid_y;   // ����y��pid����
float Kpx_angle,Kix_angle,Kdx_angle;   // ������������֡�΢��ϵ��
float Kpy_angle,Kiy_angle,Kdy_angle;   // ������������֡�΢��ϵ��
PID_Para_speed pid_m;   // ����x��pid����
PID_Para_speed pid_n;   // ����y��pid����
float Kpx_speed,Kix_speed,Kdx_speed;   // ������������֡�΢��ϵ��
float Kpy_speed,Kiy_speed,Kdy_speed;   // ������������֡�΢��ϵ��
//extern unsigned int pwm_x;
//extern unsigned int pwm_y;

float Servo_Speed = 10;// ���ÿ���������ת����PWMֵ
float Target_x = 750, Target_y = 750;		// X��Y���Ŀ��PWMֵ
float position_x= 750, position_y = 750;	// X��Y�����ǰPWMֵ
float increase_x, increase_y;				// X��Y�������PWMֵ

//����̨ת���Ŀ���ϵͳ�£����ýǶȻ��Լ��ٶȻ�����PID�ջ�ϵͳ
/**************************************************************************
�������ܣ��ǶȻ�����nano����������ת��Ϊ��̨��Ҫ�ƶ��ĽǶȣ����PID����
��ڲ�������
����  ֵ����
**************************************************************************/
/*���ÿ����ֱ��͵ı����PID���ƣ�������ɢ����ת��Ϊ���λ���
���ÿ����ֱ��͵�PID����
��ǰ�����error_angle �������ֵʱ�������ۼ�
��ǰ�����error_angleС����Сֵʱ�������ۼ�
**************************************************************************
���û��ַ���ʽPID����
�������������趨ֵƫ��ϴ�ʱ��ȡ���������ã��Ա������ڻ�������ʹϵͳ�ȶ��Խ��ͣ�����������
���������ӽ�����ֵʱ��������ֿ��ƣ��Ա����������߿��ƾ��ȡ�
��ϵͳ���趨��ֵΪthr = 0.3cm
�����ľ���ֵ����thrʱ������PD���ƣ����Ա����������ĳ�������ʹϵͳ�нϿ����Ӧ
�����ľ���ֵС�ڵ���thrʱ������PID���ƣ��Ա�֤ϵͳ�Ŀ��ƾ���
*/
void PID_angle_init(void)

{
	Kpx_angle=KPx_angle; // ʹ�ú궨�壬��������ֱ�Ӷ�pid.h�еĺ궨������޸ļ���
	Kix_angle=KIx_angle; //ͬ��
	Kdx_angle=KDx_angle; //ͬ��
	
	Kpy_angle=KPy_angle; // ʹ�ú궨�壬��������ֱ�Ӷ�pid.h�еĺ궨������޸ļ���
	Kiy_angle=KIy_angle; //ͬ��
	Kdy_angle=KDy_angle; //ͬ��
	

	pid_x.Set_angle=0.9; // x�����趨ֵ
	pid_x.Actual_angle=0.0;// x����ʵ��ֵ
	pid_x.error_angle=0.0;//��ǰxƫ��ֵ
	pid_x.error_last_angle=0.0;//��һ��xƫ��ֵ
	pid_x.thr_angle=0.94;//x�趨��ֵ
	pid_x.umax_angle=1.2;//x�Ƕ�ʽPID�ļ��޷�Χ
	pid_x.umin_angle=-0.6;//x�Ƕ�ʽPID�ļ��޷�Χ
	pid_x.integral_angle=0.0;
	pid_x.pox=0.0;
	
	pid_y.Set_angle=0.9;//y�����趨ֵ
	pid_y.Actual_angle=0.0;// y����ʵ��ֵ
	pid_y.error_angle=0.0;//��һ��yƫ��ֵ
	pid_y.error_last_angle=0.0;//��һ��yƫ��ֵ
	pid_y.thr_angle=0.63;//y�趨��ֵ
	pid_y.umax_angle=1.2;//y�Ƕ�ʽPID�ļ��޷�Χ
	pid_y.umin_angle=-0.6;//y�Ƕ�ʽPID�ļ��޷�Χ
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
	
	//x��
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
			
	//pid_x.pox = Kp_angle * pid_x.error_angle + indexx_angle * Ki_angle * pid_x.integral_angle + Kd_angle * (pid_x.error_angle - pid_x.error_last_angle��;
	pid_x.pox = Kpx_angle * pid_x.error_angle + indexx_angle * Kix_angle * pid_x.integral_angle + Kdx_angle * (pid_x.error_angle - pid_x.error_last_angle);
	//�Ƕ�ת��
	
	pid_x.error_last_angle = pid_x.error_angle;
		
	//Ȼ��ͨ��control.c���м���Ƕ�
		
		
		
	//y��
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
			
	//pid_x.pox = Kp_angle * pid_x.error_angle + indexx_angle * Ki_angle * pid_x.integral_angle + Kd_angle * (pid_x.error_angle - pid_x.error_last_angle��;
	pid_y.pox = Kpy_angle * pid_y.error_angle + indexx_angle * Kiy_angle * pid_y.integral_angle + Kdy_angle * (pid_y.error_angle - pid_y.error_last_angle);
	//�Ƕ�ת��

	pid_y.error_last_angle = pid_y.error_angle;
		
		//Ȼ��ͨ��control.c���м���Ƕ�
	
	
	pid_x.Actual_angle  = pid_x.pox * 1.0;
	pid_y.Actual_angle  = pid_y.pox * 1.0;
	return pid_x.Actual_angle ,pid_y.Actual_angle;
		
}





/**************************************************************************
�������ܣ��ٶȻ������Ƕ���Ϣת��ΪPWM�����PID����
��ڲ�������
����  ֵ����
**************************************************************************/
//���ÿ����ֱ��͵ı����PID���ƣ�������ɢ����ת��Ϊ���λ���

void PID_speed_init(void)
{
	Kpx_speed=KPx_speed; // ʹ�ú궨�壬��������ֱ�Ӷ�pid.h�еĺ궨������޸ļ���
	Kix_speed=KIx_speed; //ͬ��
	Kdx_speed=KDx_speed; //ͬ��
	Kpy_speed=KPy_speed; // ʹ�ú궨�壬��������ֱ�Ӷ�pid.h�еĺ궨������޸ļ���
	Kiy_speed=KIy_speed; //ͬ��
	Kdy_speed=KDy_speed; //ͬ��
	
	
//	pid_m.Set_speed=0.0; // x�����趨ֵ
//	pid_m.Actual_speed=0.0;// x����ʵ��ֵ
//	pid_m.error_speed=0.0;//��ǰxƫ��ֵ
//	pid_m.error_last_speed=0.0;//��һ��xƫ��ֵ
//	pid_m.thr_speed=0.94;//x�趨��ֵ
//	pid_m.umax_speed=0.0;//x�Ƕ�ʽPID�ļ��޷�Χ
//	pid_m.umin_speed=0.0;//x�Ƕ�ʽPID�ļ��޷�Χ
//	
//	pid_n.Set_speed=0.0;//y�����趨ֵ
//	pid_n.Actual_speed=0.0;// y����ʵ��ֵ
//	pid_n.error_speed=0.0;//��ǰyƫ��ֵ
//	pid_n.error_last_speed=0.0;//��һ��yƫ��ֵ
//	pid_n.thr_speed=0.63;//y�趨��ֵ
//	pid_n.umax_speed=0.0;//y�Ƕ�ʽPID�ļ��޷�Χ
//	pid_n.umin_speed=0.0;//y�Ƕ�ʽPID�ļ��޷�Χ
//	
	pid_m.Target = Target_x;//����Ŀ��ֵ
	pid_m.Encoder = position_x;//����ʵ��ֵ
	pid_m.bias = 0.0;//��ǰƫ��ֵ
	pid_m.last_bias = 0.0;//��һ��ƫ��ֵ
	pid_m.integral = 0.0;//Ŀ�������
	pid_m.umax = 1200.0;//�ٶȻ�PID�ļ��޷�Χ
	pid_m.umin = 300.0;//�ٶȻ�PID�ļ��޷�Χ
	pid_m.vel = 10.0;//�趨��ֵ
	
	
	pid_n.Target = Target_y;//����Ŀ��ֵ
	pid_n.Encoder = position_y;//����ʵ��ֵ
	pid_n.bias = 0.0;//��ǰƫ��ֵ
	pid_n.last_bias = 0.0;//��һ��ƫ��ֵ
	pid_n.integral = 0.0;//Ŀ�������
	pid_n.umax = 1200.0;//�ٶȻ�PID�ļ��޷�Χ
	pid_n.umin = 300.0;//�ٶȻ�PID�ļ��޷�Χ
	pid_n.vel = 10.0;//�趨��ֵ
	
	
	
	
}

int  PID_speed_realize(float speed_x , float speed_y)
{
	
	int index_speed_x;
	int index_speed_y;
	pid_m.Encoder  = speed_x;
	pid_n.Encoder = speed_y;
	pid_m.bias = pid_m.Target - pid_m.Encoder;
	pid_n.bias = pid_n.Target - pid_n.Encoder;
	
	//x��
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
	
	
	
	//y��
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
�������ܣ��ײ���� �Ƕ���PWMֵ����
ͨ����������ٶȻ����ԺͽǶȣ�λ�û���ϵ���������ǶȻ����ص������꣬���ͨ��������ʼ���
������ö���Ķ�����Ȼ��ִ�У��Ϳ��Ժ��ٶȻ���ϵ���������߹�ͬ����
��ڲ�����angle ������Ƕ�
����  ֵ����
**************************************************************************/
// �Ƕ� -> PWM
float ServoX_Angle2PWM(float angle_x)	
{
	return angle_x*1000/270+250; 
}

// PWM -> �Ƕ�
float ServoX_PWM2Angle(float pwm_x)	
{
	return (pwm_x-250)/1000*270;
}


/**************************************************************************
�������ܣ�ҡ�۶�� �Ƕ���PWMֵ����
ͨ����������ٶȻ����ԺͽǶȣ�λ�û���ϵ���������ǶȻ����ص������꣬���ͨ��������ʼ���
������ö���Ķ�����Ȼ��ִ�У��Ϳ��Ժ��ٶȻ���ϵ���������߹�ͬ����
��ڲ�����
����  ֵ����
**************************************************************************/
// �Ƕ� -> PWM
float ServoY_Angle2PWM(float angle_y)	
{
	return angle_y*1000/180+250; 
}

// PWM -> �Ƕ�
float ServoY_PWM2Angle(float pwm_y)	
{
	return (pwm_y-250)/1000*180;
}

/**************************************************************************
�������ܣ��������ö��PWMֵ(�ڵ�ǰλ�û����������ƶ�)�����ڶ����ƽ��XY��ϳ��˶� �� PID�ٶȻ�
��ڲ��������X �� ���Y ����������ֵ
����  ֵ����
**************************************************************************/
void Set_Pwm_Increase(float incrsX,float incrsY)	
{	
	// �����õ���PWMֵ
	position_x += incrsX;		   
	position_y += incrsY;
	
	// ����PWMֵ
	Set_Pwm_ServoX(position_x);
	Set_Pwm_ServoY(position_y);
	
//	TIM8->CCR1 = positionX;    // �ײ���� PC6 - TIM8_CH1
//	TIM8->CCR2 = positionY;    // ҡ�۶�� PC7 - TIM8_CH2
}

/**************************************************************************
�������ܣ����ö��PWM
��ڲ�������
����  ֵ����
**************************************************************************/
// �ײ���� PC6 - TIM8_CH1
void Set_Pwm_ServoX(float pos)	
{	

	//TIM8->CCR1 = pos;   
}

// ҡ�۶�� PC7 - TIM8_CH2
void Set_Pwm_ServoY(float pos)	
{	
	//TIM8->CCR2 = pos;    
}

/**************************************************************************
�������ܣ�PWM�޷�����ֹPWM���������÷�Χ�����ջٶ��
��ڲ�������
����  ֵ����
**************************************************************************/
void limit_PWM(void)
{	
	int Amplitude_H=1200, Amplitude_L=300;       
   
	//���X������ֵ�������ƶ��ת�� 13.5�� -256.5��
	if(Target_x<Amplitude_L)  Target_x=Amplitude_L;	
	if(Target_x>Amplitude_H)  Target_x=Amplitude_H;	
	
	//���Y������ֵ�������ƶ��ת��    9�� -171��
	if(Target_y<Amplitude_L)  Target_y=Amplitude_L;	
	if(Target_y>Amplitude_H)  Target_y=Amplitude_H;	
}
/**************************************************************************
�������ܣ��ٶ��޷� ��ֹPWM���������÷�Χ�����ջٶ��
��ڲ�������
����  ֵ����
**************************************************************************/
void limit_Speed(void)
{	
	int Amplitude_H = Servo_Speed, Amplitude_L = -Servo_Speed;  
	
    if(increase_x < Amplitude_L)  increase_x = Amplitude_L;	
	if(increase_x > Amplitude_H)  increase_x = Amplitude_H;	
	
	if(increase_y < Amplitude_L)  increase_y = Amplitude_L;	
	if(increase_y > Amplitude_H)  increase_y = Amplitude_H;	
}






































