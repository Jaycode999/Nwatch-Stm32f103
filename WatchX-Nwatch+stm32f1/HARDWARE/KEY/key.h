#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


#define External_interrupt_Disable()   { EXTI->IMR &= ~(EXTI_Line0) ;  EXTI->IMR &= ~(EXTI_Line2) ;EXTI->IMR &= ~(EXTI_Line3) ; EXTI->IMR &= ~(EXTI_Line4) ;}
#define External_interrupt_Enable()     {EXTI->IMR |= EXTI_Line0 ;EXTI->IMR |= EXTI_Line2 ;EXTI->IMR |= EXTI_Line3 ;EXTI->IMR |= EXTI_Line4 ;}


#define KEY0  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//��ȡ���� ��
#define KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//��ȡ����  ��
#define KEY2  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//��ȡ���� ȷ��
#define KEY3   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����3(WK_UP) ��λ

 



void KEY_Init(void);//IO��ʼ��
#endif
