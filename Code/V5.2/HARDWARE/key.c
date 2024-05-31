#include "key.h"

/**************************************************************************
函数功能：按键初始化
入口参数：无
返回  值：无 
**************************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOB时钟
  GPIO_InitStructure.GPIO_Pin = KEY_PIN; //KEY对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE0	
} 
/**************************************************************************
函数功能：按键扫描
入口参数：无
返回  值：按键状态 0：无动作 1：单击 
**************************************************************************/
u8 click(void)
{
	//Press the release sign
	//按键按松开标志
	static u8 flag_key=1;
	
	if(flag_key&&KEY==0)
	{
	 flag_key=0; //The key is pressed //按键按下
	 return 1;	
	}
	else if(1==KEY)			
		flag_key=1;
	return 0; //No key is pressed //无按键按下
}


uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*等待按键释放 */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}

/**************************************************************************
函数功能：延迟函数
入口参数：无
返 回 值：无
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
函数功能：按键扫描
入口参数：双击等待时间
返回  值：按键状态: 0-无动作, 1-单击, 2-双击 
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
					return 2; //Double click //双击
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
			return 1; //Click //单击
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
函数功能：按键扫描。因为使用到了静态变量，当多处需要使用按键扫描函数时，需要再定义一个不同名函数
入口参数：无
返 回 值：按键状态: 0-无动作, 1-单击, 2-双击 
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
					return 2; //Double click //双击
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
			return 1; //Click //单击
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
函数功能：长按检测
入口参数：无
返回  值：按键状态 0：无动作 1：长按3s
**************************************************************************/
u8 Long_Press(void)
{
	static u16 Long_Press_count,Long_Press;

	if(Long_Press==0&&KEY==0)  Long_Press_count++; 
	else                       Long_Press_count=0;

	if(Long_Press_count>300)	//3 seconds //3秒	
	{
		Long_Press=1;	
		Long_Press_count=0;
		return 1;
	}				
	 if(Long_Press==1) //Long press position 1 //长按标志位置1
	{
			Long_Press=0;
	}
	return 0;
}

/**************************************************************************
函数功能：选择运行的模式
入口参数：无
返回  值：无
**************************************************************************/
//u8  select(void)
//{
//	  int Flag_Next=0;
//	  static u8 flag=1;
//	
//	  oled_show_once();  //OLED显示
//	  if(click_N_Double(150) == 1)
//		{
//			Move+=1;
//			if(Move>6)
//				Move=0;
//		}
//	  if(Move==0)			 ELE_ON_Flag=0,CCD_ON_Flag=1,APP_ON_Flag=0,Along_wall=0,Follow_ON_Flag=0,Avoid_ON_Flag=0;  //CCD遥控模式 
//		else if(Move==1) ELE_ON_Flag=1,CCD_ON_Flag=0,APP_ON_Flag=0,Along_wall=0,Follow_ON_Flag=0,Avoid_ON_Flag=0;  //ELE遥控模式
//		else if(Move==2) ELE_ON_Flag=0,CCD_ON_Flag=0,APP_ON_Flag=1,Along_wall=0,Follow_ON_Flag=0,Avoid_ON_Flag=0;	//oth模式
//	  else if(Move==3) ELE_ON_Flag=0,CCD_ON_Flag=0,APP_ON_Flag=0,Along_wall=1,Follow_ON_Flag=0,Avoid_ON_Flag=0;  //走直线模式
//	  else if(Move==4) ELE_ON_Flag=0,CCD_ON_Flag=0,APP_ON_Flag=0,Along_wall=0,Follow_ON_Flag=1,Avoid_ON_Flag=0;  //跟随模式
//	  else if(Move==5) ELE_ON_Flag=0,CCD_ON_Flag=0,APP_ON_Flag=0,Along_wall=0,Follow_ON_Flag=0,Avoid_ON_Flag=1;  //避障模式
//    if(Long_Press())Flag_Next=1;//长按按键3秒进入运行模式
//	  if(Flag_Next==1)OLED_Clear(),flag=0;  //清除OLED屏幕 程序往下运行
//	  return flag;	  
//}
