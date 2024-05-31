#include "key.h"

/**************************************************************************
�������ܣ�������ʼ��
��ڲ�������
����  ֵ���� 
**************************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOBʱ��
  GPIO_InitStructure.GPIO_Pin = KEY_PIN; //KEY��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE0	
} 
/**************************************************************************
�������ܣ�����ɨ��
��ڲ�������
����  ֵ������״̬ 0���޶��� 1������ 
**************************************************************************/
u8 click(void)
{
	//Press the release sign
	//�������ɿ���־
	static u8 flag_key=1;
	
	if(flag_key&&KEY==0)
	{
	 flag_key=0; //The key is pressed //��������
	 return 1;	
	}
	else if(1==KEY)			
		flag_key=1;
	return 0; //No key is pressed //�ް�������
}


uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*�ȴ������ͷ� */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}

/**************************************************************************
�������ܣ��ӳٺ���
��ڲ�������
�� �� ֵ����
**************************************************************************/
void Delay_ms(void)
{
   int ii,i;    
   for(ii=0;ii<50;ii++)
   {
	   for(i=0;i<50;i++);
	 }	
}
/**************************************************************************
�������ܣ�����ɨ��
��ڲ�����˫���ȴ�ʱ��
����  ֵ������״̬: 0-�޶���, 1-����, 2-˫�� 
**************************************************************************/
u8 click_N_Double (u8 time)
{
		static	u8 flag_key,count_key,double_key;	
		static	u16 count_single,Forever_count;
	
	  if(KEY==0)  Forever_count++;   
    else        Forever_count=0;
	
		if(0==KEY&&0==flag_key)		flag_key=1;	
	  if(0==count_key)
		{
				if(flag_key==1) 
				{
					double_key++;
					count_key=1;	
				}
				if(double_key==2) 
				{
					double_key=0;
					count_single=0;
					return 2; //Double click //˫��
				}
		}
		if(1==KEY)			flag_key=0,count_key=0;
		
		if(1==double_key)
		{
			count_single++;
			if(count_single>time&&Forever_count<time)
			{
			double_key=0;
			count_single=0;	
			return 1; //Click //����
			}
			if(Forever_count>time)
			{
			double_key=0;
			count_single=0;	
			}
		}	
		return 0;
}
/**************************************************************************
�������ܣ�����ɨ�衣��Ϊʹ�õ��˾�̬���������ദ��Ҫʹ�ð���ɨ�躯��ʱ����Ҫ�ٶ���һ����ͬ������
��ڲ�������
�� �� ֵ������״̬: 0-�޶���, 1-����, 2-˫�� 
**************************************************************************/
u8 click_N_Double_MPU6050 (u8 time)
{
		static	u8 flag_key,count_key,double_key;	
		static	u16 count_single,Forever_count;
	
	  if(KEY==0)  Forever_count++;  
    else        Forever_count=0;
		if(0==KEY&&0==flag_key)		flag_key=1;	
	  if(0==count_key)
		{
				if(flag_key==1) 
				{
					double_key++;
					count_key=1;	
				}
				if(double_key==2) 
				{
					double_key=0;
					count_single=0;
					return 2; //Double click //˫��
				}
		}
		if(1==KEY)			flag_key=0,count_key=0;
		
		if(1==double_key)
		{
			count_single++;
			if(count_single>time&&Forever_count<time)
			{
			double_key=0;
			count_single=0;	
			return 1; //Click //����
			}
			if(Forever_count>time)
			{
			double_key=0;
			count_single=0;	
			}
		}	
		return 0;
}
/**************************************************************************
�������ܣ��������
��ڲ�������
����  ֵ������״̬ 0���޶��� 1������3s
**************************************************************************/
u8 Long_Press(void)
{
	static u16 Long_Press_count,Long_Press;

	if(Long_Press==0&&KEY==0)  Long_Press_count++; 
	else                       Long_Press_count=0;

	if(Long_Press_count>300)	//3 seconds //3��	
	{
		Long_Press=1;	
		Long_Press_count=0;
		return 1;
	}				
	 if(Long_Press==1) //Long press position 1 //������־λ��1
	{
			Long_Press=0;
	}
	return 0;
}

/**************************************************************************
�������ܣ�ѡ�����е�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
//u8  select(void)
//{
//	  int Flag_Next=0;
//	  static u8 flag=1;
//	
//	  oled_show_once();  //OLED��ʾ
//	  if(click_N_Double(150) == 1)
//		{
//			Move+=1;
//			if(Move>6)
//				Move=0;
//		}
//	  if(Move==0)			 ELE_ON_Flag=0,CCD_ON_Flag=1,APP_ON_Flag=0,Along_wall=0,Follow_ON_Flag=0,Avoid_ON_Flag=0;  //CCDң��ģʽ 
//		else if(Move==1) ELE_ON_Flag=1,CCD_ON_Flag=0,APP_ON_Flag=0,Along_wall=0,Follow_ON_Flag=0,Avoid_ON_Flag=0;  //ELEң��ģʽ
//		else if(Move==2) ELE_ON_Flag=0,CCD_ON_Flag=0,APP_ON_Flag=1,Along_wall=0,Follow_ON_Flag=0,Avoid_ON_Flag=0;	//othģʽ
//	  else if(Move==3) ELE_ON_Flag=0,CCD_ON_Flag=0,APP_ON_Flag=0,Along_wall=1,Follow_ON_Flag=0,Avoid_ON_Flag=0;  //��ֱ��ģʽ
//	  else if(Move==4) ELE_ON_Flag=0,CCD_ON_Flag=0,APP_ON_Flag=0,Along_wall=0,Follow_ON_Flag=1,Avoid_ON_Flag=0;  //����ģʽ
//	  else if(Move==5) ELE_ON_Flag=0,CCD_ON_Flag=0,APP_ON_Flag=0,Along_wall=0,Follow_ON_Flag=0,Avoid_ON_Flag=1;  //����ģʽ
//    if(Long_Press())Flag_Next=1;//��������3���������ģʽ
//	  if(Flag_Next==1)OLED_Clear(),flag=0;  //���OLED��Ļ ������������
//	  return flag;	  
//}
