

#include "lidar.h"
#include <string.h>
#include "sys.h"

float Diff_Along_Distance_KP = -0.030f,Diff_Along_Distance_KD = -0.245f,Diff_Along_Distance_KI = -0.001f;	//差速小车走直线距离调整PID参数
float Akm_Along_Distance_KP = -0.115f*1000,Akm_Along_Distance_KD = -1000.245f*1000,Akm_Along_Distance_KI = -0.001f*1000;	//阿克曼走直线距离调整PID参数
float FourWheel_Along_Distance_KP = -0.115f*1000,FourWheel_Along_Distance_KD = -100.200f*1000,FourWheel_Along_Distance_KI = -0.001f*1000;	//四驱走直线距离调整PID参数
float Along_Distance_KP = 0.163f*1000,Along_Distance_KD = 0.123*1000,Along_Distance_KI = 0.001f*1000;	//麦轮和全向走直线距离调整PID参数

float Distance_KP = -0.150f,Distance_KD = -0.052f,Distance_KI = -0.001;	//跟随距离调整PID参数

float Follow_KP = -2.566f,Follow_KD = -1.368f,Follow_KI = -0.001f;  //转向PID
float Follow_KP_Akm = -0.550f,Follow_KD_Akm = -0.121f,Follow_KI_Akm = -0.001f;
//float Distance_KP = 0.653f*1000,Distance_KD = 2.431f*1000,Distance_KI = 0.001*1000;	//距离调整PID参数

PointDataProcessDef PointDataProcess[1200];//更新225个数据
LiDARFrameTypeDef Pack_Data;
PointDataProcessDef Dataprocess[1200];      //用于小车避障、跟随、走直线、ELE雷达避障的雷达数据

/**************************************************************************
函数功能：雷达串口初始化
入口参数: 无 
返回  值：无
**************************************************************************/	 	
//串口5

void LIDAR_USART_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	USART_DeInit(UART5);  //复位串口5

	// 打开串口GPIO的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOC D时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); //Enable the Usart clock //使能USART时钟
	 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2 ,GPIO_AF_UART5);	 

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;            //输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;       //高速50MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);  		          //初始化

	// 将USART Rx的GPIO配置为浮空输入模式
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;            //输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;       //高速50MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);  		          //初始化

	//配置USART为中断源 
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	//抢断优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	//子优先级 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	//使能中断 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//初始化配置NVIC
	NVIC_Init(&NVIC_InitStructure);


	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate =460800;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(UART5, &USART_InitStructure);

	// 使能串口接收中断
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);	


	// 使能串口
	USART_Cmd(UART5, ENABLE);	    

}

/**************************************************************************
函数功能：数据处理函数
入口参数：无
返回  值：无
**************************************************************************/
//完成一帧接收后进行处理
int data_cnt = 0;
void data_process(void) //数据处理
{
	int m;
	u32 distance_sum[32]={0};//2个点的距离和的数组
	float start_angle = (((u16)Pack_Data.start_angle_h<<8)+Pack_Data.start_angle_l)/100.0;//计算32个点的开始角度
	float end_angle = (((u16)Pack_Data.end_angle_h<<8)+Pack_Data.end_angle_l)/100.0;//计算32个点的结束角度
	float area_angle[32]={0};
	
	if(start_angle>end_angle)//结束角度和开始角度被0度分割的情况
		end_angle +=360;

	for(m=0;m<32;m++)
	{
		area_angle[m]=start_angle+(end_angle-start_angle)/32*m;
		if(area_angle[m]>360)  area_angle[m] -=360;
		
		distance_sum[m] +=((u16)Pack_Data.point[m].distance_h<<8)+Pack_Data.point[m].distance_l;//数据高低8位合并

		Dataprocess[data_cnt].angle=area_angle[m];
		Dataprocess[data_cnt++].distance=distance_sum[m];  //一帧数据为32个点
		if(data_cnt == 1152) data_cnt = 0;
	}
	
		
}

/**************************************************************************
函数功能：串口5接收中断
入口参数：无
返回  值：无
**************************************************************************/
void UART5_IRQHandler(void)
{	
//	static u8 Count=0;

	static u8 state = 0;//状态位	
	static u8 crc_sum = 0;//校验和
	static u8 cnt = 0;//用于一帧16个点的计数
	u8 temp_data;

	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) //接收到数据
	{	
		//USART_ClearITPendingBit(UART5, USART_IT_RXNE);
		temp_data=USART_ReceiveData(UART5);	
		switch(state)
		{
			case 0:
				if(temp_data == HEADER_0)//头固定
				{
					Pack_Data.header_0= temp_data;
					state++;
					//校验
					crc_sum += temp_data;
				} else state = 0,crc_sum = 0;
				break;
			case 1:
				if(temp_data == HEADER_1)//头固定
				{
					Pack_Data.header_1 = temp_data;
					state++;
					crc_sum += temp_data;
				} else state = 0,crc_sum = 0;
				break;
			case 2:
				if(temp_data == Length_)//字长固定
				{
					Pack_Data.ver_len = temp_data;
					state++;
					crc_sum += temp_data;
				} else state = 0,crc_sum = 0;
				break;
			case 3:
				Pack_Data.speed_h = temp_data;//速度高八位
				state++;
				crc_sum += temp_data;			
				break;
			case 4:
				Pack_Data.speed_l = temp_data;//速度低八位
				state++;
				crc_sum += temp_data;
				break;
			case 5:
				Pack_Data.start_angle_h = temp_data;//开始角度高八位
				state++;
				crc_sum += temp_data;
				break;
			case 6:
				Pack_Data.start_angle_l = temp_data;//开始角度低八位
				state++;
				crc_sum += temp_data;
				break;
			
			case 7:case 10:case 13:case 16:
			case 19:case 22:case 25:case 28:
			case 31:case 34:case 37:case 40:
			case 43:case 46:case 49:case 52:
				
			case 55:case 58:case 61:case 64:
			case 67:case 70:case 73:case 76:
			case 79:case 82:case 85:case 88:
			case 91:case 94:case 97:case 100:
				Pack_Data.point[cnt].distance_h = temp_data & 0x7f ;//16个点的距离数据，高字节
				state++;
				crc_sum += temp_data;
				break;
			
			case 8:case 11:case 14:case 17:
			case 20:case 23:case 26:case 29:
			case 32:case 35:case 38:case 41:
			case 44:case 47:case 50:case 53:
				
			case 56:case 59:case 62:case 65:
			case 68:case 71:case 74:case 77:
			case 80:case 83:case 86:case 89:
			case 92:case 95:case 98:case 101:
				Pack_Data.point[cnt].distance_l = temp_data;//16个点的距离数据，低字节
				state++;
				crc_sum += temp_data;
				break;
			
			case 9:case 12:case 15:case 18:
			case 21:case 24:case 27:case 30:
			case 33:case 36:case 39:case 42:
			case 45:case 48:case 51:case 54:
				
			case 57:case 60:case 63:case 66:
			case 69:case 72:case 75:case 78:
			case 81:case 84:case 87:case 90:
			case 93:case 96:case 99:case 102:
				Pack_Data.point[cnt].Strong = temp_data;//16个点的强度数据
				state++;
				crc_sum += temp_data;
				cnt++;
				break;
			case 103:case 104:
				state++;
				crc_sum += temp_data;
				cnt++;
				break;
			case 105:
				Pack_Data.end_angle_h = temp_data;//结束角度的高八位
				state++;
				crc_sum += temp_data;			
				break;
			case 106:
				Pack_Data.end_angle_l = temp_data;//结束角度的低八位
				state++;
				crc_sum += temp_data;
				break;
			case 107:
				Pack_Data.crc = temp_data;//校验
				state = 0;
				cnt = 0;
				if(crc_sum == Pack_Data.crc)
				{
					data_process();//数据处理，校验正确不断刷新存储的数据	
				}
				else 
				{
					memset(&Pack_Data,0,sizeof(Pack_Data));//清零
				}
				crc_sum = 0;//校验和清零
				break;
			default: break;
		}
	}		
}

/**************************************************************************
函数功能：走直线雷达距离pid
入口参数: 当前距离和目标距离
返回  值：电机目标速度
**************************************************************************/	 	
//走直线雷达距离调整pid

float Along_Adjust_PID(float Current_Distance,float Target_Distance)//距离调整PID
{
	static float Bias,OutPut,Integral_bias,Last_Bias;
	Bias=Target_Distance-Current_Distance;                          	//计算偏差
	Integral_bias+=Bias;	                                 			//求出偏差的积分
	
	if(Integral_bias>1000)	Integral_bias=1000;
	else if(Integral_bias<-1000) Integral_bias=-1000;
	
	if(Car_Mode == FourWheel_Car)
		OutPut=FourWheel_Along_Distance_KP*Bias/100000+FourWheel_Along_Distance_KI*Integral_bias/100000+FourWheel_Along_Distance_KD*(Bias-Last_Bias)/100000;//位置式PID控制器
	else if(Car_Mode == Akm_Car)
		OutPut=Akm_Along_Distance_KP*Bias/100000+Akm_Along_Distance_KI*Integral_bias/100000+Akm_Along_Distance_KD*(Bias-Last_Bias)/1000;//位置式PID控制器
	else if(Car_Mode == Diff_Car)
		OutPut=Diff_Along_Distance_KP*Bias/100+Diff_Along_Distance_KI*Integral_bias/100+Diff_Along_Distance_KD*(Bias-Last_Bias);//位置式PID控制器
	else
	  OutPut=-Along_Distance_KP*Bias/100000-Along_Distance_KI*Integral_bias/100000-Along_Distance_KD*(Bias-Last_Bias)/1000;//位置式PID控制器
	
	Last_Bias=Bias;                                       		 			//保存上一次偏差
	
	if(Turn_Off(Voltage)== 1)								//电机关闭，此时积分清零
		Integral_bias = 0;
	
	return OutPut;                                          	
}


/**************************************************************************
函数功能：跟随雷达距离pid
入口参数: 当前距离和目标距离
返回  值：电机目标速度
**************************************************************************/	 	
//跟随雷达距离调整pid
float Distance_Adjust_PID(float Current_Distance,float Target_Distance)//距离调整PID
{
	static float Bias,OutPut,Integral_bias,Last_Bias;
	Bias = Target_Distance - Current_Distance;                          //计算偏差
	Integral_bias += Bias;	                                 			//求出偏差的积分
	if(Integral_bias>1000) 
		Integral_bias = 1000;
	else if(Integral_bias<-1000) 
		Integral_bias = -1000;
	
	OutPut=Distance_KP*Bias/100+Distance_KI*Integral_bias/100+Distance_KD*(Bias-Last_Bias)/100;	//位置式PID控制器
	Last_Bias=Bias;    		//保存上一次偏差
	
	if(Turn_Off(Voltage)== 1)								//电机关闭，此时积分清零
		Integral_bias = 0;
	
	return OutPut;                                          	
}

/**************************************************************************
函数功能：跟随雷达转向pid
入口参数: 当前角度和目标角度
返回  值：电机转向速度
**************************************************************************/	 	
//跟随雷达转向pid
float Follow_Turn_PID(float Current_Angle,float Target_Angle)
{
	static float Bias,OutPut,Integral_bias,Last_Bias;
	Bias=Target_Angle-Current_Angle;                         				 //计算偏差
	Integral_bias+=Bias;	                                 				 //求出偏差的积分
	
	if(Integral_bias>1000)	Integral_bias=1000;
	else if(Integral_bias<-1000)	Integral_bias=-1000;
	
	if(Car_Mode == Akm_Car)
		OutPut=(Follow_KP_Akm/100)*Bias+(Follow_KI_Akm/100)*Integral_bias+(Follow_KD_Akm/100)*(Bias-Last_Bias);	//位置式PID控制器
	else
		OutPut=(Follow_KP/100)*Bias+(Follow_KI/100)*Integral_bias+(Follow_KD/100)*(Bias-Last_Bias);	//位置式PID控制器
	
	Last_Bias=Bias;                                       					 		//保存上一次偏差
	
	if(Turn_Off(Voltage)== 1)								//电机关闭，此时积分清零
		Integral_bias = 0;
	
	return OutPut;                                           					 	//输出
	
}


/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/

//int Incremental_PI_Left (int Encoder,int Target)
//{ 	
//	 static int Bias,Pwm,Last_bias;
//	 Bias=Target-Encoder;                					//计算偏差
//	 Pwm+=11*(Bias-Last_bias)+10*Bias;   	//增量式PI控制器
//	 Last_bias=Bias;	                   					//保存上一次偏差 
//	 return Pwm;                         					//增量输出
//}


//int Incremental_PI_Right (int Encoder,int Target)
//{ 	
//	 static int Bias,Pwm,Last_bias;
//	 Bias=Target-Encoder;                					//计算偏差
//	 Pwm+=11*(Bias-Last_bias)+10*Bias;   	//增量式PI控制器
//	 Last_bias=Bias;	                   					//保存上一次偏差 
//	 return Pwm;                         					//增量输出
//}
