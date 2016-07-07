#include "stm32f10x.h"
#include "bsp_ov7725.h"
#include "bsp_ili9341_lcd.h"
#include "EasyTracer.h"
#include "bsp_led.h"  
#include "bsp_usart1.h"


extern uint8_t Ov7725_vsync;         
RESULT Resu;           
TARGET_CONDI Condition={50,80,20,250,20,200,40,40,320,240};

//==========================================================================
//
//==========================================================================
int main(void) 	
{
	LED_GPIO_Init();
	//USART1_Config();//115200
	LCD_Init();                       //Һ����ʼ�� 
	Ov7725_GPIO_Config();             //ov7725 gpio ��ʼ�� 
	while(Ov7725_Init() != SUCCESS);  //ov7725 �Ĵ������ó�ʼ�� 	
	VSYNC_Init();	                    //ov7725 ���ź��߳�ʼ�� 
	Ov7725_vsync = 0;
	
	while(1)
	{
		if( Ov7725_vsync == 2 )
		{
            Ov7725_vsync = 0;	
			if(Trace(&Condition,&Resu))
				{		
					//USART_printf(USART1, "X:%d  Y:%d  H:%d  W:%d\n", Resu.x, Resu.y, Resu.h, Resu.w);
					LCD_Clear(Resu.x-Resu.w/2,Resu.y-Resu.h/2,Resu.w,1,0xf800);
					LCD_Clear(Resu.x-Resu.w/2,Resu.y-Resu.h/2,1,Resu.h,0xf800);
					LCD_Clear(Resu.x-Resu.w/2,Resu.y+Resu.h/2,Resu.w,1,0xf800);
					LCD_Clear(Resu.x+Resu.w/2,Resu.y-Resu.h/2,1,Resu.h,0xf800);
          LCD_Clear(Resu.x-2,Resu.y-2,4,4,0xf800);
							if(Trace_1(&Condition,&Resu))
							{		
							//USART_printf(USART1, "X:%d  Y:%d  H:%d  W:%d\n", Resu.x, Resu.y, Resu.h, Resu.w);
							LCD_Clear(Resu.x-Resu.w/2,Resu.y-Resu.h/2,Resu.w,1,0x3333);
							LCD_Clear(Resu.x-Resu.w/2,Resu.y-Resu.h/2,1,Resu.h,0x3333);
							LCD_Clear(Resu.x-Resu.w/2,Resu.y+Resu.h/2,Resu.w,1,0x3333);
							LCD_Clear(Resu.x+Resu.w/2,Resu.y-Resu.h/2,1,Resu.h,0x3333);
							LCD_Clear(Resu.x-2,Resu.y-2,4,4,0x6666);
							}	
				}	
    }
	}
}


