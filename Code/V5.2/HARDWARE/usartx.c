#include "usartx.h"
#include "balance.h"
#include "DHT11.h" // 引入DHT11温湿度传感器库
#include "MQ2.h"   // 引入MQ2空气质量传感器库
#include "BH1750.h" // 引入BH1750光照强度传感器库
#define USART3_REC_LEN  200 

#define PATH_L 0
#define PATH_R 1

u8  USART3_RX_BUF[USART3_REC_LEN]; 
u16 USART3_RX_STA; 

int biass = 0;			// openmv巡线偏差
int Car_STOP = 0;		// 停止线停车控制变量
int Car_path = PATH_L;	// 小车走的路线 0为内圈(左侧路线) 1为外圈(右侧路线)
int Car_LapNum = 0;		// 小车已完成的圈数 通过停止位计数，经过一个停止位为一圈
int Car_LapNum_Tar = 1;	// 小车目标圈数

SEND_DATA Send_Data;
RECEIVE_DATA Receive_Data;
extern int Time_count;

/**************************************************************************
函数功能：串口1初始化
入口参数：无
返 回 值：无
**************************************************************************/
void uart1_init(u32 bound)
{  	 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	 //使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);   //使能USART时钟

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10 ,GPIO_AF_USART1);	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;            //输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;       //高速50MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);  		      //初始化
	
    //UsartNVIC配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	//IRQ通道使能
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	//根据指定的参数初始化VIC寄存器	
	NVIC_Init(&NVIC_InitStructure);	
	
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound; 					//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 	//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//收发模式
	USART_Init(USART1, &USART_InitStructure); 											//初始化串口1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 					//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                     			 		//使能串口1
}
/**************************************************************************
函数功能：串口2初始化
入口参数：无
返回  值：无
**************************************************************************/
void uart2_init(u32 bound)
{  	 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);		//使能GPIO时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 		//使能USART时钟
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6 ,GPIO_AF_USART2);	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;            //输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;       //高速50MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);  		      //初始化
	
	//UsartNVIC配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;
	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    //IRQ通道使能	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//根据指定的参数初始化VIC寄存器		
	NVIC_Init(&NVIC_InitStructure);	
	
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;                 		//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 		//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 				//一个停止
	USART_InitStructure.USART_Parity = USART_Parity_No;					//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//收发模式
	USART_Init(USART2, &USART_InitStructure);      						//初始化串口2
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 						//开启串口接受中断
	USART_Cmd(USART2, ENABLE);                    						//使能串口2 
}

/**************************************************************************
函数功能：串口3初始化
入口参数：无
返回  值：无
**************************************************************************/
void uart3_init(u32 bound)
{  	 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//UART_InitTypeDef UART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);		//使能GPIO时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 		//使能USART时钟
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3);	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11 ,GPIO_AF_USART3);	 
	
	// PC10 TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;            //复用
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;       //高速50MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);  		      //初始化
	
	//UsartNVIC配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    //IRQ通道使能	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//根据指定的参数初始化VIC寄存器		
	NVIC_Init(&NVIC_InitStructure);	
	
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;                 		//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 		//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 				//一个停止
	USART_InitStructure.USART_Parity = USART_Parity_No;					//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//收发模式
	USART_Init(USART3, &USART_InitStructure);      						//初始化串口3
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 						//开启串口接受中断
	USART_Cmd(USART3, ENABLE);                    						//使能串口3 
	USART_ClearFlag(USART3, USART_FLAG_TC);
}

/**************************************************************************
函数功能：串口1接收中断
入口参数：无
返 回 值：无
**************************************************************************/
int USART1_IRQHandler(void)
{	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //Check if data is received //判断是否接收到数据
	{
		u8 Usart_Receive;		
		Usart_Receive = USART_ReceiveData(USART1); //Read the data //读取数据	
	}
	return 0;	
}
/**************************************************************************
函数功能：串口2接收中断
入口参数：无
返回  值：无
**************************************************************************/
int USART2_IRQHandler(void)
{	
	static int temp_count = 0;								//用于记录前进的指令的次数，第一次连接蓝牙的时候需要用到
	int Usart_Receive;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 	//判断是否接收到数据
	{	      
		static u8 Flag_PID,i,j,Receive[50];
		static float Data;
				
		Usart_Receive=USART2->DR; 				//读取数据

	  
		if(Usart_Receive==0x4B)  	//进入APP转向控制界面
			Turn_Flag=1;  
		else if(Usart_Receive==0x49||Usart_Receive==0x4A) 	//进入APP方向控制界面	
			Turn_Flag=0;	
		
		if(Turn_Flag==0) 
		{
			//APP摇杆控制界面命令
			if(Usart_Receive>=0x41&&Usart_Receive<=0x48)  
			{	
				Flag_Direction=Usart_Receive-0x40;
			}
			else if(Usart_Receive<=8)   
			{			
				Flag_Direction=Usart_Receive;
			}	
			else Flag_Direction=0;
		}
		else if(Turn_Flag==1)
		{
			//APP转向控制界面命令
			if     (Usart_Receive==0x43) Flag_Left=0,Flag_Right=1; //Right rotation //右自转
			else if(Usart_Receive==0x47) Flag_Left=1,Flag_Right=0; //Left rotation  //左自转
			else                         Flag_Left=0,Flag_Right=0;
			if     (Usart_Receive==0x41||Usart_Receive==0x45) 
			{
				if((++temp_count) == 5)					//需要连续发送5次前进的指令，上拉转盘一段时间可开始app控制
				{
					temp_count = 0;
					APP_ON_Flag = RC_ON;
				}
				Flag_Direction=Usart_Receive-0x40;
			}
			else  Flag_Direction=0;
		}
		if(Usart_Receive==0x58)  RC_Velocity=RC_Velocity+100; //加速按键，+100mm/s
		if(Usart_Receive==0x59)  RC_Velocity=RC_Velocity-100; //减速按键，-100mm/s
	  
	 //以下是与APP调试界面通讯
	 if(Usart_Receive==0x7B) Flag_PID=1;   	//APP参数指令起始位
	 if(Usart_Receive==0x7D) Flag_PID=2;   	//APP参数指令停止位

	 if(Flag_PID==1) 	//采集数据
	 {
		Receive[i]=Usart_Receive;
		i++;
	 }
	 if(Flag_PID==2) 	//分析数据
	 {
			if(Receive[3]==0x50) 	 PID_Send=1;
			else  if(Receive[1]!=0x23) 
      {								
				for(j=i;j>=4;j--)
				{
					Data+=(Receive[j-1]-48)*pow(10,i-j);
				}
				
				if(Mode == APP_Control_Mode)
					{
					switch(Receive[1])			//APP电机调参
					 {
						 case 0x30:  RC_Velocity=Data;break;
						 case 0x31:  Velocity_KP=Data;break;
						 case 0x32:  Velocity_KI=Data;break;
						 case 0x33:  break;
						 case 0x34:  break;
						 case 0x35:  break;
						 case 0x36:  break;
						 case 0x37:  break;
						 case 0x38:  break; 				
					 } 
					}
//					else if(Mode == ELE_Line_Patrol_Mode)		//电磁巡线z轴速度调参
//					{
//					switch(Receive[1])
//					 {
//						 case 0x30:  RC_Velocity_ELE=Data;break;
//						 case 0x31:  ELE_KP=Data;break;
//						 case 0x32:  ELE_KI=Data;break;
//						 case 0x33:  break;
//						 case 0x34:  break;
//						 case 0x35:  break;
//						 case 0x36:  break;
//						 case 0x37:  break;
//						 case 0x38:  break; 				
//					 } 
//					}
					else if(Mode == CCD_Line_Patrol_Mode)									//CCD巡线z轴速度调参
					{
					switch(Receive[1])
					 {
						 case 0x30:  RC_Velocity_CCD=Data;break;
						 case 0x31:  CCD_KP=Data;break;
						 case 0x32:  CCD_KI=Data;break;
						 case 0x33:  break;
						 case 0x34:  break;
						 case 0x35:  break;
						 case 0x36:  break;
						 case 0x37:  break;
						 case 0x38:  break; 				
					 } 
					}
					else if(Mode == OPENMV_Line_Patrol_Mode)									//OPENMV巡线z轴速度调参
					{
					switch(Receive[1])
					 {
						 case 0x30:  RC_Velocity_OPENMV=Data;break;
						 case 0x31:  OPENMV_KP=Data;break;
						 case 0x32:  OPENMV_KI=Data;break;
						 case 0x33:  Car_path = Data; break;		// 当前圈所走路径调节
						 case 0x34:  Car_LapNum_Tar = Data; break;	// 总目标圈数调节
						 case 0x35:  break;
						 case 0x36:  break;
						 case 0x37:  break;
						 case 0x38:  break; 				
					 } 
					}
					else if(Mode == Lidar_Along_Mode)
					{
						switch(Receive[1])
						{
						 case 0x30:  Along_Distance_KP=Data;break;
						 case 0x31:  Along_Distance_KD=Data;break;
						 case 0x32:  Along_Distance_KI=Data;break;
						 case 0x33:  break;
						 case 0x34:  break;
						 case 0x35:  break;
						 case 0x36:  break;
						 case 0x37:  break;
						 case 0x38:  break; 	
						}
					}
					else if(Mode == Lidar_Follow_Mode)
					{
						switch(Receive[1])
						{
						 case 0x30:  break;
						 case 0x31:  break;
						 case 0x32:  break;
						 case 0x33:  break;
						 case 0x34:  break;
						 case 0x35:  break;
						 case 0x36:  break;
						 case 0x37:  break;
						 case 0x38:  break; 	
						}
					}
					else if(Mode == Environment_Patrol_Mode)
					
					{
						
						switch(Receive[1])
						{
						 case 0x30:  //(rec_data[2]+rec_data[3]/10) = Data ;break;
						 case 0x31:  //(rec_data[0]+rec_data[0]/8) = Data;break;
						 case 0x32:  ppm = Data ; break;
						 case 0x33:  result1750  =Data; break;
						 case 0x34:  break;
						 case 0x35:  break;
						 case 0x36:  break;
						 case 0x37:  break;
						 case 0x38:  break; 	
						}
						
						
					}
				}				
      //相关标志位清零			
			Flag_PID=0;
			i=0;
			j=0;
			Data=0;
			memset(Receive, 0, sizeof(u8)*50); 	//数组清零
	 }
	if(RC_Velocity<0)   RC_Velocity=0; 
	if(RC_Velocity_CCD<0)   RC_Velocity_CCD=0; 
	if(RC_Velocity_OPENMV<0)  RC_Velocity_OPENMV=0; 
  }
  return 0;	
}

/**************************************************************************
函数功能：串口3全局中断函数 openmv
入口参数：无
返 回 值：无

int USART3_IRQHandler(void)			 
{
		u8 com_data; 
		u8 i;
		static u8 RxCounter1=0;
		static uint8_t  RxBuffer1[10]={0};
		static u8 RxState = 0;	
		static u8 RxFlag1 = 0;
		static u8 Recv_cnt = 0;		// 用于停止位的收包计数，开始判断停止位后若一定包数内没有满足停止位条件，则停止判断停止位，并清零包数    停止位判断：3 2 1按顺序出现
		static u8 stopROI3 = 0;		// 是否在3区域识别到停止位
		static u8 stopROI2 = 0;		// 是否在2区域识别到停止位
		static u8 stopFlag = 0;		// 是否确认遇到停止位标记
		
		int datalen = 6;	// 数据包长度为6, bb bbb b (2c 12 data1 data2 data3 5b): data1为内圈路线偏移量, data2为外圈路线偏差值, data3为停止位识别区域

		if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)  	   //接收中断  
		{
				USART_ClearITPendingBit(USART3,USART_IT_RXNE);   //清除中断标志
				com_data = USART_ReceiveData(USART3);
			
				if(RxState==0 && com_data==0x2C)  //0x2c帧头
				{
					RxState=1;
					RxBuffer1[RxCounter1++]=com_data;
				}
		
				else if(RxState==1 && com_data==0x12)  //0x12帧头
				{
					RxState=2;
					RxBuffer1[RxCounter1++]=com_data;
				}
		
				else if(RxState==2)
				{
					RxBuffer1[RxCounter1++]=com_data;

					if(RxCounter1 >= datalen || com_data == 0x5B)       //RxBuffer1接受到了1个数据包长度 或 收到停止位,接收数据结束
					{
						RxState=3;
						RxFlag1=1;
						if(Recv_cnt > 0) Recv_cnt++;	// 在 判断停止位 及 调整停车位置 的过程中，收包计数
						if(stopFlag == 0 && Recv_cnt > 150)	// 处于判断停止位的过程中		(150个包是小车以10cm/s的速度行进时，停止线从进入摄像头图像区域到超出区域收到的数据包数量)
						{
							Recv_cnt = 0;	// 停止计数
							stopROI2 = 0;	// 重置标记位
							stopROI3 = 0;
						}
						if(stopFlag == 1 && Recv_cnt > 15)	// 确定遇到停止位	再接受一定数量的包就停车，便于调整小车停止位置与停止线的相对位置  250mm->23 300mm->19  350mm->
						{
							Car_STOP = 1;
							OLED_ShowString(50,10,"st");
							
							Recv_cnt = 0;
							stopFlag = 0;
						}
						
						
						// 选择内外圈路线 接收偏移量
						if(Car_path == PATH_L)	// 左侧路线 内圈
							biass = (int)RxBuffer1[RxCounter1-4];
						else 					// 右侧路线 外圈
							biass = (int)RxBuffer1[RxCounter1-3]; 
						biass -= 80;	// 中心偏移量-80
						//biass -=90;		// 偏移角-90
					
						
						// 判断停止线
						if((int)RxBuffer1[RxCounter1-2] == 3) // 区域3发现停止线
						{
							stopROI3 = 1;	// 修改标记位
							OLED_ShowNumber(90,00, 3 ,1,12);	// 输出3
							Recv_cnt++;		// 从0变成1，开始计数
						}
						else if((int)RxBuffer1[RxCounter1-2] == 2 && stopROI3 == 1) // 在3基础上，区域2发现停止线
						{
							OLED_ShowNumber(90,00, 2 ,1,12);	// 输出2
							stopROI2 = 1;	// 修改标记位
						}
						else if((int)RxBuffer1[RxCounter1-2] == 1 && stopROI2 == 1) // 在2基础上，区域1发现停止线		可以确认是停止标志
						{
							OLED_ShowNumber(90,00, 1 ,1,12);	// 输出1
							OLED_ShowString(50,10,"ws");		// 输出等待停止标记
							
							Recv_cnt = 0;	// 重置计数器
							stopROI2 = 0;	// 重置标记位
							stopROI3 = 0;
							
							Car_LapNum++;	// 完成圈数+1
							OLED_ShowNumber(110,00, Car_LapNum ,1,12);	// 右上角输出当前完成圈数
							
							if(Car_LapNum%2 == 1) Car_path = PATH_R;	// 内外圈交替走，第一圈走左侧内圈
							else Car_path = PATH_L;
							if(Car_LapNum == Car_LapNum_Tar) 			// 到达目标圈数，停车
							{	
								stopFlag = 1;
								Recv_cnt = 1;	// 从1开始计数，通过延时一定数量的接收到的包来调整停止位置			
							}
						}
						else if((int)RxBuffer1[RxCounter1-2] == 1 && stopROI3 == 1 && stopROI2 == 0) // 3区域发现停止线后没收到2，收到1了，停止判断，说明不是停止线
						{
							OLED_ShowNumber(90,00, 0 ,1,12);	// 重置 停止识别
							stopROI3 = 0;	
							Recv_cnt = 0;
						}
						
					}
				}
				else if(RxState==3)		//检测是否接受到结束标志
				{
						if(RxBuffer1[RxCounter1-1] == 0x5B)
						{
									USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);//关闭DTSABLE中断
										
									RxFlag1 = 0;
									RxCounter1 = 0;
									RxState = 0;
									USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
						}
						else   //接收错误
						{
									RxState = 0;
									RxCounter1=0;
									for(int i = 0; i < datalen+1; i++)	RxBuffer1[i] = 0;		// 多清零一位，保险
									//memset(RxBuffer1, 0, sizeof RxBuffer1); 	//数组清零	// 可能出问题
						}
				}  
	
				else   //接收异常
				{
						RxState = 0;
						RxCounter1=0;
						for(int i = 0; i < datalen+1; i++)	RxBuffer1[i] = 0;
						//memset(RxBuffer1, 0, sizeof RxBuffer1); 	//数组清零	// 可能出问题
				}
		}

	return 0;
		
}
**************************************************************************/

int USART3_IRQHandler(void)	
{
    	u8 com_data; 
		static u8 RxCounter1=0;
		static uint8_t  RxBuffer1[7]={0};
		static u8 RxState = 0;	
		static u8 RxFlag1 = 0;
		int datalen = 4;
		
		
		
		if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)  	   //接收中断  
		{
				USART_ClearITPendingBit(USART3,USART_IT_RXNE);   //清除中断标志
				com_data = USART_ReceiveData(USART3);
			
				if(RxState==0 && com_data==0x2C)  //0x2c帧头
				{
					RxState=1;
					RxBuffer1[RxCounter1++]=com_data;
				}
		
				else if(RxState==1 && com_data==0x12)  //0x12帧头
				{
					RxState=2;
					
				}
		
				else if(RxState==2)
				{
					RxBuffer1[RxCounter1++]=com_data;

					if(RxCounter1 >= datalen || com_data == 0x5B)       //RxBuffer1接受到了1个数据包长度 或 收到停止位,接收数据结束
					{
						
						RxBuffer1[RxCounter1++]=com_data;
						RxBuffer1[3] = rec_data[2]+rec_data[3]/10 ;
						RxBuffer1[4] = rec_data[0]+rec_data[0]/8 ;
						RxBuffer1[5] = ppm  ;
						RxBuffer1[6] = result1750 ;  
						RxState=3;
						RxFlag1=1;
						
					}
				}
				else if(RxState==3)		//检测是否接受到结束标志
				{
						if(RxBuffer1[RxCounter1-1] == 0x5B)
						{
									USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);//关闭DTSABLE中断
										
									RxFlag1 = 0;
									RxCounter1 = 0;
									RxState = 0;
									USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
						}
						else   //接收错误
						{
									RxState = 0;
									RxCounter1=0;
									for(int i = 0; i < datalen+1; i++)	RxBuffer1[i] = 0;		// 多清零一位，保险
									//memset(RxBuffer1, 0, sizeof RxBuffer1); 	//数组清零	// 可能出问题
						}
				}  
	
				else   //接收异常
				{
						RxState = 0;
						RxCounter1=0;
						for(int i = 0; i < datalen+1; i++)	RxBuffer1[i] = 0;
						//memset(RxBuffer1, 0, sizeof RxBuffer1); 	//数组清零	// 可能出问题
				}
		}

		
		
	return 0;	
		
		

}



    





/**************************************************************************
函数功能：将上位机发过来目标前进速度Vx、目标角速度Vz，转换为阿克曼小车的右前轮转角
入口参数：目标前进速度Vx、目标角速度Vz，单位：m/s，rad/s
返回  值：阿克曼小车的右前轮转角，单位：rad
**************************************************************************/
float Vz_to_Akm_Angle(float Vx, float Vz)
{
	float R, AngleR, Min_Turn_Radius;
	//float AngleL;
	
	
	//阿克曼小车需要设置最小转弯半径
	//如果目标速度要求的转弯半径小于最小转弯半径，
	//会导致小车运动摩擦力大大提高，严重影响控制效果
	Min_Turn_Radius=MINI_AKM_MIN_TURN_RADIUS;
	
	if(Vz!=0 && Vx!=0)
	{
		//如果目标速度要求的转弯半径小于最小转弯半径
		if(float_abs(Vx/Vz)<=Min_Turn_Radius)
		{
			//降低目标角速度，配合前进速度，提高转弯半径到最小转弯半径
			if(Vz>0)
				Vz= float_abs(Vx)/(Min_Turn_Radius);
			else	
				Vz=-float_abs(Vx)/(Min_Turn_Radius);	
		}		
		R=Vx/Vz;
		//AngleL=atan(Axle_spacing/(R-0.5*Wheel_spacing));
		AngleR=atan(Axle_spacing/(R+0.5f*Wheel_spacing));
	}
	else
	{
		AngleR=0;
	}
	
	return AngleR;
}

/**************************************************************************
函数功能：将上位机发过来的高8位和低8位数据整合成一个short型数据后，再做单位还原换算
入口参数：高8位，低8位
返回  值：机器人X/Y/Z轴的目标速度
**************************************************************************/
float XYZ_Target_Speed_transition(u8 High,u8 Low)
{
	//数据转换的中间变量
	short transition; 
	
	//将高8位和低8位整合成一个16位的short型数据
	transition=((High<<8)+Low); 
	
	return transition/1000+(transition%1000)*0.001; //Unit conversion, mm/s->m/s //单位转换, mm/s->m/s						
}

/**************************************************************************
函数功能：串口1发送数据
入口参数：要发送的数据
返回  值：无
**************************************************************************/
void usart1_send(u8 data)
{
	USART1->DR = data;
	while((USART1->SR&0x40)==0);	
}

/**************************************************************************
函数功能：串口2发送数据
入口参数：要发送的数据
返回  值：无
**************************************************************************/
void usart2_send(u8 data)
{
	USART2->DR = data;
	while((USART2->SR&0x40)==0);	
}
/**************************************************************************
函数功能：串口2发送数据
入口参数：要发送的数据
返回  值：无
**************************************************************************/
void usart3_send(u8 data)
{
	USART3->DR = data;
	while((USART3->SR&0x40)==0);	
}


/**************************************************************************
函数功能：舵机PWM引脚初始化
入口参数：arr：自动重装值  psc：时钟预分频数 
返回  值：无
**************************************************************************/
void ServoX_PWM_Init(u16 arr,u16 psc)	// 底部舵机 控制x轴左右移动 PC6 - TIM8_CH1
{ 
	GPIO_InitTypeDef GPIO_InitStructure;           //IO
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //定时器	
	TIM_OCInitTypeDef  TIM_OCInitStructure;        //PWM输出
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//使能PORTC时钟	
		
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; 	// 此处云台代码为 GPIO_Speed_2MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);

	/*** Initialize timer 1 || 初始化定时器1 ***/
	TIM_TimeBaseStructure.TIM_Period = arr; 						//设定计数器自动重装值 ARR
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 						//预分频器 PSC
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//TIM向上计数模式	
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); 				//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//-----------舵机初始化-----------//
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; 								//设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   	//输出极性:TIM输出比较极性高	
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;   
	TIM_OC1Init(TIM8, &TIM_OCInitStructure); 						//根据TIM_OCInitStruct中指定的参数初始化外设TIMx	
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);					//通道预装载使能	 
	
	//-----------舵机初始化-----------// 
	TIM_CtrlPWMOutputs(TIM8,ENABLE); 			//高级定时器输出必须使能这句	
	TIM_Cmd(TIM8, ENABLE); 		 				//使能定时器


	//通道值初始化为750，舵机零点对应值
	//TIM8->CCR1=750;
}

/**************************************************************************
函数功能：舵机PWM引脚初始化
入口参数：arr：自动重装值  psc：时钟预分频数 
返回  值：无
**************************************************************************/
void ServoY_PWM_Init(u16 arr,u16 psc)	// 摇臂舵机 控制y轴上下移动 PC7 - TIM8_CH2
{ 
	GPIO_InitTypeDef GPIO_InitStructure;           //IO
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //定时器	
	TIM_OCInitTypeDef  TIM_OCInitStructure;        //PWM输出
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//使能PORTC时钟	
		
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; 	// 此处云台代码为 GPIO_Speed_2MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);

	/*** Initialize timer 1 || 初始化定时器1 ***/
	
	TIM_TimeBaseStructure.TIM_Period = arr; 		//设定计数器自动重装值 ARR
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 		//预分频器 PSC
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//TIM向上计数模式	
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); 	//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//-----------舵机初始化-----------//
	
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 	//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; 				//设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;  
	TIM_OC2Init(TIM8, &TIM_OCInitStructure); 		//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);	//通道预装载使能	 
	//-----------舵机初始化-----------// 
	
	//高级定时器输出必须使能这句	
	TIM_CtrlPWMOutputs(TIM8,ENABLE); 	
	//使能定时器
	TIM_Cmd(TIM8, ENABLE); 		 


	//通道值初始化为750，舵机零点对应值
	//TIM8->CCR2=750;
}
