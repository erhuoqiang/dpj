#include"msp430g2553.h"

/*****************************************************
      ���ܣ�UART ��ʼ�� ֻ����RX���ж�
 ��������Ҫ�Լ��������ֲ�435ҳ�޸� UCMCTL�Ĵ���
Ĭ�� SMCLK ʱ��Դ  9600������ 8λ���� ����֤λ 1ֹͣλ
*****************************************************/
void UART_A0_Init()
{
  //BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
 // DCOCTL = CALDCO_1MHZ;
  P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  UCA0CTL1 |= UCSSEL_2;                    //  UCSSEL_2��SMCLK
  UCA0BR0 = 104;                       //��Ӧ���� �������ֲ�435P
  UCA0BR1 = 0;                      //������ = ʱ��ԴƵ��/(BR0+256*BR1+UCBRSx/8)��MCTL һ����� 
  UCA0MCTL |= UCBRS0;  //  UCBRS0���USBRSx ��˵�ǵ�һλ ������1 UCBRS1����2
  UCA0CTL1 &= ~UCSWRST;
  IE2 |=  UCA0RXIE;           //һ��ֻ�������ж� ���ж���������
  //IE2 |= UCA0TXIE; 
}

void Uart_A0_Send_Str(char *s)
{
  while(*s!='\0')
  {
    UCA0TXBUF=*s;
    while((IFG2&UCA0TXIFG)==0); //��ѯ�����Ƿ����
   // IFG2&=~UCA0TXIFG; //�������һ��־λ
    s++;//ָ��λ�ü�һ
  }
}

