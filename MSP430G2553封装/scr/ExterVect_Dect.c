#include"msp430g2553.h"
// ��Ϊ�ⲿ�ж�����ֻ��һ������ÿ�ν�ȥ��ʱ��Ҫ�ж��Ǿ�����һλ 
//��Ҫ����GPIO���ж�����

//�޸��ⲿ�жϵ�IO ʱ �ǵø��µ�ExterVect.h������º�������
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
  Push_Key = P1IFG & (~P1DIR);//�ų����λ�ĸ��� ���λ�������ж�
  __delay_cycles(10000);
  if((P1IN & Push_Key)==0) //�ų����ж�λ������ ʹ�䲻Ӱ���ж�
  {
  switch(Push_Key)//�õ��ĸ�λ���ĸ�λ
    { 
     // case  BIT0:    P10_Onclick();    break;
     // case  BIT1:    P11_Onclick();    break;
     // case  BIT2:    P12_Onclick();    break;
      case  BIT3:    P13_Onclick();    break;
      // case  BIT4:    P14_Onclick();    break;
      // case  BIT5:    P15_Onclick();    break;
      // case  BIT6:    P16_Onclick();    break;
      // case  BIT7:    P17_Onclick();    break;
      default:            break;    //�κ�����¾�����default
     }
  
   }
}

void P2_IODect(void)
{
  unsigned int Push_Key;
  Push_Key = P2IFG & (~P2DIR);//�ų����λ�ĸ��� ���λ�������ж�
  __delay_cycles(10000);
  if((P2IN & Push_Key)==0) //�ų����ж�λ������ ʹ�䲻Ӱ���ж�
  {
  switch(Push_Key)//�õ��ĸ�λ���ĸ�λ
    { 
      case  BIT0:    P20_Onclick();    break;
     // case  BIT1:    P21_Onclick();    break;
     // case  BIT2:    P22_Onclick();    break;
     // case  BIT3:    P23_Onclick();    break;
      // case  BIT4:    P24_Onclick();    break;
      // case  BIT5:    P25_Onclick();    break;
      // case  BIT6:    P26_Onclick();    break;
      // case  BIT7:    P27_Onclick();    break;
       default:            break;    //�κ�����¾�����default
     }
  
   }
}