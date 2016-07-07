#include<msp430g2553.h>

void GPIO_Init(void)
{//P1.5��DS18B20ʹ���� ����ظ�ռ����Ҫ�޸�
  P2DIR |= BIT3 + BIT4 + BIT5; //12864���нӷ� ��3��IO��

  P1DIR |= BIT7 + BIT3 +BIT4;//���
  
  //LED 1.6  1.0
 P1DIR |= BIT6 + BIT0;
  P1OUT |= BIT6;
  P1OUT &= ~BIT0;
 
  //����1.3��������
 P1REN |= BIT3;
  P1OUT |= BIT3;
  P1DIR &= ~BIT3;

  //�ж�ѡ��
  P1IES |= BIT3;
  P1IE |= BIT3;
 P1IFG = 0x00;
  
/* //����2.0��������
 P2REN |= BIT0;
  P2OUT |= BIT0;
  P2DIR &= ~BIT0;

  //�ж�ѡ��
  P2IES |= BIT0;
  P2IE |= BIT0;
 P2IFG = 0x00;*/
}
void WDT_Init()
{
  //��ʱ�ж� WDT_ADLY_1000; 1000MS  WDT_ADLY_250; 250MS  WDT_ADLY_16; 16ms WDT_ADLY_1_9; 1.9ms
  // ��ʱ��λ WDT_MRST_32; 32ms WDT_MRST_8;   8ms  WDT_MRST_0_5; 0.5ms WDT_MRST_0_064 ; 0.064ms
  //ʱ��Դ��ACLK ��MCLK ���� ����Ҳ�������� һ��RST һ��DLY
  WDTCTL = WDT_ADLY_16;
  IE1 |= WDTIE;
}

/********************************************************
     ��λΪ0.1 S  ���2S 
******************************************************/
void Timer_A0_Init(char Interval)//ѡ��ʱ��ԴACLK
{   
  TA0CTL |= TASSEL_1+TACLR+MC_1;
  TA0CCTL0 |= CCIE;
  TA0CCR0 = (unsigned int)(3276.75*Interval);
   
}

void Timer_A1_Init(char Interval)//ѡ��ʱ��ԴACLK
{   
  TA1CTL |= TASSEL_1+TACLR+MC_1;
  TA1CCTL0 |= CCIE;
  TA1CCR0 = (unsigned int)(3276.75*Interval);
   
}

void DCO_Init()
{   //���������� ʱ��Ƶ�� ������õ�UART ����Ҫ����UART��
    //�����ʺ�ʱ��Դ ��ΪUART ����������������DCO 8M ������
  
  //ע�������Ƶ���н����õ��ں�  ��ΪĬ�� ��һ��ȫΪ0 ���Ҫ������λ������0����
    BCSCTL1 = DIVA_0 + CALBC1_1MHZ; DCOCTL = CALDCO_1MHZ;// 1/ 8/12/16  MHZ
    BCSCTL2 = DIVM_0 +DIVS_0; //0/1/2/3�ֱ��Ӧ 1 2 4 8��Ƶ
     //ע�� CALBC1_8MHZ_;��CALBC1_8MHZ; �ǲ�һ����
}

      
  
  