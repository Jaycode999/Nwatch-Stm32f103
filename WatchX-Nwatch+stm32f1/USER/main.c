//���������� keil5  stm32f103c8t6  1.3��oled��Ļ
//
//Nwatch�Ѿ��ɹ���ֲ����Stm32����Ŀǰ�Ѿ���ɵ���ֲ����Ϊ��
//                                                     ��ֲ��stm32f103c8t6�ϡ����һ�����PCB����������һ�����̡�RTCʱ�ӡ�������ʾ���ϵ�flashģ��eeprom�������Դ�������ͼ�꣬Sleep˯��ģʽ���͹���ģʽ������ѹ���
//                                 ����ɣ�             �����BKP�洢 ���������� ��USB��� ��ֲ������ ���Ǽ��ٶȴ�����
//
//��ʵ�ֵ�Ч�������ӡ��ֵ�Ͳ������ʱ��3D������ʾ����Ϸ���������ƻ�С��
//             ����->���ڵ�������˯��ʱ�����ã�����ʾ���á��汾��Ϣ�鿴��FPS��ʾ
//����Ȥ��С�������Լ�����Ŷ���ɱ����Լ�����100		
//         
//Ŀǰ64k�ռ�ʹ�����Ϊ��Program Size: Code=36034 RO-data=10414 RW-data=432 ZI-data=4944     51824B
//��Ҫ�Ĳ˵���������Ķ������setting.c �ĵ���������  static void itemLoader(byte num)   ͬʱע�� #define OPTION_COUNT	5�м��OPTION_COUNT ����ʱ������������Ӽ���
//                          void setMenuOption_P(byte num, const char* name, const byte* icon, menu_f actionFunc)  �е�actionFunc  ���������Go to Definition  of "xxxxx" �Ϳ��Խ����Ӧ���ļ������޸ģ�Ҳ����ģ�½�������ļ�
//                        
//����һ�ֲ˵�ģʽ�����Խ�� timedate.c �� diag.c
//
//���ȱ�ݣ�
//1.���ڵ�ʱ�뽫��������������һ������Ԥ��usart1������ģ����,����û�����ch340оƬ����Ҫst-link����j-link���س���
//2.û��Ƶ�Դ������Ӳ����λ���������������Ҫôѡ�����ˢ�̼���Ҫô�ȴ���ع���������ȡ�µ�Դ�ߡ�
//3.��������ʱ����֤��Ƭ���������磬��ʹ�õ�st-link�����硣
//4.������Ƶ�ʱ��û�п�����ӵ�mpu6050��pw02�ĸ߶ȣ����Դ���ȱ�ݣ�
//5.ʱ��оƬ�ķ�װ����һ�ţ�����Ҳ�ܺ���ȥ��
//6.Ϊ�˿����ֹ����ӣ��󲿷�Ԫ������ʹ�õĶ���0805��װ��������Խ���һ��
//7.���Լ��޸ĳ����ʱ�򣬽���ע�͵��������䣺����Ȼ��ֻ���ð���������ʱ����д�̼�����Ϊͣ��ģʽ����ʱ��ֹͣ��������
//     	WWDG_Init(0x7F,0X5F,WWDG_Prescaler_8); 	//������ֵΪ7f,���ڼĴ���Ϊ5f,��Ƶ��Ϊ8	       
//      pwrmgr_update();     //����ͣ��ģʽ
//8.����������һ˿������Ӳ��ȱ�ݰ������İ��ӣ������
//9.usb�����޷����
//10.���������޷�ʵ��
//��ֲ�͸Ķ��ļ������ʱ����Ҫ��Ҫ��  common.h
//����Ԥ���벻����Ŀ����޸ģ�        config.h
//
//ʹ�õ���Ļ���о�԰���ӵ�1.3��oled �ǳ���������ʹ�õ������ҵ�ģ��spi 
//
//����ʵ�����ޣ�ϣ����λ��������ָ��������
//
//                                                   ���ߣ�YUMMY
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "oled.h"
#include "common.h"
#include "key.h"
#include "wwdg.h"

void c_setup()
{
  SystemInit();	
 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();             //��ʼ����ʱ����
#if COMPILE_UART
		uart_init(UART_BAUD);		//��ʼ�����ڲ�����Ϊ115200
#endif
  // Everything else
	memset(&oledBuffer, 0x00, FRAME_BUFFER_SIZE);
	OLED_Init();              //��ʼ��oled ����ģ��spiģʽ
	led_init();               //��ʼ��LED
  buzzer_init();	
  global_init();
	battery_init();
	appconfig_init();
	alarm_init();             //�����޷��������ӣ�ÿ�������Ժ���Ҫ�Զ���

  buttons_init();
  millis_init();
  time_init();
	rtc_init();
	
	// Set watchface
	display_set(watchface_normal);

	display_load();

	WWDG_Init(0x7F,0X5F,WWDG_Prescaler_8); 	//������ֵΪ7f,���ڼĴ���Ϊ5f,��Ƶ��Ϊ8	   

 //��ʼ����ʱ��������Ҫ�ı��״̬	
	OLED_ColorTurn(appConfig.invert );
}

void c_loop()
{
    time_update();
  if(pwrmgr_userActive())
  {
      battery_update();
      buttons_update();
  }
    
  	buzzer_update();
	
    led_update();
	
	
#if COMPILE_STOPWATCH
  stopwatch_update();
#endif

  if(pwrmgr_userActive())
  {
     alarm_update();
     display_update();
  }

 pwrmgr_update();     //����ͣ��ģʽ
 

}




int main(void)
{ 

	c_setup();	    //��ʼ��

  while(1)
		{

	    c_loop(); //ѭ��			
			if(KEY3 ==0 ){        //������İ����������¸�λ
	    __set_FAULTMASK(1);
     NVIC_SystemReset(); } 

	  }




}







