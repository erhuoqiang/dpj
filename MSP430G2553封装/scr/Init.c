#include<msp430g2553.h>

void GPIO_Init(void)
{//P1.5给DS18B20使用了 如果重复占用需要修改
  P2DIR |= BIT3 + BIT4 + BIT5; //12864串行接法 的3个IO口

  P1DIR |= BIT7 + BIT3 +BIT4;//电机
  
  //LED 1.6  1.0
 P1DIR |= BIT6 + BIT0;
  P1OUT |= BIT6;
  P1OUT &= ~BIT0;
 
  //按键1.3上拉输入
 P1REN |= BIT3;
  P1OUT |= BIT3;
  P1DIR &= ~BIT3;

  //中断选择
  P1IES |= BIT3;
  P1IE |= BIT3;
 P1IFG = 0x00;
  
/* //按键2.0上拉输入
 P2REN |= BIT0;
  P2OUT |= BIT0;
  P2DIR &= ~BIT0;

  //中断选择
  P2IES |= BIT0;
  P2IE |= BIT0;
 P2IFG = 0x00;*/
}
void WDT_Init()
{
  //定时中断 WDT_ADLY_1000; 1000MS  WDT_ADLY_250; 250MS  WDT_ADLY_16; 16ms WDT_ADLY_1_9; 1.9ms
  // 定时复位 WDT_MRST_32; 32ms WDT_MRST_8;   8ms  WDT_MRST_0_5; 0.5ms WDT_MRST_0_064 ; 0.064ms
  //时钟源有ACLK 和MCLK 两种 功能也都有两种 一种RST 一种DLY
  WDTCTL = WDT_ADLY_16;
  IE1 |= WDTIE;
}

/********************************************************
     单位为0.1 S  最大2S 
******************************************************/
void Timer_A0_Init(char Interval)//选择时钟源ACLK
{   
  TA0CTL |= TASSEL_1+TACLR+MC_1;
  TA0CCTL0 |= CCIE;
  TA0CCR0 = (unsigned int)(3276.75*Interval);
   
}

void Timer_A1_Init(char Interval)//选择时钟源ACLK
{   
  TA1CTL |= TASSEL_1+TACLR+MC_1;
  TA1CCTL0 |= CCIE;
  TA1CCR0 = (unsigned int)(3276.75*Interval);
   
}

void DCO_Init()
{   //如果这里改了 时钟频率 如果有用到UART 就需要改下UART的
    //波特率和时钟源 因为UART 里面又重新设置了DCO 8M 波特率
  
  //注意如果分频不行建议用等于号  因为默认 不一定全为0 如果要改其他位则先清0后与
    BCSCTL1 = DIVA_0 + CALBC1_1MHZ; DCOCTL = CALDCO_1MHZ;// 1/ 8/12/16  MHZ
    BCSCTL2 = DIVM_0 +DIVS_0; //0/1/2/3分别对应 1 2 4 8分频
     //注意 CALBC1_8MHZ_;和CALBC1_8MHZ; 是不一样的
}

      
  
  