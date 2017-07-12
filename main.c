#define FCY 4*7372800   //instruction frequency in Hz
#define FOSC 7372800    //crystal vibration frequency in Hz
#include <p30F4011.h>   //defines dos dspic registers
#include <stdio.h>	    //standart IO library C
#include <string.h>     //string related functions library
#include <libpic30.h>	//C30 compiler definitions
#include <xc.h>         //Compiler

void PWM_init();

int main(void) {
    
    PWM_init();
    
    while(1);
    
    return 0;
}
void PWM_init(){

    PTCONbits.PTMOD=0b10;  //Working mode up-down counting 0b10
    PTCONbits.PTCKPS=0b01; //Source-Timer Prescaler (0,1,2,3) (1:1, 1:4, 1:16, 1:64)
    PTPER=1000; //max 32767
    PTMR=0;     //Set the timer to incial value 0
    PDC1=500*2; //PDC is double of duty-cycle
    //PDC2=32700/4;
    //PDC3=PDC1;
    PWMCON1bits.PEN1H=1;
    
    PTCONbits.PTEN=1;
    
}
