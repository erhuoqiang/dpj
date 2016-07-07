#include "stm32f10x_it.h"
#include "bsp_ov7725.h"
#include "bsp_led.h" 
#include "EasyTracer.h"
#include "bsp_ili9341_lcd.h"


extern u8 Ov7725_vsync;

//===================================================================
//ov7725 场中断 服务程序 
//===================================================================
void EXTI0_IRQHandler(void)
{
    if ( EXTI_GetITStatus(EXTI_Line0) != RESET ) 	//检查EXTI_Line0线路上的中断请求是否发送到了NVIC 
    {
			if( Ov7725_vsync == 0 )
			{
					FIFO_WRST_L(); 	                      //拉低使FIFO写(数据from摄像头)指针复位
					FIFO_WE_H();	                        //拉高使FIFO写允许
					
					Ov7725_vsync = 1;	   	
					FIFO_WE_H();                          //使FIFO写允许
					FIFO_WRST_H();                        //允许使FIFO写(数据from摄像头)指针运动
			}
			else if( Ov7725_vsync == 1 )
			{
					FIFO_WE_L();                          //拉低使FIFO写暂停
					Ov7725_vsync = 2;
					FIFO_PREPARE;                         //FIFO准备		
					ImagDisp();					                  //采集并显示   
				  LED1_TOGGLE
			}        
			EXTI_ClearITPendingBit(EXTI_Line0);		    //清除EXTI_Line0线路挂起标志位        
    }    
}


void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{ 	
		
	}  
}



