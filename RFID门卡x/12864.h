
#define uchar unsigned char
#define uint unsigned int

extern void Delay_ms(uint xms);
extern void SendByte_LCD(uchar date);
extern void SendCMD_LCD(uchar dat);
extern void SendDAT_LCD(uchar dat);
extern void DisplayC_LCD( uchar x, uchar * date,  uchar y);
extern void ClrScreen(void);
extern void LCD_Init(void);

