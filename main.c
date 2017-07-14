#define MY_PTPER 818

#include <iostream>
using namespace std;

int PDC1, PDC2, PDC3;
//int tab[20] = {0, 43, 86, 128, 170, 212, 253, 293, 333, 371, 409, 446, 481, 515, 547, 578, 608, 636, 662, 686};
int tab[128] = {0, 8, 15, 23, 30, 38, 45, 53, 60, 68, 75, 83, 90, 98, 105, 113, 120, 128, 135, 143, 150, 157, 165, 172, 
		180, 187, 194, 202, 209, 216, 224, 231, 238, 246, 253, 260, 267, 275, 282, 289, 296, 303, 310, 317, 324, 331,
		338, 345, 352, 359, 366, 373, 380, 387, 394, 401, 407, 414, 421, 427, 434, 441, 447, 454, 461, 467, 473, 480,
		486, 493, 499, 505, 512, 518, 524, 530, 536, 543, 549, 555, 561, 567, 573, 578, 584, 590, 596, 602, 607, 613,
		619, 624, 630, 635, 641, 646, 651, 657, 662, 667, 672, 677, 682, 687, 692, 697, 702, 707, 712, 717, 721, 726,
		731, 735, 740, 744, 749, 753, 757, 762, 766, 770, 774, 778, 782, 786, 790, 794};

void setPDC(int setor, int angB) {
	int tA, tB, t02, angA;
	angA = 19 - angB;
	tB = 2 * tab[angB];
	tA = 2 * tab[angA];
	t02 = (2*MY_PTPER - tA - tB)/2;

	//switch
	    switch(setor){
	        case 1:
				PDC1 = g_m*2*MY_PTPER - t02; //Phase R duty cycle T0/2
				PDC2 = g_m*tB + t02;//Phase Y duty cycle T0/2+TA
				PDC3 = g_m*t02;//Phase B duty cycle TS-T0/2
	            break;
	        case 2:    
				PDC1 = g_m*t02 + tA;//Phase R duty cycle T0/2+TB
				PDC2 = g_m*2*MY_PTPER - t02;//Phase Y duty cycle T0/2
				PDC3 = g_m*t02;//Phase B duty le TS-T0/2    
	            break;
	        case 3:
				PDC1 = g_m*t02;//Phase R duty cycle TS-T0/2
				PDC2 = g_m*2*MY_PTPER - t02;//Phase Y duty cycle T0/2
				PDC3 = g_m*t02 + tB; //Phase B duty cycle T0/2+TA
	            break;
	        case 4:
				PDC1 = g_m*t02;//Phase R duty cycle TS-T0/2
				PDC2 = g_m*tA + t02;//Phase Y duty cycle T0/2+TB
				PDC3 = g_m*2*MY_PTPER - t02;//Phase B duty cycle T0/2
	            break;
	        case 5:
				PDC1 = g_m*t02 + tB;//Phase R duty cycle T0/2+TA
				PDC2 = g_m*t02;//Phase Y duty cycle TS-T0/2
				PDC3 = g_m*2*MY_PTPER - t02;//Phase B duty cycle T0/2
	            break;
	        case 6:
				PDC1 = g_m*2*MY_PTPER - t02;//Phase R duty cycle T0/2
				PDC2 = g_m*t02;//Phase Y duty cycle TS-T0/2
				PDC3 = g_m*t02 + tA;//Phase B duty cycle T0/2+TB
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
