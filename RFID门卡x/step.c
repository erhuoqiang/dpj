#include<reg52.h>
#define Step 500
void step()
{
    unsigned int i;
    P1 = 0x01;//0000 0001
    for(i = 0;i<Step;i++);
    P1 = 0x02;//0000 0010
    for(i = 0;i<Step;i++);
    P1 = 0x04;//0000 0100
    for(i = 0;i<Step;i++);
    P1 = 0x08;//0000 1000
    for(i = 0;i<Step;i++);
     
}