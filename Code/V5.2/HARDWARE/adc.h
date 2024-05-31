#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
#include "system.h"
#define Battery_Ch    8 //Battery voltage, ADC channel 14 //电池电压，ADC通道14
#define Potentiometer 9  //Potentiometer, ADC channel 8 //电位器，ADC通道8

#define ADC_STK_SIZE   512 
#define ADC_TASK_PRIO  5

#define TSL_SI    PCout(4)   //SI  
#define TSL_CLK   PAout(5)   //CLK 

#define TSL_SI_HIGH								PAout(4) = 1
#define TSL_SI_LOW								PAout(4) = 0
#define TSL_CLK_HIGH							PAout(5) = 1
#define TSL_CLK_LOW								PAout(5) = 0

void Dly_us(void);
void RD_TSL(void); 
void Find_CCD_Median (void);
void ccd_Init(void);
void OPENMV_Init(void);
void Adc_Init(void);
void adc_task(void *pvParameters);
u16 Get_Adc(u8 ch);
float Get_battery_volt(void) ;
u16 Get_adc_Average(u8 chn, u8 times);
extern float Voltage,Voltage_Count,Voltage_All; 
extern u16 CCD_Median,CCD_Threshold, ADV[128];
extern u16 OPENMV_Median;
extern int Sensor_Left,Sensor_Middle,Sensor_Right,Sensor,sum;
#endif 


