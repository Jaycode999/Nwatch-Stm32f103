#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h" 

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//STM32�ڲ�FLASH��д ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 




u8 WriteToBackupReg(u16 index,u16 *FirstBackupData,u8 writetonum);

void ReadToBackupReg(u16 index,u16 *FirstBackupData,u8 readtonum);




#endif

















