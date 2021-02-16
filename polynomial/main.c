#include<stdio.h>
#include<stdlib.h>
#include"polynomial.h"

static	void
myPrint(char *pn, Polynomial *pp, int testZero, int showDegree)
{
	printf("%s(x)=", pn);
	Polynomial_Print(pp);
	if (testZero) {
		printf(", is %sa zero polynomial",
			(Polynomial_isZero(pp) ? "" : "not ")
		);
	}
	if (showDegree) {
		printf(", degree = %d",
			Polynomial_Degree(pp)
		);
	}
	printf("\n");
}


int
main(int argc, char *argv[])
{
	Polynomial PA, PB, PC, PD, PE, PF;

	Polynomial_Zero(&PA);
	myPrint("A", &PA, YES, YES);

	Polynomial_Attach(&PA, 3, 0);	myPrint("A", &PA, NO, YES);
	Polynomial_Attach(&PA, -12, 1);	myPrint("A", &PA, NO, YES);
	Polynomial_Attach(&PA, 5, 6);	myPrint("A", &PA, NO, YES);
	Polynomial_Attach(&PA, -1, 4);	myPrint("A", &PA, NO, YES);
	Polynomial_Attach(&PA, 3, 3);	myPrint("A", &PA, NO, YES);
	Polynomial_Attach(&PA, -2, 7);	myPrint("A", &PA, NO, YES);
	Polynomial_Remove(&PA, 1);		myPrint("A", &PA, NO, YES);
	Polynomial_Remove(&PA, 7);		myPrint("A", &PA, NO, YES);
	printf("coef of x^0 is %f\n", Polynomial_Coef(&PA, 0));
	printf("coef of x^4 is %f\n", Polynomial_Coef(&PA, 4));

	Polynomial_Zero(&PB);
	Polynomial_Attach(&PB, -3, 3);	myPrint("B", &PB, NO, YES);

	Polynomial_Add(&PA, &PB, &PC);	myPrint("C", &PC, NO, YES);

	Polynomial_Zero(&PD);
	Polynomial_Attach(&PD, 1, 0);
	Polynomial_Attach(&PD, 1, 50);	myPrint("D", &PD, NO, YES);
	Polynomial_Zero(&PE);
	Polynomial_Attach(&PE, 1, 0);
	Polynomial_Attach(&PE, 1, 50);	myPrint("E", &PE, NO, YES);
	Polynomial_Mul(&PD, &PE, &PF);	myPrint("F", &PF, NO, YES);

	return 0;
}
