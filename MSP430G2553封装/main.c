#include "msp430g2553.h"
#include"msp430_conf.h"


#define uint  unsigned int
#define uchar   unsigned char

 //ENABLE 1.7 DIR+ 1.3 DIR- 1.4 pwm 2.2 
#define Moter_OFF         P1OUT |= BIT7
#define Moter_ON          P1OUT &= ~BIT7
#define Moter_DIR_CW      P1OUT |= BIT3;   P1OUT &= ~BIT4    //3+ 4- 顺
#define Moter_DIR_CCW     P1OUT |= BIT4;   P1OUT &= ~BIT3

float Get_T = 0;  //温度
float Get_V = 0;  //电压
char *str[2] = {"514514","dian3"};//发送字符串数组
char temp[20]= "514";//12864
char temp1[10] = "";
int value;
/**********************************************
切记： 记得打开总中断 和设定GPIO 和外设的初始化
*********************************************/
void main()
{ 
  WDTCTL = WDTPW + WDTHOLD;//关闭看门狗 提醒：如果有中断要开总中断

  /*************************************************
    DCO=1MHZ    MCLK 和  SMCLK 复位默认DCO    0分频
   进去设置即可调MCLK SMCLK 改动后记得修改延时 和UART的波特率和时钟
 ***************************************************/
   DCO_Init();
   
  
  GPIO_Init();  //记得去里面开IO
 
  /***************************************************
   选择看门狗定时复位 还是定时中断 进去里面改参数就行
  **************************************************/
  // WDT_Init(); //默认设置为16MS 
  
  /*******************************************
  PWM 函数内部时钟和GPIO 都设定好所以无需调用Init
  **********************************************/
  // TA0_PWM_SetPeriod(65535);  
   //TA1_PWM_SetPeriod(65535);
   //TA0CTL |= TACLR;//
 // TA1CTL |= TACLR;
  // TA0_PWM_Init('A',1,'o','f',0);
  //TA1_PWM_Init('A',1,'o','f',0);// 通道0 如果开启无论什么MC 都是50%
  //TA0_PWM_SetPermill(1, 100);////MSP430G2553只有1通道可以调空占比
  //TA1_PWM_SetPermill(1, 100); //（通道，千分比）

  /*****************************************
                   串口通讯
  *****************************************/
  //UART_A0_Init(); //9600 比特率 0验证 1停止   IO口已初始化
   //Uart_A0_Send_Str(str[1]);//发送字符串  如Uart_A0_Send_Str(str[1]);

   /*****************************************************
  定时器 0.1S为 单位 如1MS Timer_A0_Init(10);
  如果时间不够 可以去DCO 中的BCSCTL1中改DIVA 
  *******************************************************/
 // Timer_A0_Init(10);
    //Timer_A1_Init(20);
  
  /**********************************************************
            ADC进去选择通道  自带温度传感器 A10 不需要设置GPIO
            基准电压改了 记得也要到中断去修改
        如果需要多通道 或者 定时采样 可以 用定时器采样
  *********************************************************/
    //ADC_Init();
    
 
   
   /********************************************
              12864 IO 到GPIO 里开
  *******************************************/
    LCD_Init();   // 12864 IO自己开
   
    //Get_T = Do1Convert(); //获取DS18B20温度
   // sprintf(temp,"温度%5.2fHZ",Get_T);//temp是数组名 将字符串和变量转换成数组
    //sscanf(temp,"%d",&value); //将字符串转换成数字 
    DisplayC_LCD(1, temp, 0);//（行，数组名，列（0~7））
   
  _BIS_SR(GIE); //总中断记得开
  while(1){;}
  //_BIS_SR(LPM3_bits);	// 如果开低功耗模式一定要放最后 因为提前开CPU还没吧程序执行完就睡眠
  
}









/****************************************************************
功能 :中断 记得开总中断GIE 
****************************************************************/
//P1外部中断 因为P1所有IO 公用一个向量 所以需要用到 按键检测KEY_Dect
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
  
  P1_IODect();
  P1IFG = 0x00;
  
}
//P2外部中断
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
  //ADC 定时采样 
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

//看门狗 定时器 初始化在GPIO_Init里面
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
   // P1OUT ^= BIT6 + BIT0;
  // Key_SM();
   //
}

//UART 接收 和发送中断
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
   
   
 // Uart_A0_Send_Str(temp);//发送字符串 

   while (!(IFG2&UCA0TXIFG)); 
    UCA0TXBUF = UCA0RXBUF;      // 接收后立即发送
 /* 
  //12864输出接收字符串 给temp 
 static unsigned char num; 
  while (!(IFG2&UCA0TXIFG));    // TXBUF 要先把之前的数据发给移位寄存器后TXIFG才会置1 所以要等
   temp[num++] = UCA0RXBUF;      //RXBUF 只有接受完后才会置位RXIFG 所以才会进中断
  if(temp[num-1] == '*')
  {
    //输入'.'表示复位和清空Temp  为了避免上次比这次数据个数多 产生数字保留 可在这次输入多打几个空格
    num = 0; 
  
  }*/
   IFG2&=~UCA0RXIFG;
   
}

#pragma vector=USCIAB0TX_VECTOR//接收终端在UART_A0_Init()里面还没开
__interrupt void USCI0TX_ISR(void)
{
 //IFG2&=~UCA0TXIFG;
}

//ADC
#pragma vector=ADC10_VECTOR 
__interrupt void ADC10IN(void)
{
  //unsigned char table[]={0};  //需要发给串口的时候才需要开
 // int a=0,b=0,c=0,d=0;
  static char num;
  
  if(num < 10)
  {
    num++;
  Get_V += ADC10MEM*3.54/1024/10;  //电压  基准电压改变时需要修改
   //Get_V += (ADC10MEM*422.5/1024  -  277.7)/10;  //温度
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
      while(UCA0STAT&BUSY);                   //判断发送是否完成
      UCA0TXBUF = table[i];  
   } */
   Get_V = 0;
  }
}