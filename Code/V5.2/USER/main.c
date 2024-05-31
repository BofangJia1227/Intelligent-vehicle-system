
#include "system.h"

 //�������ȼ�
#define START_TASK_PRIO	1

 //�����ջ��С	
#define START_STK_SIZE 	256  

 //������
TaskHandle_t StartTask_Handler;

 //������
void start_task(void *pvParameters);


 //������
int main(void)
{ 
  systemInit();  //Ӳ����ʼ��
	
	//������ʼ����
	xTaskCreate((TaskFunction_t )start_task,     //������
							(const char*    )"start_task",               //��������
							(uint16_t       )START_STK_SIZE,         //�����ջ��С
							(void*          )NULL,                   //���ݸ��������Ĳ���
							(UBaseType_t    )START_TASK_PRIO,         //�������ȼ�
							(TaskHandle_t*  )&StartTask_Handler);      //������    					
	vTaskStartScheduler();  //�����������	
}
 
 //��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL(); //�����ٽ���
	//vTaskDelay(1000);
	//��������
	xTaskCreate(Balance_task,  "Balance_task",  BALANCE_STK_SIZE,  NULL, BALANCE_TASK_PRIO,  NULL);		//С���˶���������
	//xTaskCreate(MPU6050_task,  "MPU6050_task",  MPU6050_STK_SIZE,  NULL, MPU6050_TASK_PRIO,  NULL);		//IMU���ݶ�ȡ���� 
    xTaskCreate(show_task,     "show_task",     SHOW_STK_SIZE,     NULL, SHOW_TASK_PRIO,     NULL);  	//OLED��ʾ����ʾ����
    xTaskCreate(led_task,      "led_task",      LED_STK_SIZE,      NULL, LED_TASK_PRIO,      NULL);	 	//LED����˸����
	xTaskCreate(adc_task,      "ADC_task",      ADC_STK_SIZE,      NULL, ADC_TASK_PRIO,      NULL); 	//ADC�ɼ�����
	//xTaskCreate(data_task,     "DATA_task",     DATA_STK_SIZE,     NULL, DATA_TASK_PRIO,     NULL);	//Usartx3, Usartx1 and CAN send data task //����3������1��CAN������������
    vTaskDelete(StartTask_Handler);  //ɾ����ʼ����

    taskEXIT_CRITICAL();           //�˳��ٽ���
}






