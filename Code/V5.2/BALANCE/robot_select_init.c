#include "robot_select_init.h"


//��ʼ�������˲����ṹ��
Robot_Parament_InitTypeDef  Robot_Parament; 
/**************************************************************************
�������ܣ����ݵ�λ���л���Ҫ���Ƶ�С������
��ڲ�������
����  ֵ����
**************************************************************************/
void Robot_Select(void) //��Ҫ�ֶ�ѡ���� ��4�� 0=Mec_Car��1=Akm_Car��2=Diff_Car��3=FourWheel_Car
{
	Car_Mode = FourWheel_Car;
	
//	Robot_Init(MEC_wheelspacing,         MEC_axlespacing,			HALL_30F, Hall_13, Mecanum_75);            //�����ķ��С��
//	Robot_Init(Akm_wheelspacing,         Akm_axlespacing,           HALL_30F, Hall_13, Black_WheelDiameter);   //������С��
//	Robot_Init(Diff_wheelSpacing,        0,                         HALL_30F, Hall_13, Black_WheelDiameter);   //���ֲ���С��
	Robot_Init(Four_Mortor_wheelSpacing, Four_Mortor__axlespacing, 	HALL_30F, Hall_13, Black_WheelDiameter);   //������
	
}

/**************************************************************************
�������ܣ���ʼ��С������
��ڲ������־� ��� ��ת�뾶 ������ٱ� ������������� ��ֱ̥��
����  ֵ����
**************************************************************************/
void Robot_Init(double wheelspacing, float axlespacing, float gearratio,float Accuracy,float tyre_diameter) // 
{

	//�־� ���ֳ�Ϊ���־�
	Robot_Parament.WheelSpacing=wheelspacing; 

	//��� ���ֳ�Ϊ�����	
	Robot_Parament.AxleSpacing=axlespacing;   

	//������ٱ�
	Robot_Parament.GearRatio=gearratio; 

	//����������(����������)	
	Robot_Parament.EncoderAccuracy=Accuracy;

	//������ֱ��	
	Robot_Parament.WheelDiameter=tyre_diameter;       
	
	//���(����)ת1Ȧ��Ӧ�ı�������ֵ
	Encoder_precision=EncoderMultiples*Robot_Parament.EncoderAccuracy*Robot_Parament.GearRatio;

	//�������ܳ�	
	Wheel_perimeter=Robot_Parament.WheelDiameter*PI;

	//�־� ���ֳ�Ϊ���־�  
	Wheel_spacing=Robot_Parament.WheelSpacing; 

	//��� ���ֳ�Ϊ�����	
	Axle_spacing=Robot_Parament.AxleSpacing; 

	
}


