
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#define STCTRL *((volatile long *) 0xE000E010)    // control and status
#define STRELOAD *((volatile long *) 0xE000E014)    // reload value
#define STCURRENT *((volatile long *) 0xE000E018)    // current value

#define COUNT_FLAG  (1 << 16)   // bit 16 of CSR automatically set to 1
                                //   when timer expires
#define ENABLE      (1 << 0)    // bit 0 of CSR to enable the timer
#define CLKINT      (1 << 2)    // bit 2 of CSR to specify CPU clock

#define CLOCK_MHZ 16
void Delay(int ms)
{
    STRELOAD =ms;                       // reload value for 'ms' milliseconds
    STCTRL |= (CLKINT | ENABLE);        // set internal clock, enable the timer

    while ((STCTRL & COUNT_FLAG) == 0)  // wait until flag is set
    {
        ;   // do nothing
    }
    STCTRL = 0;                // stop the timer

    return;
}
int main(void)
{
        SYSCTL_RCGC2_R |= 0x00000020;       /* enable clock to GPIOF */
        GPIO_PORTF_LOCK_R = 0x4C4F434B;     /* unlock commit register */
        GPIO_PORTF_CR_R = 0x1F;             /* make PORTF0 configurable */
        GPIO_PORTF_DEN_R = 0x1F;    //1E        /* set PORTF pins 4 pin */
        GPIO_PORTF_DIR_R = 0x0E;            /* set PORTF4 pin as input user switch pin */
        GPIO_PORTF_PUR_R = 0x11;
        //int see=1;
        int constant_on=80;
        int constant_off=80;//by calculation




            int i;
            int x=0;
            int prev=0,present=0,present_long=0;               /*to detect the edge*/
            while(1){
                present=(GPIO_PORTF_DATA_R & 0x10);
            if(prev==0x10 & present==0x0)       /*a leading edge is detected*/
            {
                   x=1;                          /*sequential setting of the LEDs*/
                   for(i=0;i<800000;i++){}  //long check
                    present_long=(GPIO_PORTF_DATA_R & 0x10);
                    if(present==0x0 & present_long==0x0){
                        x=2;
                        for(i=0;i<800;i++){}

                    }
                    if(x%3==1)
                                     {constant_on=constant_on+8;
                                      constant_off=constant_off-8;}

                    if(x%3==2)
                                     {constant_on=constant_on-8;
                                     constant_off=constant_off+8;}
                    if(constant_on<2 & constant_off>75 ){
                                                      constant_on=1;
                                                      constant_off=160-1;
                                                                             }
                    if(constant_off<2 & constant_on>75){
                                                      constant_on=160-1;
                                                      constant_off=1;
                                                                             }

            }

//

            GPIO_PORTF_DATA_R = 0x04;
                    Delay(constant_on);
                    GPIO_PORTF_DATA_R = 0x00;
                    Delay(constant_off);
                 prev=present;



                }

                }


