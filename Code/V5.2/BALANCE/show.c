#include "show.h"
#include "balance.h"
#include "DHT11.h"
#include "MQ2.h"
#include "BH1750.h"
int Voltage_Show;
unsigned char i;          
unsigned char Send_Count; 
extern SEND_DATA Send_Data;
extern int Time_count;
extern int Car_path;		// 小车走的路线 定义在usartx.c中
extern int Car_LapNum;		// 小车已完成的圈数  定义在usartx.c中
extern int Car_STOP;		// 小车是否到达停止位 定义在usartx.c中
extern int Car_LapNum_Tar;	// 小车总目标圈数，定义在usartx.c中

/**************************************************************************
函数功能：读取电池电压、蜂鸣器报警、开启自检、向APP发送数据、OLED显示屏显示任务
入口参数：无
返回  值：无
**************************************************************************/
int Buzzer_count=25;
void show_task(void *pvParameters)
{
   u32 lastWakeTime = getSysTickCnt();

   while(1)
   {	
		int i=0;
		static int LowVoltage_1=0, LowVoltage_2=0;
		vTaskDelayUntil(&lastWakeTime, F2T(RATE_20_HZ)); 	//此任务以10Hz的频率运行
		
		//开机时蜂鸣器短暂蜂鸣，开机提醒
		if(Time_count<50)Buzzer=1; 
		else if(Time_count>=51 && Time_count<100)Buzzer=0;
		 
		if(LowVoltage_1==1 || LowVoltage_2==1) Buzzer_count=0;
		if(Buzzer_count<5) Buzzer_count++;
		if(Buzzer_count<5) Buzzer=1; 		//蜂鸣器蜂鸣
		else if(Buzzer_count==5) Buzzer=0;
		
		//读取电池电压
		for(i=0;i<10;i++)
		{
			Voltage_All+=Get_battery_volt(); 
		}
		Voltage=Voltage_All/10;
		Voltage_All=0;
		
		if(LowVoltage_1==1)LowVoltage_1++;  //确保蜂鸣器只响0.5秒
		if(LowVoltage_2==1)LowVoltage_2++; 	//确保蜂鸣器只响0.5秒
		if(Voltage>=12.6f)Voltage=12.6f;
		else if(10<=Voltage && Voltage<10.5f && LowVoltage_1<2)LowVoltage_1++; 	//10.5V，低电量时蜂鸣器第一次报警
		else if(Voltage<10 && LowVoltage_1<2)LowVoltage_2++; 	//10V，小车禁止控制时蜂鸣器第二次报警
					
		APP_Show();		//向APP发送数据
		oled_show(); 	//显示屏显示任务
   }
}  

/**************************************************************************
函数功能：OLED显示屏显示任务
入口参数：无
返回  值：无
**************************************************************************/
void oled_show(void)
{  
    static int count=0;	 
	int Car_Mode_Show = 3;	// 手动指定车型编号为3-四驱车
	
	Voltage_Show=Voltage*100; 
	count++;
	
	//CCD巡线模式显示屏内容显示 CCD线+电机/舵机状态
	if(Mode == CCD_Line_Patrol_Mode)											
	{
		//OLED_Clear();
		//显示屏第1行显示内容//
		OLED_Show_CCD(); 	//动态显示CCD图像
				

		//显示屏第2行显示内容//
		OLED_ShowString(00,10,"Median :");
		OLED_ShowNumber(40,10, CCD_Median ,5,12); 	//CCD中线的数值，巡线最中心位置为64
			
		switch(Car_Mode_Show)	//根据车型编号显示车型			
		{
			case Mec_Car:       OLED_ShowString(86,10,"Mec "); break; 
			case Akm_Car:       OLED_ShowString(86,10,"Akm "); break; 
			case Diff_Car:      OLED_ShowString(86,10,"Diff"); break; 
			case FourWheel_Car: OLED_ShowString(86,10,"4WD "); break; 
		} 
			
		//显示屏第3行显示内容//
		OLED_ShowString(00,20,"Threshold :");						
		OLED_ShowNumber(90,20, CCD_Threshold,5,12);     //显示CCD阈值
			
		//显示屏第4行显示内容//
		//显示所有车型电机A和B的目标速度和当前实际速度//
		OLED_ShowString(00,30,"A:");
		
		if(MOTOR_A.Target<0) 
		{
			OLED_ShowString(15,30,"-");
			OLED_ShowNumber(25,30,-MOTOR_A.Target*1000,5,12);   	//负数转换显示
		}
		else 
		{
			OLED_ShowString(15,30,"+");
			OLED_ShowNumber(25,30, MOTOR_A.Target*1000,5,12);    	//电机目标转速
		}
				
		OLED_ShowString(60,30,"B:");
		
		if(MOTOR_B.Target<0)
		{
			OLED_ShowString(75,30,"-");
			OLED_ShowNumber(85,30,-MOTOR_B.Target*1000,5,12);   	//负数转换显示
		}
		else          
		{
			OLED_ShowString(75,30,"+");
			OLED_ShowNumber(85,30, MOTOR_B.Target*1000,5,12);    	//电机目标转速
		}
				
		//显示屏第5行显示内容
		if(Car_Mode==Mec_Car||Car_Mode==FourWheel_Car)			//麦轮车、四驱车显示电机C和D的目标速度和当前实际速度
		{
			
			OLED_ShowString(00,40,"C:");
			if(MOTOR_C.Target<0)
			{
				OLED_ShowString(15,40,"-");
				OLED_ShowNumber(25,40,-MOTOR_C.Target*1000,5,12);    		//负数转换显示
			}
			else
			{
				OLED_ShowString(15,40,"+");
				OLED_ShowNumber(25,40, MOTOR_C.Target*1000,5,12);    		//电机目标转速
			}
						
			OLED_ShowString(60,40,"D:");
			
			if(MOTOR_D.Target<0)
			{				
				OLED_ShowString(75,40,"-");
				OLED_ShowNumber(85,40,-MOTOR_D.Target*1000,5,12);    		//负数转换显示
			}
			else
			{
				OLED_ShowString(75,40,"+");
				OLED_ShowNumber(85,40, MOTOR_D.Target*1000,5,12);    		//电机目标转速
			}
		}
		else if(Car_Mode==Akm_Car)		//阿克曼小车显示舵机的PWM的数值
		{	
			OLED_ShowString(00,40,"SERVO:");
			if( Servo<0)
			{
				OLED_ShowString(60,40,"-");
				OLED_ShowNumber(80,40,-Servo,4,12);
			}
			else
			{
				OLED_ShowString(60,40,"+");
				OLED_ShowNumber(80,40, Servo,4,12);
			}				
		}	
		else if(Car_Mode==Diff_Car)		//差速小车显示左右电机的PWM的数值
		{
					
			OLED_ShowString(00,40,"MA");
			
			if(MOTOR_A.Motor_Pwm<0)
			{
				OLED_ShowString(20,40,"-");
				OLED_ShowNumber(30,40,-MOTOR_A.Motor_Pwm,4,12);
			}
			else 
			{
				OLED_ShowString(20,40,"+");
				OLED_ShowNumber(30,40, MOTOR_A.Motor_Pwm,4,12); 
			}
			
			OLED_ShowString(60,40,"MB");
					 
			if(MOTOR_B.Motor_Pwm<0) 
			{
				OLED_ShowString(80,40,"-");
				OLED_ShowNumber(90,40,-MOTOR_B.Motor_Pwm,4,12);
			}
			else
			{				
				OLED_ShowString(80,40,"+");
				OLED_ShowNumber(90,40, MOTOR_B.Motor_Pwm,4,12);
			}
		}	 
	}
	
	//OPENMV巡线模式显示屏内容显示 OPENMV线+电机/舵机状态
	else if(Mode == OPENMV_Line_Patrol_Mode)											
	{
		extern int biass;
		
		

   
		//OLED_Clear();
		
		//显示屏第1行显示内容
		
		OLED_ShowString(00,00,"biass:");
		if(biass < 0) 
		{
			OLED_ShowString(50,0,"-");
		}
		else if(biass > 0)		// 可能出问题 不要写else!!(导致电机C不转)
		{	
			OLED_ShowString(50,0," ");
			// OLED_ShowString(50,0,"+");	// 可能出问题 (导致电机C不转)
		}
		
		OLED_ShowNumber(55,00, myabs(biass) ,3,12); 	//显示偏差

		//显示屏第2行显示内容
		if(biass < 0)	
			OLED_ShowString(00,10, "LEFT"); 	//左转
		else if(biass == 0)
			OLED_ShowString(00,10, "FORW"); 		//直行
		else 
			OLED_ShowString(00,10, "RIGH"); 	//右转
			
		switch(Car_Mode_Show)	//根据车型编号显示车型			
		{
			case Mec_Car:       OLED_ShowString(86,10,"Mec "); break; 
			case Akm_Car:       OLED_ShowString(86,10,"Akm "); break; 
			case Diff_Car:      OLED_ShowString(86,10,"Diff"); break; 
			case FourWheel_Car: OLED_ShowString(86,10,"4WD "); break; 
		} 
			
		//显示屏第3行显示内容//
		//OLED_ShowString(00,20,"Threshold :");						
		//OLED_ShowNumber(90,20, CCD_Threshold,5,12);     //显示CCD阈值
			
		//显示屏第4行显示内容//
		//显示所有车型电机A和B的目标速度
		OLED_ShowString(00,30,"A:");
		
		if(MOTOR_A.Target<0) 
		{
			OLED_ShowString(15,30,"-");
			OLED_ShowNumber(25,30,-MOTOR_A.Target*1000,5,12);   	//负数转换显示
		}
		else 
		{
			OLED_ShowString(15,30,"+");
			OLED_ShowNumber(25,30, MOTOR_A.Target*1000,5,12);    	//电机目标转速
		}
				
		OLED_ShowString(60,30,"B:");
		
		if(MOTOR_B.Target<0)
		{
			OLED_ShowString(75,30,"-");
			OLED_ShowNumber(85,30,-MOTOR_B.Target*1000,5,12);   	//负数转换显示
		}
		else          
		{
			OLED_ShowString(75,30,"+");
			OLED_ShowNumber(85,30, MOTOR_B.Target*1000,5,12);    	//电机目标转速
		}
				
		//显示屏第5行显示内容
		if(Car_Mode==Mec_Car||Car_Mode==FourWheel_Car)			//麦轮车、四驱车显示电机C和D的目标速度
		{
			
			OLED_ShowString(00,40,"C:");
			if(MOTOR_C.Target<0)
			{
				OLED_ShowString(15,40,"-");
				OLED_ShowNumber(25,40,-MOTOR_C.Target*1000,5,12);    		//负数转换显示
			}
			else
			{
				OLED_ShowString(15,40,"+");
				OLED_ShowNumber(25,40, MOTOR_C.Target*1000,5,12);    		//电机目标转速
			}
						
			OLED_ShowString(60,40,"D:");
			
			if(MOTOR_D.Target<0)
			{				
				OLED_ShowString(75,40,"-");
				OLED_ShowNumber(85,40,-MOTOR_D.Target*1000,5,12);    		//负数转换显示
			}
			else
			{
				OLED_ShowString(75,40,"+");
				OLED_ShowNumber(85,40, MOTOR_D.Target*1000,5,12);    		//电机目标转速
			}
		}
		else if(Car_Mode==Akm_Car)		//阿克曼小车显示舵机的PWM的数值
		{	
			OLED_ShowString(00,40,"SERVO:");
			if( Servo<0)
			{
				OLED_ShowString(60,40,"-");
				OLED_ShowNumber(80,40,-Servo,4,12);
			}
			else
			{
				OLED_ShowString(60,40,"+");
				OLED_ShowNumber(80,40, Servo,4,12);
			}				
		}	
		else if(Car_Mode==Diff_Car)		//差速小车显示左右电机的PWM的数值
		{
					
			OLED_ShowString(00,40,"MA");
			
			if(MOTOR_A.Motor_Pwm<0)
			{
				OLED_ShowString(20,40,"-");
				OLED_ShowNumber(30,40,-MOTOR_A.Motor_Pwm,4,12);
			}
			else 
			{
				OLED_ShowString(20,40,"+");
				OLED_ShowNumber(30,40, MOTOR_A.Motor_Pwm,4,12); 
			}
			
			OLED_ShowString(60,40,"MB");
					 
			if(MOTOR_B.Motor_Pwm<0) 
			{
				OLED_ShowString(80,40,"-");
				OLED_ShowNumber(90,40,-MOTOR_B.Motor_Pwm,4,12);
			}
			else
			{				
				OLED_ShowString(80,40,"+");
				OLED_ShowNumber(90,40, MOTOR_B.Motor_Pwm,4,12);
			}
		}
			 
	}
	
	
		//环境检测
	else if (Mode == Environment_Patrol_Mode) {	
    EnvironmentData envData = Get_environment();

    // 在 OLED 上显示数据
	
	OLED_ShowString(0, 00, "tem");
    OLED_ShowFloat_1(50, 00,envData.temperature, 12);
	OLED_ShowString(75, 00, "C");
    // 显示湿度
    OLED_ShowString(0, 10, "hum");
    OLED_ShowFloat_1(50, 10, envData.humidity, 12);
    OLED_ShowString(75, 10, "%RH");
    // 显示气体 PPM 值
    OLED_ShowString(0, 20, "PPM:"); 
    OLED_ShowNumber(50, 20, envData.gas_ppm, 5, 12);
	OLED_ShowString(80, 20, "ppm");
    // 显示 BH1750 光强值
    OLED_ShowString(0, 30, "Light:"); 
    OLED_ShowNumber(50, 30, envData.light_intensity, 5, 12);


}
	
	
	
	
	
	else	//除ccd,openmv的其他模式，显示的电机状态信息									
	{
			//OLED_Clear();

		 //显示屏第1行显示内容
		 switch(Car_Mode_Show)
		 {
			case Mec_Car:       OLED_ShowString(0,0,"Mec "); break; 
			case Akm_Car:       OLED_ShowString(0,0,"Akm "); break; 
			case Diff_Car:      OLED_ShowString(0,0,"Diff"); break; 
			case FourWheel_Car: OLED_ShowString(0,0,"4WD "); break; 
		 }
		 
//		OLED_ShowString(40,0, "Mode:");	//显示模式
//		if(Mode==Normal_Mode)					OLED_ShowString(80,0,"Normal ");
//		else if(Mode==Lidar_Avoid_Mode)			OLED_ShowString(80,0,"Avoid  ");
//		else if(Mode==Lidar_Follow_Mode)		OLED_ShowString(80,0,"Follow ");
		//else if(Mode==ELE_Line_Patrol_Mode)		OLED_ShowString(50,0,"ELE    ");
		 
		 
		//显示屏第2行显示内容
		if(Car_Mode==Mec_Car || Car_Mode==FourWheel_Car)
		{
			//麦轮、四驱车显示电机A的目标速度和当前实际速度
			OLED_ShowString(0,10,"A");
			if( MOTOR_A.Target<0)	
				OLED_ShowString(15,10,"-"),
				OLED_ShowNumber(20,10,-MOTOR_A.Target*1000,5,12);
			else                 	
				OLED_ShowString(15,10,"+"),
				OLED_ShowNumber(20,10, MOTOR_A.Target*1000,5,12); 
			
			if( MOTOR_A.Encoder<0)
				OLED_ShowString(60,10,"-"),
				OLED_ShowNumber(75,10,-MOTOR_A.Encoder*1000,5,12);
			else                 	
				OLED_ShowString(60,10,"+"),
				OLED_ShowNumber(75,10, MOTOR_A.Encoder*1000,5,12);
		}
		else if(Car_Mode==Akm_Car || Car_Mode==Diff_Car)
		{
			//阿克曼、差速车显示电机A的目标速度和当前实际速度
			OLED_ShowString(0,10,"L:");
			if( MOTOR_A.Target<0)
				OLED_ShowString(15,10,"-"),
				OLED_ShowNumber(20,10,-MOTOR_A.Target*1000,5,12);
			else                 	
				OLED_ShowString(15,10,"+"),
				OLED_ShowNumber(20,10, MOTOR_A.Target*1000,5,12);  
			
			if( MOTOR_A.Encoder<0)	
				OLED_ShowString(60,10,"-"),
				OLED_ShowNumber(75,10,-MOTOR_A.Encoder*1000,5,12);
			else                 	
				OLED_ShowString(60,10,"+"),
				OLED_ShowNumber(75,10, MOTOR_A.Encoder*1000,5,12);
		 }
		 
		//显示屏第3行显示内容
		//阿克曼、差速显示电机B的目标速度和当前实际速度
		OLED_ShowString(0,20,"R:");
		 
		if( MOTOR_B.Target<0)	
			OLED_ShowString(15,20,"-"),
			OLED_ShowNumber(20,20,-MOTOR_B.Target*1000,5,12);
		else                 	
			OLED_ShowString(15,20,"+"),
			OLED_ShowNumber(20,20,  MOTOR_B.Target*1000,5,12); 
		if( MOTOR_B.Encoder<0)	
			OLED_ShowString(60,20,"-"),
			OLED_ShowNumber(75,20,-MOTOR_B.Encoder*1000,5,12);
		else                 	
			OLED_ShowString(60,20,"+"),
			OLED_ShowNumber(75,20, MOTOR_B.Encoder*1000,5,12);
		 

		if(Car_Mode==Mec_Car || Car_Mode==FourWheel_Car) //麦轮、四驱车显示电机B的目标速度和当前实际速度
		{
			
			OLED_ShowString(0,20,"B");		
				
			if( MOTOR_B.Target<0)	
				OLED_ShowString(15,20,"-"),
				OLED_ShowNumber(20,20,-MOTOR_B.Target*1000,5,12);
			else                 	
				OLED_ShowString(15,20,"+"),
				OLED_ShowNumber(20,20, MOTOR_B.Target*1000,5,12); 
			
			if( MOTOR_B.Encoder<0)
				OLED_ShowString(60,20,"-"),
				OLED_ShowNumber(75,20,-MOTOR_B.Encoder*1000,5,12);
			else                 	
				OLED_ShowString(60,20,"+"),
				OLED_ShowNumber(75,20, MOTOR_B.Encoder*1000,5,12);
		}
		 
			
		//显示屏第4行显示内容
			
		if(Car_Mode==Mec_Car || Car_Mode==FourWheel_Car)	//麦轮、四驱车、全向轮显示电机C的目标速度和当前实际速度
		{
			OLED_ShowString(0,30,"C");
			
			if( MOTOR_C.Target<0)
				OLED_ShowString(15,30,"-"),
				OLED_ShowNumber(20,30,- MOTOR_C.Target*1000,5,12);
			else
				OLED_ShowString(15,30,"+"),
				OLED_ShowNumber(20,30,  MOTOR_C.Target*1000,5,12); 
				
			if( MOTOR_C.Encoder<0)
				OLED_ShowString(60,30,"-"),
				OLED_ShowNumber(75,30,-MOTOR_C.Encoder*1000,5,12);
			else                 	
				OLED_ShowString(60,30,"+"),
				OLED_ShowNumber(75,30, MOTOR_C.Encoder*1000,5,12);
		}
		if(Car_Mode==Akm_Car)	//阿克曼小车显示舵机的PWM的数值
		{
			OLED_ShowString(00,30,"SERVO:");
			if( Servo<0)		     
				OLED_ShowString(60,30,"-"),
				OLED_ShowNumber(80,30,-Servo,4,12);
			else                 	
				OLED_ShowString(60,30,"+"),
				OLED_ShowNumber(80,30, Servo,4,12); 
		}
		else if(Car_Mode==Diff_Car) //差速小车显示左电机的PWM的数值
		{
			
			OLED_ShowString(00,30,"MA");
			 
			if( MOTOR_A.Motor_Pwm<0)
				OLED_ShowString(40,30,"-"),
				OLED_ShowNumber(50,30,-MOTOR_A.Motor_Pwm,4,12);
			else                 	 
				OLED_ShowString(40,30,"+"),
				OLED_ShowNumber(50,30, MOTOR_A.Motor_Pwm,4,12); 
		}	
		 

		//显示屏第5行显示内容
		if(Car_Mode==Mec_Car||Car_Mode==FourWheel_Car)  //麦轮小车显示电机D的目标速度和当前实际速度
		{
				
			OLED_ShowString(0,40,"D");
				
			if( MOTOR_D.Target<0)	
				OLED_ShowString(15,40,"-"),
				OLED_ShowNumber(20,40,- MOTOR_D.Target*1000,5,12);
			else                 	
				OLED_ShowString(15,40,"+"),
				OLED_ShowNumber(20,40,  MOTOR_D.Target*1000,5,12); 			
			if( MOTOR_D.Encoder<0)
				OLED_ShowString(60,40,"-"),
				OLED_ShowNumber(75,40,-MOTOR_D.Encoder*1000,5,12);
			else                 	
				OLED_ShowString(60,40,"+"),
				OLED_ShowNumber(75,40, MOTOR_D.Encoder*1000,5,12);
		}
		else if(Car_Mode==Diff_Car)	//差速小车显示右电机的PWM的数值
		{
			 
			OLED_ShowString(00,40,"MB");
			
			if(MOTOR_B.Motor_Pwm<0) 
				OLED_ShowString(40,40,"-"),
				OLED_ShowNumber(50,40,-MOTOR_B.Motor_Pwm,4,12);
			else                 	 
				OLED_ShowString(40,40,"+"),
				OLED_ShowNumber(50,40, MOTOR_B.Motor_Pwm,4,12);
		 }			 
	}

	

	//显示屏第6行显示内容
	
	//显示当前控制模式
	if(Mode == CCD_Line_Patrol_Mode)		OLED_ShowString(0,50,"CCD  ");
	else if(Mode == Lidar_Avoid_Mode) 		OLED_ShowString(0,50,"AVO");
	else if(Mode == Lidar_Follow_Mode) 		OLED_ShowString(0,50,"Fol");
	else if(Mode == Lidar_Along_Mode) 		OLED_ShowString(0,50,"Alo");
	else if(Mode == APP_Control_Mode)		OLED_ShowString(0,50,"APP");
	else if(Mode == OPENMV_Line_Patrol_Mode)	OLED_ShowString(0,50,"Open");	// 可能出问题
	else if(Mode == Environment_Patrol_Mode)	OLED_ShowString(0,50,"env");
			
	//显示当前小车是否允许控制
	if(Flag_Stop == 0 && EN==1 && Car_STOP == 0)   
		OLED_ShowString(45,50,"O N");  
	else                      	
		OLED_ShowString(45,50,"OFF"); 
			
	OLED_ShowNumber(75,50,Voltage_Show/100,2,12);
	OLED_ShowString(88,50,".");
	OLED_ShowNumber(98,50,Voltage_Show%100,2,12);
	OLED_ShowString(110,50,"V");
	if(Voltage_Show%100<10) 		
		OLED_ShowNumber(92,50,0,2,12);
		
    //OLED_Clear();		
	OLED_Refresh_Gram();
}
	
/**************************************************************************
函数功能：向APP发送数据
入口参数：无
返回  值：无
**************************************************************************/
void APP_Show(void)
{    
	static u8 flag_show;
	int Left_Figure,Right_Figure,Voltage_Show;
	
	//对电池电压处理成百分比形式
	Voltage_Show=(Voltage*1000-10000)/27;
	if(Voltage_Show>100)Voltage_Show=100; 
	
	//车轮速度单位转换为0.01m/s，方便在APP显示
	Left_Figure = MOTOR_A.Encoder*100;  
	if(Left_Figure<0)	Left_Figure=-Left_Figure;	
	Right_Figure = MOTOR_B.Encoder*100; 
	if(Right_Figure<0)	Right_Figure=-Right_Figure;
	
	//用于交替打印APP数据和显示波形
	flag_show = !flag_show;
	
	if(PID_Send == 1) 
	{	 
		if(Mode == CCD_Line_Patrol_Mode)		//CCD巡线调参
		{
			//发送参数到APP，APP在调试界面显示
			printf("{C%d:%d:%d}$",(int)RC_Velocity_CCD,(int)CCD_KP,(int)CCD_KI);
		}
		else if(Mode == OPENMV_Line_Patrol_Mode)	// OPENMV巡线模式调参
		{
			//发送参数到APP，APP在调试界面显示
			printf("{C%d:%d:%d:%d:%d}$",(int)RC_Velocity_OPENMV,(int)OPENMV_KP,(int)OPENMV_KI, (int)Car_path, (int)Car_LapNum_Tar);
		}
		else if(Mode == Lidar_Along_Mode)  //走直线模式下APP调整PID参数
		{
			if(Car_Mode == Akm_Car)
				printf("{C%d:%d:%d}$",(int)Akm_Along_Distance_KP,(int)Akm_Along_Distance_KD,(int)Akm_Along_Distance_KI);
			else if(Car_Mode == Diff_Car)
				printf("{C%d:%d:%d}$",(int)Diff_Along_Distance_KP,(int)Diff_Along_Distance_KD,(int)Diff_Along_Distance_KI);
			else if(Car_Mode == FourWheel_Car)
				printf("{C%d:%d:%d}$",(int)FourWheel_Along_Distance_KP,(int)FourWheel_Along_Distance_KD,(int)FourWheel_Along_Distance_KI);
			else
				printf("{C%d:%d:%d}$",(int)Along_Distance_KP,(int)Along_Distance_KD,(int)Along_Distance_KI);
		}
		else if(Mode == Lidar_Follow_Mode)   //跟随模式下APP调整PID距离参数
		{
			printf("{C%d:%d:%d:%d:%d:%d}$",(int)Distance_KP,(int)Distance_KD,(int)Distance_KI,(int)Follow_KP,(int)Follow_KD,(int)Follow_KI);
		}
		else	// APP 模式
		{
			//发送参数到APP，APP在调试界面显示
			//printf("{C%d:%d:%d}$",(int)RC_Velocity,(int)Velocity_KP,(int)Velocity_KI);
			printf("{C%d:%d:%d}$",(int)RC_Velocity,(int)Velocity_KP,(int)Velocity_KI);
			// printf("{B%d}$",(int)PointDataProcess[i].distance);
		}
		 
		PID_Send=0;	
		  
	}
	else if(flag_show==0)
	{
		//发送参数到APP，APP在首页显示
		printf("{A %d:%d:%d:%d:%d}\n$", (u8)Left_Figure, (u8)Right_Figure, Voltage_Show, (u8)Car_path, (u8)Car_LapNum);
	}
	else
	{
		//发送参数到APP，APP在波形界面显示，把需要显示的波形填进相应的位置即可，最多可以显示5个波形
		//printf("{B %d:%d:%d}$", (int)RC_Velocity, (u8)Left_Figure, (u8)Right_Figure);
	
	 }
 		 
}

//OLED显示CCD函数
void OLED_Show_CCD(void)
{ 
	u8 i,t;
	for(i = 0;i<128; i++)
	{
		if(ADV[i]<CCD_Threshold) t=1; else t=0;
		OLED_DrawPoint_Shu(i,0,t);
	}
}

//OLED显示CCD画点函数
void OLED_DrawPoint_Shu(u8 x,u8 y,u8 t)
{ 
	u8 i=0;
	OLED_DrawPoint(x,y,t);
	OLED_DrawPoint(x,y,t);
	for(i = 0;i<8; i++)
		OLED_DrawPoint(x,y+i,t);
	
}

//开机显示一次的内容
//void oled_show_once(void)
//{
//   OLED_ShowString(0,00,"Turn Right Wheel");  //转动右轮
//   OLED_ShowString(0,10,"TO Select Mode"); //选择模式
//	 OLED_ShowString(0,20,"Current Mode Is");//当前的模式是：
//	if(ELE_ON_Flag==1)				  OLED_ShowString(50,30,"ELE");//电磁巡线模式
//	if(CCD_ON_Flag==1)				  OLED_ShowString(50,30,"CCD");//CCD模式
//	if(APP_ON_Flag==1)				  OLED_ShowString(50,30,"APP");//其他模式，如APP、Remote
//	if(Along_wall==1)				    OLED_ShowString(50,30,"WAL");
//	if(Follow_ON_Flag==1)       OLED_ShowString(50,30,"FOL");
//	if(Avoid_ON_Flag==1)        OLED_ShowString(50,30,"Avo");
//	
//	OLED_ShowString(0,40,"Press User Key");// 按一下用户按键
//  OLED_ShowString(0,50,"TO End Selection");//结束选择
//	OLED_Refresh_Gram();	//OLED刷新
//}

