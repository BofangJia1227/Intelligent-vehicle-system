#ifndef __ROBOTSELECTINIT_H
#define __ROBOTSELECTINIT_H
#include "sys.h"
#include "system.h"


//С�������ṹ��
typedef struct  
{
	float WheelSpacing;      //�־� ���ֳ�Ϊ���־�
	float AxleSpacing;       //��� ���ֳ�Ϊ�����	
	int GearRatio;           //������ٱ�
	int EncoderAccuracy;     //����������(����������)
	float WheelDiameter;     //������ֱ��	
}Robot_Parament_InitTypeDef;


//�������ṹ��
typedef struct  
{
	int A;      
	int B; 
	int C; 
	int D; 
}Encoder;


//���������͵���Сת��뾶���ɻ�е�ṹ�������־ࡢ��ࡢǰ�����ת��
#define MINI_AKM_MIN_TURN_RADIUS 0.350f 


//�־� ������һ��
//#define MEC_wheelspacing         0.109
#define MEC_wheelspacing         0.0930 //����2021.03.30
#define Akm_wheelspacing         0.162f
#define Diff_wheelSpacing        0.177f
#define Four_Mortor_wheelSpacing 0.187f


//��� ������һ��
#define MEC_axlespacing           0.085
#define Akm_axlespacing           0.144f
#define Diff_axlespacing          0.155f
#define Four_Mortor__axlespacing  0.173f


//������ٱ�
#define   HALL_30F    30
#define   MD36N_5_18  5.18
#define   MD36N_27    27
#define   MD36N_51    51
#define   MD36N_71    71
#define   MD60N_18    18
#define   MD60N_47    47


//����������
#define		Photoelectric_500 500
#define	  Hall_13           13


//������ֱ̥��
#define		Mecanum_60  0.060f
#define		Mecanum_75  0.075f
#define		Mecanum_100 0.100f
#define		Mecanum_127 0.127f
#define		Mecanum_152 0.152f
 

//�־�ȫ����ֱ��ϵ��
#define	  FullDirecion_60  0.060
#define	  FullDirecion_75  0.075
#define	  FullDirecion_127 0.127
#define	  FullDirecion_152 0.152
#define	  FullDirecion_203 0.203
#define	  FullDirecion_217 0.217


//��ɫ��ֱ̥��
#define	  Black_WheelDiameter   0.065


//��������Ƶ����ȡ���ڱ�������ʼ������
#define   EncoderMultiples  4

//���������ݶ�ȡƵ��
#define   CONTROL_FREQUENCY 100

//#define PI 3.1415f  //PI //Բ����

void Robot_Select(void);
void Robot_Init(double wheelspacing, float axlespacing, float gearratio,float Accuracy,float tyre_diameter);

#endif
