#include<msp430g2553.h>

/*******************************************************
  PSB端接地 
  RS接VCC
  SID端接2.4
  SCLK端接2.5  /对应GPIO 在GPIO.c中进行初始化 
*******************************************************/
#define uchar unsigned char
#define uint unsigned int

//#define CS_ON  P2OUT |= BIT3;
//#define CS_OFF P2OUT &= ~BIT3;
#define SID_ON  P2OUT |= BIT4;
#define SID_OFF  P2OUT &= ~BIT4;
#define SCLK_ON  P2OUT |= BIT5;
#define SCLK_OFF  P2OUT &= ~BIT5;

void Delayms(uint xms)
{
  uint i, j;
  for(j=xms;j>0;j--)
    for(i=110;i>0;i--);
}
/*

void GPIO_Init()
{
  P2DIR |= BIT3 + BIT4 + BIT5;
}
*/
/*********************************************************
             功能： 发送一个字节 	
*********************************************************/
void SendByte_LCD(uchar date)
{
  uchar i;
  SCLK_OFF;
  //CS_ON;
  for(i=0;i<8;i++)
  {
    if(date & 0x80)
    {
      SID_ON;
    }
    else
    {
      SID_OFF;
    }  
    SCLK_ON;
    Delayms(5);
    SCLK_OFF;
    date = date << 1;
  }
 // CS_OFF;
}

/*********************************************************
      功能：发送指令
*********************************************************/
void SendCMD_LCD(uchar dat)
{ 
	SendByte_LCD( 0xf8);     // 1 1 1 1 1 0 0 0  
	SendByte_LCD( dat & 0xf0 ); 
	SendByte_LCD((dat << 4 ) & 0xf0); 
}

/*********************************************************
      功能：写数据给LCD
*********************************************************/
void SendDAT_LCD(uchar dat)
{ 
	SendByte_LCD( 0xfa);     // 1 1 1 1 1 0 1 0 
	SendByte_LCD( dat & 0xf0 ); 
	SendByte_LCD((dat << 4 ) & 0xf0); 
} 

/*********************************************************
功能:显示字符串（行数，字符串地址，列数）列数指的是汉字的列数
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
		SendDAT_LCD(*(date++));
	}
} 

/*******************************
	清屏
********************************/
void ClrScreen()
{
	SendCMD_LCD(0x01);
}

void LCD_Init()
{
		
	//Delayms(100);
	SendCMD_LCD(0x30);//设置为接收或发送8字节
	SendCMD_LCD(0x02);
	SendCMD_LCD(0x06);//显示打开，光标关，反白光
	SendCMD_LCD(0x0c);
	SendCMD_LCD(0x01); //清0
	SendCMD_LCD(0x80);
	//SendCMD_LCD(0x34);	// 打开扩充指令
	 //SendCMD_LCD(0x36);	// 打开绘图指令
	
}