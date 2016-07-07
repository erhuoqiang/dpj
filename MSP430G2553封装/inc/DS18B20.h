#ifndef DS18B20_H
#define DS18B20_H

extern unsigned char Init_18B20(void);
extern void Write_18B20(unsigned char wdata);
extern unsigned char Read_18B20(void);
extern float Temp(void);
extern float Do1Convert(void);

#endif

