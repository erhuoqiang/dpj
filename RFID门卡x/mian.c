#include<reg52.h>
#include"mian.h"
#include"rc522.h"

#define uchar unsigned char
#define uint unsigned int

volatile unsigned char sending;
sbit s2=P3^4;

void Delay_ms(uint xms)
{
	uint i, j;
	for(j=xms;j>0;j--)
		for(i=110;i>0;i--);
}
void Init_Usart1()
{
	 	TMOD=0X20;
		TH1=0Xfd;
		TL1=0XFD;	   
		TR1=1;
		SM0=0;
		SM1=1;
		REN=1;
		ES=1;
		EA=1;
}
void send(unsigned char d)		  //发送一个字节的数据，形参d即为待发送数据。
{
 
 SBUF=d; //将数据写入到串口缓冲
 sending=1;	 //设置发送标志
 while(sending); //等待发送完毕
}

void Sends(unsigned char * pd)
{
 while((*pd)!='\0') //发送字符串，直到遇到0才结束
 {
  send(*pd); //发送一个字符
  pd++;  //移动到下一个字符
 }
}
//卡号为0扇区0块内容的前四字节为卡号，且此块内容不能改写
uchar  status;
uchar g_ucTempbuf[20];
char ID[10] = "xx";
unsigned char hextoasc(unsigned char i, j)
{
	if(i<0x09)
	{
		i=i+'0';
		ID[j] = i;
		return i;
	}
	else
	{
		
		i=i+0x37;
		ID[j] = i;
		return i;
	}
}
void main()
{
    uint i;
	  uchar flagren,flagzhenque;
		PcdReset();//rc522初始化
		PcdAntennaOff(); //关闭和打开天线
		PcdAntennaOn();
		M500PcdConfigISOType('A');//设定工作模式
	  Init_Usart1();
	//防冲突，这里需要一个循环，让读卡器不断去读卡
		while(1)
		{
				status = PcdRequest(PICC_REQALL, g_ucTempbuf);
					//PICC_REQALL他是个宏定义 意思是寻找天线内的所有卡片，
					// g_ucTempbuf 是个数组，在这里函数读取卡内的前两位放在数组中
					if(status != MI_OK)//没有找到卡，继续执行PcdRequest()
				  {
							continue;
				  } 
		      status = PcdAnticoll(g_ucTempbuf);//防冲突
		      //卡片序列号，4字节,这里的status可以判PcdAnticoll的执行情况
	        //如果执行成功，表示g_ucTempbuf上面已经记在唯一的卡号了
		    //在这里g_ucTempbuf已经用了2+4个
					if(status != MI_OK)//没有找到卡，继续执行PcdRequest()
				 {
						continue;
				 }
				if(status == MI_OK)
				{
						LED_GREEN =0;
					//DisplayC_LCD(0, (char *)g_ucTempbuf, 1); 
					for(i=0;i<2;i++)//在12864上显示卡号
					{
						 hextoasc((g_ucTempbuf[i]&0xf0)>>4,i);
					}
					ID[2] = '.';
					ID[3] = '\0';
					Sends(ID);
					P2 &= ~(0x08);
					Delay_ms(300);
					P2 |= 0x08;
   	    }
			}
		}
void DelayMs(unsigned int _MS)
{
    TH1 = (unsigned char)(RCAP2_1ms>>8);
    TL1 = (unsigned char)(RCAP2_1ms);

    ET1     = 0; 	                                   // Disable timer2 interrupt
    TR1     = 1;
    while (_MS--)
    {  
        while (!TF1);
		    TF1 = 0;
        TH1 = (unsigned char)(RCAP2_1ms>>8);
        TL1 = (unsigned char)(RCAP2_1ms);
	 }
    TR1 = 0;
}

void uart(void) interrupt 4		 //串口发送中断
{
 if(RI)    //收到数据
 {
  RI=0;   //清中断请求
 }
 else      //发送完一字节数据
 {
  TI=0;
  sending=0;  //清正在发送标志
 }
}
