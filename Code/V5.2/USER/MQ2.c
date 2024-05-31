#include "MQ2.h"

#define CAL_PPM  10  // Calibration PPM value in the environment
#define RL       10  // RL resistance
#define R0       26  // R0 resistance
#define ADC_CHANNEL     ADC_Channel_12
float ppm;
void MQ2_Init(void)
{
    GPIO_InitTypeDef      GPIO_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef       ADC_InitStructure;
    
    /* Enable the clock for GPIOC and ADC1 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
 
    /* Configure the pin as analog mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;    // Analog mode
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /* ADC Common configuration */
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;        // Independent mode
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;     // 84 MHz/2 = 42 MHz
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;   // Disable DMA
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;   // ADC channel sampling interval
    ADC_CommonInit(&ADC_CommonInitStructure);
 
    /* ADC1 initialization */
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;        // Resolution
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                // Disable scanning
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;           // Continuous conversion mode
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // No external trigger needed
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;        // Data right-aligned
    ADC_InitStructure.ADC_NbrOfConversion = 1;                    // Single conversion
    ADC_Init(ADC2, &ADC_InitStructure);
    
    // Configure ADC1 to sample channel 12 on PC2
    ADC_RegularChannelConfig(ADC2, ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);
 
    /* Enable ADC1 */
    ADC_Cmd(ADC2, ENABLE);
}

u16 Get_Adcc(u8 ch)   
{
    /* Start software-triggered conversion */
    ADC_RegularChannelConfig(ADC2, ch, 1, ADC_SampleTime_3Cycles);

    ADC_SoftwareStartConv(ADC2); // ��ʼת��

    while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC)); // �ȴ�ת�����
    
    return ADC_GetConversionValue(ADC2);
}

u16 Get_Adc_Average(u8 ch, u8 times)
{
    u32 temp_val = 0;
    u8 t;

    for(t = 0; t < times; t++)
    {
        temp_val += Get_Adc(ch);
        delay_ms(5); // �򵥵���ʱ��ȷ��ת�������㹻��ʱ����
    }

    return (u16)(temp_val / times); // ����ƽ��ֵ������
}

float MeasureAirQuality(void)
{
    static u8 t = 0; // ʹ�þ�̬�����Ա��ּ���״̬
    u8 adcStr[25];   // �洢 ADC ֵ���ַ���
    u8 volStr[25];   // �洢��ѹֵ���ַ���
    u16 adcx;
    float temp;

    if (t % 10 == 0) // ÿ100ms��ȡһ��
    {
        adcx = Get_Adc(ADC_Channel_8); // ��ȡADCֵ
        temp = (float)adcx * 3.3 / 4096; // �����ѹ
		float RS = (3.3f - temp) / temp * RL;
		ppm = 98.322f * pow(RS/R0, -1.458f);

       
    }

    delay_ms(100); // ��ʱ100ms
    t++; // ���¼�����
	return ppm;
}



