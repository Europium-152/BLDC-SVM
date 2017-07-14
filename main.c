

#define FCY 29491200L

#define MY_PTPER 818          //max 32767
#define LAST_TABLE_INDEX 19
#define ANGLE_RESOLUTION 20

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

//Global Variables
int g_ang=0,           //Converted angle, stores the angle in units of its resolution, 
                       //Working with 128 resolution, the total 360 circle is 128*60=7680 units long
    g_atCount=1000,       //Actualization count g_atCount=x means duty cycles get actualized every 3 PWM periods
    g_angIncrement=1,  //angle increment step Magnetic Fiel freq. is FPWM/atCount*(angIncrement/7680)    
    g_sector=0,        //circle sector 0 to 5
    g_m=2;             //VDC divisor, g_m=x means max tension is equal to V_unreg/x
    

     
//Lookup table for sin(g_angle)*PTPER
int tab[20] = {0, 43, 86, 128, 170, 212, 253, 293, 333, 371, 409, 446, 481, 515, 547, 578, 608, 636, 662, 686};

void setPDC(int setor, int angB);

void __attribute__((interrupt, auto_psv)) _PWMInterrupt( void );

void PWM_init();

int main(void) {
    
    _TRISF0 = 0;

    
    PWM_init();
        
    while(1);
    
    return 0;
}

//Scale 1:1 e PTPER 589 => 25kHz pwm
void PWM_init(){

    PTCONbits.PTMOD=0b10;  //Working mode up-down counting 0b10
    PTCONbits.PTCKPS=0b00; //Source-Timer Prescaler (0,1,2,3) (1:1, 1:4, 1:16, 1:64)
    PTPER=MY_PTPER;        //max 32767
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
    IFS2bits.PWMIF = 0; //Clear Interrupt Flag
    
    PTCONbits.PTEN=1;      //Enable PWM at end of Initiation routine
    
}

void __attribute__((interrupt, auto_psv)) _PWMInterrupt( void )               
{
    //your code here
    IFS2bits.PWMIF = 0; //Clear Interrupt Flag
    static int i=0;
    
    
    if(i==g_atCount)
    { //Update PDC code
        setPDC(g_sector,g_ang);
        g_sector=(g_sector+(g_ang+g_angIncrement)/ANGLE_RESOLUTION)%6;        
        g_ang=(g_ang+g_angIncrement)%ANGLE_RESOLUTION;
             
        
       _LATF0=(!_LATF0); //Every g_atCount interrupts toggle F0 bit, which connects to LED D6
       i=0;
    }
    i++;
    
    
    
}


void setPDC(int setor, int angB) {
	static int tA, tB, t02, angA;
	angA = LAST_TABLE_INDEX - angB;
	tB = 2 * tab[angB]/g_m;
	tA = 2 * tab[angA]/g_m;
	t02 = (2*MY_PTPER - tA - tB)/2;

	//switch
	       switch(setor){
	        case 0:
				PDC1 = (2*MY_PTPER - t02); //Phase R duty cycle T0/2
				PDC2 = (tB + t02);//Phase Y duty cycle T0/2+TA
				PDC3 = t02;//Phase B duty cycle TS-T0/2
	            break;
	        case 1:    
				PDC1 = (t02 + tA);//Phase R duty cycle T0/2+TB
				PDC2 = (2*MY_PTPER - t02);//Phase Y duty cycle T0/2
				PDC3 = t02;//Phase B duty cycle TS-T0/2    
	            break;
	        case 2:
				PDC1 = t02;//Phase R duty cycle TS-T0/2
				PDC2 = (2*MY_PTPER - t02);//Phase Y duty cycle T0/2
				PDC3 = (t02 + tB); //Phase B duty cycle T0/2+TA
	            break;
	        case 3:
				PDC1 = t02;//Phase R duty cycle TS-T0/2
				PDC2 = (tA + t02);//Phase Y duty cycle T0/2+TB
				PDC3 = (2*MY_PTPER - t02);//Phase B duty cycle T0/2
	            break;
	        case 4:
				PDC1 = (t02 + tB);//Phase R duty cycle T0/2+TA
				PDC2 = t02;//Phase Y duty cycle TS-T0/2
				PDC3 = (2*MY_PTPER - t02);//Phase B duty cycle T0/2
	            break;
	        case 5:
				PDC1 = (2*MY_PTPER - t02);//Phase R duty cycle T0/2
				PDC2 = t02;//Phase Y duty cycle TS-T0/2
				PDC3 = (t02 + tA);//Phase B duty cycle T0/2+TB
	            //break;							
	  }

}
