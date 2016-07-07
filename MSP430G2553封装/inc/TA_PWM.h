//========================  TA_PWM.h====================//
#ifndef TA_PWM_H_
#define TA_PWM_H_

extern  char TA0_PWM_Init();//确定 PWM 的输出通道是哪几个，同时确定PWM波的类型，和一些初始化
extern  char TA0_PWM_SetPeriod();//确定PWM波的频率
extern  char TA0_PWM_SetPermill();//确定PWM波的空占比  调用TA0_PWM_Init();才可以调用 因为空占比函数要调用通道和MOD 
extern  char TA1_PWM_Init();
extern  char TA1_PWM_SetPeriod();
extern  char TA1_PWM_SetPermill();




#endif 

