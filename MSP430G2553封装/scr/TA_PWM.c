//==========================TA_PWM.c========================//
#include"msp430g2553.h"
#define DEADTIME  20  //Ԥ�������ʱ�䣬��TA��CLK����λ

//P1.5 P2.0  TAx.0ֻ�ܿ���ģʽ0��4��5��ͨ��������Ƶ,����TAX.0���������ֱ�����ģʽ4 %50��ս��
#define TA00_SET    P1SEL |= BIT5; P1DIR |= BIT5; TA0CCTL0 |= OUTMOD_4   
#define TA01_SET    P1DIR |= BIT6; P1SEL |= BIT6    //P1.6
#define TA02_SET    P3SEL |= BIT0; P3DIR |= BIT0    //P3.0    G2553 20Pin��TA0.2  ֻ��P1��P2
#define TA10_SET    P2SEL |= BIT0; P2DIR |= BIT0; TA1CCTL0 |= OUTMOD_4    //P2.0  
#define TA11_SET    P2SEL |= BIT2; P2DIR |= BIT2    //P2.2
#define TA12_SET    P2SEL |= BIT4; P2DIR |= BIT4    //P2.4   

#define TA00_OFF    P1SEL&= ~BIT5           //P1.5
#define TA01_OFF    P1SEL&= ~BIT6         //P1.6
#define TA02_OFF    P3SEL&= ~BIT0          //P3.0
#define TA10_OFF    P2SEL&= ~BIT0          //P2.0
#define TA11_OFF    P2SEL&= ~BIT2         //P2.2
#define TA12_OFF    P2SEL&= ~BIT4        //P2.4

/******************************************************************************************************
*  ��  �ƣ�TA0_PWM_Init()
*  ��  �ܣ�TA0��ʱ����ΪPWM�������ĳ�ʼ�����ú���
*  ��ڲ�����  Clk:ʱ��Դ  'S'=SMCLK;    'A'=ACLK ;    'E'=TACLK( �ⲿ����)��  'e'= TACLKȡ��
Div:ʱ�ӷ�Ƶϵ��: 1/2/4/8
Mode0:ͨ��0 Ĭ�����4  'o'��ģʽ4��� 0�ر����
Mode1:ͨ��1�����ģʽ  'F'��Ϊ��ǰPWM��ģʽ7����'B'�ͺ�PWM��ģʽ3����'D'
��������PWM��ģʽ6����0=����
Mode2:ͨ��2�����ģʽ  'F'��Ϊ��ǰPWM��ģʽ7����'B'�ͺ�PWM��ģʽ3����'D'
��������PWM��ģʽ2����0=���� 
MCģʽ��ѡ����Mode1ȷ�� ���� 
����������������Ƶ�PWMʱ����ͨ������ʹ�ã��Ҿ�Ϊ����ģʽ������ģʽ��MC ������ ���� �ᵼ������ͨ����Ƶ����Ӱ��
*  ���ڲ�����1��ʾ���óɹ���0��ʾ������������ʧ�ܡ�
*  ˵  �����ڵ���PWM��غ���֮ǰ����Ҫ���øú�������TA ��ģʽ��ʱ��Դ��
*  ��  ����TA0_PWM_Init('A',1,'F','F')TAʱ����ΪACLK,ͨ��1��ͨ��2��Ϊ��ǰPWM���
TA0_PWM_Init('S',4,'D','D')TAʱ����ΪSMCLK/4,  ͨ��1Ϊ������PWM��ͨ��2Ϊ����
��PWM
TA0_PWM_Init('A',1,'F',0)TAʱ����ΪACLK,ͨ��1��ǰPWM�����ͨ��2����TA�á�
**************************************************** **************************************************/
char TA0_PWM_Init(char Clk, unsigned int Div, char Mode0, char Mode1, char Mode2)
{
  TA0CTL = 0;
  
  switch(Clk)
  {
  case 'S':case 's':  TA0CTL  |= TASSEL_2; break;        //SMCLK
  case 'A':case 'a':  TA0CTL  |= TASSEL_1; break;        //ACLK
  case 'E':  TA0CTL |= TASSEL_0; break;                 //�ⲿ����TACLK
  case 'e':  TA0CTL |= TASSEL_3; break;                 //�ⲿ����ȡ��TACLK
  default:   return(0);                                //����������󷵻�0�����ڼ�����λ��
  }
  
  switch(Div)
  {
  case  1:    TA0CTL |=ID_0;  break;                      //1
  case  2:    TA0CTL|=ID_1;  break;                        //2
  case  4:    TA0CTL|=ID_2;  break;                       //4
  case  8:    TA0CTL|=ID_3;  break;                        //8
  default  :   return(0);                 //���ò������󣬷���0
  }
  //PWM�������������MCx��OUTMODx ��ͬ������   ������Mode1 Ҳ����ͨ��1����MCx
  //TA0CTL ���ܿ��ƶ� ����MCx  ����3��TA0CTL0 TA0CTL1 TA0CTL2 ����OUTMODx
 
  switch(Mode0)
  {
  case 'o':case 'O':TA00_SET;break;
  default: TA00_OFF;break;
  }
   switch(Mode1)
  {
  case 'F':case 'f': case 'B':case 'b': TA0CTL  |= MC_1;   break;
  case 'D':case 'd': TA0CTL |= MC_3; break;
  default: TA0CTL |= MC_2; break;
  }
 
  
  switch(Mode1)
  {
  case  'F' :  case  'f' :   TA0CCTL1 |= OUTMOD_7; TA01_SET;break;
  case  'B' :  case  'b' :    TA0CCTL1 |= OUTMOD_3; TA01_SET;break;
  case  'D':  case   'd' :  TA0CCTL1 |= OUTMOD_6; TA01_SET;break;
  case  '0' :  case     0:    TA01_OFF; break;       //�������Ϊ����TA0.1�ָ�Ϊ��ͨIO��
  default  :   return(0);            //���ò������󣬷���0
  }
  
    switch(Mode2)
  {
  case  'F' :  case  'f' :   TA0CCTL2 |= OUTMOD_7; TA02_SET;break;
  case  'B' :  case  'b' :    TA0CCTL2 |= OUTMOD_3; TA02_SET;break;
  case  'D' :  case  'd' :  TA0CCTL2 |= OUTMOD_6; TA02_SET;break;
  case  '0' :  case   0  :    TA02_OFF; break;       //�������Ϊ����TA0.1�ָ�Ϊ��ͨIO��
  default   :   return(0);   break;         //���ò������󣬷���0
  }
  
  return(1);
  
}


/*******************************************************************************
TA0_PWM_Init(char Clk, char Div, char Mode0, char Mode1, char Mode2);
��ʹ�÷��� �� 
CLK :     �� S,A,E,e �����н��� 
              
DIV :     4�ַ�Ƶ1 2 4 8 
             
Mode0 :   'o' 'O'������ OUTMOD Ϊģʽ4 %50�Ŀ�ս�� ����0��ʾ����
              
Mode1:    ���Ʊ���ͨ����OUTMODx �ֱ���F,f(forward ),B,b(behind)��D��d��dead�� ����0 ����
          ͬʱMode1������ 3��ͨ����MCx F,f��B,bΪ ��ģʽMC1 D,dΪ��������ģʽ������Ϊ��������ģʽ
          �����ú�����ǰ���� Ĭ��MC0 �رռ���
              
Mode2 :   ��Mode1һ�� ����MCֻ��MODE1���Կ��� MODE2ֻ���� ͨ��2��OUTMODx
          ����F,f(forward ),B,b(behind)��D��d��dead�� ����0 ���� 
*******************************************************************************/






/******************************************************************************************************
*  ��  �ƣ�TA1_PWM_Init()
*  ��  �ܣ�TA0��ʱ����ΪPWM�������ĳ�ʼ�����ú���
*  ��ڲ�����  Clk:ʱ��Դ  'S'=SMCLK;    'A'=ACLK ;    'E'=TACLK( �ⲿ����)��  'e'= TACLKȡ��
Div:ʱ�ӷ�Ƶϵ��: 1/2/4/8
Mode0:ͨ��0 Ĭ�����4  'o'��ģʽ4��� 0�ر����
Mode1:ͨ��1�����ģʽ  'F'��Ϊ��ǰPWM��ģʽ7����'B'�ͺ�PWM��ģʽ3����'D'
��������PWM��ģʽ6����0=����
Mode2:ͨ��2�����ģʽ  'F'��Ϊ��ǰPWM��ģʽ7����'B'�ͺ�PWM��ģʽ3����'D'
��������PWM��ģʽ2����0=���� 
MCģʽ��ѡ����Mode1ȷ�� ���� 
����������������Ƶ�PWMʱ����ͨ������ʹ�ã��Ҿ�Ϊ����ģʽ������ģʽ��MC ������ ���� �ᵼ������ͨ����Ƶ����Ӱ��
*  ���ڲ�����1��ʾ���óɹ���0��ʾ������������ʧ�ܡ�
*  ˵  �����ڵ���PWM��غ���֮ǰ����Ҫ���øú�������TA ��ģʽ��ʱ��Դ��
*  ��  ����TA0_PWM_Init('A',1,'F','F')TAʱ����ΪACLK,ͨ��1��ͨ��2��Ϊ��ǰPWM���
TA1_PWM_Init('S',4,'D','D')TAʱ����ΪSMCLK/4,  ͨ��1Ϊ������PWM��ͨ��2Ϊ����
��PWM
TA1_PWM_Init('A',1,'F',0)TAʱ����ΪACLK,ͨ��1��ǰPWM�����ͨ��2����TA�á�
**************************************************** **************************************************/
char TA1_PWM_Init(char Clk, unsigned int Div, char Mode0, char Mode1, char Mode2)
{
  TA1CTL = 0;
  
  switch(Clk)
  {
  case 'S':case 's':  TA1CTL  |= TASSEL_2; break;        //SMCLK
  case 'A':case 'a':  TA1CTL  |= TASSEL_1; break;        //ACLK
  case 'E':  TA1CTL |= TASSEL_0; break;                 //�ⲿ����TACLK
  case 'e':  TA1CTL |= TASSEL_3; break;                 //�ⲿ����ȡ��TACLK
  default:   return(0);                                //����������󷵻�0�����ڼ�����λ��
  }
  
  switch(Div)
  {
  case  1:    TA1CTL |=ID_0;  break;                      //1
  case  2:    TA1CTL|=ID_1;  break;                        //2
  case  4:    TA1CTL|=ID_2;  break;                       //4
  case  8:    TA1CTL|=ID_3;  break;                        //8
  default  :   return(0);                 //���ò������󣬷���0
  }
  //PWM�������������MCx��OUTMODx ��ͬ������   ������Mode1 Ҳ����ͨ��1����MCx
  //TA0CTL ���ܿ��ƶ� ����MCx  ����3��TA0CTL0 TA0CTL1 TA0CTL2 ����OUTMODx
  switch(Mode1)
  {
  case 'F':case 'f': case 'B':case 'b': TA1CTL  |= MC_1;   break;
  case 'D':case 'd': TA1CTL |= MC_3; break;
  default: TA1CTL |= MC_2; break;
  }
  switch(Mode0)
  {
  case 'o':case 'O': TA10_SET;  break;
  default: TA10_OFF;break;
  }
 
  
  switch(Mode1)
  {
  case  'F' :  case  'f' :   TA1CCTL1 |= OUTMOD_7; TA11_SET;break;
  case  'B' :  case  'b' :    TA1CCTL1 |= OUTMOD_3; TA11_SET;break;
  case  'D':  case   'd' :  TA1CCTL1 |= OUTMOD_6; TA11_SET;break;
  case  '0' :  case     0:    TA11_OFF; break;       //�������Ϊ����TA0.1�ָ�Ϊ��ͨIO��
  default  :   return(0);            //���ò������󣬷���0
  }
  
    switch(Mode2)
  {
  case  'F' :  case  'f' :   TA1CCTL2 |= OUTMOD_7; TA12_SET;break;
  case  'B' :  case  'b' :    TA1CCTL2 |= OUTMOD_3; TA12_SET;break;
  case  'D' :  case  'd' :  TA1CCTL2 |= OUTMOD_6; TA12_SET;break;
  case  '0' :  case   0  :    TA12_OFF; break;       //�������Ϊ����TA0.1�ָ�Ϊ��ͨIO��
  default   :   return(0);   break;         //���ò������󣬷���0
  }
  
  return(1);
  
}


/*******************************************************************************
TA1_PWM_Init(char Clk, char Div, char Mode0, char Mode1, char Mode2);
��ʹ�÷��� �� 
CLK :     �� S,A,E,e �����н��� 
              
DIV :     4�ַ�Ƶ1 2 4 8 
             
Mode0 :   'o' 'O'������ OUTMOD Ϊģʽ4 %50�Ŀ�ս�� ����0��ʾ����
              
Mode1:    ���Ʊ���ͨ����OUTMODx �ֱ���F,f(forward ),B,b(behind)��D��d��dead�� ����0 ����
          ͬʱMode1������ 3��ͨ����MCx F,f��B,bΪ ��ģʽMC1 D,dΪ��������ģʽ������Ϊ��������ģʽ
          �����ú�����ǰ���� Ĭ��MC0 �رռ���
              
Mode2 :   ��Mode1һ�� ����MCֻ��MODE1���Կ��� MODE2ֻ���� ͨ��2��OUTMODx
          ����F,f(forward ),B,b(behind)��D��d��dead�� ����0 ���� 
*******************************************************************************/




/****************************************************************************
*  ��  �ƣ�TA_PWM_SetPeriod()
*  ��  �ܣ�����PWM������������
*  ��ڲ�����Period:����(0~65535)  ʱ�Ӹ��� ����ģʽMC �����ͼӵ�65535��0 
    �������͵�TACCR0 ��0 ���������͵�TACCR0 �ݼ�
*  ���ڲ�����1�����óɹ�    0������ʧ��
*  ˵  ����  ��ͨPWM�������PWM�������һ��
*  ��  ����TA0_PWM_SetPeriod(500)����PWM��������Ϊ500��1000��ʱ������
****************************************************************************/
#include"msp430g2553.h"
char TA0_PWM_SetPeriod(unsigned  int  Period)
{
  if  (Period>65535)  
    
    TA0CCR0 = 65535;
  
  else 
  
    TA0CCR0 = Period;
    return(1);
 
}

char TA1_PWM_SetPeriod(unsigned  int  Period)
{
  if  (Period>65535) 
    
    TA0CCR0 = 65535;
  
  else
  
    TA1CCR0 = Period;
    return(1);
  
}






/******************************************************************************************************
*  ��  �ƣ�TA0_PWM_SetPermill()
*  ��  �ܣ�����PWM�����ռ�ձ�(ǧ�ֱ�)
*  ��ڲ�����Channel: ��ǰ���õ�ͨ����  1/2
Duty: PWM�ߵ�ƽ��Чʱ���ǧ�ֱ�  (0~1000)��
*  ���ڲ�����1���óɹ���0����ʧ��
*  ˵  ��: 1000=100.0%  500=50.0%  ���������ơ�����ģʽʱ����channelͬʱ�趨��
*  ��  ��: TA0_PWM_SetPermill(1,300)����PWMͨ��1������ռ�ձ�Ϊ30.0%
TA0_PWM_SetPermill(2,825)����PWMͨ��2������ռ�ձ�Ϊ82.5%
************************************************************************************* *****************/
char TA0_PWM_SetPermill(char  Channel,unsigned  int  Duty)
{
  unsigned  char  Mod = 0;
  unsigned  int  DeadPermill=0;
  unsigned  long  int  Percent=0;          //��ֹ�˷�����ʱ���
  Percent=Duty;
  DeadPermill=((DEADTIME*1000)/TACCR0);      //����������ʱ�任���ǧ�ֱ�����ʱ�� 
  switch (Channel)              //���жϳ�ͨ���Ĺ���ģʽ
  {
  case  1:
    Mod = (TA0CCTL1& 0x00e0)>>5;break;     //��ȡ���ģʽ��OUTMOD0λ��5-7λ
  case  2:
  Mod = (TA0CCTL2 & 0x00e0)>>5;break;    //��ȡ���ģʽ��OUTMOD1λ��5-7λ
  default:  return(0);
  
  }
  switch(Mod)                //����ģʽ�趨TACCRx
  {
    case  2: case  6:   
    //-----����ģʽ2,6ʱ����Ҫ�ж���������ʱ�䣬��ͬʱ�趨TA0CCR1/2  ��ֵ-----
      {
      
        if((1000-2*Percent)<= DeadPermill)   //Ԥ������ʱ��
    Percent=(1000-  DeadPermill)/2;
    TA0CCR1=Percent*TA0CCR0/1000;
    TA0CCR2= TA0CCR0-TA0CCR1;
    break;
    }
    case  7:
  {
  if(Percent>1000)  Percent=1000;
  if(Channel==1) TA0CCR1=Percent* TA0CCR0/1000;
  if(Channel==2) TA0CCR2=Percent* TA0CCR0/1000;
  break;
  }
  case  3:      //ռ�ձ�һ��Ϊ������������Ҫ  TA0CCR0��ȥռ�ձ�
  {
  if(Percent>1000)  Percent=1000;
  if(Channel==1) TA0CCR1 = TA0CCR0-Percent*TA0CCR0/1000;
  if(Channel==2) TA0CCR2= TA0CCR0 -Percent*TA0CCR0/1000;
  break;
  }
  default:  return(0);
  }
return (1);

}





/******************************************************************************************************
*  ��  �ƣ�TA1_PWM_SetPermill()
*  ��  �ܣ�����PWM�����ռ�ձ�(ǧ�ֱ�)
*  ��ڲ�����Channel: ��ǰ���õ�ͨ����  1/2
Duty: PWM�ߵ�ƽ��Чʱ���ǧ�ֱ�  (0~1000)��
*  ���ڲ�����1���óɹ���0����ʧ��
*  ˵  ��: 1000=100.0%  500=50.0%  ���������ơ�����ģʽʱ����channelͬʱ�趨��
*  ��  ��: TA1_PWM_SetPermill(1,300)����PWMͨ��1������ռ�ձ�Ϊ30.0%
TA1_PWM_SetPermill(2,825)����PWMͨ��2������ռ�ձ�Ϊ82.5%
************************************************************************************* *****************/
char TA1_PWM_SetPermill(char  Channel,unsigned  int  Duty)
{
  unsigned  char  Mod = 0;
  unsigned  int  DeadPermill=0;
  unsigned  long  int  Percent=0;          //��ֹ�˷�����ʱ���
  Percent=Duty;
  DeadPermill=((DEADTIME*1000)/TA1CCR0);      //����������ʱ�任���ǧ�ֱ�����ʱ�� 
  switch (Channel)              //���жϳ�ͨ���Ĺ���ģʽ
  {
  case  1:
    Mod = (TA1CCTL1& 0x00e0)>>5;break;     //��ȡ���ģʽ��OUTMOD0λ��5-7λ
  case  2:
  Mod = (TA1CCTL2 & 0x00e0)>>5;break;    //��ȡ���ģʽ��OUTMOD1λ��5-7λ
  default:  return(0);
  
  }
  switch(Mod)                //����ģʽ�趨TACCRx
  {
    case  2: case  6:   
    //-----����ģʽ2,6ʱ����Ҫ�ж���������ʱ�䣬��ͬʱ�趨TA0CCR1/2  ��ֵ-----
      {
      
        if((1000-2*Percent)<= DeadPermill)   //Ԥ������ʱ��
    Percent=(1000-  DeadPermill)/2;
    TA1CCR1=Percent*TA1CCR0/1000;
    TA1CCR2= TA1CCR0-TA1CCR1;
    break;
    }
    case  7:
  {
  if(Percent>1000)  Percent=1000;
  if(Channel==1) TA1CCR1=Percent* TA1CCR0/1000;
  if(Channel==2) TA1CCR2=Percent* TA1CCR0/1000;
  break;
  }
  case  3:      //ռ�ձ�һ��Ϊ������������Ҫ  TA0CCR0��ȥռ�ձ�
  {
  if(Percent>1000)  Percent=1000;
  if(Channel==1) TA1CCR1 = TA1CCR0-Percent*TA0CCR0/1000;
  if(Channel==2) TA1CCR2= TA1CCR0 -Percent*TA0CCR0/1000;
  break;
  }
  default:  return(0);
  }
return (1);

}