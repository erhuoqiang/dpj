#ifndef 12864_H
#define 12864_H

extern void Delayms(uint xms);
extern void SendByte_LCD(uchar date);
extern void SendCMD_LCD(uchar dat);
extern void SendDAT_LCD(uchar dat);
extern void DisplayC_LCD( uchar x, uchar * date,  uchar y);
extern void ClrScreen(void);
extern void LCD_Init(void);

#endif 