#define MY_PTPER 818

#include <iostream>
using namespace std;

int PDC1, PDC2, PDC3;
//int tab[20] = {0, 43, 86, 128, 170, 212, 253, 293, 333, 371, 409, 446, 481, 515, 547, 578, 608, 636, 662, 686};
int tab[128] = {0, 5, 10, 14, 19, 24, 29, 34, 39, 43, 48, 53, 58, 63, 67, 72, 77, 82, 86, 91, 96, 101, 105, 110, 115,
		120, 124, 129, 134, 138, 143, 148, 152, 157, 162, 166, 171, 176, 180, 185, 189, 194, 198, 203, 207, 212,
		216, 221, 225, 230, 234, 239, 243, 247, 252, 256, 261, 265, 269, 273, 278, 282, 286, 290, 295, 299, 303,
		307, 311, 315, 319, 323, 327, 331, 335, 339, 343, 347, 351, 355, 359, 362, 366, 370, 374, 377, 381, 385,
		388, 392, 396, 399, 403, 406, 410, 413, 416, 420, 423, 427, 430, 433, 436, 440, 443, 446, 449, 452, 455, 
		458, 461, 464, 467, 470, 473, 476, 479, 482, 484, 487, 490, 492, 495, 498, 500, 503, 505, 508};

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
