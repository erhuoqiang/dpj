#include"msp430g2553.h"

/*****************************************************
      功能：UART 初始化 只开了RX的中断
 波特率需要自己查数据手册435页修改 UCMCTL寄存器
默认 SMCLK 时钟源  9600波特率 8位数据 无验证位 1停止位
*****************************************************/
void UART_A0_Init()
{
  //BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
 // DCOCTL = CALDCO_1MHZ;
  P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  UCA0CTL1 |= UCSSEL_2;                    //  UCSSEL_2是SMCLK
  UCA0BR0 = 104;                       //相应数据 查数据手册435P
  UCA0BR1 = 0;                      //波特率 = 时钟源频率/(BR0+256*BR1+UCBRSx/8)和MCTL 一起控制 
  UCA0MCTL |= UCBRS0;  //  UCBRS0相对USBRSx 来说是第一位 所以是1 UCBRS1就是2
  UCA0CTL1 &= ~UCSWRST;
  IE2 |=  UCA0RXIE;           //一般只开接收中断 总中断主函数开
  //IE2 |= UCA0TXIE; 
}

void Uart_A0_Send_Str(char *s)
{
  while(*s!='\0')
  {
    UCA0TXBUF=*s;
    while((IFG2&UCA0TXIFG)==0); //查询发送是否结束
   // IFG2&=~UCA0TXIFG; //清除发送一标志位
    s++;//指针位置加一
  }
}

