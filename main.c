#define FCY 29491200L

/* Include List */
#include <libpic30.h>
#include <p30F4011.h>
#include <stdio.h>

/* Global Definitions */

/* For XC16 Ver <= 1.24 */
//_FOSC(CSW_FSCM_OFF & XT_PLL16); //oscillator at 16x crystal frequency
//_FWDT(WDT_OFF);                 //watchdog timer off
//_FBORPOR(MCLR_EN & PWRT_OFF);   //Turn MCLR pin ON and Energy Timers OFF

/* For XC16 Ver >= 1.24 */
#pragma config FCKSMEN=CSW_FSCM_OFF
#pragma config FOS=PRI
#pragma config FPR=XT_PLL16 
#pragma config WDT=WDT_OFF
#pragma config MCLRE=MCLR_EN
#pragma config FPWRT=PWRT_OFF

void __attribute__((interrupt, auto_psv)) _PWMInterrupt( void );

void PWM_init();

int main(void) {
    
    _TRISF0 = 0;

    
    PWM_init();
        
    while(1);
    
    return 0;
}

//Scale 1:1 e PTPER 818 => 18kHz pwm
void PWM_init(){

    PTCONbits.PTMOD=0b10;  //Working mode up-down counting 0b10
    PTCONbits.PTCKPS=0b00; //Source-Timer Prescaler (0,1,2,3) (1:1, 1:4, 1:16, 1:64)
    PTPER=818;             //max 32767
    PTMR=0;                //Set the timer to initial value 0
    PDC1=PTPER;            //if PDC is PTPER dc=50%, PDC is double of duty-cycle
    PDC2=PTPER/2;          //dc=25%
    PDC3=PTPER+PTPER/2;    //dc=75%
    PWMCON1bits.PEN1H=1;   //Enable PWMxH pins
    PWMCON1bits.PEN2H=1;
    PWMCON1bits.PEN3H=1;
    
    PWMCON1bits.PEN1L=0;   //Disable PWMxL pins, use as I/O for enabling L6234
    PWMCON1bits.PEN2L=0;
    PWMCON1bits.PEN3L=0;
    
    _TRISE2 = 0;
    _TRISE0 = 0;
    _TRISE4 = 0;
    
    _LATE2 = 1;
    _LATE0 = 1;
    _LATE4 = 1;        
    
    IEC2bits.PWMIE=1;      //Interrupt Enable, Flag is IFS2bits.PWMIF
    
    
    PTCONbits.PTEN=1;      //Enable PWM at end of Initiation routine
    
}

void __attribute__((interrupt, auto_psv)) _PWMInterrupt( void )               
{
    //your code here
    IFS2bits.PWMIF = 0; //Clear Interrupt Flag
    static int i=0;
    if(i==3)
    {
       _LATF0=(!_LATF0); //Every 3 interrupts toggle F0 bit, which connects to LED 26
       i=0;
    }
    i++;
    
    
    
}
