#include "usartx.h"
#include "balance.h"
#include "DHT11.h" // ����DHT11��ʪ�ȴ�������
#include "MQ2.h"   // ����MQ2����������������
#include "BH1750.h" // ����BH1750����ǿ�ȴ�������
#define USART3_REC_LEN  200 

#define PATH_L 0
#define PATH_R 1

u8  USART3_RX_BUF[USART3_REC_LEN]; 
u16 USART3_RX_STA; 

int biass = 0;			// openmvѲ��ƫ��
int Car_STOP = 0;		// ֹͣ��ͣ�����Ʊ���
int Car_path = PATH_L;	// С���ߵ�·�� 0Ϊ��Ȧ(���·��) 1Ϊ��Ȧ(�Ҳ�·��)
int Car_LapNum = 0;		// С������ɵ�Ȧ�� ͨ��ֹͣλ����������һ��ֹͣλΪһȦ
int Car_LapNum_Tar = 1;	// С��Ŀ��Ȧ��

SEND_DATA Send_Data;
RECEIVE_DATA Receive_Data;
extern int Time_count;

/**************************************************************************
�������ܣ�����1��ʼ��
��ڲ�������
�� �� ֵ����
**************************************************************************/
void uart1_init(u32 bound)
{  	 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	 //ʹ��GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);   //ʹ��USARTʱ��

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10 ,GPIO_AF_USART1);	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;            //���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;       //����50MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //����
	GPIO_Init(GPIOA, &GPIO_InitStructure);  		      //��ʼ��
	
    //UsartNVIC����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	//IRQͨ��ʹ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	//����ָ���Ĳ�����ʼ��VIC�Ĵ���	
	NVIC_Init(&NVIC_InitStructure);	
	
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound; 					//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;				//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); 											//��ʼ������1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 					//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                     			 		//ʹ�ܴ���1
}
/**************************************************************************
�������ܣ�����2��ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void uart2_init(u32 bound)
{  	 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);		//ʹ��GPIOʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 		//ʹ��USARTʱ��
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6 ,GPIO_AF_USART2);	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;            //���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;       //����50MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //����
	GPIO_Init(GPIOD, &GPIO_InitStructure);  		      //��ʼ��
	
	//UsartNVIC����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;
	//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    //IRQͨ��ʹ��	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//����ָ���Ĳ�����ʼ��VIC�Ĵ���		
	NVIC_Init(&NVIC_InitStructure);	
	
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;                 		//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 		//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 				//һ��ֹͣ
	USART_InitStructure.USART_Parity = USART_Parity_No;					//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure);      						//��ʼ������2
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 						//�������ڽ����ж�
	USART_Cmd(USART2, ENABLE);                    						//ʹ�ܴ���2 
}

/**************************************************************************
�������ܣ�����3��ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void uart3_init(u32 bound)
{  	 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//UART_InitTypeDef UART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);		//ʹ��GPIOʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 		//ʹ��USARTʱ��
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3);	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11 ,GPIO_AF_USART3);	 
	
	// PC10 TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;            //����
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;       //����50MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //����
	GPIO_Init(GPIOC, &GPIO_InitStructure);  		      //��ʼ��
	
	//UsartNVIC����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    //IRQͨ��ʹ��	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//����ָ���Ĳ�����ʼ��VIC�Ĵ���		
	NVIC_Init(&NVIC_InitStructure);	
	
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;                 		//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 		//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 				//һ��ֹͣ
	USART_InitStructure.USART_Parity = USART_Parity_No;					//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure);      						//��ʼ������3
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 						//�������ڽ����ж�
	USART_Cmd(USART3, ENABLE);                    						//ʹ�ܴ���3 
	USART_ClearFlag(USART3, USART_FLAG_TC);
}

/**************************************************************************
�������ܣ�����1�����ж�
��ڲ�������
�� �� ֵ����
**************************************************************************/
int USART1_IRQHandler(void)
{	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //Check if data is received //�ж��Ƿ���յ�����
	{
		u8 Usart_Receive;		
		Usart_Receive = USART_ReceiveData(USART1); //Read the data //��ȡ����	
	}
	return 0;	
}
/**************************************************************************
�������ܣ�����2�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
int USART2_IRQHandler(void)
{	
	static int temp_count = 0;								//���ڼ�¼ǰ����ָ��Ĵ�������һ������������ʱ����Ҫ�õ�
	int Usart_Receive;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 	//�ж��Ƿ���յ�����
	{	      
		static u8 Flag_PID,i,j,Receive[50];
		static float Data;
				
		Usart_Receive=USART2->DR; 				//��ȡ����

	  
		if(Usart_Receive==0x4B)  	//����APPת����ƽ���
			Turn_Flag=1;  
		else if(Usart_Receive==0x49||Usart_Receive==0x4A) 	//����APP������ƽ���	
			Turn_Flag=0;	
		
		if(Turn_Flag==0) 
		{
			//APPҡ�˿��ƽ�������
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
			//APPת����ƽ�������
			if     (Usart_Receive==0x43) Flag_Left=0,Flag_Right=1; //Right rotation //����ת
			else if(Usart_Receive==0x47) Flag_Left=1,Flag_Right=0; //Left rotation  //����ת
			else                         Flag_Left=0,Flag_Right=0;
			if     (Usart_Receive==0x41||Usart_Receive==0x45) 
			{
				if((++temp_count) == 5)					//��Ҫ��������5��ǰ����ָ�����ת��һ��ʱ��ɿ�ʼapp����
				{
					temp_count = 0;
					APP_ON_Flag = RC_ON;
				}
				Flag_Direction=Usart_Receive-0x40;
			}
			else  Flag_Direction=0;
		}
		if(Usart_Receive==0x58)  RC_Velocity=RC_Velocity+100; //���ٰ�����+100mm/s
		if(Usart_Receive==0x59)  RC_Velocity=RC_Velocity-100; //���ٰ�����-100mm/s
	  
	 //��������APP���Խ���ͨѶ
	 if(Usart_Receive==0x7B) Flag_PID=1;   	//APP����ָ����ʼλ
	 if(Usart_Receive==0x7D) Flag_PID=2;   	//APP����ָ��ֹͣλ

	 if(Flag_PID==1) 	//�ɼ�����
	 {
		Receive[i]=Usart_Receive;
		i++;
	 }
	 if(Flag_PID==2) 	//��������
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
					switch(Receive[1])			//APP�������
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
//					else if(Mode == ELE_Line_Patrol_Mode)		//���Ѳ��z���ٶȵ���
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
					else if(Mode == CCD_Line_Patrol_Mode)									//CCDѲ��z���ٶȵ���
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
					else if(Mode == OPENMV_Line_Patrol_Mode)									//OPENMVѲ��z���ٶȵ���
					{
					switch(Receive[1])
					 {
						 case 0x30:  RC_Velocity_OPENMV=Data;break;
						 case 0x31:  OPENMV_KP=Data;break;
						 case 0x32:  OPENMV_KI=Data;break;
						 case 0x33:  Car_path = Data; break;		// ��ǰȦ����·������
						 case 0x34:  Car_LapNum_Tar = Data; break;	// ��Ŀ��Ȧ������
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
      //��ر�־λ����			
			Flag_PID=0;
			i=0;
			j=0;
			Data=0;
			memset(Receive, 0, sizeof(u8)*50); 	//��������
	 }
	if(RC_Velocity<0)   RC_Velocity=0; 
	if(RC_Velocity_CCD<0)   RC_Velocity_CCD=0; 
	if(RC_Velocity_OPENMV<0)  RC_Velocity_OPENMV=0; 
  }
  return 0;	
}

/**************************************************************************
�������ܣ�����3ȫ���жϺ��� openmv
��ڲ�������
�� �� ֵ����

int USART3_IRQHandler(void)			 
{
		u8 com_data; 
		u8 i;
		static u8 RxCounter1=0;
		static uint8_t  RxBuffer1[10]={0};
		static u8 RxState = 0;	
		static u8 RxFlag1 = 0;
		static u8 Recv_cnt = 0;		// ����ֹͣλ���հ���������ʼ�ж�ֹͣλ����һ��������û������ֹͣλ��������ֹͣ�ж�ֹͣλ�����������    ֹͣλ�жϣ�3 2 1��˳�����
		static u8 stopROI3 = 0;		// �Ƿ���3����ʶ��ֹͣλ
		static u8 stopROI2 = 0;		// �Ƿ���2����ʶ��ֹͣλ
		static u8 stopFlag = 0;		// �Ƿ�ȷ������ֹͣλ���
		
		int datalen = 6;	// ���ݰ�����Ϊ6, bb bbb b (2c 12 data1 data2 data3 5b): data1Ϊ��Ȧ·��ƫ����, data2Ϊ��Ȧ·��ƫ��ֵ, data3Ϊֹͣλʶ������

		if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)  	   //�����ж�  
		{
				USART_ClearITPendingBit(USART3,USART_IT_RXNE);   //����жϱ�־
				com_data = USART_ReceiveData(USART3);
			
				if(RxState==0 && com_data==0x2C)  //0x2c֡ͷ
				{
					RxState=1;
					RxBuffer1[RxCounter1++]=com_data;
				}
		
				else if(RxState==1 && com_data==0x12)  //0x12֡ͷ
				{
					RxState=2;
					RxBuffer1[RxCounter1++]=com_data;
				}
		
				else if(RxState==2)
				{
					RxBuffer1[RxCounter1++]=com_data;

					if(RxCounter1 >= datalen || com_data == 0x5B)       //RxBuffer1���ܵ���1�����ݰ����� �� �յ�ֹͣλ,�������ݽ���
					{
						RxState=3;
						RxFlag1=1;
						if(Recv_cnt > 0) Recv_cnt++;	// �� �ж�ֹͣλ �� ����ͣ��λ�� �Ĺ����У��հ�����
						if(stopFlag == 0 && Recv_cnt > 150)	// �����ж�ֹͣλ�Ĺ�����		(150������С����10cm/s���ٶ��н�ʱ��ֹͣ�ߴӽ�������ͷͼ�����򵽳��������յ������ݰ�����)
						{
							Recv_cnt = 0;	// ֹͣ����
							stopROI2 = 0;	// ���ñ��λ
							stopROI3 = 0;
						}
						if(stopFlag == 1 && Recv_cnt > 15)	// ȷ������ֹͣλ	�ٽ���һ�������İ���ͣ�������ڵ���С��ֹͣλ����ֹͣ�ߵ����λ��  250mm->23 300mm->19  350mm->
						{
							Car_STOP = 1;
							OLED_ShowString(50,10,"st");
							
							Recv_cnt = 0;
							stopFlag = 0;
						}
						
						
						// ѡ������Ȧ·�� ����ƫ����
						if(Car_path == PATH_L)	// ���·�� ��Ȧ
							biass = (int)RxBuffer1[RxCounter1-4];
						else 					// �Ҳ�·�� ��Ȧ
							biass = (int)RxBuffer1[RxCounter1-3]; 
						biass -= 80;	// ����ƫ����-80
						//biass -=90;		// ƫ�ƽ�-90
					
						
						// �ж�ֹͣ��
						if((int)RxBuffer1[RxCounter1-2] == 3) // ����3����ֹͣ��
						{
							stopROI3 = 1;	// �޸ı��λ
							OLED_ShowNumber(90,00, 3 ,1,12);	// ���3
							Recv_cnt++;		// ��0���1����ʼ����
						}
						else if((int)RxBuffer1[RxCounter1-2] == 2 && stopROI3 == 1) // ��3�����ϣ�����2����ֹͣ��
						{
							OLED_ShowNumber(90,00, 2 ,1,12);	// ���2
							stopROI2 = 1;	// �޸ı��λ
						}
						else if((int)RxBuffer1[RxCounter1-2] == 1 && stopROI2 == 1) // ��2�����ϣ�����1����ֹͣ��		����ȷ����ֹͣ��־
						{
							OLED_ShowNumber(90,00, 1 ,1,12);	// ���1
							OLED_ShowString(50,10,"ws");		// ����ȴ�ֹͣ���
							
							Recv_cnt = 0;	// ���ü�����
							stopROI2 = 0;	// ���ñ��λ
							stopROI3 = 0;
							
							Car_LapNum++;	// ���Ȧ��+1
							OLED_ShowNumber(110,00, Car_LapNum ,1,12);	// ���Ͻ������ǰ���Ȧ��
							
							if(Car_LapNum%2 == 1) Car_path = PATH_R;	// ����Ȧ�����ߣ���һȦ�������Ȧ
							else Car_path = PATH_L;
							if(Car_LapNum == Car_LapNum_Tar) 			// ����Ŀ��Ȧ����ͣ��
							{	
								stopFlag = 1;
								Recv_cnt = 1;	// ��1��ʼ������ͨ����ʱһ�������Ľ��յ��İ�������ֹͣλ��			
							}
						}
						else if((int)RxBuffer1[RxCounter1-2] == 1 && stopROI3 == 1 && stopROI2 == 0) // 3������ֹͣ�ߺ�û�յ�2���յ�1�ˣ�ֹͣ�жϣ�˵������ֹͣ��
						{
							OLED_ShowNumber(90,00, 0 ,1,12);	// ���� ֹͣʶ��
							stopROI3 = 0;	
							Recv_cnt = 0;
						}
						
					}
				}
				else if(RxState==3)		//����Ƿ���ܵ�������־
				{
						if(RxBuffer1[RxCounter1-1] == 0x5B)
						{
									USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);//�ر�DTSABLE�ж�
										
									RxFlag1 = 0;
									RxCounter1 = 0;
									RxState = 0;
									USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
						}
						else   //���մ���
						{
									RxState = 0;
									RxCounter1=0;
									for(int i = 0; i < datalen+1; i++)	RxBuffer1[i] = 0;		// ������һλ������
									//memset(RxBuffer1, 0, sizeof RxBuffer1); 	//��������	// ���ܳ�����
						}
				}  
	
				else   //�����쳣
				{
						RxState = 0;
						RxCounter1=0;
						for(int i = 0; i < datalen+1; i++)	RxBuffer1[i] = 0;
						//memset(RxBuffer1, 0, sizeof RxBuffer1); 	//��������	// ���ܳ�����
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
		
		
		
		if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)  	   //�����ж�  
		{
				USART_ClearITPendingBit(USART3,USART_IT_RXNE);   //����жϱ�־
				com_data = USART_ReceiveData(USART3);
			
				if(RxState==0 && com_data==0x2C)  //0x2c֡ͷ
				{
					RxState=1;
					RxBuffer1[RxCounter1++]=com_data;
				}
		
				else if(RxState==1 && com_data==0x12)  //0x12֡ͷ
				{
					RxState=2;
					
				}
		
				else if(RxState==2)
				{
					RxBuffer1[RxCounter1++]=com_data;

					if(RxCounter1 >= datalen || com_data == 0x5B)       //RxBuffer1���ܵ���1�����ݰ����� �� �յ�ֹͣλ,�������ݽ���
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
				else if(RxState==3)		//����Ƿ���ܵ�������־
				{
						if(RxBuffer1[RxCounter1-1] == 0x5B)
						{
									USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);//�ر�DTSABLE�ж�
										
									RxFlag1 = 0;
									RxCounter1 = 0;
									RxState = 0;
									USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
						}
						else   //���մ���
						{
									RxState = 0;
									RxCounter1=0;
									for(int i = 0; i < datalen+1; i++)	RxBuffer1[i] = 0;		// ������һλ������
									//memset(RxBuffer1, 0, sizeof RxBuffer1); 	//��������	// ���ܳ�����
						}
				}  
	
				else   //�����쳣
				{
						RxState = 0;
						RxCounter1=0;
						for(int i = 0; i < datalen+1; i++)	RxBuffer1[i] = 0;
						//memset(RxBuffer1, 0, sizeof RxBuffer1); 	//��������	// ���ܳ�����
				}
		}

		
		
	return 0;	
		
		

}



    





/**************************************************************************
�������ܣ�����λ��������Ŀ��ǰ���ٶ�Vx��Ŀ����ٶ�Vz��ת��Ϊ������С������ǰ��ת��
��ڲ�����Ŀ��ǰ���ٶ�Vx��Ŀ����ٶ�Vz����λ��m/s��rad/s
����  ֵ��������С������ǰ��ת�ǣ���λ��rad
**************************************************************************/
float Vz_to_Akm_Angle(float Vx, float Vz)
{
	float R, AngleR, Min_Turn_Radius;
	//float AngleL;
	
	
	//������С����Ҫ������Сת��뾶
	//���Ŀ���ٶ�Ҫ���ת��뾶С����Сת��뾶��
	//�ᵼ��С���˶�Ħ���������ߣ�����Ӱ�����Ч��
	Min_Turn_Radius=MINI_AKM_MIN_TURN_RADIUS;
	
	if(Vz!=0 && Vx!=0)
	{
		//���Ŀ���ٶ�Ҫ���ת��뾶С����Сת��뾶
		if(float_abs(Vx/Vz)<=Min_Turn_Radius)
		{
			//����Ŀ����ٶȣ����ǰ���ٶȣ����ת��뾶����Сת��뾶
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
�������ܣ�����λ���������ĸ�8λ�͵�8λ�������ϳ�һ��short�����ݺ�������λ��ԭ����
��ڲ�������8λ����8λ
����  ֵ��������X/Y/Z���Ŀ���ٶ�
**************************************************************************/
float XYZ_Target_Speed_transition(u8 High,u8 Low)
{
	//����ת�����м����
	short transition; 
	
	//����8λ�͵�8λ���ϳ�һ��16λ��short������
	transition=((High<<8)+Low); 
	
	return transition/1000+(transition%1000)*0.001; //Unit conversion, mm/s->m/s //��λת��, mm/s->m/s						
}

/**************************************************************************
�������ܣ�����1��������
��ڲ�����Ҫ���͵�����
����  ֵ����
**************************************************************************/
void usart1_send(u8 data)
{
	USART1->DR = data;
	while((USART1->SR&0x40)==0);	
}

/**************************************************************************
�������ܣ�����2��������
��ڲ�����Ҫ���͵�����
����  ֵ����
**************************************************************************/
void usart2_send(u8 data)
{
	USART2->DR = data;
	while((USART2->SR&0x40)==0);	
}
/**************************************************************************
�������ܣ�����2��������
��ڲ�����Ҫ���͵�����
����  ֵ����
**************************************************************************/
void usart3_send(u8 data)
{
	USART3->DR = data;
	while((USART3->SR&0x40)==0);	
}


/**************************************************************************
�������ܣ����PWM���ų�ʼ��
��ڲ�����arr���Զ���װֵ  psc��ʱ��Ԥ��Ƶ�� 
����  ֵ����
**************************************************************************/
void ServoX_PWM_Init(u16 arr,u16 psc)	// �ײ���� ����x�������ƶ� PC6 - TIM8_CH1
{ 
	GPIO_InitTypeDef GPIO_InitStructure;           //IO
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //��ʱ��	
	TIM_OCInitTypeDef  TIM_OCInitStructure;        //PWM���
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//ʹ��PORTCʱ��	
		
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; 	// �˴���̨����Ϊ GPIO_Speed_2MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);

	/*** Initialize timer 1 || ��ʼ����ʱ��1 ***/
	TIM_TimeBaseStructure.TIM_Period = arr; 						//�趨�������Զ���װֵ ARR
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 						//Ԥ��Ƶ�� PSC
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//TIM���ϼ���ģʽ	
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); 				//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//-----------�����ʼ��-----------//
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; 								//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   	//�������:TIM����Ƚϼ��Ը�	
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;   
	TIM_OC1Init(TIM8, &TIM_OCInitStructure); 						//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx	
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);					//ͨ��Ԥװ��ʹ��	 
	
	//-----------�����ʼ��-----------// 
	TIM_CtrlPWMOutputs(TIM8,ENABLE); 			//�߼���ʱ���������ʹ�����	
	TIM_Cmd(TIM8, ENABLE); 		 				//ʹ�ܶ�ʱ��


	//ͨ��ֵ��ʼ��Ϊ750���������Ӧֵ
	//TIM8->CCR1=750;
}

/**************************************************************************
�������ܣ����PWM���ų�ʼ��
��ڲ�����arr���Զ���װֵ  psc��ʱ��Ԥ��Ƶ�� 
����  ֵ����
**************************************************************************/
void ServoY_PWM_Init(u16 arr,u16 psc)	// ҡ�۶�� ����y�������ƶ� PC7 - TIM8_CH2
{ 
	GPIO_InitTypeDef GPIO_InitStructure;           //IO
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //��ʱ��	
	TIM_OCInitTypeDef  TIM_OCInitStructure;        //PWM���
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//ʹ��PORTCʱ��	
		
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; 	// �˴���̨����Ϊ GPIO_Speed_2MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);

	/*** Initialize timer 1 || ��ʼ����ʱ��1 ***/
	
	TIM_TimeBaseStructure.TIM_Period = arr; 		//�趨�������Զ���װֵ ARR
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 		//Ԥ��Ƶ�� PSC
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//TIM���ϼ���ģʽ	
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); 	//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//-----------�����ʼ��-----------//
	
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 	//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; 				//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;  
	TIM_OC2Init(TIM8, &TIM_OCInitStructure); 		//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);	//ͨ��Ԥװ��ʹ��	 
	//-----------�����ʼ��-----------// 
	
	//�߼���ʱ���������ʹ�����	
	TIM_CtrlPWMOutputs(TIM8,ENABLE); 	
	//ʹ�ܶ�ʱ��
	TIM_Cmd(TIM8, ENABLE); 		 


	//ͨ��ֵ��ʼ��Ϊ750���������Ӧֵ
	//TIM8->CCR2=750;
}
