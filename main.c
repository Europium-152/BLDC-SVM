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

#define LAST_TABLE_INDEX 127
#define ANGLE_RESOLUTION 128

#define MY_PTPER 589
#define INVERSE_FACTOR 301979822
#define ACELERACAO 10.0
#define PR2_INITIAL_VALUE 65000
#define PR2_FINAL_VALUE 383
#define MY_PR3 30000

float g_inverse=INVERSE_FACTOR/ACELERACAO, 
      g_truePR2=PR2_INITIAL_VALUE;

//Global Variables
int g_ang=0,           //Converted angle, stores the angle in units of its resolution, 
                       //Working with 128 resolution, the total 360 circle is 128*60=7680 units long
    g_angIncrement=8,  //angle increment step Magnetic Fiel freq. is FPWM/atCount*(angIncrement/7680)    
    g_sector=0,       //circle sector 0 to 5
    g_m=5;             //VDC divisor, g_m=x means max tension is equal to V_unreg/x
    

     
//Lookup table for sin(g_angle)*PTPER
int tab[128] = {0, 5, 10, 14, 19, 24, 29, 34, 39, 43, 48, 53, 58, 63, 67, 72, 77, 82, 86, 91, 96, 101, 105, 110, 115,
		120, 124, 129, 134, 138, 143, 148, 152, 157, 162, 166, 171, 176, 180, 185, 189, 194, 198, 203, 207, 212,
		216, 221, 225, 230, 234, 239, 243, 247, 252, 256, 261, 265, 269, 273, 278, 282, 286, 290, 295, 299, 303,
		307, 311, 315, 319, 323, 327, 331, 335, 339, 343, 347, 351, 355, 359, 362, 366, 370, 374, 377, 381, 385,
		388, 392, 396, 399, 403, 406, 410, 413, 416, 420, 423, 427, 430, 433, 436, 440, 443, 446, 449, 452, 455, 
		458, 461, 464, 467, 470, 473, 476, 479, 482, 484, 487, 490, 492, 495, 498, 500, 503, 505, 508};

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void);

void setPDC(int setor, int angB);

void TIMER2_config();

void TIMER3_config();

void PWM_init();

int main(void) {
    
    _TRISF0 = 0;
    _TRISF1 = 0;
    _TRISD3 = 0;
    _TRISD1 = 0;

    _LATF0 = 1;
    _LATF1 = 0;
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
    
    
   

    g_truePR2-=(g_truePR2*g_truePR2/g_inverse);
     

        if(g_truePR2<PR2_FINAL_VALUE )
    {
        _T3IE = 0; //Disable aceleration
        _LATF1=!_LATF1;
        _LATD3=1; //Toggle D3/OC4 pin for viewing in osciloscope
        //start_breaking();
        //regen();
    }    
       
    else{
    
        PR2=(unsigned int)g_truePR2;
       

        }
    _LATD3=0;
}

void setPDC(int setor, int angB) {
    _LATD1=0;
	static int tA, tB, t02, angA;
    static int tab_angA,tab_angB;
	angA = LAST_TABLE_INDEX - angB;
    tab_angA=tab[angA];
    tab_angB=tab[angB];
	tB = 2 * tab[angB]*g_m/10;
	tA = 2 * tab[angA]*g_m/10;
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
           _LATD1=1;
           
}

void TIMER2_config(){
    
    T2CONbits.TON = 0;      //Timer_2 is OFF
    TMR2 = 0;               //resets Timer_2
    PR2 = PR2_INITIAL_VALUE;               //sets the maximum count for Timer_2
    T2CONbits.TCS = 0;      //choose FCY as clock source for Timer_2
    T2CONbits.TCKPS = 1;    //sets the Timer_2 pre-scaler to 1:64
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
