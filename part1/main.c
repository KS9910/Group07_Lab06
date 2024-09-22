/* SysTick memory-mapped registers */
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

#define SW1 (1U << 4)           // switch pin 4
#define SW2 (1U << 0)         //switch pin 3

#define STCTRL *((volatile long *) 0xE000E010)    // control and status
#define STRELOAD *((volatile long *) 0xE000E014)    // reload value
#define STCURRENT *((volatile long *) 0xE000E018)    // current value

#define COUNT_FLAG  (1 << 16)   // bit 16 of CSR automatically set to 1
                                //   when timer expires
#define ENABLE      (1 << 0)    // bit 0 of CSR to enable the timer
#define CLKINT      (1 << 2)    // bit 2 of CSR to specify CPU clock


#define CLOCK_MHZ 16
void initial_setup(void);
void Delay(int);
void GPIO_Handler(int, int);

void GPIO_Handler(int i, int d) {
    if (GPIO_PORTF_MIS_R & 0x10){
        GPIO_PORTF_ICR_R |= SW1;   // Clear the interrupt flag for SW1
        i = i+1;
        d = d-1;
    }
    if (GPIO_PORTF_MIS_R & 0x01){
            GPIO_PORTF_ICR_R |= SW2;   // Clear the interrupt flag for SW1
            i = i-1;
            d = d + 1;
        }
}

void Delay(int s)
{
    STRELOAD = s;                       // reload value for 'ms' milliseconds
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
    int i = 10;
    int d = 10;
        initial_setup();
//        int i = 10;

        int c = 8;  // no. of ticks for 5%
        while(1){
            GPIO_PORTF_DATA_R = 0x02;
            Delay(i*c);
            GPIO_PORTF_DATA_R = 0x00;
            Delay(d*c);
        }
}

void initial_setup(void) {
    // Enable clock for GPIO port F
    SYSCTL_RCGC2_R |= 0x00000020;       /* enable clock to GPIOF */
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     /* unlock commit register */
    GPIO_PORTF_CR_R = 0x1F;             /* make PORTF0 configurable */
    GPIO_PORTF_DEN_R = 0x1F;    //1E        /* set PORTF pins 4 pin */
    GPIO_PORTF_DIR_R = 0x0E;            /* set PORTF4 pin as input user switch pin */
    GPIO_PORTF_PUR_R = 0x11;    //10        /* PORTF4 is pulled up */

    // Interrupt configuration
    GPIO_PORTF_IS_R &= (~SW1 & ~SW2);    // Make PF4 edge-sensitive
    GPIO_PORTF_IBE_R &= (~SW1 & ~SW2);   // Disable both edges, we use IEV
    GPIO_PORTF_IEV_R &= (~SW1 & ~SW2);   // Falling edge trigger (when switch is pressed)
    GPIO_PORTF_ICR_R |= (SW1 | SW2);    // Clear any prior interrupt
    GPIO_PORTF_IM_R |= (SW1 | SW2);     // Unmask interrupt for SW1

    // Enable interrupt in NVIC
    NVIC_EN0_R |= (1 << 30);    // Enable IRQ30 (Port F)

}
