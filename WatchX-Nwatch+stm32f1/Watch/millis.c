/*
 * Project: Lightweight millisecond tracking library
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/millisecond-tracking-library-for-avr/
 */

#ifndef ARDUINO


#include "common.h"
#include "sys.h"
#include "led.h"
//#include "misc.h"

 millis_t milliseconds;


//static inline bool millis_interrupt_off(void)
//{
//	TIM_Cmd(TIM2,DISABLE);//����ʱ��
//	return true;
//}

//static inline bool millis_interrupt_on(void)
//{
//	TIM_Cmd(TIM2,ENABLE);//����ʱ��
//	return false;
//}

////// NEED TO DO A RESTORE THING! if paused then millis() is called it will unpause
//#define MILLIS_ATOMIC() for(bool cs = millis_interrupt_off(); cs; cs = millis_interrupt_on())


// Initialise library
void millis_init()
{
	// Timer settings
	  //1������Ƕ���жϿ�����NVIC
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//���ȼ�����
     NVIC_InitTypeDef nvic;
     nvic.NVIC_IRQChannel=TIM2_IRQn;
     nvic.NVIC_IRQChannelCmd=ENABLE;
     nvic.NVIC_IRQChannelPreemptionPriority=0;
     nvic.NVIC_IRQChannelSubPriority=0;
     NVIC_Init(&nvic);

	 //2����ʱ����ʼ������
     TIM_TimeBaseInitTypeDef tim;//�ṹ��
     //���ȼ���������
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//����ʱ��
     TIM_DeInit(TIM2);//����������
     tim.TIM_ClockDivision=TIM_CKD_DIV1;//������Ƶ
     tim.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
     tim.TIM_Period=1000;//�Զ���װ�ؼĴ�����ֵ
     tim.TIM_Prescaler=35;//ʱ��Ԥ��Ƶ
     //tim.TIM_RepetitionCounter=
     TIM_TimeBaseInit(TIM2,&tim);//��ʼ���ṹ��
     TIM_ClearFlag(TIM2,TIM_FLAG_Update);//�������жϱ�־
     TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
     TIM_Cmd(TIM2,ENABLE);//����ʱ��
     
	
}

// Get current milliseconds
millis_t millis_get()
{
	millis_t ms;
		ms = milliseconds;
	return ms;
}




 //��ʱ���жϺ������� //TIM2ͨ�ö�ʱ��
 void TIM2_IRQHandler(void)
{
		if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)//�ж��жϱ�־�Ƿ���
		{
				milliseconds++;		
				update = true;
			
			
			if(milliseconds%3600==0)
			{
				#ifndef RTC_SRC
				++timeDate.time.secs;   //ÿ���ӱ�־λ+1;
				#endif
//				 LED1=!LED1;	
			}
		
			
	 }
	 TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);     //�ֶ�����жϱ�־λ
}










#endif
