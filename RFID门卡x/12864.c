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
������:Send_Byte()
����ֵ����
��  ��:	д���ݵ�LCD
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
������: SendCMD_LCD(uchar dat)
����ֵ����
��  ��:	д���LCD
*********************************************************/
void SendCMD_LCD(uchar dat)
{ 
	SendByte_LCD( 0xf8);     // 1 1 1 1 1 0 0 0  
	SendByte_LCD( dat & 0xf0 ); 
	SendByte_LCD((dat << 4 ) & 0xf0); 
}
/*********************************************************
������: SendDAT_LCD()
����ֵ����
��  ��:	д��ʾ���ݵ�LCD
*********************************************************/
void SendDAT_LCD(uchar dat)
{ 
	SendByte_LCD( 0xfa);     // 1 1 1 1 1 0 1 0 
	SendByte_LCD( dat & 0xf0 ); 
	SendByte_LCD((dat << 4 ) & 0xf0); 
} 
/*********************************************************
������:Display_LCD_String()
����ֵ����
��  ��:��ʾ�ַ���
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
							����
********************************/
void ClrScreen()
{
	SendCMD_LCD(0x01);
}

/*********************************************************
������: LCD_Clear_Txt
����ֵ����
��  ��:	�ı������
*********************************************************/

void LCD_Clear_TXT( void )
{
     uchar i;
     SendCMD_LCD(0x30);      //8BitMCU,����ָ���
     SendCMD_LCD(0x80);      //AC����ʼλ
     for(i=0;i<64;i++)
	  {
       SendCMD_LCD(0x20);
	  }
}

/******************************
��ʼ�� LCD����
********************************/

void LCD_Init()
{
	//PSB = 0;	//����ѡ��
	Delay_ms(100);
	SendCMD_LCD(0x30);//���� ����� ����Ϊ8BIT 
	SendCMD_LCD(0x02);
	SendCMD_LCD(0x06);//��ʾ�򿪣���꿪�����׹�
	SendCMD_LCD(0x0c);
	SendCMD_LCD(0x01); //��0
	SendCMD_LCD(0x80);
	//SendCMD_LCD(0x34);	// ����ָ��
	 //SendCMD_LCD(0x36);	// �򿪻�ͼָ��
	
}

