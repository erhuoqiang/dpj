//((double)8000000) 语句里 8000000 修改成你当前MSP430 CPU的主频频率，即CPU的MCLK。 
#define CPU_F                               ((double)1000000) //8M
#define delay_us(x)                       __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x)                      __delay_cycles((long)(CPU_F*(double)x/1000.0)) 