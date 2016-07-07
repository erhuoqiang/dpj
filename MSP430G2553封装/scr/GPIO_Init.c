#include<msp430g2553.h>

void GPIO_Init(void)
{
  //P2DIR |= BIT3 + BIT4 + BIT5; //12864串行接法 的3个IO口

   //LED 1.6  1.0
  P1DIR |= BIT6 + BIT0;
  P1OUT |= BIT6;
  P1OUT &= ~BIT0;
  //按键1.3上拉输入
  //P1REN |= BIT3;
  //P1OUT |= BIT3;
  //P1DIR &= ~BIT3;

  //中断选择
  //P1IES |= BIT3;
  //P1IE |= BIT3;
  //P1IFG = 0x00;
}
void WDT_Init(void)
{
  WDTCTL = WDT_ADLY_16;
  IE1 |= WDTIE;
}

void Timer_Enable(void)
{
  TA0CCTL0 |= CCIE;
  //TA0CCTL1 |= CCIE;
  //TA1CCTL0 |= CCIE;
  //TA1CCTL1 |= CCIE;
}
  
  