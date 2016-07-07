#include<reg52.h>
#include"12864.h"


#define uchar unsigned char
#define uint unsigned int
sbit SID = P2^1;
sbit SCLK = P2^0;

void Delay_ms(uint xms)
{
	uint i, j;
	for(j=xms;j>0;j--)
		for(i=110;i>0;i--);
}
/*********************************************************
函数名:Send_Byte()
返回值：无
功  能:	写数据到LCD
*********************************************************/
void SendByte_LCD(uchar date)
{
	uchar i;
	SCLK = 0;
	//CS = 1;
	for(i=0;i<8;i++)
	{
		SID = (bit)(date & 0x80);
		SCLK = 1;
		Delay_ms(5);
		SCLK = 0;
		date = date << 1;
	}
	//CS = 0;
}
/*********************************************************
函数名: SendCMD_LCD(uchar dat)
返回值：无
功  能:	写命令到LCD
*********************************************************/
void SendCMD_LCD(uchar dat)
{ 
	SendByte_LCD( 0xf8);     // 1 1 1 1 1 0 0 0  
	SendByte_LCD( dat & 0xf0 ); 
	SendByte_LCD((dat << 4 ) & 0xf0); 
}
/*********************************************************
函数名: SendDAT_LCD()
返回值：无
功  能:	写显示内容到LCD
*********************************************************/
void SendDAT_LCD(uchar dat)
{ 
	SendByte_LCD( 0xfa);     // 1 1 1 1 1 0 1 0 
	SendByte_LCD( dat & 0xf0 ); 
	SendByte_LCD((dat << 4 ) & 0xf0); 
} 
/*********************************************************
函数名:Display_LCD_String()
返回值：无
功  能:显示字符串
*********************************************************/
void DisplayC_LCD( uchar x, uchar * date,  uchar y)   
{ 
	uchar addr; 
	switch(x)
	{
		  case 0: addr=0x80;break;
			case 1: addr=0x90;break;
			case 2: addr=0x88;break;
			case 3: addr=0x98;break;
	}
	SendCMD_LCD( addr + y ); 
	while(*date != '\0')
	{
		SendDAT_LCD(*date++);
	}
} 
/*******************************
							清屏
********************************/
void ClrScreen()
{
	SendCMD_LCD(0x01);
}

/*********************************************************
函数名: LCD_Clear_Txt
返回值：无
功  能:	文本区清除
*********************************************************/

void LCD_Clear_TXT( void )
{
     uchar i;
     SendCMD_LCD(0x30);      //8BitMCU,基本指令集合
     SendCMD_LCD(0x80);      //AC归起始位
     for(i=0;i<64;i++)
	  {
       SendCMD_LCD(0x20);
	  }
}

/******************************
初始化 LCD函数
********************************/

void LCD_Init()
{
	//PSB = 0;	//串并选择
	Delay_ms(100);
	SendCMD_LCD(0x30);//发送 或接受 数据为8BIT 
	SendCMD_LCD(0x02);
	SendCMD_LCD(0x06);//显示打开，光标开，反白关
	SendCMD_LCD(0x0c);
	SendCMD_LCD(0x01); //清0
	SendCMD_LCD(0x80);
	//SendCMD_LCD(0x34);	// 扩充指令
	 //SendCMD_LCD(0x36);	// 打开绘图指令
	
}

