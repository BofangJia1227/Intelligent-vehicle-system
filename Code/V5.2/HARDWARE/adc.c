#include "adc.h"

float Voltage,Voltage_Count,Voltage_All; //Variables related to battery voltage sampling //电池电压采样相关的变量  
const float Revise=1.03;

//CCD相关变量
u16 CCD_Median,CCD_Threshold,ADV[128]={0}; 

//电磁巡线相关变量																										
int Sensor_Left,Sensor_Middle,Sensor_Right,Sensor,sum; 
/**************************************************************************
函数功能：ADC初始化电池电压检测
入口参数：无
返回  值：无
**************************************************************************/
void  Adc_Init(void)
{  
		GPIO_InitTypeDef       GPIO_InitStructure;
		ADC_CommonInitTypeDef ADC_CommonInitStructure;
		ADC_InitTypeDef       ADC_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOA时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟


		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PB0 通道8
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化  
	
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//PB1 通道9
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化


		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 

		ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
		ADC_CommonInit(&ADC_CommonInitStructure);//初始化

		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
		ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
		ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
		ADC_Cmd(ADC1, ENABLE);//开启AD转换器	 
}		
/**************************************************************************
函数功能：AD采样
入口参数：ADC的通道
返回  值：AD转换结果
**************************************************************************/
u16 Get_Adc(u8 ch)   
{
	//Sets the specified ADC rule group channel, one sequence, and sampling time
	//设置指定ADC的规则组通道，一个序列，采样时间
	
	//ADC1,ADC通道,采样时间为480周期
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );
  //Enable the specified ADC1 software transformation startup function	
  //使能指定的ADC1的软件转换启动功能	
	ADC_SoftwareStartConv(ADC1);
	//Wait for the conversion to finish
  //等待转换结束	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	//Returns the result of the last ADC1 rule group conversion
	//返回最近一次ADC1规则组的转换结果
	return ADC_GetConversionValue(ADC1);	
}

/**************************************************************************
函数功能：采集多次ADC值求平均值函数
入口参数：ADC通道和采集次数
返 回 值：AD转换结果
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
函数功能：读取电池电压 
入口参数：无
返回  值：电池电压，单位mv
**************************************************************************/
float Get_battery_volt(void)   
{  
	float Volt;
	
	//The resistance partial voltage can be obtained by simple analysis according to the schematic diagram
	//电阻分压，具体根据原理图简单分析可以得到	
	Volt=Get_Adc(Battery_Ch)*3.3*11.0*Revise/1.0/4096;	
	return Volt;
}
/**************************************************************************
函数功能：adc数据采集处理 
入口参数：无
返回  值：无
**************************************************************************/
void adc_task(void *pvParameters)									
{
	 u32 lastWakeTime = getSysTickCnt();
	
   while(1)
    {	
			//The task is run at 50hz
			//此任务以50Hz的频率运行
			vTaskDelayUntil(&lastWakeTime, F2T(RATE_50_HZ));

			if(Mode  == CCD_Line_Patrol_Mode)
			{
			  RD_TSL();  																//数据采集处理
			  Find_CCD_Median ();			          		 //===提取中线 
			}

//			else if(Mode  == ELE_Line_Patrol_Mode)
//			{
//				Sensor_Left=Get_Adc(14);
//			  Sensor_Right=Get_Adc(4);
//			  Sensor_Middle=Get_Adc(5);
//					
//			  sum=Sensor_Left*1+Sensor_Middle*100+Sensor_Right*199;		//归一化处理
//			  Sensor=sum/(Sensor_Left+Sensor_Middle+Sensor_Right);		//提取中线
//			}
		}
}
/**************************************************************************
函数功能：线性CCD初始化
入口参数：无
返回  值：无
**************************************************************************/
void  ccd_Init(void)
{    
 //先初始化ADC1通道7 IO口
	   GPIO_InitTypeDef        GPIO_InitStructure;
    ADC_InitTypeDef         ADC_InitStructure;
    ADC_CommonInitTypeDef   ADC_CommonInitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOC时钟
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);    // 使能ADC1时钟
   
    /*=============================配置ADC对应的GPIO=============================*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//PC1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//2MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//2MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
   
    /*=============================ADC时钟使能=============================*/

    RCC_AHB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);    // ADC1复位
    RCC_AHB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);   // 复位结束
   
		ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
		ADC_CommonInit(&ADC_CommonInitStructure);//初始化
		
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
		ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
		ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
		ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
}	

/**************************************************************************
函数功能：OPENMV初始化
入口参数：无
返回  值：无
**************************************************************************/
void  OPENMV_Init(void)
{    
 //先初始化ADC1通道7 IO口
	   GPIO_InitTypeDef        GPIO_InitStructure;
    ADC_InitTypeDef         ADC_InitStructure;
    ADC_CommonInitTypeDef   ADC_CommonInitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOC时钟
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);    // 使能ADC1时钟
   
    /*=============================配置ADC对应的GPIO=============================*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//PC1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//2MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//2MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
   
    /*=============================ADC时钟使能=============================*/

    RCC_AHB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);    // ADC1复位
    RCC_AHB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);   // 复位结束
   
		ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
		ADC_CommonInit(&ADC_CommonInitStructure);//初始化
		
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
		ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
		ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
		ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
}	

/**************************************************************************
函数功能：延时
入口参数：无
返回  值：无
**************************************************************************/
void Dly_us(void)
{
   int ii;    
   for(ii=0;ii<30;ii++); 
}
/**************************************************************************
函数功能：CCD数据采集
入口参数：无
返回  值：无
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
		Dly_us();  //调节曝光时间

		ADV[tslp]=(Get_Adc(4))>>4;
		++tslp;
		TSL_CLK=1;
		Dly_us();
	}  
}
/**************************************************************************
函数功能：线性CCD取中值
入口参数：无
返回  值：无
**************************************************************************/
void  Find_CCD_Median (void)
{ 
	 static u16 i,j,Left,Right,Last_CCD_Median;
	 static u16 value1_max,value1_min;
	 //阈值说明：CCD采集回来的128个数据，每个数据单独与阈值进行比较，比阈值大为白色，比阈值小为黑色
	 //动态阈值算法，读取每次采集数据的最大和最小值的平均数作为阈值 
	 value1_max=ADV[0];  
   for(i=5;i<123;i++)   //两边各去掉5个点
     {
       if(value1_max<=ADV[i])
       value1_max=ADV[i];
     }
	  value1_min=ADV[0];  //最小值
    for(i=5;i<123;i++) 
     {
       if(value1_min>=ADV[i])
       value1_min=ADV[i];
     }
   CCD_Threshold =(value1_max+value1_min)/2;	  //计算出本次中线提取的阈值
		 
	 for(i = 5;i<118; i++)   //寻找左边跳变沿，连续三个白像素后连续三个黑像素判断左边跳变沿
	 {
		 if(ADV[i]>CCD_Threshold &&ADV[i+1]>CCD_Threshold &&ADV[i+2]>CCD_Threshold &&ADV[i+3]<CCD_Threshold &&ADV[i+4]<CCD_Threshold &&ADV[i+5]<CCD_Threshold )
		 {	
			 Left=i+2;
			 break;	
		 }
	 }
	 for(j = 118;j>5; j--)//寻找右边跳变沿，连续三个黑像素后连续三个白像素判断右边跳变沿
   {
		if(ADV[j]<CCD_Threshold &&ADV[j+1]<CCD_Threshold &&ADV[j+2]<CCD_Threshold &&ADV[j+3]>CCD_Threshold &&ADV[j+4]>CCD_Threshold &&ADV[j+5]>CCD_Threshold )
		 {	
		   Right=j+2;
		   break;	
		 }
   }
	CCD_Median =(Right+Left)/2;//计算中线位置
	if(myabs(CCD_Median-Last_CCD_Median) > 90)   //计算中线的偏差，如果太大
		CCD_Median=Last_CCD_Median;    //则取上一次的值
	Last_CCD_Median=CCD_Median;  //保存上一次的偏差
} 
