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
函数名称：Init_18B20
功    能：对DS18B20进行复位操作
参    数：无
返回值  ：初始化状态标志：1--失败，0--成功
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
        Error = 1;          //初始化失败
    }
    else
    {
        Error = 0;          //初始化成功
    }
    DQ_out;
    DQ1;
    _EINT();
    
    delay_us(400);
    
    return Error;
}
/*******************************************
函数名称：Write_18B20
功    能：向DS18B20写入一个字节的数据
参    数：wdata--写入的数据
返回值  ：无
********************************************/
void Write_18B20(uchar wdata)
{
    uchar i;    
    _DINT();
    for(i = 0; i < 8;i++)
    {
        DQ0;
        delay_us(6);            //延时6us
        if(wdata & 0X01)    DQ1;
        else                DQ0;
        wdata >>= 1;
        delay_us(50);           //延时50us
        DQ1;
        delay_us(10);           //延时10us
    }
    _EINT();
}
/*******************************************
函数名称：Read_18B20
功    能：从DS18B20读取一个字节的数据
参    数：无
返回值  ：读出的一个字节数据
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
        delay_us(6);            //延时6us
        DQ1;
        delay_us(9);            //延时9us
        DQ_in;
        _NOP();
        if(DQ_val)   temp |= 0x80;
        delay_us(45);           //延时45us
        DQ_out;
        DQ1;
        delay_us(10);           //延时10us
    }
    _EINT();
    
    return  temp;
}

/*******************************************
函数名称：ReadTemp
功    能：从DS18B20的ScratchPad读取温度转换结果
参    数：无
返回值  ：读取的温度数值
********************************************/
float ReadTemp(void)
{
    uchar temp_data[2];
    int  temp;
    int i;
    float j;
    
    temp_data[0]=Read_18B20();  //温度低8位
    temp_data[1]=Read_18B20();  //温度高8位

	temp = temp_data[1];
	temp <<= 8;
	temp |= temp_data[0];
	i = temp;
        j = i * 0.0625;
    return  j;
}
/*******************************************
函数名称：Do1Convert
功    能：控制DS18B20完成一次温度转换
参    数：无
返回值  ：测量的温度数值
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
        delay_us(60000); //延时800ms以上
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

