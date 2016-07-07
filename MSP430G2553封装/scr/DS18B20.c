#include <msp430g2553.h>

#define  uchar unsigned char
#define  uint unsigned int

#define CPU_F ((double)1000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define DQ1 P1OUT |= BIT5
#define DQ0 P1OUT &= ~BIT5
#define DQ_in   P1DIR &= ~BIT5
#define DQ_out  P1DIR |= BIT5
#define DQ_val  (P1IN & BIT5)

/*******************************************
�������ƣ�Init_18B20
��    �ܣ���DS18B20���и�λ����
��    ������
����ֵ  ����ʼ��״̬��־��1--ʧ�ܣ�0--�ɹ�
********************************************/
uchar Init_18B20(void)
{
    uchar Error;
    
    DQ_out;
    _DINT();
    DQ0;
    delay_us(500);
    DQ1;
    delay_us(55);
    DQ_in;
    _NOP();
    if(DQ_val)      
    {
        Error = 1;          //��ʼ��ʧ��
    }
    else
    {
        Error = 0;          //��ʼ���ɹ�
    }
    DQ_out;
    DQ1;
    _EINT();
    
    delay_us(400);
    
    return Error;
}
/*******************************************
�������ƣ�Write_18B20
��    �ܣ���DS18B20д��һ���ֽڵ�����
��    ����wdata--д�������
����ֵ  ����
********************************************/
void Write_18B20(uchar wdata)
{
    uchar i;    
    _DINT();
    for(i = 0; i < 8;i++)
    {
        DQ0;
        delay_us(6);            //��ʱ6us
        if(wdata & 0X01)    DQ1;
        else                DQ0;
        wdata >>= 1;
        delay_us(50);           //��ʱ50us
        DQ1;
        delay_us(10);           //��ʱ10us
    }
    _EINT();
}
/*******************************************
�������ƣ�Read_18B20
��    �ܣ���DS18B20��ȡһ���ֽڵ�����
��    ������
����ֵ  ��������һ���ֽ�����
********************************************/
uchar Read_18B20(void)
{
    uchar i;
    uchar temp = 0;
    
    _DINT();
    for(i = 0;i < 8;i++)
    {
        temp >>= 1;
        DQ0;
        delay_us(6);            //��ʱ6us
        DQ1;
        delay_us(9);            //��ʱ9us
        DQ_in;
        _NOP();
        if(DQ_val)   temp |= 0x80;
        delay_us(45);           //��ʱ45us
        DQ_out;
        DQ1;
        delay_us(10);           //��ʱ10us
    }
    _EINT();
    
    return  temp;
}

/*******************************************
�������ƣ�ReadTemp
��    �ܣ���DS18B20��ScratchPad��ȡ�¶�ת�����
��    ������
����ֵ  ����ȡ���¶���ֵ
********************************************/
float ReadTemp(void)
{
    uchar temp_data[2];
    int  temp;
    int i;
    float j;
    
    temp_data[0]=Read_18B20();  //�¶ȵ�8λ
    temp_data[1]=Read_18B20();  //�¶ȸ�8λ

	temp = temp_data[1];
	temp <<= 8;
	temp |= temp_data[0];
	i = temp;
        j = i * 0.0625;
    return  j;
}
/*******************************************
�������ƣ�Do1Convert
��    �ܣ�����DS18B20���һ���¶�ת��
��    ������
����ֵ  ���������¶���ֵ
********************************************/
float Do1Convert(void)
{
    uchar i;
   float t;
    do
    {
        i = Init_18B20();
    }
    while(i);
    Write_18B20(0xCC);
    Write_18B20(0x44);
    for(i = 20;i > 0;i--)  
        delay_us(60000); //��ʱ800ms����
    do
    {
        i = Init_18B20();
    }
    while(i);
    Write_18B20(0xCC);
    Write_18B20(0xBE);
    t = ReadTemp();
    return t;
}

