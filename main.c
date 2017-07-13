#define MY_PTPER 818

#include <iostream>
using namespace std;

int PDC1, PDC2, PDC3;
int tab[20] = {0, 43, 86, 128, 170, 212, 253, 293, 333, 371, 409, 446, 481, 515, 547, 578, 608, 636, 662, 686};

void setPDC(int setor, int angB) {
	int tA, tB, t02, angA;
	angA = 19 - angB;
	tB = 2 * tab[angB];
	tA = 2 * tab[angA];
	t02 = (2*MY_PTPER - tA - tB)/2;

	//switch
	    switch(setor){
	        case 1:
				PDC1 = 2*MY_PTPER - t02; //Phase R duty cycle T0/2
				PDC2 = tB + t02;//Phase Y duty cycle T0/2+TA
				PDC3 = t02;//Phase B duty cycle TS-T0/2
	            break;
	        case 2:    
				PDC1 = t02 + tA;//Phase R duty cycle T0/2+TB
				PDC2 = 2*MY_PTPER - t02;//Phase Y duty cycle T0/2
				PDC3 = t02;//Phase B duty cycle TS-T0/2    
	            break;
	        case 3:
				PDC1 = t02;//Phase R duty cycle TS-T0/2
				PDC2 = 2*MY_PTPER - t02;//Phase Y duty cycle T0/2
				PDC3 = t02 + tB; //Phase B duty cycle T0/2+TA
	            break;
	        case 4:
				PDC1 = t02;//Phase R duty cycle TS-T0/2
				PDC2 = tA + t02;//Phase Y duty cycle T0/2+TB
				PDC3 = 2*MY_PTPER - t02;//Phase B duty cycle T0/2
	            break;
	        case 5:
				PDC1 = t02 + tA;//Phase R duty cycle T0/2+TA
				PDC2 = t02;//Phase Y duty cycle TS-T0/2
				PDC3 = 2*MY_PTPER - t02;//Phase B duty cycle T0/2
	            break;
	        case 6:   
				PDC1 = 2*MY_PTPER - t02;//Phase R duty cycle T0/2
				PDC2 = t02;//Phase Y duty cycle TS-T0/2
				PDC3 = t02 + tA;//Phase B duty cycle T0/2+TB
	            break;				
	  }

}

int main() {


	for (int setor = 1; setor < 7; setor++) {
		
		for (int angB = 0; angB < 20; angB++) {
			setPDC(setor, angB);
			//cout << "setor " << setor << "   angB " << angB << 
		}

	}

		
		



	cout << "working!" << endl;
	return 0;
}
