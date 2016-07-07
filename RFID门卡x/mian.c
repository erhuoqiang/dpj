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
void send(unsigned char d)		  //����һ���ֽڵ����ݣ��β�d��Ϊ���������ݡ�
{
 
 SBUF=d; //������д�뵽���ڻ���
 sending=1;	 //���÷��ͱ�־
 while(sending); //�ȴ��������
}

void Sends(unsigned char * pd)
{
 while((*pd)!='\0') //�����ַ�����ֱ������0�Ž���
 {
  send(*pd); //����һ���ַ�
  pd++;  //�ƶ�����һ���ַ�
 }
}
//����Ϊ0����0�����ݵ�ǰ���ֽ�Ϊ���ţ��Ҵ˿����ݲ��ܸ�д
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
		PcdReset();//rc522��ʼ��
		PcdAntennaOff(); //�رպʹ�����
		PcdAntennaOn();
		M500PcdConfigISOType('A');//�趨����ģʽ
	  Init_Usart1();
	//����ͻ��������Ҫһ��ѭ�����ö���������ȥ����
		while(1)
		{
				status = PcdRequest(PICC_REQALL, g_ucTempbuf);
					//PICC_REQALL���Ǹ��궨�� ��˼��Ѱ�������ڵ����п�Ƭ��
					// g_ucTempbuf �Ǹ����飬�����ﺯ����ȡ���ڵ�ǰ��λ����������
					if(status != MI_OK)//û���ҵ���������ִ��PcdRequest()
				  {
							continue;
				  } 
		      status = PcdAnticoll(g_ucTempbuf);//����ͻ
		      //��Ƭ���кţ�4�ֽ�,�����status������PcdAnticoll��ִ�����
	        //���ִ�гɹ�����ʾg_ucTempbuf�����Ѿ�����Ψһ�Ŀ�����
		    //������g_ucTempbuf�Ѿ�����2+4��
					if(status != MI_OK)//û���ҵ���������ִ��PcdRequest()
				 {
						continue;
				 }
				if(status == MI_OK)
				{
						LED_GREEN =0;
					//DisplayC_LCD(0, (char *)g_ucTempbuf, 1); 
					for(i=0;i<2;i++)//��12864����ʾ����
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

void uart(void) interrupt 4		 //���ڷ����ж�
{
 if(RI)    //�յ�����
 {
  RI=0;   //���ж�����
 }
 else      //������һ�ֽ�����
 {
  TI=0;
  sending=0;  //�����ڷ��ͱ�־
 }
}
