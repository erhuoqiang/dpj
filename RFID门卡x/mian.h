#ifndef __MIAN_H_
#define __MIAN_H_

//////////////////////////////////
//端口定义
/////////////////////////////////////////////////////////////////////
//MFRC500
sbit     MF522_RST  =    P1^1;                   //RC500??
sbit     MF522_NSS  =    P1^5;
sbit     MF522_SCK  =    P1^4;
sbit     MF522_SI   =    P1^3;
sbit     MF522_SO   =    P1^2;
//???
sbit     LED_GREEN  =    P1^0;  
/////////////////////////////////////////////////////////////////////
//函数原型
/////////////////////////////////////////////////////////////////////
void InitializeSystem(); 
void step();                                  
                                    

#define  OSC_FREQ        7372800L
#define  RCAP2_50us      65536L - OSC_FREQ/40417L
#define  RCAP2_1ms       65536L - OSC_FREQ/2000L
#define  RCAP2_10ms      65536L - OSC_FREQ/1200L
#define  TIME0_500us     65536L - OSC_FREQ/8000L
#define  TIME0_10ms      65536L - OSC_FREQ/200
void DelayMs(unsigned int _MS);

#endif


