T2CONbits.TON = 0;      //Timer_2 is OFF
    TMR2 = 0;               //resets Timer_2
    PR2 = 46;            //sets the maximum count for Timer_2
    T2CONbits.TCS = 0;      //choose FCY as clock source for Timer_2
    T2CONbits.TCKPS = 0x00; //sets the Timer_2 pre-scaler to 1
    IFS0bits.T2IF = 0;      //clears Timer_2 interrupt flag
    _T2IE = 1;              //enables Timer_2 Interrupts
    //T2CONbits.TON = 0;      //turns Timer_2 OFF     

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){    
    IFS0bits.T2IF = 0;
    if(j<11)
    {
        OC4CONbits.OCM = 4;
        OC2CONbits.OCM = 4;
        ++j;
    }
}
