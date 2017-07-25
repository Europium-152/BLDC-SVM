#include <libpic30.h>
#include <p30F4011.h>
#include <stdio.h>

#define LED1 _LATC13
#define LED2 _LATC14
#define LED3 _LATF1
#define LED4 _LATF0


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

#define LAST_TABLE_INDEX 127
#define ANGLE_RESOLUTION 128

#define MY_PTPER 256
#define INVERSE_FACTOR 301979822.0
#define ACELERACAO 0.5
#define PR2_INITIAL_VALUE 65000
#define PR2_FINAL_VALUE 2800
#define MY_PR3 30000

#define GM_1UD_PR 10000
#define GM_1UD_VALUE 5

#define GM_2UD_PR 5000
#define GM_2UD_VALUE 6

#define GM_3UD_PR 3200
#define GM_3UD_VALUE 6

float g_inverse=INVERSE_FACTOR/ACELERACAO, 
      g_truePR2=PR2_INITIAL_VALUE;

//Global Variables
int g_ang=0,           //Converted angle, stores the angle in units of its resolution, 
                       //Working with 128 resolution, the total 360 circle is 128*60=7680 units long
    g_angIncrement=8,  //angle increment step Magnetic Fiel freq. is FPWM/atCount*(angIncrement/7680)    
    g_sector=0,       //circle sector 0 to 5
    g_m=4;            //Power factor 1-8
    

     
//Lookup table for sin(g_angle)*1024
int tab[128] = {0, 8, 17, 25, 34, 42, 50, 59, 67, 75, 84, 92, 100, 109, 117, 125, 134, 142, 150, 159, 167, 175, 183, 192, 200, 208, 
 216, 224, 233, 241, 249, 257, 265, 273, 281, 289, 297, 305, 313, 321, 329, 337, 345, 353, 361, 369, 376, 384, 392, 
 400, 407, 415, 423, 430, 438, 445, 453, 460, 468, 475, 483, 490, 497, 505, 512, 519, 526, 534, 541, 548, 555, 562, 
 569, 576, 583, 590, 596, 603, 610, 617, 623, 630, 637, 643, 650, 656, 662, 669, 675, 681, 688, 694, 700, 706, 712, 
 718, 724, 730, 736, 742, 747, 753, 759, 764, 770, 775, 781, 786, 792, 797, 802, 807, 812, 817, 822, 827, 832, 837, 
 842, 847, 851, 856, 861, 865, 870, 874, 878, 883};

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void);

void setPDC(int setor, int angB);

void TIMER2_config();

void TIMER3_config();

void PWM_init();

int main(void) {
    
    
    
    _TRISC13= 0; //LED1
    _TRISC14= 0; //LED2
    _TRISF1 = 0; //LED3
    _TRISF0 = 0; //LED4
    
    _LATC13= 1; //LED1
    _LATC14= 1; //LED2
    _LATF1 = 1; //LED3
    _LATF0 = 1; //LED4
    
    _TRISD3 = 0;
    _TRISD1 = 0;
    
    _LATD3 = 0;
    _LATD1 = 0;
    
    PWM_init();
    
    TIMER2_config();
    TIMER3_config();
    
    
    
    while(1);
    
    return 0;
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){    
   
    
         _LATD3=1;
    
    IFS0bits.T3IF = 0;      
    
    static int gm_1UD=1, gm_2UD=1, gm_3UD=1;
   

    g_truePR2-=(g_truePR2*g_truePR2/g_inverse);
     

        if(g_truePR2<PR2_FINAL_VALUE )
    {
        _T3IE = 0; //Disable aceleration
        LED4=0;
        _LATD3=1; //Toggle D3/OC4 pin for viewing in osciloscope
        //start_breaking();
        //regen();
    }    
       
    else{
    
        PR2=(unsigned int)g_truePR2;
       
        if(g_truePR2<GM_1UD_PR && gm_1UD){
            LED1=0;
            gm_1UD=0;
            g_m=GM_1UD_VALUE;
        }
        
        if(g_truePR2<GM_2UD_PR && gm_2UD){
            LED2=0;
            gm_2UD=0;
            g_m=GM_2UD_VALUE;
        }
        
        if(g_truePR2<GM_3UD_PR && gm_3UD){
            LED3=0;
            gm_3UD=0;
            g_m=GM_3UD_VALUE;
        }

        }
    _LATD3=0;
}

void setPDC(int setor, int angB) {
    
	static int tA, tB, t02, angA;
//    static int tab_angA,tab_angB;
	angA = LAST_TABLE_INDEX - angB;
//    tab_angA=tab[angA];
//    tab_angB=tab[angB];
	tB = (tab[angB]*g_m)/16; 
	tA = (tab[angA]*g_m)/16;
	t02 = (2*MY_PTPER - tA - tB)/2;

	//switch
    switch(setor){
	        case 0:
				PDC1 = (t02); //Phase R duty cycle T0/2
				PDC2 = (tA + t02);//Phase Y duty cycle T0/2+TA
				PDC3 = (2*MY_PTPER-t02);//Phase B duty cycle TS-T0/2
	            break;
	        case 1:    
				PDC1 = (t02 + tB);//Phase R duty cycle T0/2+TB
				PDC2 = (t02);//Phase Y duty cycle T0/2
				PDC3 = 2*MY_PTPER-t02;//Phase B duty cycle TS-T0/2    
	            break;
	        case 2:
				PDC1 = 2*MY_PTPER-t02;//Phase R duty cycle TS-T0/2
				PDC2 = (t02);//Phase Y duty cycle T0/2
				PDC3 = (t02 + tA); //Phase B duty cycle T0/2+TA
	            break;
	        case 3:
				PDC1 = 2*MY_PTPER-t02;//Phase R duty cycle TS-T0/2
				PDC2 = (tB + t02);//Phase Y duty cycle T0/2+TB
				PDC3 = (t02);//Phase B duty cycle T0/2
	            break;
	        case 4:
				PDC1 = (t02 + tA);//Phase R duty cycle T0/2+TA
				PDC2 = 2*MY_PTPER-t02;//Phase Y duty cycle TS-T0/2
				PDC3 = (t02);//Phase B duty cycle T0/2
	            break;
	        case 5:
				PDC1 = (t02);//Phase R duty cycle T0/2
				PDC2 = 2*MY_PTPER-t02;//Phase Y duty cycle TS-T0/2
				PDC3 = (t02 + tB);//Phase B duty cycle T0/2+TB
	            //break;							
	  }
//	       switch(setor){
//	        case 0:
//				PDC1 = (2*MY_PTPER - t02); //Phase R duty cycle T0/2
//				PDC2 = (tB + t02);//Phase Y duty cycle T0/2+TA
//				PDC3 = t02;//Phase B duty cycle TS-T0/2
//	            break;
//	        case 1:    
//				PDC1 = (t02 + tA);//Phase R duty cycle T0/2+TB
//				PDC2 = (2*MY_PTPER - t02);//Phase Y duty cycle T0/2
//				PDC3 = t02;//Phase B duty cycle TS-T0/2    
//	            break;
//	        case 2:
//				PDC1 = t02;//Phase R duty cycle TS-T0/2
//				PDC2 = (2*MY_PTPER - t02);//Phase Y duty cycle T0/2
//				PDC3 = (t02 + tB); //Phase B duty cycle T0/2+TA
//	            break;
//	        case 3:
//				PDC1 = t02;//Phase R duty cycle TS-T0/2
//				PDC2 = (tA + t02);//Phase Y duty cycle T0/2+TB
//				PDC3 = (2*MY_PTPER - t02);//Phase B duty cycle T0/2
//	            break;
//	        case 4:
//				PDC1 = (t02 + tB);//Phase R duty cycle T0/2+TA
//				PDC2 = t02;//Phase Y duty cycle TS-T0/2
//				PDC3 = (2*MY_PTPER - t02);//Phase B duty cycle T0/2
//	            break;
//	        case 5:
//				PDC1 = (2*MY_PTPER - t02);//Phase R duty cycle T0/2
//				PDC2 = t02;//Phase Y duty cycle TS-T0/2
//				PDC3 = (t02 + tA);//Phase B duty cycle T0/2+TB
//	            //break;							
//	  }
           
           
}

void TIMER2_config(){
    
    T2CONbits.TON = 0;      //Timer_2 is OFF
    TMR2 = 0;               //resets Timer_2
    PR2 = PR2_INITIAL_VALUE;               //sets the maximum count for Timer_2
    T2CONbits.TCS = 0;      //choose FCY as clock source for Timer_2
    T2CONbits.TCKPS = 0;    //sets the Timer_2 pre-scaler to 1:1
    IFS0bits.T2IF = 0;      //clears Timer_2 interrupt flag
    _T2IE = 1;              //enables Timer_2 Interrupts
    //IPC1bits.T2IP=5;
    T2CONbits.TON = 1;      //turns Timer_2 ON    
    
}

void TIMER3_config(){
    
    T3CONbits.TON = 0;      //Timer_3 is OFF
    TMR3 = 0;               //resets Timer_3
    PR3 = MY_PR3;               //sets the maximum count for Timer_3
    T3CONbits.TCS = 0;      //choose FCY as clock source for Timer_3
    T3CONbits.TCKPS = 0b00; //sets the Timer_3 pre-scaler to 1:1
    IFS0bits.T3IF = 0;      //clears Timer_3 interrupt flagPR2_FINAL_VALUE
    _T3IE = 1;              //enables Timer_3 Interrupts
    //IPC1bits.T3IP=3;
    T3CONbits.TON = 1;    //turns Timer_3 OFF     
    
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){    
    
    _LATD1=1;
             
    IFS0bits.T2IF = 0;
    
    
    //Update PDC code
    
    g_sector=(g_sector+(g_ang+g_angIncrement)/ANGLE_RESOLUTION)%6;        
    g_ang=(g_ang+g_angIncrement)%ANGLE_RESOLUTION;      
    setPDC(g_sector,g_ang);
    
    _LATD1=0;
        
}

void PWM_init(){

    PTCONbits.PTEN=0;
    
    PWMCON2bits.IUE=1;
    
    PTCONbits.PTMOD=0b10;  //Working mode up-down counting 0b10
    PTCONbits.PTCKPS=0b00; //Source-Timer Prescaler (0,1,2,3) (1:1, 1:4, 1:16, 1:64)
    PTPER=MY_PTPER;        //max 32767
    PTMR=0;                //Set the timer to initial value 0
    PDC1=0;                //if PDC is PTPER dc=50%, PDC is double of duty-cycle
    PDC2=0;                //dc=25%
    PDC3=0;                //dc=75%
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
    
    IEC2bits.PWMIE=0;      //Interrupt Disable, Flag is IFS2bits.PWMIF
    IFS2bits.PWMIF = 0;    //Clear Interrupt Flag
    
    PTCONbits.PTEN=1;      //Enable PWM at end of Initiation routine
    
}
