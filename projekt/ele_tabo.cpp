#include "ele_tabo.h"

int ele_tabo::licznik = 0;
ele_tabo::ele_tabo(int a,int b,int c)
{
	x = a;
	y = b;
	rodz_sas = c;
	licznik++;
}


ele_tabo::~ele_tabo()
{
	licznik--;
	cout << "dziala destruktor sasiedztwa\n";
}
