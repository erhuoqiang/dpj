//((double)8000000) ����� 8000000 �޸ĳ��㵱ǰMSP430 CPU����ƵƵ�ʣ���CPU��MCLK�� 
#define CPU_F                               ((double)1000000) //8M
#define delay_us(x)                       __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x)                      __delay_cycles((long)(CPU_F*(double)x/1000.0)) 