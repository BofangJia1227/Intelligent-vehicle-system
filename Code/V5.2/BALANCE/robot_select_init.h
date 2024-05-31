#ifndef __ROBOTSELECTINIT_H
#define __ROBOTSELECTINIT_H
#include "sys.h"
#include "system.h"


//小车参数结构体
typedef struct  
{
	float WheelSpacing;      //轮距 麦轮车为半轮距
	float AxleSpacing;       //轴距 麦轮车为半轴距	
	int GearRatio;           //电机减速比
	int EncoderAccuracy;     //编码器精度(编码器线数)
	float WheelDiameter;     //主动轮直径	
}Robot_Parament_InitTypeDef;


//编码器结构体
typedef struct  
{
	int A;      
	int B; 
	int C; 
	int D; 
}Encoder;


//阿克曼车型的最小转弯半径，由机械结构决定：轮距、轴距、前轮最大转角
#define MINI_AKM_MIN_TURN_RADIUS 0.350f 


//轮距 麦轮是一半
//#define MEC_wheelspacing         0.109
#define MEC_wheelspacing         0.0930 //修正2021.03.30
#define Akm_wheelspacing         0.162f
#define Diff_wheelSpacing        0.177f
#define Four_Mortor_wheelSpacing 0.187f


//轴距 麦轮是一半
#define MEC_axlespacing           0.085
#define Akm_axlespacing           0.144f
#define Diff_axlespacing          0.155f
#define Four_Mortor__axlespacing  0.173f


//电机减速比
#define   HALL_30F    30
#define   MD36N_5_18  5.18
#define   MD36N_27    27
#define   MD36N_51    51
#define   MD36N_71    71
#define   MD60N_18    18
#define   MD60N_47    47


//编码器精度
#define		Photoelectric_500 500
#define	  Hall_13           13


//麦轮轮胎直径
#define		Mecanum_60  0.060f
#define		Mecanum_75  0.075f
#define		Mecanum_100 0.100f
#define		Mecanum_127 0.127f
#define		Mecanum_152 0.152f
 

//轮径全向轮直径系列
#define	  FullDirecion_60  0.060
#define	  FullDirecion_75  0.075
#define	  FullDirecion_127 0.127
#define	  FullDirecion_152 0.152
#define	  FullDirecion_203 0.203
#define	  FullDirecion_217 0.217


//黑色轮胎直径
#define	  Black_WheelDiameter   0.065


//编码器倍频数，取决于编码器初始化设置
#define   EncoderMultiples  4

//编码器数据读取频率
#define   CONTROL_FREQUENCY 100

//#define PI 3.1415f  //PI //圆周率

void Robot_Select(void);
void Robot_Init(double wheelspacing, float axlespacing, float gearratio,float Accuracy,float tyre_diameter);

#endif
