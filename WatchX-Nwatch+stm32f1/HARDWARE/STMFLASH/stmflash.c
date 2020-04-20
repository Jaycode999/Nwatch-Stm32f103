#include "stmflash.h"
#include "delay.h"
#include "usart.h" 
#include "common.h"


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
//��ҳ�������ԣ�http://www.openedv.com/posts/list/45775.htm ��л�������ṩ������
////////////////////////////////////////////////////////////////////////////////// 

 
 



u16  BKPDataR[]={BKP_DR2,BKP_DR3,BKP_DR4,BKP_DR5,BKP_DR6,
BKP_DR7,BKP_DR8,BKP_DR9,BKP_DR10,BKP_DR11,BKP_DR12,BKP_DR13,BKP_DR14,
BKP_DR15,BKP_DR16,BKP_DR17,BKP_DR18,BKP_DR19,BKP_DR20,BKP_DR21,
 BKP_DR22,BKP_DR23,BKP_DR24/*,BKP_DR25,BKP_DR26,BKP_DR27,BKP_DR28,BKP_DR29,
BKP_DR30,BKP_DR31,BKP_DR32,BKP_DR33,BKP_DR34,BKP_DR35,BKP_DR36*/};

//_calendar_obj calendar;//ʱ�ӽṹ��
/*
���ܣ����β�����uint16_t Dataд��ĳĳ�ű��������ݼĴ���uint16_t BKP_DR��
ע�⣺�ڱ��������βΡ�uint16_t BKP_DR����uint16_t���ͣ���Ϊ������ֻ�ܶ�д���������ݣ�
ͬʱ�ִ����������ݼĴ����ļĴ����ţ�
С������CM3ֻ��20�ֽڵĴ洢�ռ䣬������84�ֽڣ���Ӧ��ַƫ�ƣ�0x04 �� 0x28��0x40��0xBC
��λֵ��0x0000 0000
void BKP_WriteBackupRegister(uint16_t BKP_DR, uint16_t Data)
{
 __IO uint32_t tmp = 0;//tmp��u32���ͣ�����ָ�룡ֻ�Ǵ���ָ���u32���ͱ�ţ�

 //  Check the parameters
  assert_param(IS_BKP_DR(BKP_DR));
//#define BKP_BASE  (APB1PERIPH_BASE + 0x6C00);#define APB1PERIPH_BASE  PERIPH_BASE
 //#define PERIPH_BASE  ((uint32_t)0x400000 00)
  tmp = (uint32_t)BKP_BASE; //BKP_BASE =(uint32_t)0x40000000+0x6C00;
  tmp += BKP_DR;

  *(__IO uint32_t *) tmp = Data;//��tmpǿ������ת��Ϊ��__IO uint32_t *�����ٸ�ֵ��
}
*/

u8 WriteToBackupReg(u16 index,u16 *FirstBackupData,u8 writetonum)
{
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //ʹ��PWR��BKP����ʱ��  
	RCC_RTCCLKCmd(ENABLE);
 PWR_BackupAccessCmd(ENABLE); //ʹ�ܺ󱸼Ĵ�������
 RTC_WaitForLastTask(); //�ȴ����һ�ζ�RTC�Ĵ�����д�������

  RTC_WaitForLastTask(); //�ȴ����һ�ζ�RTC�Ĵ�����д�������
  for(;index<writetonum;index++)
   {
   // BKPDataR[0]����BKP_DR2�Ĵ����������ݴ�BKP_DR2�Ĵ�����ʼд��  
     BKP_WriteBackupRegister(BKPDataR[index],FirstBackupData[index]); 
		 
//   delay_ms(50);
   debug_printf("д�������=��%c\n",FirstBackupData[index]); //��д������ݴ�ӡ����
   }
return 0;
}




void ReadToBackupReg(u16 index,u16 *FirstBackupData,u8 readtonum)
{
 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //ʹ��PWR��BKP����ʱ��  
  PWR_BackupAccessCmd(ENABLE); //ʹ�ܺ󱸼Ĵ������� 
  RTC_WaitForLastTask(); //�ȴ����һ�ζ�RTC�Ĵ�����д�������

	RCC_RTCCLKCmd(ENABLE);
 for(;index<readtonum;index++)
   {
   //  FirstBackupData[index]���Դ�Ŷ��������� ,��BKP_DR2�Ĵ�����ʼ��
     FirstBackupData[index]=BKP_ReadBackupRegister(BKPDataR[index]); 
 //   delay_ms(50);
   debug_printf("����RTC���������ݼĴ�����ֵBKP_DRx=��%c\n",FirstBackupData[index]);
   }

}


//������
//int main(void)
//{
// 

// u16 rb[20];//���Դ�Ŷ�����RTC���������ݼĴ�������
////u16  buf[]={0x12a,0x13a,0x14a,0x15a,0x16a,0x17a,0x18a,0x19a,0x45,0x34,
////0xaaaa,0xbbbb,0xcccc,0x12a,0x13a,0x14a,0x15a,0x16a,0x17a,0x18a,0x19a};
// u16 n, m=0;
//  u8  buf[]={"hello!i'm wangyan."};

//   
// uart_init(9600);
// delay_init();
// //  RTC_Init();
// 

//  WriteToBackupReg((u16*)buf,sizeof(buf));// д�뻺���ַ���
//    delay_ms(1000); 
//    
////    printf("д���������sizeof(buf)=��%d\n",sizeof(buf));
//  
//   delay_ms(1000);
//   ReadToBackupReg(rb,sizeof(buf));//  ��BKP_DR2�Ĵ�����ʼ��,һ������20������
//      delay_ms(1000);


// /* ��������ĳ���Ĵ�����ֵ������֤�Ƿ�д��ɹ�����ӡ��12a */
// //  m=BKP_ReadBackupRegister(BKP_DR14);
////    printf("����RTC���������ݼĴ�����ֵBKP_DR14=��%x\n",m);
//  //   n=BKP_ReadBackupRegister(BKP_DR1);
////    printf("����RTC���������ݼĴ�����ֵBKP_DR1=��%x\n",n);
// while (1)
// {   };

//}









