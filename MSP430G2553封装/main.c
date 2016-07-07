#include "msp430g2553.h"
#include"msp430_conf.h"


#define uint  unsigned int
#define uchar   unsigned char

 //ENABLE 1.7 DIR+ 1.3 DIR- 1.4 pwm 2.2 
#define Moter_OFF         P1OUT |= BIT7
#define Moter_ON          P1OUT &= ~BIT7
#define Moter_DIR_CW      P1OUT |= BIT3;   P1OUT &= ~BIT4    //3+ 4- ˳
#define Moter_DIR_CCW     P1OUT |= BIT4;   P1OUT &= ~BIT3

float Get_T = 0;  //�¶�
float Get_V = 0;  //��ѹ
char *str[2] = {"514514","dian3"};//�����ַ�������
char temp[20]= "514";//12864
char temp1[10] = "";
int value;
/**********************************************
�мǣ� �ǵô����ж� ���趨GPIO ������ĳ�ʼ��
*********************************************/
void main()
{ 
  WDTCTL = WDTPW + WDTHOLD;//�رտ��Ź� ���ѣ�������ж�Ҫ�����ж�

  /*************************************************
    DCO=1MHZ    MCLK ��  SMCLK ��λĬ��DCO    0��Ƶ
   ��ȥ���ü��ɵ�MCLK SMCLK �Ķ���ǵ��޸���ʱ ��UART�Ĳ����ʺ�ʱ��
 ***************************************************/
   DCO_Init();
   
  
  GPIO_Init();  //�ǵ�ȥ���濪IO
 
  /***************************************************
   ѡ���Ź���ʱ��λ ���Ƕ�ʱ�ж� ��ȥ����Ĳ�������
  **************************************************/
  // WDT_Init(); //Ĭ������Ϊ16MS 
  
  /*******************************************
  PWM �����ڲ�ʱ�Ӻ�GPIO ���趨�������������Init
  **********************************************/
  // TA0_PWM_SetPeriod(65535);  
   //TA1_PWM_SetPeriod(65535);
   //TA0CTL |= TACLR;//
 // TA1CTL |= TACLR;
  // TA0_PWM_Init('A',1,'o','f',0);
  //TA1_PWM_Init('A',1,'o','f',0);// ͨ��0 �����������ʲôMC ����50%
  //TA0_PWM_SetPermill(1, 100);////MSP430G2553ֻ��1ͨ�����Ե���ռ��
  //TA1_PWM_SetPermill(1, 100); //��ͨ����ǧ�ֱȣ�

  /*****************************************
                   ����ͨѶ
  *****************************************/
  //UART_A0_Init(); //9600 ������ 0��֤ 1ֹͣ   IO���ѳ�ʼ��
   //Uart_A0_Send_Str(str[1]);//�����ַ���  ��Uart_A0_Send_Str(str[1]);

   /*****************************************************
  ��ʱ�� 0.1SΪ ��λ ��1MS Timer_A0_Init(10);
  ���ʱ�䲻�� ����ȥDCO �е�BCSCTL1�и�DIVA 
  *******************************************************/
 // Timer_A0_Init(10);
    //Timer_A1_Init(20);
  
  /**********************************************************
            ADC��ȥѡ��ͨ��  �Դ��¶ȴ����� A10 ����Ҫ����GPIO
            ��׼��ѹ���� �ǵ�ҲҪ���ж�ȥ�޸�
        �����Ҫ��ͨ�� ���� ��ʱ���� ���� �ö�ʱ������
  *********************************************************/
    //ADC_Init();
    
 
   
   /********************************************
              12864 IO ��GPIO �￪
  *******************************************/
    LCD_Init();   // 12864 IO�Լ���
   
    //Get_T = Do1Convert(); //��ȡDS18B20�¶�
   // sprintf(temp,"�¶�%5.2fHZ",Get_T);//temp�������� ���ַ����ͱ���ת��������
    //sscanf(temp,"%d",&value); //���ַ���ת�������� 
    DisplayC_LCD(1, temp, 0);//���У����������У�0~7����
   
  _BIS_SR(GIE); //���жϼǵÿ�
  while(1){;}
  //_BIS_SR(LPM3_bits);	// ������͹���ģʽһ��Ҫ����� ��Ϊ��ǰ��CPU��û�ɳ���ִ�����˯��
  
}









/****************************************************************
���� :�ж� �ǵÿ����ж�GIE 
****************************************************************/
//P1�ⲿ�ж� ��ΪP1����IO ����һ������ ������Ҫ�õ� �������KEY_Dect
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
  
  P1_IODect();
  P1IFG = 0x00;
  
}
//P2�ⲿ�ж�
#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
  
  P2_IODect();
  P2IFG = 0x00;
  
}

//TA0.0
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TA00_ISR(void)
{
  //ADC ��ʱ���� 
    //P1OUT ^= BIT6 + BIT0;
  TA0CCTL0 &= ~CCIFG;
}

//TA0.1
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TA01_ISR(void)
{
 // P1OUT ^= BIT6 + BIT0;
  TA0CCTL1 &= ~CCIFG;
}

//TA1.0
#pragma vector = TIMER1_A0_VECTOR
__interrupt void TA10_ISR(void)
{
 // P1OUT ^= BIT6 + BIT0;
  TA1CCTL0 &= ~CCIFG;
}

//TA1.1
#pragma vector = TIMER1_A1_VECTOR
__interrupt void TA11_ISR(void)
{
 // P1OUT ^= BIT6 + BIT0;
  TA1CCTL1 &= ~CCIFG;
}

//���Ź� ��ʱ�� ��ʼ����GPIO_Init����
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
   // P1OUT ^= BIT6 + BIT0;
  // Key_SM();
   //
}

//UART ���� �ͷ����ж�
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
   
   
 // Uart_A0_Send_Str(temp);//�����ַ��� 

   while (!(IFG2&UCA0TXIFG)); 
    UCA0TXBUF = UCA0RXBUF;      // ���պ���������
 /* 
  //12864��������ַ��� ��temp 
 static unsigned char num; 
  while (!(IFG2&UCA0TXIFG));    // TXBUF Ҫ�Ȱ�֮ǰ�����ݷ�����λ�Ĵ�����TXIFG�Ż���1 ����Ҫ��
   temp[num++] = UCA0RXBUF;      //RXBUF ֻ�н������Ż���λRXIFG ���ԲŻ���ж�
  if(temp[num-1] == '*')
  {
    //����'.'��ʾ��λ�����Temp  Ϊ�˱����ϴα�������ݸ����� �������ֱ��� ������������򼸸��ո�
    num = 0; 
  
  }*/
   IFG2&=~UCA0RXIFG;
   
}

#pragma vector=USCIAB0TX_VECTOR//�����ն���UART_A0_Init()���滹û��
__interrupt void USCI0TX_ISR(void)
{
 //IFG2&=~UCA0TXIFG;
}

//ADC
#pragma vector=ADC10_VECTOR 
__interrupt void ADC10IN(void)
{
  //unsigned char table[]={0};  //��Ҫ�������ڵ�ʱ�����Ҫ��
 // int a=0,b=0,c=0,d=0;
  static char num;
  
  if(num < 10)
  {
    num++;
  Get_V += ADC10MEM*3.54/1024/10;  //��ѹ  ��׼��ѹ�ı�ʱ��Ҫ�޸�
   //Get_V += (ADC10MEM*422.5/1024  -  277.7)/10;  //�¶�
  }
  if(num == 10)
  { 
    
   sprintf(temp,"voltage is %3.2fV",Get_V);
   DisplayC_LCD(1,temp,0);
    num = 0;
    
    /* a=(int)Get_V;
    b=(int)(Get_V*10);b=b%10;
    c=(int)(Get_V*100);c=c%10;
    d=(int)(Get_V*1000);d=d%10;
    a=a+0x30;table[0]=a;
    table[1]='.';
    b=b+0x30;table[2]=b;
    c=c+0x30;table[3]=c;
    d=d+0x30;table[4]=d;
    table[5]=' ';
    table[6]=' ';
    for(int i=0;i<7;i++)      
   {       
      while(UCA0STAT&BUSY);                   //�жϷ����Ƿ����
      UCA0TXBUF = table[i];  
   } */
   Get_V = 0;
  }
}