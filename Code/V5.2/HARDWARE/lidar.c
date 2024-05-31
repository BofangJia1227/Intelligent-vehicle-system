

#include "lidar.h"
#include <string.h>
#include "sys.h"

float Diff_Along_Distance_KP = -0.030f,Diff_Along_Distance_KD = -0.245f,Diff_Along_Distance_KI = -0.001f;	//����С����ֱ�߾������PID����
float Akm_Along_Distance_KP = -0.115f*1000,Akm_Along_Distance_KD = -1000.245f*1000,Akm_Along_Distance_KI = -0.001f*1000;	//��������ֱ�߾������PID����
float FourWheel_Along_Distance_KP = -0.115f*1000,FourWheel_Along_Distance_KD = -100.200f*1000,FourWheel_Along_Distance_KI = -0.001f*1000;	//������ֱ�߾������PID����
float Along_Distance_KP = 0.163f*1000,Along_Distance_KD = 0.123*1000,Along_Distance_KI = 0.001f*1000;	//���ֺ�ȫ����ֱ�߾������PID����

float Distance_KP = -0.150f,Distance_KD = -0.052f,Distance_KI = -0.001;	//����������PID����

float Follow_KP = -2.566f,Follow_KD = -1.368f,Follow_KI = -0.001f;  //ת��PID
float Follow_KP_Akm = -0.550f,Follow_KD_Akm = -0.121f,Follow_KI_Akm = -0.001f;
//float Distance_KP = 0.653f*1000,Distance_KD = 2.431f*1000,Distance_KI = 0.001*1000;	//�������PID����

PointDataProcessDef PointDataProcess[1200];//����225������
LiDARFrameTypeDef Pack_Data;
PointDataProcessDef Dataprocess[1200];      //����С�����ϡ����桢��ֱ�ߡ�ELE�״���ϵ��״�����

/**************************************************************************
�������ܣ��״ﴮ�ڳ�ʼ��
��ڲ���: �� 
����  ֵ����
**************************************************************************/	 	
//����5

void LIDAR_USART_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	USART_DeInit(UART5);  //��λ����5

	// �򿪴���GPIO��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOC Dʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); //Enable the Usart clock //ʹ��USARTʱ��
	 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2 ,GPIO_AF_UART5);	 

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;            //���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;       //����50MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //����
	GPIO_Init(GPIOC, &GPIO_InitStructure);  		          //��ʼ��

	// ��USART Rx��GPIO����Ϊ��������ģʽ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;            //���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;       //����50MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //����
	GPIO_Init(GPIOD, &GPIO_InitStructure);  		          //��ʼ��

	//����USARTΪ�ж�Դ 
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	//�������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	//�����ȼ� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	//ʹ���ж� 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//��ʼ������NVIC
	NVIC_Init(&NVIC_InitStructure);


	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate =460800;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(UART5, &USART_InitStructure);

	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);	


	// ʹ�ܴ���
	USART_Cmd(UART5, ENABLE);	    

}

/**************************************************************************
�������ܣ����ݴ�����
��ڲ�������
����  ֵ����
**************************************************************************/
//���һ֡���պ���д���
int data_cnt = 0;
void data_process(void) //���ݴ���
{
	int m;
	u32 distance_sum[32]={0};//2����ľ���͵�����
	float start_angle = (((u16)Pack_Data.start_angle_h<<8)+Pack_Data.start_angle_l)/100.0;//����32����Ŀ�ʼ�Ƕ�
	float end_angle = (((u16)Pack_Data.end_angle_h<<8)+Pack_Data.end_angle_l)/100.0;//����32����Ľ����Ƕ�
	float area_angle[32]={0};
	
	if(start_angle>end_angle)//�����ǶȺͿ�ʼ�Ƕȱ�0�ȷָ�����
		end_angle +=360;

	for(m=0;m<32;m++)
	{
		area_angle[m]=start_angle+(end_angle-start_angle)/32*m;
		if(area_angle[m]>360)  area_angle[m] -=360;
		
		distance_sum[m] +=((u16)Pack_Data.point[m].distance_h<<8)+Pack_Data.point[m].distance_l;//���ݸߵ�8λ�ϲ�

		Dataprocess[data_cnt].angle=area_angle[m];
		Dataprocess[data_cnt++].distance=distance_sum[m];  //һ֡����Ϊ32����
		if(data_cnt == 1152) data_cnt = 0;
	}
	
		
}

/**************************************************************************
�������ܣ�����5�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
void UART5_IRQHandler(void)
{	
//	static u8 Count=0;

	static u8 state = 0;//״̬λ	
	static u8 crc_sum = 0;//У���
	static u8 cnt = 0;//����һ֡16����ļ���
	u8 temp_data;

	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) //���յ�����
	{	
		//USART_ClearITPendingBit(UART5, USART_IT_RXNE);
		temp_data=USART_ReceiveData(UART5);	
		switch(state)
		{
			case 0:
				if(temp_data == HEADER_0)//ͷ�̶�
				{
					Pack_Data.header_0= temp_data;
					state++;
					//У��
					crc_sum += temp_data;
				} else state = 0,crc_sum = 0;
				break;
			case 1:
				if(temp_data == HEADER_1)//ͷ�̶�
				{
					Pack_Data.header_1 = temp_data;
					state++;
					crc_sum += temp_data;
				} else state = 0,crc_sum = 0;
				break;
			case 2:
				if(temp_data == Length_)//�ֳ��̶�
				{
					Pack_Data.ver_len = temp_data;
					state++;
					crc_sum += temp_data;
				} else state = 0,crc_sum = 0;
				break;
			case 3:
				Pack_Data.speed_h = temp_data;//�ٶȸ߰�λ
				state++;
				crc_sum += temp_data;			
				break;
			case 4:
				Pack_Data.speed_l = temp_data;//�ٶȵͰ�λ
				state++;
				crc_sum += temp_data;
				break;
			case 5:
				Pack_Data.start_angle_h = temp_data;//��ʼ�Ƕȸ߰�λ
				state++;
				crc_sum += temp_data;
				break;
			case 6:
				Pack_Data.start_angle_l = temp_data;//��ʼ�ǶȵͰ�λ
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
				Pack_Data.point[cnt].distance_h = temp_data & 0x7f ;//16����ľ������ݣ����ֽ�
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
				Pack_Data.point[cnt].distance_l = temp_data;//16����ľ������ݣ����ֽ�
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
				Pack_Data.point[cnt].Strong = temp_data;//16�����ǿ������
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
				Pack_Data.end_angle_h = temp_data;//�����Ƕȵĸ߰�λ
				state++;
				crc_sum += temp_data;			
				break;
			case 106:
				Pack_Data.end_angle_l = temp_data;//�����ǶȵĵͰ�λ
				state++;
				crc_sum += temp_data;
				break;
			case 107:
				Pack_Data.crc = temp_data;//У��
				state = 0;
				cnt = 0;
				if(crc_sum == Pack_Data.crc)
				{
					data_process();//���ݴ���У����ȷ����ˢ�´洢������	
				}
				else 
				{
					memset(&Pack_Data,0,sizeof(Pack_Data));//����
				}
				crc_sum = 0;//У�������
				break;
			default: break;
		}
	}		
}

/**************************************************************************
�������ܣ���ֱ���״����pid
��ڲ���: ��ǰ�����Ŀ�����
����  ֵ�����Ŀ���ٶ�
**************************************************************************/	 	
//��ֱ���״�������pid

float Along_Adjust_PID(float Current_Distance,float Target_Distance)//�������PID
{
	static float Bias,OutPut,Integral_bias,Last_Bias;
	Bias=Target_Distance-Current_Distance;                          	//����ƫ��
	Integral_bias+=Bias;	                                 			//���ƫ��Ļ���
	
	if(Integral_bias>1000)	Integral_bias=1000;
	else if(Integral_bias<-1000) Integral_bias=-1000;
	
	if(Car_Mode == FourWheel_Car)
		OutPut=FourWheel_Along_Distance_KP*Bias/100000+FourWheel_Along_Distance_KI*Integral_bias/100000+FourWheel_Along_Distance_KD*(Bias-Last_Bias)/100000;//λ��ʽPID������
	else if(Car_Mode == Akm_Car)
		OutPut=Akm_Along_Distance_KP*Bias/100000+Akm_Along_Distance_KI*Integral_bias/100000+Akm_Along_Distance_KD*(Bias-Last_Bias)/1000;//λ��ʽPID������
	else if(Car_Mode == Diff_Car)
		OutPut=Diff_Along_Distance_KP*Bias/100+Diff_Along_Distance_KI*Integral_bias/100+Diff_Along_Distance_KD*(Bias-Last_Bias);//λ��ʽPID������
	else
	  OutPut=-Along_Distance_KP*Bias/100000-Along_Distance_KI*Integral_bias/100000-Along_Distance_KD*(Bias-Last_Bias)/1000;//λ��ʽPID������
	
	Last_Bias=Bias;                                       		 			//������һ��ƫ��
	
	if(Turn_Off(Voltage)== 1)								//����رգ���ʱ��������
		Integral_bias = 0;
	
	return OutPut;                                          	
}


/**************************************************************************
�������ܣ������״����pid
��ڲ���: ��ǰ�����Ŀ�����
����  ֵ�����Ŀ���ٶ�
**************************************************************************/	 	
//�����״�������pid
float Distance_Adjust_PID(float Current_Distance,float Target_Distance)//�������PID
{
	static float Bias,OutPut,Integral_bias,Last_Bias;
	Bias = Target_Distance - Current_Distance;                          //����ƫ��
	Integral_bias += Bias;	                                 			//���ƫ��Ļ���
	if(Integral_bias>1000) 
		Integral_bias = 1000;
	else if(Integral_bias<-1000) 
		Integral_bias = -1000;
	
	OutPut=Distance_KP*Bias/100+Distance_KI*Integral_bias/100+Distance_KD*(Bias-Last_Bias)/100;	//λ��ʽPID������
	Last_Bias=Bias;    		//������һ��ƫ��
	
	if(Turn_Off(Voltage)== 1)								//����رգ���ʱ��������
		Integral_bias = 0;
	
	return OutPut;                                          	
}

/**************************************************************************
�������ܣ������״�ת��pid
��ڲ���: ��ǰ�ǶȺ�Ŀ��Ƕ�
����  ֵ�����ת���ٶ�
**************************************************************************/	 	
//�����״�ת��pid
float Follow_Turn_PID(float Current_Angle,float Target_Angle)
{
	static float Bias,OutPut,Integral_bias,Last_Bias;
	Bias=Target_Angle-Current_Angle;                         				 //����ƫ��
	Integral_bias+=Bias;	                                 				 //���ƫ��Ļ���
	
	if(Integral_bias>1000)	Integral_bias=1000;
	else if(Integral_bias<-1000)	Integral_bias=-1000;
	
	if(Car_Mode == Akm_Car)
		OutPut=(Follow_KP_Akm/100)*Bias+(Follow_KI_Akm/100)*Integral_bias+(Follow_KD_Akm/100)*(Bias-Last_Bias);	//λ��ʽPID������
	else
		OutPut=(Follow_KP/100)*Bias+(Follow_KI/100)*Integral_bias+(Follow_KD/100)*(Bias-Last_Bias);	//λ��ʽPID������
	
	Last_Bias=Bias;                                       					 		//������һ��ƫ��
	
	if(Turn_Off(Voltage)== 1)								//����رգ���ʱ��������
		Integral_bias = 0;
	
	return OutPut;                                           					 	//���
	
}


/**************************************************************************
�������ܣ�����PI������
��ڲ���������������ֵ��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/

//int Incremental_PI_Left (int Encoder,int Target)
//{ 	
//	 static int Bias,Pwm,Last_bias;
//	 Bias=Target-Encoder;                					//����ƫ��
//	 Pwm+=11*(Bias-Last_bias)+10*Bias;   	//����ʽPI������
//	 Last_bias=Bias;	                   					//������һ��ƫ�� 
//	 return Pwm;                         					//�������
//}


//int Incremental_PI_Right (int Encoder,int Target)
//{ 	
//	 static int Bias,Pwm,Last_bias;
//	 Bias=Target-Encoder;                					//����ƫ��
//	 Pwm+=11*(Bias-Last_bias)+10*Bias;   	//����ʽPI������
//	 Last_bias=Bias;	                   					//������һ��ƫ�� 
//	 return Pwm;                         					//�������
//}
