#include"msp430g2553.h"
// 因为外部中断向量只有一个所以每次进去的时候都要判断是具体哪一位 
//需要配置GPIO和中断向量

//修改外部中断的IO 时 记得改下到ExterVect.h里面改下函数名字
void P13_Onclick(void)
{
  P1OUT ^= BIT6 +BIT0;
  /*static unsigned int freq=0;
  freq++;
  if(freq > 3)
  {
    freq = 0;
  }
  switch(freq)
  {
    case 0: DCOCTL = CALDCO_1MHZ_; BCSCTL1 = CALBC1_1MHZ_; BCSCTL2 = DIVM_0;  break;
    case 1: DCOCTL = CALDCO_1MHZ_; BCSCTL1 = CALBC1_1MHZ_; BCSCTL2 = DIVM_1;   break;
    case 2: DCOCTL = CALDCO_1MHZ_; BCSCTL1 = CALBC1_1MHZ_; BCSCTL2 = DIVM_2;  break;
    case 3: DCOCTL = CALDCO_1MHZ_; BCSCTL1 = CALBC1_1MHZ_; BCSCTL2 = DIVM_3;  break;
    default : break;
  } */
}

void P20_Onclick(void)
{
  
  P1OUT ^= BIT6 +BIT0;
}

void P1_IODect(void)
{
  unsigned int Push_Key;
  Push_Key = P1IFG & (~P1DIR);//排除输出位的干扰 输出位不会有中断
  __delay_cycles(10000);
  if((P1IN & Push_Key)==0) //排除非中断位的输入 使其不影响判断
  {
  switch(Push_Key)//用到哪个位就哪个位
    { 
     // case  BIT0:    P10_Onclick();    break;
     // case  BIT1:    P11_Onclick();    break;
     // case  BIT2:    P12_Onclick();    break;
      case  BIT3:    P13_Onclick();    break;
      // case  BIT4:    P14_Onclick();    break;
      // case  BIT5:    P15_Onclick();    break;
      // case  BIT6:    P16_Onclick();    break;
      // case  BIT7:    P17_Onclick();    break;
      default:            break;    //任何情况下均加上default
     }
  
   }
}

void P2_IODect(void)
{
  unsigned int Push_Key;
  Push_Key = P2IFG & (~P2DIR);//排除输出位的干扰 输出位不会有中断
  __delay_cycles(10000);
  if((P2IN & Push_Key)==0) //排除非中断位的输入 使其不影响判断
  {
  switch(Push_Key)//用到哪个位就哪个位
    { 
      case  BIT0:    P20_Onclick();    break;
     // case  BIT1:    P21_Onclick();    break;
     // case  BIT2:    P22_Onclick();    break;
     // case  BIT3:    P23_Onclick();    break;
      // case  BIT4:    P24_Onclick();    break;
      // case  BIT5:    P25_Onclick();    break;
      // case  BIT6:    P26_Onclick();    break;
      // case  BIT7:    P27_Onclick();    break;
       default:            break;    //任何情况下均加上default
     }
  
   }
}