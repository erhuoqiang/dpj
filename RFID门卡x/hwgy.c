#include<reg52.h>
#include"12864.h"

sbit led = P3^5;
sbit hongwai = P3^6;

uchar flag;

uchar hwgy()
{
    hongwai = 1;
   
    while(1)
     {            
         if(hongwai ==1)//��⵽�ߵ�ƽ�Ļ�����ʱһ��
         {
             Delay_ms(20);
             if(hongwai == 1)//�ٴμ��һ��
             {
                 led = 0;
				 flag = 1;
				 return flag;
				 break;
             }
        }
        else
        {
		led = 1;
		flag = 0;
		return flag;
		}
        
   }

}