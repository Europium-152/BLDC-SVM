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
    
    
    return 0;
}
void PWM_init(){

    PTCONbits.PTMOD=0b10;
    PTPERbits.PTPER=32700; //max 32767
    PDC1=32700/2;
    PDC2=32700/2;
    PDC3=PDC1;
    
}