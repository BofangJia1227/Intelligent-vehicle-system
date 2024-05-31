#include "balance.h"
#include "DHT11.h"
#include "MQ2.h"
#include "BH1750.h"
#include "I2C.h"
int Time_count=0;					//��ʱ���� 
u8 Lidar_Detect = Lidar_Detect_ON;	//���Ѳ��ģʽ�״����ϰ��Ĭ�Ͽ���
u8 Mode = 0;
float RC_Velocity_CCD=350; 
float RC_Velocity_OPENMV=350;
Encoder OriginalEncoder; 		//������ԭʼ����  
extern int Car_STOP;	// �Ƿ񵽴�ֹͣλ��������usartx.c
extern int biass;		// openmv������ƫ��ֵ��������usartx.c
float light ;
/**************************************************************************
�������ܣ��˶�ѧ��⣬��������Ŀ���ٶȼ��������Ŀ��ת��
��ڲ�����X��Y��Z�᷽���Ŀ���˶��ٶ�
����  ֵ����
**************************************************************************/
void Drive_Motor(float Vx,float Vy,float Vz)
{
	float amplitude=3.5;  //����Ŀ���ٶ��޷�
	
	 
	//���ֿ����ٶ�ƽ������
	if(Car_Mode == Mec_Car)
	{
		Smooth_control(Vx,Vy,Vz); //Smoothing the input speed //�������ٶȽ���ƽ������
  
		//��ȡƽ������������			
		Vx = smooth_control.VX;     
		Vy = smooth_control.VY;
		Vz = smooth_control.VZ;
	}
		
	//�����ķ��С��
	if (Car_Mode==Mec_Car) 
    {
		//�˶�ѧ���
		MOTOR_A.Target = +Vy+Vx-Vz*(Axle_spacing+Wheel_spacing);
		MOTOR_B.Target = -Vy+Vx-Vz*(Axle_spacing+Wheel_spacing);
		MOTOR_C.Target = +Vy+Vx+Vz*(Axle_spacing+Wheel_spacing);
		MOTOR_D.Target = -Vy+Vx+Vz*(Axle_spacing+Wheel_spacing);
		
			
		//����(���)Ŀ���ٶ��޷�
		MOTOR_A.Target = target_limit_float(MOTOR_A.Target,-amplitude,amplitude); 
		MOTOR_B.Target = target_limit_float(MOTOR_B.Target,-amplitude,amplitude); 
		MOTOR_C.Target = target_limit_float(MOTOR_C.Target,-amplitude,amplitude); 
		MOTOR_D.Target = target_limit_float(MOTOR_D.Target,-amplitude,amplitude); 
	} 
	//������С��
	else if (Car_Mode==Akm_Car) 
	{
		//������С��ר����ر���
		float R, Ratio=636.56, AngleR, Angle_Servo;
			
		//���ڰ�����С��Vz������ǰ��ת��Ƕ�
		AngleR=Vz;
		R=Axle_spacing/tan(AngleR)-0.5f*Wheel_spacing;
			
		//ǰ��ת��Ƕ��޷�(�������ǰ��ת��Ƕ�)����λ��rad
		AngleR=target_limit_float(AngleR,-0.49f,0.32f);
			
		//�˶�ѧ���
		if(AngleR!=0)
		{
			MOTOR_A.Target = Vx*(R-0.5f*Wheel_spacing)/R;
			MOTOR_B.Target = Vx*(R+0.5f*Wheel_spacing)/R;			
		}
		else 
		{
			MOTOR_A.Target = Vx;
			MOTOR_B.Target = Vx;
		}
		//���PWMֵ���������ǰ��ת��Ƕ�
		//Angle_Servo = -0.628f*pow(AngleR, 3) + 1.269f*pow(AngleR, 2) - 1.772f*AngleR + 1.573f;
		Angle_Servo = -0.628f*pow(AngleR, 3) + 1.269f*pow(AngleR, 2) - 1.772f*AngleR + 1.755f;
		Servo=SERVO_INIT + (Angle_Servo - 1.755f)*Ratio;

			
		//����(���)Ŀ���ٶ��޷�
		MOTOR_A.Target=target_limit_float(MOTOR_A.Target,-amplitude,amplitude); 
		MOTOR_B.Target=target_limit_float(MOTOR_B.Target,-amplitude,amplitude); 
		MOTOR_C.Target=0;  		//û��ʹ�õ�
		MOTOR_D.Target=0;  		//û��ʹ�õ�
		Servo=target_limit_int(Servo,800,2200);		//���PWMֵ�޷�
	}
	//����С��
	else if (Car_Mode==Diff_Car) 
	{
		//�˶�ѧ���
		MOTOR_A.Target  = Vx - Vz * Wheel_spacing / 2.0f; //��������ֵ�Ŀ���ٶ�
		MOTOR_B.Target =  Vx + Vz * Wheel_spacing / 2.0f; //��������ֵ�Ŀ���ٶ�
		
		//����(���)Ŀ���ٶ��޷�
		MOTOR_A.Target=target_limit_float( MOTOR_A.Target,-amplitude,amplitude); 
	    MOTOR_B.Target=target_limit_float( MOTOR_B.Target,-amplitude,amplitude); 
		MOTOR_C.Target=0; 		//û��ʹ�õ�
		MOTOR_D.Target=0; 		//û��ʹ�õ�
			
	}
	//������
	else if(Car_Mode==FourWheel_Car) 
	{	
		//�˶�ѧ���
		MOTOR_A.Target  = Vx - Vz * (Wheel_spacing +  Axle_spacing) / 2.0f; //��������ֵ�Ŀ���ٶ�
		MOTOR_B.Target  = Vx - Vz * (Wheel_spacing +  Axle_spacing) / 2.0f; //��������ֵ�Ŀ���ٶ�
		MOTOR_C.Target  = Vx + Vz * (Wheel_spacing +  Axle_spacing) / 2.0f; //��������ֵ�Ŀ���ٶ�
		MOTOR_D.Target  = Vx + Vz * (Wheel_spacing +  Axle_spacing) / 2.0f; //��������ֵ�Ŀ���ٶ�
		
		//����(���)Ŀ���ٶ��޷�
		MOTOR_A.Target=target_limit_float( MOTOR_A.Target,-amplitude,amplitude); 
		MOTOR_B.Target=target_limit_float( MOTOR_B.Target,-amplitude,amplitude); 
		MOTOR_C.Target=target_limit_float( MOTOR_C.Target,-amplitude,amplitude); 
		MOTOR_D.Target=target_limit_float( MOTOR_D.Target,-amplitude,amplitude); 		
	}
}

/**************************************************************************
�������ܣ�FreeRTOS���񣬺����˶���������
��ڲ�������
����  ֵ����
**************************************************************************/
void Balance_task(void *pvParameters)
{ 
	static u8 Count_ctrl = 0;								//����CCD����Ƶ��
	static u8 last_mode = 0;
	u32 lastWakeTime = getSysTickCnt();
	
	
    while(1)
    {	

		//��������100Hz��Ƶ�����У�10ms����һ�Σ�
		vTaskDelayUntil(&lastWakeTime, F2T(RATE_100_HZ)); 

		//ʱ�������30�������Ҫ
		if(Time_count<3000)Time_count++;

		//��ȡ���������ݣ�������ʵʱ�ٶȣ���ת��λ���ʵ�λ
		Get_Velocity_Form_Encoder();   

        switch(click_N_Double(50)) // click_N_Double����ֵΪ0-�޲�����1-������2-˫��
		{
			case 1:    //���������л�ģʽ
				Mode += 1;
				if(Mode == CCD_Line_Patrol_Mode)	//ѡ��ccdѲ�߿���ģʽ
					ccd_Init();							//��ʼ��CCD
				else if(Mode == OPENMV_Line_Patrol_Mode)	//ѡ��OPENMVѲ�߿���ģʽ
					OPENMV_Init();							//��ʼ��OPENMV
				else if(Mode == Environment_Patrol_Mode)	//ѡ��OPENMVѲ�߿���ģʽ
					OPENMV_Init();							//��ʼ��OPENMV
				else if(Mode > 6)						//ģʽ���ã�һ��6��ģʽ
					Mode = 0;
				break;
			case 2:    // (���Ѳ��״̬ʱ) ˫�����Դ�/�ر��״����ϰ��Ĭ�ϴ�
				Lidar_Detect = !Lidar_Detect;
				if(Lidar_Detect == Lidar_Detect_OFF) memset(Dataprocess,0, sizeof(PointDataProcessDef)*225);		//�����״����ϰ������������
				break;				 
		}
				
		if(last_mode != Mode)  // �ı�ģʽʱ����oled��ʾ
		{
			last_mode++;
			OLED_Clear();
			if(last_mode > 6)	// һ��6��ģʽ
			{
				//OLED_Clear();
				last_mode = 0;
			}
		} 

		// ����ѡ��ģʽִ�й���
		if(Mode == APP_Control_Mode)          Get_RC();             //����APPң������
		else if(Mode == Lidar_Avoid_Mode)     Lidar_Avoid();        //����ģʽ
		//else if(Mode == Lidar_Follow_Mode)    Lidar_Follow();       //����ģʽ
		else if(Mode == Lidar_Along_Mode)     Lidar_along_wall();   //��ֱ��ģʽ
		else if(Mode == CCD_Line_Patrol_Mode) 		//CCDģʽ
		{
			if(++Count_ctrl== 4)									//���ڿ���Ƶ�ʣ�4*5 = 20ms����һ��
			{
				Count_ctrl = 0;
				Get_RC_CCD();											
			}
			else if(Count_ctrl>4)
				Count_ctrl = 0;
		}					
		else if(Mode == OPENMV_Line_Patrol_Mode) 		//openmvģʽ
		{
			if(++Count_ctrl == 4)									//���ڿ���Ƶ�ʣ�4*5 = 20ms����һ��
			{
				Count_ctrl = 0;
				Get_RC_OPENMV();											
			}
			else if(Count_ctrl>4)
				Count_ctrl = 0;
		}		
		else if(Mode == Environment_Patrol_Mode) 		//�������ģʽ
		{
			
			
			MQ2_Init();   // ��ʼ�� MQ2
			MeasureAirQuality();
			DHT11_REC_Data(); // ��ȡ��ʪ��ֵ   
			//read_BH1750();  //��ȡBH1750�Ĺ�ǿ����
			//read_BH1750();  //��ȡBH1750�Ĺ�ǿ����
			Convert_BH1750();
		}
		//�����ص�ѹ�������쳣������ʹ�ܿ�����ON��λ���������ʧ�ܱ�־λΪ0
		if(Turn_Off(Voltage)==0 && Car_STOP == 0) 	// ��ص�ѹ���� �� û����ֹͣλ
		{ 			
				 
			//�ٶȱջ����Ƽ�������PWMֵ��PWM������ʵ��ת��
			MOTOR_A.Motor_Pwm=Incremental_PI_A(MOTOR_A.Encoder, MOTOR_A.Target);
			MOTOR_B.Motor_Pwm=Incremental_PI_B(MOTOR_B.Encoder, MOTOR_B.Target);
			MOTOR_C.Motor_Pwm=Incremental_PI_C(MOTOR_C.Encoder, MOTOR_C.Target);
			MOTOR_D.Motor_Pwm=Incremental_PI_D(MOTOR_D.Encoder, MOTOR_D.Target);
			Limit_Pwm(16500) ;
					 
			//���ݲ�ͬС���ͺ����ò�ͬ��PWM���Ƽ���
			switch(Car_Mode)
			{
				case Mec_Car:       Set_Pwm(-MOTOR_A.Motor_Pwm, -MOTOR_B.Motor_Pwm,  MOTOR_C.Motor_Pwm, MOTOR_D.Motor_Pwm, 0    ); break;	//�����ķ��С��
				case Akm_Car:       Set_Pwm(-MOTOR_A.Motor_Pwm,  MOTOR_B.Motor_Pwm,  MOTOR_C.Motor_Pwm, MOTOR_D.Motor_Pwm, Servo); break;	//������С��
				case Diff_Car:      Set_Pwm(-MOTOR_A.Motor_Pwm,  MOTOR_B.Motor_Pwm,  MOTOR_C.Motor_Pwm, MOTOR_D.Motor_Pwm, 0    ); break;	//���ֲ���С��
				case FourWheel_Car: Set_Pwm(-MOTOR_A.Motor_Pwm, -MOTOR_B.Motor_Pwm,  MOTOR_C.Motor_Pwm, MOTOR_D.Motor_Pwm, 0    ); break;	//������ 
			}
		}

		//���Turn_Off(Voltage)����ֵΪ1�����������С�������˶���PWMֵ����Ϊ0
		else Set_Pwm(0,0,0,0,0); 
			 	
	}  
}
/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ��������Ƴ���ת���뷽��
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b,int motor_c,int motor_d,int servo)
{
	if(motor_a<0)			PWMA2=16800,PWMA1=16800+motor_a;
	else 	            PWMA1=16800,PWMA2=16800-motor_a;
			
	if(motor_b<0)			PWMB1=16800,PWMB2=16800+motor_b;
	else 	            PWMB2=16800,PWMB1=16800-motor_b;
			
	if(motor_c<0)			PWMC1=16800,PWMC2=16800+motor_c;
	else 	            PWMC2=16800,PWMC1=16800-motor_c;
	
	if(motor_d<0)			PWMD2=16800,PWMD1=16800+motor_d;
	else 	            PWMD1=16800,PWMD2=16800-motor_d;
	
	//Servo control
	//�������
	Servo_PWM =servo;
}

/**************************************************************************
�������ܣ�����PWMֵ 
��ڲ�������ֵ
����  ֵ����
**************************************************************************/
void Limit_Pwm(int amplitude)
{	
	    MOTOR_A.Motor_Pwm=target_limit_float(MOTOR_A.Motor_Pwm,-amplitude,amplitude);
	    MOTOR_B.Motor_Pwm=target_limit_float(MOTOR_B.Motor_Pwm,-amplitude,amplitude);
		MOTOR_C.Motor_Pwm=target_limit_float(MOTOR_C.Motor_Pwm,-amplitude,amplitude);
	    MOTOR_D.Motor_Pwm=target_limit_float(MOTOR_D.Motor_Pwm,-amplitude,amplitude);
}	    
/**************************************************************************
�������ܣ��޷�����
��ڲ�������ֵ
����  ֵ����
**************************************************************************///float target_limit_float(float insert,float low,float high)
float target_limit_float(float insert,float low,float high)
{
    if (insert < low)
        return low;
    else if (insert > high)
        return high;
    else
        return insert;	
}
int target_limit_int(int insert,int low,int high)
{
    if (insert < low)
        return low;
    else if (insert > high)
        return high;
    else
        return insert;	
}
/**************************************************************************
�������ܣ�����ص�ѹ��ʹ�ܿ���״̬�����ʧ�ܱ�־λ״̬
��ڲ�������ѹ
����  ֵ���Ƿ�������ƣ�1��������0����
**************************************************************************/
u8 Turn_Off( int voltage)
{
	    u8 temp;
			if(voltage<10||EN==0||Flag_Stop==1)
			{	                                                
				temp=1; 
					
      }
			else
			temp=0;
			return temp;			
}
/**************************************************************************
�������ܣ������ֵ
��ڲ�����long int
����  ֵ��unsigned int
**************************************************************************/
u32 myabs(long int a)
{ 		   
	u32 temp;
	
	if(a<0) temp=-a;  
	else temp=a;
	
	return temp;
}
/**************************************************************************
�������ܣ�����ʽPI������
��ڲ���������������ֵ(ʵ���ٶ�)��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_A (float Encoder,float Target)
{ 	
	 static float Bias,Pwm,Last_bias;
	 Bias=Target-Encoder; 	//����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias; 
	 if(Pwm>16800)Pwm=16800;
	 if(Pwm<-16800)Pwm=-16800;
	 Last_bias=Bias; 		//������һ��ƫ�� 
	 return Pwm;    
}
int Incremental_PI_B (float Encoder,float Target)
{  
	 static float Bias,Pwm,Last_bias;
	 Bias=Target-Encoder; 	//����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;  
	 if(Pwm>16800)Pwm=16800;
	 if(Pwm<-16800)Pwm=-16800;
	 Last_bias=Bias; 		//������һ��ƫ�� 
	 return Pwm;
}
int Incremental_PI_C (float Encoder,float Target)
{  
	 static float Bias,Pwm,Last_bias;
	 Bias=Target-Encoder; 	//����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias; 
	 if(Pwm>16800)Pwm=16800;
	 if(Pwm<-16800)Pwm=-16800;
	 Last_bias=Bias; 		//������һ��ƫ�� 
	 return Pwm; 
}
int Incremental_PI_D (float Encoder,float Target)
{  
	 static float Bias,Pwm,Last_bias;
	 Bias=Target-Encoder;	//����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;  
	 if(Pwm>16800)Pwm=16800;
	 if(Pwm<-16800)Pwm=-16800;
	 Last_bias=Bias; 		//������һ��ƫ�� 
	 return Pwm; 
}
/**************************************************************************
�������ܣ���APPͨ������2���͹�����������д���
��ڲ�������
����  ֵ����
**************************************************************************/
void Get_RC(void)
{
	u8 Flag_Move=1;
	if(Car_Mode==Mec_Car)  //ȫ���˶�С�����Խ��к����ƶ�
	{
		switch(Flag_Direction)  //Handle direction control commands //�������������
		{ 
			case 1:      Move_X=RC_Velocity;  	 Move_Y=0;             Flag_Move=1;    	break;
			case 2:      Move_X=RC_Velocity;  	 Move_Y=-RC_Velocity;  Flag_Move=1; 	break;
			case 3:      Move_X=0;      		 Move_Y=-RC_Velocity;  Flag_Move=1; 	break;
			case 4:      Move_X=-RC_Velocity;  	 Move_Y=-RC_Velocity;  Flag_Move=1;    	break;
			case 5:      Move_X=-RC_Velocity;  	 Move_Y=0;             Flag_Move=1;    	break;
			case 6:      Move_X=-RC_Velocity;  	 Move_Y=RC_Velocity;   Flag_Move=1;    	break;
			case 7:      Move_X=0;     	 		 Move_Y=RC_Velocity;   Flag_Move=1;    	break;
			case 8:      Move_X=RC_Velocity; 	 Move_Y=RC_Velocity;   Flag_Move=1;    	break; 
			default:     Move_X=0;               Move_Y=0;             Flag_Move=0;    	break;
		}
	 
		if(Flag_Move==0)		
		{	
			//����޷������ָ����ת�����״̬
			if     (Flag_Left ==1)  Move_Z= PI/2*(RC_Velocity/500); 	//����ת  
			else if(Flag_Right==1)  Move_Z=-PI/2*(RC_Velocity/500); 	//����ת
			else 		             Move_Z=0;                       	//ֹͣ
		}
	}	
	else   //��ȫ���ƶ�С��
	{
		switch(Flag_Direction) //Handle direction control commands //�������������
		{ 
			case 1:		Move_X=+RC_Velocity;  	Move_Z=0;       break;
			case 2:     Move_X=+RC_Velocity;  	Move_Z=-PI/2;	break;
			case 3:     Move_X=0;      			Move_Z=-PI/2;   break;	 
			case 4:     Move_X=-RC_Velocity;  	Move_Z=-PI/2;   break;		 
			case 5:     Move_X=-RC_Velocity;  	Move_Z=0;       break;	 
			case 6:     Move_X=-RC_Velocity;  	Move_Z=+PI/2;   break;	 
			case 7:     Move_X=0;     	 		Move_Z=+PI/2;   break;
			case 8:     Move_X=+RC_Velocity; 	Move_Z=+PI/2;   break; 
			default:    Move_X=0;               Move_Z=0;       break;
		}
	 
		if     (Flag_Left ==1)  Move_Z= PI/2; //left rotation  //����ת 
		else if(Flag_Right==1)  Move_Z=-PI/2; //right rotation //����ת	
	}
	
	//Z������ת��
	if(Car_Mode==Akm_Car)
	{
		//�������ṹС��ת��Ϊǰ��ת��Ƕ�
		Move_Z = Move_Z*2/9; 
	}
	else if(Car_Mode==Diff_Car || Car_Mode==FourWheel_Car)
	{
		if(Move_X<0) Move_Z = -Move_Z; //The differential control principle series requires this treatment //���ٿ���ԭ��ϵ����Ҫ�˴���
		Move_Z = Move_Z*RC_Velocity/500;
	}		
	

	//��λת����mm/s -> m/s	
	Move_X = Move_X/1000;       
	Move_Y = Move_Y/1000;         
	Move_Z = Move_Z;
	
	//�õ�����Ŀ��ֵ�������˶�ѧ����
	Drive_Motor(Move_X,Move_Y,Move_Z);
}

/**************************************************************************
�������ܣ���ȡ��������ֵ�����㳵���ٶȣ���λm/s
��ڲ�������
����  ֵ����
**************************************************************************/
void Get_Velocity_Form_Encoder(void)
{
	  //Retrieves the original data of the encoder
	  //��ȡ��������ԭʼ����
		float Encoder_A_pr,Encoder_B_pr,Encoder_C_pr,Encoder_D_pr; 
		OriginalEncoder.A=Read_Encoder(2);	
		OriginalEncoder.B=Read_Encoder(3);	
		OriginalEncoder.C=Read_Encoder(4);	
		OriginalEncoder.D=Read_Encoder(5);	

	  //Decide the encoder numerical polarity according to different car models
		//���ݲ�ͬС���ͺž�����������ֵ����
		switch(Car_Mode)
		{
			case Mec_Car:       Encoder_A_pr=OriginalEncoder.A; Encoder_B_pr=OriginalEncoder.B; Encoder_C_pr= -OriginalEncoder.C;  Encoder_D_pr= -OriginalEncoder.D; break; 
			case Akm_Car:       Encoder_A_pr=OriginalEncoder.A; Encoder_B_pr=-OriginalEncoder.B; Encoder_C_pr= OriginalEncoder.C;  Encoder_D_pr= OriginalEncoder.D; break;
			case Diff_Car:      Encoder_A_pr=OriginalEncoder.A; Encoder_B_pr=-OriginalEncoder.B; Encoder_C_pr= OriginalEncoder.C;  Encoder_D_pr= OriginalEncoder.D; break; 
			case FourWheel_Car: Encoder_A_pr=OriginalEncoder.A; Encoder_B_pr=OriginalEncoder.B; Encoder_C_pr= -OriginalEncoder.C;  Encoder_D_pr= -OriginalEncoder.D; break; 
		}
		
		//The encoder converts the raw data to wheel speed in m/s
		//������ԭʼ����ת��Ϊ�����ٶȣ���λm/s
		MOTOR_A.Encoder= Encoder_A_pr*CONTROL_FREQUENCY*Wheel_perimeter/Encoder_precision;  
		MOTOR_B.Encoder= Encoder_B_pr*CONTROL_FREQUENCY*Wheel_perimeter/Encoder_precision;  
		MOTOR_C.Encoder= Encoder_C_pr*CONTROL_FREQUENCY*Wheel_perimeter/Encoder_precision; 
		MOTOR_D.Encoder= Encoder_D_pr*CONTROL_FREQUENCY*Wheel_perimeter/Encoder_precision; 
}
/**************************************************************************
�������ܣ�������Ŀ���ٶ���ƽ������
��ڲ���������Ŀ���ٶ�
����  ֵ����
**************************************************************************/
void Smooth_control(float vx,float vy,float vz)
{
	float step=0.01;

	if	   (vx>0) 	smooth_control.VX+=step;
	else if(vx<0)		smooth_control.VX-=step;
	else if(vx==0)	smooth_control.VX=smooth_control.VX*0.9f;
	
	if	   (vy>0)   smooth_control.VY+=step;
	else if(vy<0)		smooth_control.VY-=step;
	else if(vy==0)	smooth_control.VY=smooth_control.VY*0.9f;
	
	if	   (vz>0) 	smooth_control.VZ+=step;
	else if(vz<0)		smooth_control.VZ-=step;
	else if(vz==0)	smooth_control.VZ=smooth_control.VZ*0.9f;
	
	smooth_control.VX=target_limit_float(smooth_control.VX,-float_abs(vx),float_abs(vx));
	smooth_control.VY=target_limit_float(smooth_control.VY,-float_abs(vy),float_abs(vy));
	smooth_control.VZ=target_limit_float(smooth_control.VZ,-float_abs(vz),float_abs(vz));
}
/**************************************************************************
�������ܣ����������ݼ������ֵ
��ڲ�����������
����  ֵ���������ľ���ֵ
**************************************************************************/
float float_abs(float insert)
{
	if(insert>=0) return insert;
	else return -insert;
}

/**************************************************************************
�������ܣ�CCDѲ�ߣ��ɼ�3����е����ݲ���ȡ���� 
��ڲ�������
����  ֵ����
**************************************************************************/

void  Get_RC_CCD(void)
{
	static float Bias,Last_Bias;
	float move_z = 0;
									
	Move_X = RC_Velocity_CCD;										//CCDѲ��ģʽ���ٶ�
	Bias = CCD_Median - 64;  			//��ȡƫ�64ΪѲ�ߵ����ĵ�

	move_z = -Bias*CCD_KP*0.1f-(Bias-Last_Bias)*CCD_KI*0.1f;	 	//PD���ƣ�ԭ�����ʹ��С�����ֿ���Ѳ�ߵ����ĵ�
		
	Last_Bias = Bias;   				//������һ�ε�ƫ��

	if(Car_Mode == Mec_Car)															
	{
		Move_Z = move_z * RC_Velocity_CCD / 50000;					//���ٿ���ԭ����Ҫ�����˴�����
	}

	else if(Car_Mode == Akm_Car)												
	{
		Move_Z = move_z / 450;										//���ٿ���ԭ����Ҫ�����˴�����
	}
	else if(Car_Mode == Diff_Car)		
	{	
		if(Move_X<0) move_z = -move_z;	
		Move_Z = move_z * RC_Velocity_CCD / 67000;					//���ٿ���ԭ����Ҫ�����˴�����	
	}
	else if(Car_Mode == FourWheel_Car)									
	{
		if(Move_X<0) move_z = -move_z;	
		Move_Z = move_z * RC_Velocity_CCD / 20100;					//���ٿ���ԭ����Ҫ�����˴�����
	}			
		
	//Z������ת��	
	//��λת����mm/s -> m/s
	Move_X=Move_X/1000;
	Move_Z=Move_Z;
			
	//�õ�����Ŀ��ֵ�������˶�ѧ����
	Drive_Motor(Move_X,Move_Y,Move_Z);
}

/**************************************************************************
�������ܣ�OPENMVѲ�ߣ����ݲɼ�ͨ�Ŵ���stm32
��ڲ�������
����  ֵ����
**************************************************************************/
void  Get_RC_OPENMV(void)
{
	
	static int Last_biass;
	float move_z = 0;
									
	Move_X = RC_Velocity_OPENMV;										//OPENMVѲ��ģʽ���ٶ�

	move_z = -biass*OPENMV_KP*0.1f-(biass-Last_biass)*OPENMV_KI*0.1f;	 	//PD���ƣ�ԭ�����ʹ��С�����ֿ���Ѳ�ߵ����ĵ�
		
	Last_biass = biass;   				//������һ�ε�ƫ��

	if(Car_Mode == Mec_Car)															
	{
		Move_Z = move_z * RC_Velocity_OPENMV / 50000;					//���ٿ���ԭ����Ҫ�����˴�����
	}

	else if(Car_Mode == Akm_Car)												
	{
		Move_Z = move_z / 450;										//���ٿ���ԭ����Ҫ�����˴�����
	}
	else if(Car_Mode == Diff_Car)		
	{	
		if(Move_X<0) move_z = -move_z;	
		Move_Z = move_z * RC_Velocity_OPENMV / 67000;					//���ٿ���ԭ����Ҫ�����˴�����	
	}
	else if(Car_Mode == FourWheel_Car)									
	{
		if(Move_X<0) move_z = -move_z;	
		Move_Z = move_z * RC_Velocity_OPENMV / 20100;					//���ٿ���ԭ����Ҫ�����˴�����
	}			
		
	//Z������ת��	
	//��λת����mm/s -> m/s
	Move_X=Move_X/1000;
	Move_Z=Move_Z;
			
	//�õ�����Ŀ��ֵ�������˶�ѧ����
	Drive_Motor(Move_X,Move_Y,Move_Z);
}

/**************************************************************************
�������ܣ��������
��ڲ�������
����  ֵ����
**************************************************************************/

EnvironmentData Get_environment()
{
	while(1)
	{
    EnvironmentData data;
	
	data.temperature=rec_data[2]+rec_data[3]/10;
	if(rec_data[0]+rec_data[1]/10 <= 95)
		data.humidity=rec_data[0]+rec_data[1]/10;
	else
		data.humidity=95;
		
		
		
	data.gas_ppm = ppm;
	
	
	
	data.light_intensity = result1750;  //��ȡBH1750�Ĺ�ǿ����
	
	//data.light_intensity = read_BH1750();  //��ȡBH1750�Ĺ�ǿ����

    // �����ֶεĸ�ֵ...

    return data;
		
	}
}


/**************************************************************************
�������ܣ��״���ǰ���Ƿ����ϰ���(ԭ��ͬ��һ������)
��ڲ�������
����  ֵ����
**************************************************************************/
u8 Detect_Barrier(void)
{
	int i;
	u8 point_count = 0;
	
	if(Lidar_Detect == Lidar_Detect_ON)
	{
		for(i=0;i<1152;i++)	//����Ƿ����ϰ��� 
		{
			if((Dataprocess[i].angle>300)||(Dataprocess[i].angle<60))
			{
				if( 0 < Dataprocess[i].distance && Dataprocess[i].distance < 700)//700mm���Ƿ����ϰ���
					point_count++;
		  }
	}
		if(point_count > 0)//���ϰ���
			return Barrier_Detected;
		else
			return No_Barrier;
	}
	else
		return No_Barrier;
}

/**************************************************************************
�������ܣ�С������ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Lidar_Avoid(void)
{
	int i = 0; 
	u8 calculation_angle_cnt = 0;	//�����ж�225��������Ҫ�����ϵĵ�
	float angle_sum = 0;			//���Լ����ϰ���λ���������
	u8 distance_count = 0;			//����С��ĳֵ�ļ���
	for(i=0;i<1152;i++)				//����120�ȷ�Χ�ڵľ������ݣ���120�������ҵ�����
	{
		if((Dataprocess[i].angle>300)||(Dataprocess[i].angle<60))  //���ϽǶ���300-60֮��
		{
			if( (0 < Dataprocess[i].distance) && (Dataprocess[i].distance < Avoid_Distance) )	//����С��450mm(Avoid_Distance��ֵ)��Ҫ����,ֻ��Ҫ120�ȷ�Χ�ڵ�
			{
				calculation_angle_cnt++;						 	//�������С�ڱ��Ͼ���ĵ����
				if(Dataprocess[i].angle<60)		
					angle_sum += Dataprocess[i].angle;
				else if(Dataprocess[i].angle > 300)
					angle_sum += (Dataprocess[i].angle - 360);		//300�ȵ�60��ת��Ϊ-60�ȵ�60��
				if(Dataprocess[i].distance < Avoid_Min_Distance)		//��¼С��200mm�ĵ�ļ���
					distance_count++;
			}
	  }
	}
	
	Move_X = forward_velocity;
	
	if(calculation_angle_cnt == 0)//����Ҫ����
	{
		Move_Z = 0;
	}
	else                          //������С��200mm��С��������
	{
		if(distance_count>8)
		{
			Move_X = -forward_velocity;
			Move_Z = 0;
		}
		else
		{
			Move_X = 0;
			if(angle_sum > 0)//�ϰ���ƫ��
			{
				if(Car_Mode == Mec_Car)  //����ת����Ҫ��ǰ���ٶȽ���
					Move_X = 0;
				else                     //�������ͱ���ԭ�г���
				  Move_X = forward_velocity;
				
				if(Car_Mode == Akm_Car)
					Move_Z = PI/4;
				else
				  Move_Z=other_corner_velocity;//��ת
			}
			else //ƫ��
			{
				if(Car_Mode == Mec_Car)
					Move_X = 0;
				else
				  Move_X = forward_velocity;
				
				if(Car_Mode == Akm_Car)
					Move_Z = -PI/4;
				else
					Move_Z=-other_corner_velocity;	//��ת
			}
	  }
	}
	Drive_Motor(Move_X,Move_Y,Move_Z);
}


/**************************************************************************
�������ܣ�С������ģʽ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Lidar_Follow(void)
{
	static u16 cnt = 0;
	int i;
	int calculation_angle_cnt = 0;
	static float angle = 0;				//���ϵĽǶ�
	static float last_angle = 0;		
	u16 mini_distance = 65535;
	static u8 data_count = 0;			//�����˳�һд���ļ�������
	//��Ҫ�ҳ�������Ǹ���ĽǶ�
	for(i = 0; i < 1152; i++)
	{
			if( (0 < Dataprocess[i].distance) && (Dataprocess[i].distance < Follow_Distance))
			{
				calculation_angle_cnt++;
				if(Dataprocess[i].distance < mini_distance)
				{
					mini_distance = Dataprocess[i].distance;
					angle = Dataprocess[i].angle;
				}
			}
	}
	if(angle > 180)  //0--360��ת����0--180��-180--0��˳ʱ�룩
		angle -= 360;
	if((angle-last_angle > 10)||(angle-last_angle < -10))   //��һ����������������10�ȵ���Ҫ���ж�
	{
		if(++data_count > 30)   //����30�βɼ�����ֵ(300ms��)���ϴεıȴ���10�ȣ���ʱ������Ϊ����Чֵ
		{
			data_count = 0;
			last_angle = angle;
		}
	}
	else    //����С��10�ȵĿ���ֱ����Ϊ����Чֵ
	{
		if(++data_count > 10)   //����10�βɼ�����ֵ(100ms��)����ʱ������Ϊ����Чֵ
		{
			data_count = 0;
			last_angle = angle;
		}
	}
	if(calculation_angle_cnt < 8)  //�������С��8�ҵ�cnt>40��ʱ����Ϊ��1600��û�и���Ŀ��
	{
		if(cnt < 40)
			cnt++;
		if(cnt >= 40)
		{
			Move_X = 0;
			Move_Z = 0;
		}
	}
	else
	{
		cnt = 0;
		if(Move_X > 0.06f || Move_X < -0.06f)  //��Move_X���ٶ�ʱ��ת��PID��ʼ����
		{
			if(mini_distance < 700 && (last_angle > 60 || last_angle < -60))
			{
				Move_Z = -0.0098f*last_angle;  //������ƫС�ҽǶȲ�����ֱ�ӿ���ת��
			}
			else
			{
				  Move_Z = -Follow_Turn_PID(last_angle,0);		//ת��PID����ͷ��Զ���Ÿ�����Ʒ
			}
		}
		else
		{
			Move_Z = 0;
		}
		if(angle>150 || angle<-150)  //���С���ں�60����Ҫ�������˶��Լ�����ת��
		{
			Move_X = -Distance_Adjust_PID(mini_distance, Keep_Follow_Distance);
			Move_Z = -0.0098f*last_angle;
		}
		else
		{
		  Move_X = Distance_Adjust_PID(mini_distance, Keep_Follow_Distance);  //���־��뱣����500mm
		}
		Move_X = target_limit_float(Move_X,-amplitude_limiting,amplitude_limiting);   //��ǰ���ٶ��޷�
	}
	Drive_Motor(Move_X,Move_Y,Move_Z);
}

/**************************************************************************
�������ܣ�С����ֱ��ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Lidar_along_wall(void)
{
	static u32 target_distance=0;
	static int i;

	u32 distance;
	u8 data_count = 0;			//�����˳�һд���ļ�������
	
	Move_X = forward_velocity;  //��ʼ�ٶ�
	
	for(int j=0;j<1152;j++) //225
	  {
			if(Dataprocess[j].angle>268 && Dataprocess[j].angle<272)   //ȡ�״��4�ȵĵ�
			{
				if(i==0)
				{
					target_distance=Dataprocess[j].distance;  //�״ﲶ���һ������
					i++;
				}
				 if(Dataprocess[j].distance<(target_distance+limit_distance))//����һ���״��̽�����
				 {
					 data_count++;
					 distance=Dataprocess[j].distance;//ʵʱ����
				 }
		  }
	  }
		if(Car_Mode == Mec_Car)  //ֻ�����ֿ�����Move_Y
		{
			Move_Y = Along_Adjust_PID(distance,target_distance);
			Move_X = forward_velocity;
			Move_Z = 0;
		}
		else   //��������ʹ��Move_Z������ֱ��״̬
		{
			Move_Z = Along_Adjust_PID(distance,target_distance);
			Move_X = forward_velocity;
			Move_Y = 0;
		}
		if(data_count == 0)  //��data_count����0��ֻ��ǰ���ٶ�
			{
				Move_Y = 0;
				Move_Z = 0;
			}
	Drive_Motor(Move_X,Move_Y,Move_Z);
}


