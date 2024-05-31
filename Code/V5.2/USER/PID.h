#ifndef __PID_H
#define __PID_H
/* 
#define   KP    2
#define   KI    0.15
#define   KD    2
*/
//λ�û�PID����
#define   KPx_angle    2
#define   KIx_angle    0.15
#define   KDx_angle    2
#define   KPy_angle    2
#define   KIy_angle    0.15
#define   KDy_angle    2
//�ٶȻ�PID����
#define   KPx_speed    2
#define   KIx_speed    0.15
#define   KDx_speed    2
#define   KPy_speed    2
#define   KIy_speed    0.15
#define   KDy_speed    2
 

//����̨ת���Ŀ���ϵͳ�£����ýǶȻ��Լ��ٶȻ�����PID�ջ�ϵͳ
/**************************************************************************
�������ܣ��ǶȻ�����nano����������ת��Ϊ��̨��Ҫ�ƶ��ĽǶȣ����PID����
��ڲ�������
����  ֵ����
**************************************************************************/
/*���ÿ����ֱ��͵ı����PID���ƣ�������ɢ����ת��Ϊ���λ���
���ÿ����ֱ��͵�PID����
��ǰ�����error_angle �������ֵumax_angleʱ�������ۼ�
��ǰ�����error_angleС����Сֵumin_angleʱ�������ۼ�
**************************************************************************
���û��ַ���ʽPID����
�������������趨ֵƫ��ϴ�ʱ��ȡ���������ã��Ա������ڻ�������ʹϵͳ�ȶ��Խ��ͣ�����������
���������ӽ�����ֵʱ��������ֿ��ƣ��Ա����������߿��ƾ��ȡ�
��ϵͳ���趨��ֵΪthr = 0.3cm
�����ľ���ֵ����thrʱ������PD���ƣ����Ա����������ĳ�������ʹϵͳ�нϿ����Ӧ
�����ľ���ֵС�ڵ���thrʱ������PID���ƣ��Ա�֤ϵͳ�Ŀ��ƾ���
*/
typedef struct
{
float Set_angle; // �����趨ֵ
float Actual_angle; // ����ʵ��ֵ
float umax_angle;//�Ƕ�ʽPID�ļ��޷�Χ
float umin_angle;//	�Ƕ�ʽPID�ļ��޷�Χ
float error_angle;//��ǰƫ��ֵ
float error_last_angle;//	��һ��ƫ��ֵ
float thr_angle;//�趨��ֵ
float integral_angle;//�������ֵ
float pox;//PID�����������
} PID_Para_angle;




/**************************************************************************
�������ܣ��ٶȻ������Ƕ���Ϣת��ΪPWM�����PID����
��ڲ�������
����  ֵ����
**************************************************************************/
typedef struct
{
//float Set_speed; // �����趨ֵ
//float Actual_speed; // ����ʵ��ֵ
//float umax_speed;//�Ƕ�ʽPID�ļ��޷�Χ
//float umin_speed;//	�Ƕ�ʽPID�ļ��޷�Χ
//float error_speed;//��ǰƫ��ֵ
//float error_last_speed;//	��һ��ƫ��ֵ
//float thr_speed;//�趨��ֵ
//float integral_speed;//�������ֵ
	
	float Target;//����Ŀ��ֵ
	float Encoder;//����ʵ��ֵ
	float bias;//��ǰƫ��ֵ
	float last_bias;//��һ��ƫ��ֵ
	float vel;//�趨��ֵ
	float umax;//�ٶȻ�PID�ļ��޷�Χ
	float umin;//�ٶȻ�PID�ļ��޷�Χ
	float integral;//Ŀ�������
	int pwm;
} PID_Para_speed;




/**************************************************************************/
void PID_angle_init(void);
float PID_angle_realize(float x,float y);//����ת�Ƕ�
void PID_realize_init(void);
int PID_speed_realize(float angle_x , float angle_y);//�Ƕ�תpwm
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

