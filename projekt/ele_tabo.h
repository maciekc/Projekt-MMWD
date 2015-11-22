#pragma once

#include <iostream>

using namespace std;

//klasa reprezentujaca element zabroniony
class ele_tabo
{
	int x;
	int y;
	int rodz_sas;
	
public:
	static int licznik;  //licznik elementow na tabo liscie
	int & wspx()
	{
		return x;
	}
	int & wspy()
	{
		return y;
	}
	int & nr_sas()
	{
		return rodz_sas;
	}

	ele_tabo(int a,int b,int c);
	~ele_tabo();
};

