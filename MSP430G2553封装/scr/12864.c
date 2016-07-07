#include<msp430g2553.h>

/*******************************************************
  PSB�˽ӵ� 
  RS��VCC
  SID�˽�2.4
  SCLK�˽�2.5  /��ӦGPIO ��GPIO.c�н��г�ʼ�� 
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
             ���ܣ� ����һ���ֽ� 	
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
      ���ܣ�����ָ��
*********************************************************/
void SendCMD_LCD(uchar dat)
{ 
	SendByte_LCD( 0xf8);     // 1 1 1 1 1 0 0 0  
	SendByte_LCD( dat & 0xf0 ); 
	SendByte_LCD((dat << 4 ) & 0xf0); 
}

/*********************************************************
      ���ܣ�д���ݸ�LCD
*********************************************************/
void SendDAT_LCD(uchar dat)
{ 
	SendByte_LCD( 0xfa);     // 1 1 1 1 1 0 1 0 
	SendByte_LCD( dat & 0xf0 ); 
	SendByte_LCD((dat << 4 ) & 0xf0); 
} 

/*********************************************************
����:��ʾ�ַ������������ַ�����ַ������������ָ���Ǻ��ֵ�����
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
	����
********************************/
void ClrScreen()
{
	SendCMD_LCD(0x01);
}

void LCD_Init()
{
		
	//Delayms(100);
	SendCMD_LCD(0x30);//����Ϊ���ջ���8�ֽ�
	SendCMD_LCD(0x02);
	SendCMD_LCD(0x06);//��ʾ�򿪣����أ����׹�
	SendCMD_LCD(0x0c);
	SendCMD_LCD(0x01); //��0
	SendCMD_LCD(0x80);
	//SendCMD_LCD(0x34);	// ������ָ��
	 //SendCMD_LCD(0x36);	// �򿪻�ͼָ��
	
}