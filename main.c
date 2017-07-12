#define FCY 29491200L

/* Include List */
#include <libpic30.h>
#include <p30F4011.h>
#include <stdio.h>

/* For XC16 Ver >= 1.24 */
#pragma config FCKSMEN=CSW_FSCM_OFF
#pragma config FOS=PRI
#pragma config FPR=XT_PLL16 
#pragma config WDT=WDT_OFF
#pragma config MCLRE=MCLR_EN
#pragma config FPWRT=PWRT_OFF


void __attribute__((interrupt, auto_psv)) _PWMInterrupt( void )               
{
    //your code here
    IFS2bits.PWMIF = 0;
}

