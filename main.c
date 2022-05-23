

/**
 * main.c
 */

#include "Dio.h"

void portF()
{
    SYSCTL_RCGCGPIO_R |= 0x3F;
    SYSCTL_RCGCGPIO_R |= 0x00000020;      //Initialize clock to PORTF
      while((SYSCTL_PRGPIO_R&0x00000020) == 0){};   //safety for clock initialization
      GPIO_PORTF_LOCK_R = 0x4C4F434B;
      GPIO_PORTF_CR_R = 0x0E;       //Enable change to PORTF
      GPIO_PORTF_DEN_R = 0x0E;
}

int main(void)
{
    SYSCTL_RCGCGPIO_R |= 0x3F;
int x=0;
    Port_Init(PortConfig);
    x = Dio_ReadChannel(41);



    return 0;
}
