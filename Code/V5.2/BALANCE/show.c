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
extern int Car_path;		// С���ߵ�·�� ������usartx.c��
extern int Car_LapNum;		// С������ɵ�Ȧ��  ������usartx.c��
extern int Car_STOP;		// С���Ƿ񵽴�ֹͣλ ������usartx.c��
extern int Car_LapNum_Tar;	// С����Ŀ��Ȧ����������usartx.c��

/**************************************************************************
�������ܣ���ȡ��ص�ѹ�������������������Լ졢��APP�������ݡ�OLED��ʾ����ʾ����
��ڲ�������
����  ֵ����
**************************************************************************/
int Buzzer_count=25;
void show_task(void *pvParameters)
{
   u32 lastWakeTime = getSysTickCnt();

   while(1)
   {	
		int i=0;
		static int LowVoltage_1=0, LowVoltage_2=0;
		vTaskDelayUntil(&lastWakeTime, F2T(RATE_20_HZ)); 	//��������10Hz��Ƶ������
		
		//����ʱ���������ݷ�������������
		if(Time_count<50)Buzzer=1; 
		else if(Time_count>=51 && Time_count<100)Buzzer=0;
		 
		if(LowVoltage_1==1 || LowVoltage_2==1) Buzzer_count=0;
		if(Buzzer_count<5) Buzzer_count++;
		if(Buzzer_count<5) Buzzer=1; 		//����������
		else if(Buzzer_count==5) Buzzer=0;
		
		//��ȡ��ص�ѹ
		for(i=0;i<10;i++)
		{
			Voltage_All+=Get_battery_volt(); 
		}
		Voltage=Voltage_All/10;
		Voltage_All=0;
		
		if(LowVoltage_1==1)LowVoltage_1++;  //ȷ��������ֻ��0.5��
		if(LowVoltage_2==1)LowVoltage_2++; 	//ȷ��������ֻ��0.5��
		if(Voltage>=12.6f)Voltage=12.6f;
		else if(10<=Voltage && Voltage<10.5f && LowVoltage_1<2)LowVoltage_1++; 	//10.5V���͵���ʱ��������һ�α���
		else if(Voltage<10 && LowVoltage_1<2)LowVoltage_2++; 	//10V��С����ֹ����ʱ�������ڶ��α���
					
		APP_Show();		//��APP��������
		oled_show(); 	//��ʾ����ʾ����
   }
}  

/**************************************************************************
�������ܣ�OLED��ʾ����ʾ����
��ڲ�������
����  ֵ����
**************************************************************************/
void oled_show(void)
{  
    static int count=0;	 
	int Car_Mode_Show = 3;	// �ֶ�ָ�����ͱ��Ϊ3-������
	
	Voltage_Show=Voltage*100; 
	count++;
	
	//CCDѲ��ģʽ��ʾ��������ʾ CCD��+���/���״̬
	if(Mode == CCD_Line_Patrol_Mode)											
	{
		//OLED_Clear();
		//��ʾ����1����ʾ����//
		OLED_Show_CCD(); 	//��̬��ʾCCDͼ��
				

		//��ʾ����2����ʾ����//
		OLED_ShowString(00,10,"Median :");
		OLED_ShowNumber(40,10, CCD_Median ,5,12); 	//CCD���ߵ���ֵ��Ѳ��������λ��Ϊ64
			
		switch(Car_Mode_Show)	//���ݳ��ͱ����ʾ����			
		{
			case Mec_Car:       OLED_ShowString(86,10,"Mec "); break; 
			case Akm_Car:       OLED_ShowString(86,10,"Akm "); break; 
			case Diff_Car:      OLED_ShowString(86,10,"Diff"); break; 
			case FourWheel_Car: OLED_ShowString(86,10,"4WD "); break; 
		} 
			
		//��ʾ����3����ʾ����//
		OLED_ShowString(00,20,"Threshold :");						
		OLED_ShowNumber(90,20, CCD_Threshold,5,12);     //��ʾCCD��ֵ
			
		//��ʾ����4����ʾ����//
		//��ʾ���г��͵��A��B��Ŀ���ٶȺ͵�ǰʵ���ٶ�//
		OLED_ShowString(00,30,"A:");
		
		if(MOTOR_A.Target<0) 
		{
			OLED_ShowString(15,30,"-");
			OLED_ShowNumber(25,30,-MOTOR_A.Target*1000,5,12);   	//����ת����ʾ
		}
		else 
		{
			OLED_ShowString(15,30,"+");
			OLED_ShowNumber(25,30, MOTOR_A.Target*1000,5,12);    	//���Ŀ��ת��
		}
				
		OLED_ShowString(60,30,"B:");
		
		if(MOTOR_B.Target<0)
		{
			OLED_ShowString(75,30,"-");
			OLED_ShowNumber(85,30,-MOTOR_B.Target*1000,5,12);   	//����ת����ʾ
		}
		else          
		{
			OLED_ShowString(75,30,"+");
			OLED_ShowNumber(85,30, MOTOR_B.Target*1000,5,12);    	//���Ŀ��ת��
		}
				
		//��ʾ����5����ʾ����
		if(Car_Mode==Mec_Car||Car_Mode==FourWheel_Car)			//���ֳ�����������ʾ���C��D��Ŀ���ٶȺ͵�ǰʵ���ٶ�
		{
			
			OLED_ShowString(00,40,"C:");
			if(MOTOR_C.Target<0)
			{
				OLED_ShowString(15,40,"-");
				OLED_ShowNumber(25,40,-MOTOR_C.Target*1000,5,12);    		//����ת����ʾ
			}
			else
			{
				OLED_ShowString(15,40,"+");
				OLED_ShowNumber(25,40, MOTOR_C.Target*1000,5,12);    		//���Ŀ��ת��
			}
						
			OLED_ShowString(60,40,"D:");
			
			if(MOTOR_D.Target<0)
			{				
				OLED_ShowString(75,40,"-");
				OLED_ShowNumber(85,40,-MOTOR_D.Target*1000,5,12);    		//����ת����ʾ
			}
			else
			{
				OLED_ShowString(75,40,"+");
				OLED_ShowNumber(85,40, MOTOR_D.Target*1000,5,12);    		//���Ŀ��ת��
			}
		}
		else if(Car_Mode==Akm_Car)		//������С����ʾ�����PWM����ֵ
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
		else if(Car_Mode==Diff_Car)		//����С����ʾ���ҵ����PWM����ֵ
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
	
	//OPENMVѲ��ģʽ��ʾ��������ʾ OPENMV��+���/���״̬
	else if(Mode == OPENMV_Line_Patrol_Mode)											
	{
		extern int biass;
		
		

   
		//OLED_Clear();
		
		//��ʾ����1����ʾ����
		
		OLED_ShowString(00,00,"biass:");
		if(biass < 0) 
		{
			OLED_ShowString(50,0,"-");
		}
		else if(biass > 0)		// ���ܳ����� ��Ҫдelse!!(���µ��C��ת)
		{	
			OLED_ShowString(50,0," ");
			// OLED_ShowString(50,0,"+");	// ���ܳ����� (���µ��C��ת)
		}
		
		OLED_ShowNumber(55,00, myabs(biass) ,3,12); 	//��ʾƫ��

		//��ʾ����2����ʾ����
		if(biass < 0)	
			OLED_ShowString(00,10, "LEFT"); 	//��ת
		else if(biass == 0)
			OLED_ShowString(00,10, "FORW"); 		//ֱ��
		else 
			OLED_ShowString(00,10, "RIGH"); 	//��ת
			
		switch(Car_Mode_Show)	//���ݳ��ͱ����ʾ����			
		{
			case Mec_Car:       OLED_ShowString(86,10,"Mec "); break; 
			case Akm_Car:       OLED_ShowString(86,10,"Akm "); break; 
			case Diff_Car:      OLED_ShowString(86,10,"Diff"); break; 
			case FourWheel_Car: OLED_ShowString(86,10,"4WD "); break; 
		} 
			
		//��ʾ����3����ʾ����//
		//OLED_ShowString(00,20,"Threshold :");						
		//OLED_ShowNumber(90,20, CCD_Threshold,5,12);     //��ʾCCD��ֵ
			
		//��ʾ����4����ʾ����//
		//��ʾ���г��͵��A��B��Ŀ���ٶ�
		OLED_ShowString(00,30,"A:");
		
		if(MOTOR_A.Target<0) 
		{
			OLED_ShowString(15,30,"-");
			OLED_ShowNumber(25,30,-MOTOR_A.Target*1000,5,12);   	//����ת����ʾ
		}
		else 
		{
			OLED_ShowString(15,30,"+");
			OLED_ShowNumber(25,30, MOTOR_A.Target*1000,5,12);    	//���Ŀ��ת��
		}
				
		OLED_ShowString(60,30,"B:");
		
		if(MOTOR_B.Target<0)
		{
			OLED_ShowString(75,30,"-");
			OLED_ShowNumber(85,30,-MOTOR_B.Target*1000,5,12);   	//����ת����ʾ
		}
		else          
		{
			OLED_ShowString(75,30,"+");
			OLED_ShowNumber(85,30, MOTOR_B.Target*1000,5,12);    	//���Ŀ��ת��
		}
				
		//��ʾ����5����ʾ����
		if(Car_Mode==Mec_Car||Car_Mode==FourWheel_Car)			//���ֳ�����������ʾ���C��D��Ŀ���ٶ�
		{
			
			OLED_ShowString(00,40,"C:");
			if(MOTOR_C.Target<0)
			{
				OLED_ShowString(15,40,"-");
				OLED_ShowNumber(25,40,-MOTOR_C.Target*1000,5,12);    		//����ת����ʾ
			}
			else
			{
				OLED_ShowString(15,40,"+");
				OLED_ShowNumber(25,40, MOTOR_C.Target*1000,5,12);    		//���Ŀ��ת��
			}
						
			OLED_ShowString(60,40,"D:");
			
			if(MOTOR_D.Target<0)
			{				
				OLED_ShowString(75,40,"-");
				OLED_ShowNumber(85,40,-MOTOR_D.Target*1000,5,12);    		//����ת����ʾ
			}
			else
			{
				OLED_ShowString(75,40,"+");
				OLED_ShowNumber(85,40, MOTOR_D.Target*1000,5,12);    		//���Ŀ��ת��
			}
		}
		else if(Car_Mode==Akm_Car)		//������С����ʾ�����PWM����ֵ
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
		else if(Car_Mode==Diff_Car)		//����С����ʾ���ҵ����PWM����ֵ
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
	
	
		//�������
	else if (Mode == Environment_Patrol_Mode) {	
    EnvironmentData envData = Get_environment();

    // �� OLED ����ʾ����
	
	OLED_ShowString(0, 00, "tem");
    OLED_ShowFloat_1(50, 00,envData.temperature, 12);
	OLED_ShowString(75, 00, "C");
    // ��ʾʪ��
    OLED_ShowString(0, 10, "hum");
    OLED_ShowFloat_1(50, 10, envData.humidity, 12);
    OLED_ShowString(75, 10, "%RH");
    // ��ʾ���� PPM ֵ
    OLED_ShowString(0, 20, "PPM:"); 
    OLED_ShowNumber(50, 20, envData.gas_ppm, 5, 12);
	OLED_ShowString(80, 20, "ppm");
    // ��ʾ BH1750 ��ǿֵ
    OLED_ShowString(0, 30, "Light:"); 
    OLED_ShowNumber(50, 30, envData.light_intensity, 5, 12);


}
	
	
	
	
	
	else	//��ccd,openmv������ģʽ����ʾ�ĵ��״̬��Ϣ									
	{
			//OLED_Clear();

		 //��ʾ����1����ʾ����
		 switch(Car_Mode_Show)
		 {
			case Mec_Car:       OLED_ShowString(0,0,"Mec "); break; 
			case Akm_Car:       OLED_ShowString(0,0,"Akm "); break; 
			case Diff_Car:      OLED_ShowString(0,0,"Diff"); break; 
			case FourWheel_Car: OLED_ShowString(0,0,"4WD "); break; 
		 }
		 
//		OLED_ShowString(40,0, "Mode:");	//��ʾģʽ
//		if(Mode==Normal_Mode)					OLED_ShowString(80,0,"Normal ");
//		else if(Mode==Lidar_Avoid_Mode)			OLED_ShowString(80,0,"Avoid  ");
//		else if(Mode==Lidar_Follow_Mode)		OLED_ShowString(80,0,"Follow ");
		//else if(Mode==ELE_Line_Patrol_Mode)		OLED_ShowString(50,0,"ELE    ");
		 
		 
		//��ʾ����2����ʾ����
		if(Car_Mode==Mec_Car || Car_Mode==FourWheel_Car)
		{
			//���֡���������ʾ���A��Ŀ���ٶȺ͵�ǰʵ���ٶ�
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
			//�����������ٳ���ʾ���A��Ŀ���ٶȺ͵�ǰʵ���ٶ�
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
		 
		//��ʾ����3����ʾ����
		//��������������ʾ���B��Ŀ���ٶȺ͵�ǰʵ���ٶ�
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
		 

		if(Car_Mode==Mec_Car || Car_Mode==FourWheel_Car) //���֡���������ʾ���B��Ŀ���ٶȺ͵�ǰʵ���ٶ�
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
		 
			
		//��ʾ����4����ʾ����
			
		if(Car_Mode==Mec_Car || Car_Mode==FourWheel_Car)	//���֡���������ȫ������ʾ���C��Ŀ���ٶȺ͵�ǰʵ���ٶ�
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
		if(Car_Mode==Akm_Car)	//������С����ʾ�����PWM����ֵ
		{
			OLED_ShowString(00,30,"SERVO:");
			if( Servo<0)		     
				OLED_ShowString(60,30,"-"),
				OLED_ShowNumber(80,30,-Servo,4,12);
			else                 	
				OLED_ShowString(60,30,"+"),
				OLED_ShowNumber(80,30, Servo,4,12); 
		}
		else if(Car_Mode==Diff_Car) //����С����ʾ������PWM����ֵ
		{
			
			OLED_ShowString(00,30,"MA");
			 
			if( MOTOR_A.Motor_Pwm<0)
				OLED_ShowString(40,30,"-"),
				OLED_ShowNumber(50,30,-MOTOR_A.Motor_Pwm,4,12);
			else                 	 
				OLED_ShowString(40,30,"+"),
				OLED_ShowNumber(50,30, MOTOR_A.Motor_Pwm,4,12); 
		}	
		 

		//��ʾ����5����ʾ����
		if(Car_Mode==Mec_Car||Car_Mode==FourWheel_Car)  //����С����ʾ���D��Ŀ���ٶȺ͵�ǰʵ���ٶ�
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
		else if(Car_Mode==Diff_Car)	//����С����ʾ�ҵ����PWM����ֵ
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

	

	//��ʾ����6����ʾ����
	
	//��ʾ��ǰ����ģʽ
	if(Mode == CCD_Line_Patrol_Mode)		OLED_ShowString(0,50,"CCD  ");
	else if(Mode == Lidar_Avoid_Mode) 		OLED_ShowString(0,50,"AVO");
	else if(Mode == Lidar_Follow_Mode) 		OLED_ShowString(0,50,"Fol");
	else if(Mode == Lidar_Along_Mode) 		OLED_ShowString(0,50,"Alo");
	else if(Mode == APP_Control_Mode)		OLED_ShowString(0,50,"APP");
	else if(Mode == OPENMV_Line_Patrol_Mode)	OLED_ShowString(0,50,"Open");	// ���ܳ�����
	else if(Mode == Environment_Patrol_Mode)	OLED_ShowString(0,50,"env");
			
	//��ʾ��ǰС���Ƿ��������
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
�������ܣ���APP��������
��ڲ�������
����  ֵ����
**************************************************************************/
void APP_Show(void)
{    
	static u8 flag_show;
	int Left_Figure,Right_Figure,Voltage_Show;
	
	//�Ե�ص�ѹ����ɰٷֱ���ʽ
	Voltage_Show=(Voltage*1000-10000)/27;
	if(Voltage_Show>100)Voltage_Show=100; 
	
	//�����ٶȵ�λת��Ϊ0.01m/s��������APP��ʾ
	Left_Figure = MOTOR_A.Encoder*100;  
	if(Left_Figure<0)	Left_Figure=-Left_Figure;	
	Right_Figure = MOTOR_B.Encoder*100; 
	if(Right_Figure<0)	Right_Figure=-Right_Figure;
	
	//���ڽ����ӡAPP���ݺ���ʾ����
	flag_show = !flag_show;
	
	if(PID_Send == 1) 
	{	 
		if(Mode == CCD_Line_Patrol_Mode)		//CCDѲ�ߵ���
		{
			//���Ͳ�����APP��APP�ڵ��Խ�����ʾ
			printf("{C%d:%d:%d}$",(int)RC_Velocity_CCD,(int)CCD_KP,(int)CCD_KI);
		}
		else if(Mode == OPENMV_Line_Patrol_Mode)	// OPENMVѲ��ģʽ����
		{
			//���Ͳ�����APP��APP�ڵ��Խ�����ʾ
			printf("{C%d:%d:%d:%d:%d}$",(int)RC_Velocity_OPENMV,(int)OPENMV_KP,(int)OPENMV_KI, (int)Car_path, (int)Car_LapNum_Tar);
		}
		else if(Mode == Lidar_Along_Mode)  //��ֱ��ģʽ��APP����PID����
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
		else if(Mode == Lidar_Follow_Mode)   //����ģʽ��APP����PID�������
		{
			printf("{C%d:%d:%d:%d:%d:%d}$",(int)Distance_KP,(int)Distance_KD,(int)Distance_KI,(int)Follow_KP,(int)Follow_KD,(int)Follow_KI);
		}
		else	// APP ģʽ
		{
			//���Ͳ�����APP��APP�ڵ��Խ�����ʾ
			//printf("{C%d:%d:%d}$",(int)RC_Velocity,(int)Velocity_KP,(int)Velocity_KI);
			printf("{C%d:%d:%d}$",(int)RC_Velocity,(int)Velocity_KP,(int)Velocity_KI);
			// printf("{B%d}$",(int)PointDataProcess[i].distance);
		}
		 
		PID_Send=0;	
		  
	}
	else if(flag_show==0)
	{
		//���Ͳ�����APP��APP����ҳ��ʾ
		printf("{A %d:%d:%d:%d:%d}\n$", (u8)Left_Figure, (u8)Right_Figure, Voltage_Show, (u8)Car_path, (u8)Car_LapNum);
	}
	else
	{
		//���Ͳ�����APP��APP�ڲ��ν�����ʾ������Ҫ��ʾ�Ĳ��������Ӧ��λ�ü��ɣ���������ʾ5������
		//printf("{B %d:%d:%d}$", (int)RC_Velocity, (u8)Left_Figure, (u8)Right_Figure);
	
	 }
 		 
}

//OLED��ʾCCD����
void OLED_Show_CCD(void)
{ 
	u8 i,t;
	for(i = 0;i<128; i++)
	{
		if(ADV[i]<CCD_Threshold) t=1; else t=0;
		OLED_DrawPoint_Shu(i,0,t);
	}
}

//OLED��ʾCCD���㺯��
void OLED_DrawPoint_Shu(u8 x,u8 y,u8 t)
{ 
	u8 i=0;
	OLED_DrawPoint(x,y,t);
	OLED_DrawPoint(x,y,t);
	for(i = 0;i<8; i++)
		OLED_DrawPoint(x,y+i,t);
	
}

//������ʾһ�ε�����
//void oled_show_once(void)
//{
//   OLED_ShowString(0,00,"Turn Right Wheel");  //ת������
//   OLED_ShowString(0,10,"TO Select Mode"); //ѡ��ģʽ
//	 OLED_ShowString(0,20,"Current Mode Is");//��ǰ��ģʽ�ǣ�
//	if(ELE_ON_Flag==1)				  OLED_ShowString(50,30,"ELE");//���Ѳ��ģʽ
//	if(CCD_ON_Flag==1)				  OLED_ShowString(50,30,"CCD");//CCDģʽ
//	if(APP_ON_Flag==1)				  OLED_ShowString(50,30,"APP");//����ģʽ����APP��Remote
//	if(Along_wall==1)				    OLED_ShowString(50,30,"WAL");
//	if(Follow_ON_Flag==1)       OLED_ShowString(50,30,"FOL");
//	if(Avoid_ON_Flag==1)        OLED_ShowString(50,30,"Avo");
//	
//	OLED_ShowString(0,40,"Press User Key");// ��һ���û�����
//  OLED_ShowString(0,50,"TO End Selection");//����ѡ��
//	OLED_Refresh_Gram();	//OLEDˢ��
//}

