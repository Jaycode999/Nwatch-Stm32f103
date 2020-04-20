#include "beep.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

// Buzzer
// Timer1 is used for buzzing

#include "common.h"
#include "delay.h"



static byte buzzLen;
static millis8_t startTime;
static buzzFinish_f onFinish;
static tonePrio_t prio;
static uint icr;
static uint ocr;


static void stop(void);



//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init_Init(u32 Freq)
{                                                
     


     TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
     NVIC_InitTypeDef NVIC_InitStructure;
 
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
 
     TIM_TimeBaseStructure.TIM_Period = 4000/Freq -1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ     ������5000Ϊ500ms
     TIM_TimeBaseStructure.TIM_Prescaler =71; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
     TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
     TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
     TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
 
     NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
     NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
     TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����



                                                    
}  



//��������ʼ��
//��ʼ��PF8Ϊ�����		    
//BEEP IO��ʼ��
void buzzer_init()
{
	TIM3_PWM_Init_Init(1000);
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ�� 
  //��ʼ����������Ӧ����GPIOA1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ͨ���ģʽ//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3                                                                    

  GPIO_ResetBits(GPIOA,GPIO_Pin_1);  //��������Ӧ����GPIOA1���ͣ� 


}

// Non-blocking buzz
void buzzer_buzz(byte len, tone_t tone, vol_t volType, tonePrio_t _prio, buzzFinish_f _onFinish)
{
	if(_prio < prio)
		return;
	else if(tone == TONE_STOP)
	{
		stop();
		return;
	}


	
	// Tell power manager that we are busy buzzing
//	pwrmgr_setState(PWR_ACTIVE_BUZZER, PWR_STATE_IDLE);

	prio = _prio;
	onFinish = _onFinish;
	buzzLen = len;
	startTime = millis();

	// Silent pause tone   //������ͣ��
	if(tone == TONE_PAUSE)
	{
//		CLEAR_BITS(TCCR1A, COM1A1, COM1A0);
//		power_timer1_disable();
  	TIM_Cmd(TIM3, DISABLE);  //ʹ��TIM3                                                                    
	  BEEP=0; //PB.5 ����� 

		return;
	}

	// Workout volume
	byte vol;

	switch(volType)
	{
		case VOL_UI:
			vol = appConfig.volUI;
			break;
		case VOL_ALARM:
			vol = appConfig.volAlarm;
			break;
		case VOL_HOUR:
			vol = appConfig.volHour;
			break;
		default:
			vol = 2;
			break;
	}

	// Pulse width goes down as freq goes up
	// This keeps power consumption the same for all frequencies, but volume goes down as freq goes up

	vol--;
//	if(vol > 2)     //		byte volumes;��ʼ�����ʺ�ֵ��255
//		return;

  icr = tone  ;
	ocr = icr * (256/vol);
	

  TIM3_PWM_Init_Init(icr);
	
//	TIFR1 = 0;
//	SET_BITS(TIMSK1, TOIE1);
//	TCNT1 = 0;
//	OCR1A = ocr;
//	ICR1 = icr;
	//SET_BITS(TCCR1A, COM1A1, COM1A0);
}
/*
#include "led.h"

void buzzer_buzzb(byte len, tone_t tone, vol_t volType)
{
	(void)(volType);

	led_flash(LED_GREEN, 50, 255);
	led_flash(LED_RED, 50, 255);

	power_timer1_enable();
	TCCR1A |= _BV(COM1A1)|_BV(COM1A0);

//	static uint vol = 0;
//	vol++;
//	if(vol > 790)
//		vol = 1;
//	OCR1A = vol;

//	if(vol > 3)
//		vol = 0;
//	if(vol == 0)
//		OCR1A = tone; // normal
//	else if(vol == 1)
//		OCR1A = (tone * 2) - 50; // quiet
//	else if(vol == 2)
//		OCR1A = (tone / 2); // loud
//	else if(vol == 3)
//		OCR1A = (tone / 4); // loader (acually quiter)

	OCR1A = (tone * 2) - 100;
	ICR1 = tone * 2;
	while(len--)
	{
		delay(1);
		led_update();
	}		
//	delay(20);
	TCCR1A &= ~(_BV(COM1A1)|_BV(COM1A0));
	power_timer1_disable();
}
*/
// Are we buzzing?
bool buzzer_buzzing()
{
	return buzzLen;
}

// See if its time to stop buzzing
void buzzer_update()
{
	
	if(buzzLen && (millis8_t)(millis() - startTime) >= buzzLen)
	{
		stop();

		if(onFinish != NULL)
			onFinish();
	}

			
	
	
}

static void stop()
{
//	CLEAR_BITS(TCCR1A, COM1A1, COM1A0);
//	power_timer1_disable();
//    TIM_SetCompare1(TIM3,0);
	 BEEP=0; //PA.1 ����� 
   TIM_Cmd(TIM3, DISABLE);  //ֹͣTIM3 
	
	buzzLen = 0;
	prio = PRIO_MIN;

//	pwrmgr_setState(PWR_ACTIVE_BUZZER, PWR_STATE_NONE);
}

// Sometimes the buzzer kind of 'pops' from a bad waveform output (the first HIGH pulse is too long)
// Here we wait until a timer overflow and then turn on the timer output
// It still sometimes pops, but much less so than turning on the timer output in buzzer_buzz()
 //��ʱ���жϺ������� //TIM3ͨ�ö�ʱ��
 void TIM3_IRQHandler(void)
{
		if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)//�ж��жϱ�־�Ƿ���
		{
			 TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);     //�ֶ�����жϱ�־λ

			static 	uint  time;
			time++;
		  if(time<icr)  PAout(1)=1;
			else PAout(1)=0;
			if(time>= ocr) time=0;
			
	 }
}





