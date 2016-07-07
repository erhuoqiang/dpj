#include"msp430g2553.h"


//基准电压变后 记得去需改下 中断中的数据CODE
//如果用定时器采样记得关中断
void ADC_Init()
{
    ADC10CTL0 |= SREF_0 + ADC10SHT_3 + MSC + ADC10ON + ADC10IE;  //时钟
 
   // ADC10CTL0 |= SREF_0 + ADC10ON + MSC + REFON + ADC10SHT_3 + SREF_1 + ADC10IE; //选择温度通道 的初始化 
  //ADC10CTL1 |= INCH_10 + ADC10DIV_0 + ADC10SSEL_0 + CONSEQ_2; //温度通道  时钟为默认的ADC时钟5MHZ
    
    ADC10CTL1 |= INCH_5 + ADC10DIV_0 + ADC10SSEL_0 + CONSEQ_2;
    ADC10CTL0 |= ENC + ADC10SC;//这个放最后原因是 ENC 置位后CTL0不允许修改
  
}