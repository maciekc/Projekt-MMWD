#include "sklep.h"

int sklep::licznik = 0;

sklep::sklep(int xx, int yy, int r,int z,double d)
{
	x = xx;
	y = yy;
	rodzaj = r;
	zasieg = z;
	dochod = d;
	licznik++;
}


sklep::~sklep()
{
	licznik--;

}
void sklep::zmien_rodzaj(int nowy)
{
	rodzaj = nowy; //zmienia rodzaj sklepu
}
sklep::sklep(const sklep &s)
{
	x = s.x;
	y = s.y;
	rodzaj = s.rodzaj;
	zasieg = s.zasieg;
	dochod = s.dochod;
}

ostream & operator<< (ostream & ekran, const sklep & m)
{
	ekran << m.x << " " << m.y << " " << m.rodzaj << " " << m.zasieg <<" "<<m.dochod <<"\n";
	return ekran;
}