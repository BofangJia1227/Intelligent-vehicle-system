#include "adc.h"

float Voltage,Voltage_Count,Voltage_All; //Variables related to battery voltage sampling //��ص�ѹ������صı���  
const float Revise=1.03;

//CCD��ر���
u16 CCD_Median,CCD_Threshold,ADV[128]={0}; 

//���Ѳ����ر���																										
int Sensor_Left,Sensor_Middle,Sensor_Right,Sensor,sum; 
/**************************************************************************
�������ܣ�ADC��ʼ����ص�ѹ���
��ڲ�������
����  ֵ����
**************************************************************************/
void  Adc_Init(void)
{  
		GPIO_InitTypeDef       GPIO_InitStructure;
		ADC_CommonInitTypeDef ADC_CommonInitStructure;
		ADC_InitTypeDef       ADC_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOAʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��


		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PB0 ͨ��8
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��  
	
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//PB1 ͨ��9
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��


		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 

		ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
		ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��

		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
		ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
		ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
		ADC_Cmd(ADC1, ENABLE);//����ADת����	 
}		
/**************************************************************************
�������ܣ�AD����
��ڲ�����ADC��ͨ��
����  ֵ��ADת�����
**************************************************************************/
u16 Get_Adc(u8 ch)   
{
	//Sets the specified ADC rule group channel, one sequence, and sampling time
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	
	//ADC1,ADCͨ��,����ʱ��Ϊ480����
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );
  //Enable the specified ADC1 software transformation startup function	
  //ʹ��ָ����ADC1�����ת����������	
	ADC_SoftwareStartConv(ADC1);
	//Wait for the conversion to finish
  //�ȴ�ת������	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	//Returns the result of the last ADC1 rule group conversion
	//�������һ��ADC1�������ת�����
	return ADC_GetConversionValue(ADC1);	
}

/**************************************************************************
�������ܣ��ɼ����ADCֵ��ƽ��ֵ����
��ڲ�����ADCͨ���Ͳɼ�����
�� �� ֵ��ADת�����
**************************************************************************/
u16 Get_adc_Average(u8 chn, u8 times)
{
  u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(chn);
		delay_ms(5);
	}
	return temp_val/times;
}

/**************************************************************************
�������ܣ���ȡ��ص�ѹ 
��ڲ�������
����  ֵ����ص�ѹ����λmv
**************************************************************************/
float Get_battery_volt(void)   
{  
	float Volt;
	
	//The resistance partial voltage can be obtained by simple analysis according to the schematic diagram
	//�����ѹ���������ԭ��ͼ�򵥷������Եõ�	
	Volt=Get_Adc(Battery_Ch)*3.3*11.0*Revise/1.0/4096;	
	return Volt;
}
/**************************************************************************
�������ܣ�adc���ݲɼ����� 
��ڲ�������
����  ֵ����
**************************************************************************/
void adc_task(void *pvParameters)									
{
	 u32 lastWakeTime = getSysTickCnt();
	
   while(1)
    {	
			//The task is run at 50hz
			//��������50Hz��Ƶ������
			vTaskDelayUntil(&lastWakeTime, F2T(RATE_50_HZ));

			if(Mode  == CCD_Line_Patrol_Mode)
			{
			  RD_TSL();  																//���ݲɼ�����
			  Find_CCD_Median ();			          		 //===��ȡ���� 
			}

//			else if(Mode  == ELE_Line_Patrol_Mode)
//			{
//				Sensor_Left=Get_Adc(14);
//			  Sensor_Right=Get_Adc(4);
//			  Sensor_Middle=Get_Adc(5);
//					
//			  sum=Sensor_Left*1+Sensor_Middle*100+Sensor_Right*199;		//��һ������
//			  Sensor=sum/(Sensor_Left+Sensor_Middle+Sensor_Right);		//��ȡ����
//			}
		}
}
/**************************************************************************
�������ܣ�����CCD��ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void  ccd_Init(void)
{    
 //�ȳ�ʼ��ADC1ͨ��7 IO��
	   GPIO_InitTypeDef        GPIO_InitStructure;
    ADC_InitTypeDef         ADC_InitStructure;
    ADC_CommonInitTypeDef   ADC_CommonInitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOCʱ��
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);    // ʹ��ADC1ʱ��
   
    /*=============================����ADC��Ӧ��GPIO=============================*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//PC1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//2MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//2MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
   
    /*=============================ADCʱ��ʹ��=============================*/

    RCC_AHB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);    // ADC1��λ
    RCC_AHB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);   // ��λ����
   
		ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
		ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
		
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
		ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
		ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
		ADC_Cmd(ADC1, ENABLE);//����ADת����	
}	

/**************************************************************************
�������ܣ�OPENMV��ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void  OPENMV_Init(void)
{    
 //�ȳ�ʼ��ADC1ͨ��7 IO��
	   GPIO_InitTypeDef        GPIO_InitStructure;
    ADC_InitTypeDef         ADC_InitStructure;
    ADC_CommonInitTypeDef   ADC_CommonInitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOCʱ��
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);    // ʹ��ADC1ʱ��
   
    /*=============================����ADC��Ӧ��GPIO=============================*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//PC1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//2MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//2MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
   
    /*=============================ADCʱ��ʹ��=============================*/

    RCC_AHB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);    // ADC1��λ
    RCC_AHB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);   // ��λ����
   
		ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
		ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
		
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
		ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
		ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
		ADC_Cmd(ADC1, ENABLE);//����ADת����	
}	

/**************************************************************************
�������ܣ���ʱ
��ڲ�������
����  ֵ����
**************************************************************************/
void Dly_us(void)
{
   int ii;    
   for(ii=0;ii<30;ii++); 
}
/**************************************************************************
�������ܣ�CCD���ݲɼ�
��ڲ�������
����  ֵ����
**************************************************************************/
 void RD_TSL(void) 
{
	u8 i=0,tslp=0;
	TSL_CLK=1;
	Dly_us();
	
	TSL_SI=0; 
	Dly_us();
	
	TSL_CLK=0;
	Dly_us();
	
	TSL_SI=0; 
	Dly_us();
      
	TSL_SI=1;
	Dly_us();	
	
	TSL_CLK=0;
	Dly_us();
	
	TSL_SI=1;
	Dly_us();	
	
	TSL_CLK=1;
	Dly_us();
	
      
	TSL_CLK=1;
	Dly_us(); 
	
	TSL_SI=0;
	Dly_us(); 
	for(i=0;i<128;i++)
	{ 
		TSL_CLK=0; 
		Dly_us();  //�����ع�ʱ��

		ADV[tslp]=(Get_Adc(4))>>4;
		++tslp;
		TSL_CLK=1;
		Dly_us();
	}  
}
/**************************************************************************
�������ܣ�����CCDȡ��ֵ
��ڲ�������
����  ֵ����
**************************************************************************/
void  Find_CCD_Median (void)
{ 
	 static u16 i,j,Left,Right,Last_CCD_Median;
	 static u16 value1_max,value1_min;
	 //��ֵ˵����CCD�ɼ�������128�����ݣ�ÿ�����ݵ�������ֵ���бȽϣ�����ֵ��Ϊ��ɫ������ֵСΪ��ɫ
	 //��̬��ֵ�㷨����ȡÿ�βɼ����ݵ�������Сֵ��ƽ������Ϊ��ֵ 
	 value1_max=ADV[0];  
   for(i=5;i<123;i++)   //���߸�ȥ��5����
     {
       if(value1_max<=ADV[i])
       value1_max=ADV[i];
     }
	  value1_min=ADV[0];  //��Сֵ
    for(i=5;i<123;i++) 
     {
       if(value1_min>=ADV[i])
       value1_min=ADV[i];
     }
   CCD_Threshold =(value1_max+value1_min)/2;	  //���������������ȡ����ֵ
		 
	 for(i = 5;i<118; i++)   //Ѱ����������أ��������������غ����������������ж����������
	 {
		 if(ADV[i]>CCD_Threshold &&ADV[i+1]>CCD_Threshold &&ADV[i+2]>CCD_Threshold &&ADV[i+3]<CCD_Threshold &&ADV[i+4]<CCD_Threshold &&ADV[i+5]<CCD_Threshold )
		 {	
			 Left=i+2;
			 break;	
		 }
	 }
	 for(j = 118;j>5; j--)//Ѱ���ұ������أ��������������غ����������������ж��ұ�������
   {
		if(ADV[j]<CCD_Threshold &&ADV[j+1]<CCD_Threshold &&ADV[j+2]<CCD_Threshold &&ADV[j+3]>CCD_Threshold &&ADV[j+4]>CCD_Threshold &&ADV[j+5]>CCD_Threshold )
		 {	
		   Right=j+2;
		   break;	
		 }
   }
	CCD_Median =(Right+Left)/2;//��������λ��
	if(myabs(CCD_Median-Last_CCD_Median) > 90)   //�������ߵ�ƫ����̫��
		CCD_Median=Last_CCD_Median;    //��ȡ��һ�ε�ֵ
	Last_CCD_Median=CCD_Median;  //������һ�ε�ƫ��
} 
