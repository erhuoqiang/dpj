#include "bsp_led.h"   
//====================================
//LED_GPIO端口配置
//====================================
static void LED_GPIO_Config(void)
{				
	GPIO_InitTypeDef     GPIO_InitStructure;              //定义一个GPIO_InitTypeDef类型的结构
																												//开启GPIOB和GPIOF的外设时钟  
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE); 
																												//硬件接法:led1-PB1、led2-PF7、led3-PF8
																												
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;     //设置引脚模式为通用推挽输出	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //设置引脚速率为50MHz   
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;	          //选择要控制的GPIOB引脚			  
	GPIO_Init(GPIOB, &GPIO_InitStructure);	              //调用库函数，初始化GPIOB0
																		 
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7|GPIO_Pin_8;//选择要控制的GPIOF引脚		
	GPIO_Init(GPIOF, &GPIO_InitStructure);                //操作不同的IO口  所用的库函数也不同 		
					
	GPIO_SetBits(GPIOB, GPIO_Pin_0);                      
	GPIO_SetBits(GPIOF, GPIO_Pin_7|GPIO_Pin_8);	          //关闭所有led
    
}
//====================================
//LED_GPIO端口初始化
//====================================
void LED_GPIO_Init(void)
{
  LED_GPIO_Config();
}
