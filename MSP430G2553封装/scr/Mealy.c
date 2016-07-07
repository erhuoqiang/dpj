#include"msp430g2553.h"

#define IDLE 0
#define SHORT 1
#define LONG 2
#define COUNTER_THRESHOLD 50//  50次看门狗定时器

unsigned char WDT_Counter = 0;
char LongClick_Dect()
{
  WDT_Counter++;
  if(WDT_Counter == COUNTER_THRESHOLD)
  {
    return 1;
  }
  else 
  {
    return 0;
  }
}
/*
void GPIO_Init()
{
  //LED
  P1DIR |= BIT6 + BIT0;
  P1OUT |= BIT6;
  P1OUT &= ~BIT0;
  //上拉输入
  P1REN |= BIT3;
  P1OUT |= BIT3;
  P1DIR &= ~BIT3;
}*/
/*
void WDT_Init()
{
  WDTCTL = WDT_ADLY_16;
  IE1 |= WDTIE;
}*/

void P13_OnShortRelease()
{
  P1OUT ^= BIT6;
}
  
void P13_OnLongClick()
{
  P1OUT ^= BIT0;
}

void Key_SM()
{
  unsigned char Key_Dect = 0;
  static unsigned char State = 0;
  static unsigned char Key_Now = 0;
  unsigned char Key_Past = 0;
  Key_Past = Key_Now;
  if(P1IN&BIT3) 
  { 
    Key_Now=1;
  }
  else 
  {
    Key_Now=0;
  }
//-----电平前高后低，表明按下-----
  if((Key_Past==1)&&(Key_Now==0))
  {
    Key_Dect=1;
  }
  //-----电平前低后高，表明弹起-----
  if((Key_Past==0)&&(Key_Now==1))
  {
    Key_Dect=2 ;
  }
  switch(State)
  {
  case IDLE:
    WDT_Counter=0;  //这个是清0 WDT_Counter
    if(Key_Dect==1)    State=SHORT;  break;
  
  case SHORT:
    if(Key_Dect==2)  
    {
      State = IDLE;
  P13_OnShortRelease();  
    }
    if(LongClick_Dect())
    {
      State = LONG;
      P13_OnLongClick();
    }
    break;
  case LONG:
    {
      //WDT_Counter=0; 
      if(Key_Dect==2)
      {
        State = IDLE;
      }
      break;
    }
    default :
      State = IDLE; break;
    }
  
      
    
    
   
  
}
/*
void main()
{
  WDTCTL = WDTPW + WDTHOLD;
  GPIO_Init();
  WDT_Init();
  //总中断必须在进入低功耗之前开 或者同时开
  _BIS_SR(GIE+LPM3_bits);
 
}*/
/*
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR()
{
   Key_SM();
}

*/

  
  