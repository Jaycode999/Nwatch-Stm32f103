#include "led.h"
#include "millis.h"
#include "common.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	  


// LED control

typedef struct {
  byte flashLen;			// How long to light up for
  millis8_t startTime;	//
} led_s;

static led_s ledRed;
static led_s ledGreen;


//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void led_init()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;				 //LED0-->PB.12 13 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 �����

}

static void flash(led_s* led, byte len,byte pin)
{
  led->flashLen = len;
  led->startTime = millis();

}

void led_flash(led_t led, byte len, byte brightness)
{
  if (appConfig.CTRL_LEDs)        //��������0 ���ǻ��������������ۺ����й�
	{

    switch (led)
    {
      case LED_RED:
			flash(&ledRed, len, 12);
        break;
      case LED_GREEN:
			flash(&ledGreen, len, 13);
        break;
      default:
        break;
    }

 //   pwrmgr_setState(PWR_ACTIVE_LED, PWR_STATE_IDLE);
  }
}

// Is an LED on?
BOOL led_flashing()
{
  return ledRed.flashLen || ledGreen.flashLen;
}

static BOOL _update(led_s* led,  byte pin)        //(volatile unsigned long  *)
{
  if ((millis8_t)(millis() - led->startTime) >= led->flashLen)
//	  if (led->flashLen)
  {
		PBout(pin)=0;
		led->flashLen =0;
  }
	else
		
		 PBout(pin)=1;

  return led->flashLen;
}

void led_update()
{
//  BOOL red = _update(&ledRed, &RED_CCR, _BV(RED_COM), &RED_PORT, _BV(RED_PIN));
//  BOOL green = _update(&ledGreen, &GREEN_CCR, _BV(GREEN_COM), &GREEN_PORT, _BV(GREEN_PIN));
BOOL red = _update(&ledRed,12);          //LED0 PBout(12)
BOOL green = _update(&ledGreen, 13);    //LED1 PBout(13)

	
}

// Turn off LEDs
void led_stop()
{
  led_flash(LED_GREEN, 0, 0);
  led_flash(LED_RED, 0, 0);
  led_update();
}



