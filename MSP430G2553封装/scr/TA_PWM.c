//==========================TA_PWM.c========================//
#include"msp430g2553.h"
#define DEADTIME  20  //预设的死区时间，以TA的CLK做单位

//P1.5 P2.0  TAx.0只能开启模式0，4，5，通常拿来调频,所以TAX.0开启输出就直接输出模式4 %50空战比
#define TA00_SET    P1SEL |= BIT5; P1DIR |= BIT5; TA0CCTL0 |= OUTMOD_4   
#define TA01_SET    P1DIR |= BIT6; P1SEL |= BIT6    //P1.6
#define TA02_SET    P3SEL |= BIT0; P3DIR |= BIT0    //P3.0    G2553 20Pin无TA0.2  只有P1，P2
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
*  名  称：TA0_PWM_Init()
*  功  能：TA0定时器作为PWM发生器的初始化设置函数
*  入口参数：  Clk:时钟源  'S'=SMCLK;    'A'=ACLK ;    'E'=TACLK( 外部输入)；  'e'= TACLK取反
Div:时钟分频系数: 1/2/4/8
Mode0:通道0 默认输出4  'o'打开模式4输出 0关闭输出
Mode1:通道1的输出模式  'F'设为超前PWM（模式7），'B'滞后PWM（模式3），'D'
带死区增PWM（模式6），0=禁用
Mode2:通道2的输出模式  'F'设为超前PWM（模式7），'B'滞后PWM（模式3），'D'
带死区减PWM（模式2），0=禁用 
MC模式的选择由Mode1确定 但是 
设置输出带死区控制的PWM时，两通道均需使用，且均为死区模式。死区模式的MC 是增减 所以 会导致其他通道的频率受影响
*  出口参数：1表示设置成功，0表示参数错误，设置失败。
*  说  明：在调用PWM相关函数之前，需要调用该函数设置TA 的模式和时钟源。
*  范  例：TA0_PWM_Init('A',1,'F','F')TA时钟设为ACLK,通道1和通道2均为超前PWM输出
TA0_PWM_Init('S',4,'D','D')TA时钟设为SMCLK/4,  通道1为死区增PWM、通道2为死区
减PWM
TA0_PWM_Init('A',1,'F',0)TA时钟设为ACLK,通道1超前PWM输出，通道2不作TA用。
**************************************************** **************************************************/
char TA0_PWM_Init(char Clk, unsigned int Div, char Mode0, char Mode1, char Mode2)
{
  TA0CTL = 0;
  
  switch(Clk)
  {
  case 'S':case 's':  TA0CTL  |= TASSEL_2; break;        //SMCLK
  case 'A':case 'a':  TA0CTL  |= TASSEL_1; break;        //ACLK
  case 'E':  TA0CTL |= TASSEL_0; break;                 //外部输入TACLK
  case 'e':  TA0CTL |= TASSEL_3; break;                 //外部输入取反TACLK
  default:   return(0);                                //输入参数错误返回0，便于检测错误位置
  }
  
  switch(Div)
  {
  case  1:    TA0CTL |=ID_0;  break;                      //1
  case  2:    TA0CTL|=ID_1;  break;                        //2
  case  4:    TA0CTL|=ID_2;  break;                       //4
  case  8:    TA0CTL|=ID_3;  break;                        //8
  default  :   return(0);                 //设置参数有误，返回0
  }
  //PWM的输出类型是由MCx和OUTMODx 共同决定的   这里由Mode1 也就是通道1控制MCx
  //TA0CTL 是总控制端 控制MCx  还有3个TA0CTL0 TA0CTL1 TA0CTL2 控制OUTMODx
 
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
  case  '0' :  case     0:    TA01_OFF; break;       //如果设置为禁用TA0.1恢复为普通IO口
  default  :   return(0);            //设置参数有误，返回0
  }
  
    switch(Mode2)
  {
  case  'F' :  case  'f' :   TA0CCTL2 |= OUTMOD_7; TA02_SET;break;
  case  'B' :  case  'b' :    TA0CCTL2 |= OUTMOD_3; TA02_SET;break;
  case  'D' :  case  'd' :  TA0CCTL2 |= OUTMOD_6; TA02_SET;break;
  case  '0' :  case   0  :    TA02_OFF; break;       //如果设置为禁用TA0.1恢复为普通IO口
  default   :   return(0);   break;         //设置参数有误，返回0
  }
  
  return(1);
  
}


/*******************************************************************************
TA0_PWM_Init(char Clk, char Div, char Mode0, char Mode1, char Mode2);
的使用方法 ： 
CLK :     有 S,A,E,e 上面有介绍 
              
DIV :     4种分频1 2 4 8 
             
Mode0 :   'o' 'O'代表开启 OUTMOD 为模式4 %50的空战比 数字0表示禁用
              
Mode1:    控制本身通道的OUTMODx 分别有F,f(forward ),B,b(behind)，D，d（dead） 还有0 禁用
          同时Mode1还控制 3个通道的MCx F,f，B,b为 增模式MC1 D,d为增减计数模式，其他为连续计数模式
          不掉用函数的前提下 默认MC0 关闭计数
              
Mode2 :   和Mode1一样 但是MC只有MODE1可以控制 MODE2只控制 通道2的OUTMODx
          别有F,f(forward ),B,b(behind)，D，d（dead） 还有0 禁用 
*******************************************************************************/






/******************************************************************************************************
*  名  称：TA1_PWM_Init()
*  功  能：TA0定时器作为PWM发生器的初始化设置函数
*  入口参数：  Clk:时钟源  'S'=SMCLK;    'A'=ACLK ;    'E'=TACLK( 外部输入)；  'e'= TACLK取反
Div:时钟分频系数: 1/2/4/8
Mode0:通道0 默认输出4  'o'打开模式4输出 0关闭输出
Mode1:通道1的输出模式  'F'设为超前PWM（模式7），'B'滞后PWM（模式3），'D'
带死区增PWM（模式6），0=禁用
Mode2:通道2的输出模式  'F'设为超前PWM（模式7），'B'滞后PWM（模式3），'D'
带死区减PWM（模式2），0=禁用 
MC模式的选择由Mode1确定 但是 
设置输出带死区控制的PWM时，两通道均需使用，且均为死区模式。死区模式的MC 是增减 所以 会导致其他通道的频率受影响
*  出口参数：1表示设置成功，0表示参数错误，设置失败。
*  说  明：在调用PWM相关函数之前，需要调用该函数设置TA 的模式和时钟源。
*  范  例：TA0_PWM_Init('A',1,'F','F')TA时钟设为ACLK,通道1和通道2均为超前PWM输出
TA1_PWM_Init('S',4,'D','D')TA时钟设为SMCLK/4,  通道1为死区增PWM、通道2为死区
减PWM
TA1_PWM_Init('A',1,'F',0)TA时钟设为ACLK,通道1超前PWM输出，通道2不作TA用。
**************************************************** **************************************************/
char TA1_PWM_Init(char Clk, unsigned int Div, char Mode0, char Mode1, char Mode2)
{
  TA1CTL = 0;
  
  switch(Clk)
  {
  case 'S':case 's':  TA1CTL  |= TASSEL_2; break;        //SMCLK
  case 'A':case 'a':  TA1CTL  |= TASSEL_1; break;        //ACLK
  case 'E':  TA1CTL |= TASSEL_0; break;                 //外部输入TACLK
  case 'e':  TA1CTL |= TASSEL_3; break;                 //外部输入取反TACLK
  default:   return(0);                                //输入参数错误返回0，便于检测错误位置
  }
  
  switch(Div)
  {
  case  1:    TA1CTL |=ID_0;  break;                      //1
  case  2:    TA1CTL|=ID_1;  break;                        //2
  case  4:    TA1CTL|=ID_2;  break;                       //4
  case  8:    TA1CTL|=ID_3;  break;                        //8
  default  :   return(0);                 //设置参数有误，返回0
  }
  //PWM的输出类型是由MCx和OUTMODx 共同决定的   这里由Mode1 也就是通道1控制MCx
  //TA0CTL 是总控制端 控制MCx  还有3个TA0CTL0 TA0CTL1 TA0CTL2 控制OUTMODx
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
  case  '0' :  case     0:    TA11_OFF; break;       //如果设置为禁用TA0.1恢复为普通IO口
  default  :   return(0);            //设置参数有误，返回0
  }
  
    switch(Mode2)
  {
  case  'F' :  case  'f' :   TA1CCTL2 |= OUTMOD_7; TA12_SET;break;
  case  'B' :  case  'b' :    TA1CCTL2 |= OUTMOD_3; TA12_SET;break;
  case  'D' :  case  'd' :  TA1CCTL2 |= OUTMOD_6; TA12_SET;break;
  case  '0' :  case   0  :    TA12_OFF; break;       //如果设置为禁用TA0.1恢复为普通IO口
  default   :   return(0);   break;         //设置参数有误，返回0
  }
  
  return(1);
  
}


/*******************************************************************************
TA1_PWM_Init(char Clk, char Div, char Mode0, char Mode1, char Mode2);
的使用方法 ： 
CLK :     有 S,A,E,e 上面有介绍 
              
DIV :     4种分频1 2 4 8 
             
Mode0 :   'o' 'O'代表开启 OUTMOD 为模式4 %50的空战比 数字0表示禁用
              
Mode1:    控制本身通道的OUTMODx 分别有F,f(forward ),B,b(behind)，D，d（dead） 还有0 禁用
          同时Mode1还控制 3个通道的MCx F,f，B,b为 增模式MC1 D,d为增减计数模式，其他为连续计数模式
          不掉用函数的前提下 默认MC0 关闭计数
              
Mode2 :   和Mode1一样 但是MC只有MODE1可以控制 MODE2只控制 通道2的OUTMODx
          别有F,f(forward ),B,b(behind)，D，d（dead） 还有0 禁用 
*******************************************************************************/




/****************************************************************************
*  名  称：TA_PWM_SetPeriod()
*  功  能：设置PWM发生器的周期
*  入口参数：Period:周期(0~65535)  时钟个数 三种模式MC 连续型加到65535清0 
    增计数型到TACCR0 变0 增减计数型到TACCR0 递减
*  出口参数：1：设置成功    0：设置失败
*  说  明：  普通PWM与带死区PWM周期相差一倍
*  范  例：TA0_PWM_SetPeriod(500)设置PWM方波周期为500或1000个时钟周期
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
*  名  称：TA0_PWM_SetPermill()
*  功  能：设置PWM输出的占空比(千分比)
*  入口参数：Channel: 当前设置的通道号  1/2
Duty: PWM高电平有效时间的千分比  (0~1000)，
*  出口参数：1设置成功，0设置失败
*  说  明: 1000=100.0%  500=50.0%  ，依次类推。死区模式时，两channel同时设定。
*  范  例: TA0_PWM_SetPermill(1,300)设置PWM通道1方波的占空比为30.0%
TA0_PWM_SetPermill(2,825)设置PWM通道2方波的占空比为82.5%
************************************************************************************* *****************/
char TA0_PWM_SetPermill(char  Channel,unsigned  int  Duty)
{
  unsigned  char  Mod = 0;
  unsigned  int  DeadPermill=0;
  unsigned  long  int  Percent=0;          //防止乘法运算时溢出
  Percent=Duty;
  DeadPermill=((DEADTIME*1000)/TACCR0);      //将绝对死区时间换算成千分比死区时间 
  switch (Channel)              //先判断出通道的工作模式
  {
  case  1:
    Mod = (TA0CCTL1& 0x00e0)>>5;break;     //读取输出模式，OUTMOD0位于5-7位
  case  2:
  Mod = (TA0CCTL2 & 0x00e0)>>5;break;    //读取输出模式，OUTMOD1位于5-7位
  default:  return(0);
  
  }
  switch(Mod)                //根据模式设定TACCRx
  {
    case  2: case  6:   
    //-----死区模式2,6时，需要判断修正死区时间，且同时设定TA0CCR1/2  的值-----
      {
      
        if((1000-2*Percent)<= DeadPermill)   //预留死区时间
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
  case  3:      //占空比一律为正脉宽，所以需要  TA0CCR0减去占空比
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
*  名  称：TA1_PWM_SetPermill()
*  功  能：设置PWM输出的占空比(千分比)
*  入口参数：Channel: 当前设置的通道号  1/2
Duty: PWM高电平有效时间的千分比  (0~1000)，
*  出口参数：1设置成功，0设置失败
*  说  明: 1000=100.0%  500=50.0%  ，依次类推。死区模式时，两channel同时设定。
*  范  例: TA1_PWM_SetPermill(1,300)设置PWM通道1方波的占空比为30.0%
TA1_PWM_SetPermill(2,825)设置PWM通道2方波的占空比为82.5%
************************************************************************************* *****************/
char TA1_PWM_SetPermill(char  Channel,unsigned  int  Duty)
{
  unsigned  char  Mod = 0;
  unsigned  int  DeadPermill=0;
  unsigned  long  int  Percent=0;          //防止乘法运算时溢出
  Percent=Duty;
  DeadPermill=((DEADTIME*1000)/TA1CCR0);      //将绝对死区时间换算成千分比死区时间 
  switch (Channel)              //先判断出通道的工作模式
  {
  case  1:
    Mod = (TA1CCTL1& 0x00e0)>>5;break;     //读取输出模式，OUTMOD0位于5-7位
  case  2:
  Mod = (TA1CCTL2 & 0x00e0)>>5;break;    //读取输出模式，OUTMOD1位于5-7位
  default:  return(0);
  
  }
  switch(Mod)                //根据模式设定TACCRx
  {
    case  2: case  6:   
    //-----死区模式2,6时，需要判断修正死区时间，且同时设定TA0CCR1/2  的值-----
      {
      
        if((1000-2*Percent)<= DeadPermill)   //预留死区时间
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
  case  3:      //占空比一律为正脉宽，所以需要  TA0CCR0减去占空比
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