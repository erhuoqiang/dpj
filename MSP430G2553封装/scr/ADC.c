#include"msp430g2553.h"


//��׼��ѹ��� �ǵ�ȥ����� �ж��е�����CODE
//����ö�ʱ�������ǵù��ж�
void ADC_Init()
{
    ADC10CTL0 |= SREF_0 + ADC10SHT_3 + MSC + ADC10ON + ADC10IE;  //ʱ��
 
   // ADC10CTL0 |= SREF_0 + ADC10ON + MSC + REFON + ADC10SHT_3 + SREF_1 + ADC10IE; //ѡ���¶�ͨ�� �ĳ�ʼ�� 
  //ADC10CTL1 |= INCH_10 + ADC10DIV_0 + ADC10SSEL_0 + CONSEQ_2; //�¶�ͨ��  ʱ��ΪĬ�ϵ�ADCʱ��5MHZ
    
    ADC10CTL1 |= INCH_5 + ADC10DIV_0 + ADC10SSEL_0 + CONSEQ_2;
    ADC10CTL0 |= ENC + ADC10SC;//��������ԭ���� ENC ��λ��CTL0�������޸�
  
}